#include "network/srt/FFSrtStreamManager.hpp"

namespace ff {
	FFSrtStreamManager::FFSrtStreamManager() {

	}

	FFSrtStreamManager::~FFSrtStreamManager() {

	}

	FFSrtStreamChannelPtr FFSrtStreamManager::getOrCreate(const std::string& streamId) {
		std::lock_guard lockGuard(this->mutex);

		FFSrtStreamChannelPtr returnStreamChannel;

		auto iter = this->channels.find(streamId);
		if (iter != this->channels.end()) {
			returnStreamChannel = iter->second;
		} else {
			returnStreamChannel = FFSrtStreamChannel::create();
			this->channels.emplace(streamId, returnStreamChannel);
		}

		return returnStreamChannel;
	}

	FFSrtStreamChannelPtr FFSrtStreamManager::find(const std::string& streamId) {

		return nullptr;
	}

	void FFSrtStreamManager::removeIfEmpty(const std::string& streamId) {

	}

	void FFSrtStreamManager::remove(const std::string& streamId) {

	}
}