#include "network/srt/FFSrtStreamChannel.hpp"
#include "network/srt/FFSrtSession.hpp"

namespace ff {
    FFSrtStreamChannel::FFSrtStreamChannel(std::string id) : streamId(id) {}

    FFSrtStreamChannel::~FFSrtStreamChannel() {
        std::cout << "[Channel] Destroyed: " << streamId << std::endl;
    }

    void FFSrtStreamChannel::addSubscriber(std::shared_ptr<FFSrtSession> sub) {
        std::lock_guard<std::mutex> lock(subscribersMtx);
        subscribers.insert(sub);
    }

    void FFSrtStreamChannel::removeSubscriber(std::shared_ptr<FFSrtSession> sub) {
        std::lock_guard<std::mutex> lock(subscribersMtx);
        subscribers.erase(sub);
    }

    void FFSrtStreamChannel::broadcast(PacketPtr packet) {
        std::lock_guard<std::mutex> lock(subscribersMtx);
        for (auto& sub : subscribers) {
            sub->enqueuePacket(packet);
        }
    }

    size_t FFSrtStreamChannel::getSubscriberCount() {
        std::lock_guard<std::mutex> lock(subscribersMtx);
        return subscribers.size();
    }
}