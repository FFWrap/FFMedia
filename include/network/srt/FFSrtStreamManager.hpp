#pragma once

#include "network/srt/FFSrtStreamChannel.hpp"

#include <memory>
#include <string>
#include <mutex>
#include <unordered_map>

namespace ff {
	class FFSrtStreamManager {
	public:
		explicit FFSrtStreamManager();
		virtual ~FFSrtStreamManager();

	public:

	public:
		FFSrtStreamChannelPtr getOrCreate(const std::string& streamId);
		FFSrtStreamChannelPtr find(const std::string& streamId);

		void removeIfEmpty(const std::string& streamId);
		void remove(const std::string& streamId);

	private:
		std::mutex mutex;
		std::unordered_map<std::string, FFSrtStreamChannelPtr> channels;
	};
};