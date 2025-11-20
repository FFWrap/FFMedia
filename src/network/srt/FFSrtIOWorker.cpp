#include "network/srt/FFSrtIOWorker.hpp"
#include "network/srt/FFSrtSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
    FFSrtIOWorker::FFSrtIOWorker(int id) : workerId(id) {}

    FFSrtIOWorker::~FFSrtIOWorker() {
        stop();
    }

    void FFSrtIOWorker::start() {
        if (running) return;
        running = true;
        th = std::thread(&FFSrtIOWorker::runLoop, this);
    }

    void FFSrtIOWorker::stop() {
        if (!running) return;
        running = false;
        if (th.joinable()) th.join();
    }

    void FFSrtIOWorker::addSession(std::shared_ptr<FFSrtSession> session) {
        std::lock_guard<std::mutex> lock(newSessionsMtx);
        newSessions.push_back(session);
    }

    void FFSrtIOWorker::runLoop() {
        std::cout << "[Worker " << workerId << "] Started." << std::endl;

        while (running) {
            {// 脚痹 技记
                std::lock_guard<std::mutex> lock(newSessionsMtx);
                if (!newSessions.empty()) {
                    sessions.insert(sessions.end(), newSessions.begin(), newSessions.end());
                    newSessions.clear();
                }
            }

            // 技记 风橇
            for (auto it = sessions.begin(); it != sessions.end(); ) {
                auto& session = *it;
                bool keepAlive = true;

                if (session->type == SessionType::PUBLISHER) {
                    // PUBLISHER 贸府 (弥措 5俺)
                    for (int i = 0; i < 5; i++) {
                        if (!session->tryRecv()) {
                            keepAlive = false;
                            break;
                        }
                    }
                }
                else { // SUBSCRIBER
                    for (int i = 0; i < 5; i++) {
                        if (!session->trySend()) {
                            keepAlive = false;
                            break;
                        }
                        std::lock_guard<std::mutex> lk(session->queueMtx);
                        if (session->sendQueue.empty()) break;
                    }
                }

                // 辆丰 贸府
                if (!keepAlive) {
                    if (session->type == SessionType::SUBSCRIBER && session->channel) {
                        session->channel->removeSubscriber(session);
                    }

                    if (session->sock != SRT_INVALID_SOCK) {
                        srt_close(session->sock);
                        session->sock = SRT_INVALID_SOCK;
                    }
                    it = sessions.erase(it);
                } else {
                    ++it;
                }
            }

            // CPU 苞何窍 规瘤
            std::this_thread::sleep_for(std::chrono::microseconds(0));
        }
    }
}