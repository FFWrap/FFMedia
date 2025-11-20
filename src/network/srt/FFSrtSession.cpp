#include "network/srt/FFSrtSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
    FFSrtSession::FFSrtSession(SRTSOCKET s, std::string id, SessionType t)
        : sock(s), streamId(id), type(t) {
        // Non-blocking 설정
        bool sync = false;
        srt_setsockopt(sock, 0, SRTO_RCVSYN, &sync, sizeof sync);
        srt_setsockopt(sock, 0, SRTO_SNDSYN, &sync, sizeof sync);
    }

    FFSrtSession::~FFSrtSession() {
        if (sock != SRT_INVALID_SOCK) {
            srt_close(sock);
        }
        std::cout << "[Session] Destroyed: " << streamId
            << " (" << (type == SessionType::PUBLISHER ? "Pub" : "Sub") << ")" << std::endl;
    }

    void FFSrtSession::enqueuePacket(PacketPtr packet) {
        std::lock_guard<std::mutex> lock(queueMtx);
        if (sendQueue.size() > 1000) {
            sendQueue.pop_front(); // Drop head
        }
        sendQueue.push_back(packet);
    }

    bool FFSrtSession::tryRecv() {
        char buf[2048];
        int ret = srt_recvmsg(sock, buf, sizeof(buf));

        if (ret == SRT_ERROR) {
            int error = srt_getlasterror(NULL);
            if (error == SRT_EASYNCRCV) {
                return true; // 데이터 없음 (정상)
            }
            std::cout << "[Session] Pub Error/Disconnected: " << streamId << " Err: " << error << std::endl;
            return false;
        }

        if (ret > 0 && channel) {
            auto packet = std::make_shared<FFSrtPacket>(buf, ret);
            channel->broadcast(packet);
        }

        return true;
    }

    bool FFSrtSession::trySend() {
        std::shared_ptr<FFSrtPacket> pkt;

        {
            std::lock_guard<std::mutex> lock(queueMtx);
            if (sendQueue.empty()) return true;
            pkt = sendQueue.front();
        }

        int ret = srt_sendmsg(sock, pkt->data.data(), pkt->data.size(), -1, true);

        if (ret == SRT_ERROR) {
            int error = srt_getlasterror(NULL);
            if (error == SRT_EASYNCSND) {
                return true; // 소켓 버퍼 꽉 참 (나중에 재시도)
            }
            std::cout << "[Session] Sub Error/Disconnected: " << streamId << " Err: " << error << std::endl;
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(queueMtx);
            sendQueue.pop_front();
        }
        return true;
    }
}