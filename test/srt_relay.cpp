#include "gtest/gtest.h"

#include "network/ffnetwork.hpp"




TEST(SRT, RELAY_SERVER) {
    int port = 9000;

    ff::FFSrtRelayServer server;
    if (!server.start(port)) {
        std::cerr << "Failed to start server." << std::endl;
    }
}