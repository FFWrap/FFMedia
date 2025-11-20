#pragma once

#include "network/srt/FFSrtCommon.hpp"

#include <unordered_set>
#include <mutex>

namespace ff {
    class FFSrtSession; // 전방 선언

    class FFSrtStreamChannel {
    private:
        std::string streamId;
        std::mutex subscribersMtx;
        std::unordered_set<std::shared_ptr<FFSrtSession>> subscribers;

    public:
        FFSrtStreamChannel(std::string id);
        ~FFSrtStreamChannel();

        void addSubscriber(std::shared_ptr<FFSrtSession> sub);
        void removeSubscriber(std::shared_ptr<FFSrtSession> sub);
        void broadcast(PacketPtr packet);
        size_t getSubscriberCount();
    };
}