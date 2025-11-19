#include "gtest/gtest.h"

#include "network/ffnetwork.hpp"




TEST(SRT, RELAY_SERVER) {
	ff::FFSrtInitialize();

	ff::FFSrtRelayServer relayServer;
	relayServer.start();

	std::cout << "Wait close input..." << std::endl;
	int a;
	std::cin >> a;

	relayServer.stop();
	ff::FFSrtFinalize();
}