#include "network/srt/FFSrtStreamManager.hpp"

namespace ff {
	FFSrtStreamManager::FFSrtStreamManager() {

	}

	FFSrtStreamManager::~FFSrtStreamManager() {

	}

	std::shared_ptr<FFSrtStreamChannel> FFSrtStreamManager::getOrCreate(const std::string& streamId) {

		return nullptr;
	}

	std::shared_ptr<FFSrtStreamChannel> FFSrtStreamManager::find(const std::string& streamId) {

		return nullptr;
	}

	void FFSrtStreamManager::removeIfEmpty(const std::string& streamId) {

	}

	void FFSrtStreamManager::remove(const std::string& streamId) {

	}
}