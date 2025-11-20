#include "network/srt/FFSrtRelayServer.hpp"
#include "network/srt/FFSrtIOWorker.hpp"
#include "network/srt/FFSrtSession.hpp"
#include "network/srt/FFSrtStreamManager.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace ff {
    FFSrtRelayServer::FFSrtRelayServer(int workerCount) {
        for (int i = 0; i < workerCount; i++) {
            workers.push_back(std::make_unique<FFSrtIOWorker>(i));
        }
    }

    FFSrtRelayServer::~FFSrtRelayServer() {
        stop();
    }

    bool FFSrtRelayServer::start(int port) {
        srt_startup();

        listenSock = srt_create_socket();
        if (listenSock == SRT_INVALID_SOCK) return false;

        sockaddr_in sa;
        memset(&sa, 0, sizeof sa);
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
        sa.sin_addr.s_addr = INADDR_ANY;

        int yes = 1;
        srt_setsockopt(listenSock, 0, SRTO_RCVSYN, &yes, sizeof yes);

        if (srt_bind(listenSock, (sockaddr*)&sa, sizeof sa) == SRT_ERROR) {
            std::cerr << "Bind failed" << std::endl;
            return false;
        }

        if (srt_listen(listenSock, 5) == SRT_ERROR) {
            std::cerr << "Listen failed" << std::endl;
            return false;
        }

        for (auto& w : workers) w->start();
        running = true;

        std::cout << "SRT Relay Server listening on port " << port << " with " << workers.size() << " workers." << std::endl;

        acceptLoop();
        return true;
    }

    void FFSrtRelayServer::stop() {
        if (!running) return;
        running = false;
        srt_close(listenSock);
        for (auto& w : workers) w->stop();
        srt_cleanup();
    }

    void FFSrtRelayServer::acceptLoop() {
        while (running) {
            sockaddr_in sc;
            int len = sizeof sc;

            SRTSOCKET newSock = srt_accept(listenSock, (sockaddr*)&sc, &len);
            if (newSock == SRT_INVALID_SOCK) {
                if (!running) break;
                continue;
            }

            char streamIdBuf[512] = { 0 };
            int size = sizeof(streamIdBuf);
            if (srt_getsockopt(newSock, 0, SRTO_STREAMID, streamIdBuf, &size) == SRT_ERROR) {
                srt_close(newSock);
                continue;
            }

            std::string rawStreamId(streamIdBuf);
            if (rawStreamId.empty()) {
                srt_close(newSock);
                continue;
            }

            // -------------------------------------------------------------------
            // streamId 파싱: "pub:room1" -> type=PUB, id="room1"
            // -------------------------------------------------------------------
            SessionType type = SessionType::SUBSCRIBER; // 기본값
            std::string channelId = rawStreamId;

            if (rawStreamId.rfind("pub:", 0) == 0) {
                type = SessionType::PUBLISHER;
                channelId = rawStreamId.substr(4); // "pub:" 제거
            } else if (rawStreamId.rfind("sub:", 0) == 0) {
                type = SessionType::SUBSCRIBER;
                channelId = rawStreamId.substr(4); // "sub:" 제거
            }

            if (channelId.empty()) {
                std::cerr << "[Reject] Invalid Channel ID derived from: " << rawStreamId << std::endl;
                srt_close(newSock);
                continue;
            }

            std::cout << "[Accept] Raw: " << rawStreamId
                << " -> Mode: " << (type == SessionType::PUBLISHER ? "PUB" : "SUB")
                << ", Channel: " << channelId << std::endl;

            auto channel = FFSrtStreamManager::instance().getOrCreate(channelId);
            auto session = std::make_shared<FFSrtSession>(newSock, rawStreamId, type);
            session->channel = channel;

            if (type == SessionType::SUBSCRIBER) {
                channel->addSubscriber(session);
            }

            workers[nextWorkerIdx]->addSession(session);
            nextWorkerIdx = (nextWorkerIdx + 1) % workers.size();
        }
    }
}