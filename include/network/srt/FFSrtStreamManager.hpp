#pragma once

#include "network/srt/FFSrtCommon.hpp"

#include <unordered_map>
#include <mutex>

namespace ff {
    class FFSrtStreamChannel;

    class FFSrtStreamManager {
    public:
        static FFSrtStreamManager& instance();

        std::shared_ptr<FFSrtStreamChannel> getOrCreate(const std::string& id);

    private:
        FFSrtStreamManager() = default;
        std::mutex mapMtx;
        std::unordered_map<std::string, std::shared_ptr<FFSrtStreamChannel>> channels;
    };
}