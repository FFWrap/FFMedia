#pragma once

#include "network/srt/FFSrtStreamChannel.hpp"

#include <memory>
#include <string>

namespace ff {
	class FFSrtStreamManager {
	public:
		explicit FFSrtStreamManager();
		virtual ~FFSrtStreamManager();

	public:
		std::shared_ptr<FFSrtStreamChannel> getOrCreate(const std::string& streamId);
		std::shared_ptr<FFSrtStreamChannel> find(const std::string& streamId);
		void removeIfEmpty(const std::string& streamId);
		void remove(const std::string& streamId);
	};
};