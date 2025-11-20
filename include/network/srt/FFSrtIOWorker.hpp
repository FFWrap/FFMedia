#pragma once

#include "network/srt/FFSrtCommon.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

namespace ff {
    class FFSrtSession;

    class FFSrtIOWorker {
    private:
        int workerId;
        std::thread th;
        std::atomic<bool> running{ false };

        std::mutex newSessionsMtx;
        std::vector<std::shared_ptr<FFSrtSession>> newSessions;
        std::vector<std::shared_ptr<FFSrtSession>> sessions;

        void runLoop();

    public:
        FFSrtIOWorker(int id);
        ~FFSrtIOWorker();

        void start();
        void stop();
        void addSession(std::shared_ptr<FFSrtSession> session);
    };
}