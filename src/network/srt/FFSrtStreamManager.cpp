#include "network/srt/FFSrtStreamManager.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
    FFSrtStreamManager& FFSrtStreamManager::instance() {
        static FFSrtStreamManager inst;
        return inst;
    }

    std::shared_ptr<FFSrtStreamChannel> FFSrtStreamManager::getOrCreate(const std::string& id) {
        std::lock_guard<std::mutex> lock(mapMtx);
        auto it = channels.find(id);
        if (it != channels.end()) {
            return it->second;
        }
        auto newChannel = std::make_shared<FFSrtStreamChannel>(id);
        channels[id] = newChannel;
        std::cout << "[Manager] New Channel Created: " << id << std::endl;
        return newChannel;
    }
}