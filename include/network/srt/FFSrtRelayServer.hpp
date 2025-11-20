#pragma once

#include "network/srt/FFSrtCommon.hpp"

#include <vector>
#include <atomic>

namespace ff {
    class FFSrtIOWorker;

    class FFSrtRelayServer {
    private:
        std::vector<std::unique_ptr<FFSrtIOWorker>> workers;
        SRTSOCKET listenSock;
        std::atomic<bool> running{ false };
        int nextWorkerIdx = 0;

        void acceptLoop();

    public:
        FFSrtRelayServer(int workerCount = 4);
        ~FFSrtRelayServer();

        bool start(int port);
        void stop();
    };
};