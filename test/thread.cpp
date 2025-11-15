#include "gtest/gtest.h"

#include "utils/thread/FFThreadPool.hpp"

TEST(UTIL_TEST, FFThreadPool) {
	ff::FFThreadPool<bool> threadPool(2);
	threadPool.setShowDebugMessage(true);
	threadPool.setThreadAutoScale(true);

	for (int i = 0; i < 12; i++) {
		threadPool.enqueue([i]() {
			std::cout << "Thread: " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return true;
		});
	}


	std::this_thread::sleep_for(std::chrono::seconds(100));
}