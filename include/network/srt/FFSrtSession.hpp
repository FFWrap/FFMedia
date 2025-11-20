#pragma once

#include "network/srt/FFSrtCommon.hpp"

#include <deque>
#include <mutex>

namespace ff {
    class FFSrtStreamChannel;

    class FFSrtSession : public std::enable_shared_from_this<FFSrtSession> {
    public:
        SRTSOCKET sock;
        std::string streamId;
        SessionType type;

        // Subscriber용 전송 큐
        std::deque<PacketPtr> sendQueue;
        std::mutex queueMtx;

        // 채널 참조
        std::shared_ptr<FFSrtStreamChannel> channel;

        FFSrtSession(SRTSOCKET s, std::string id, SessionType t);
        ~FFSrtSession();

        // [Subscriber] 큐에 패킷 추가
        void enqueuePacket(PacketPtr packet);

        // [Publisher] 소켓 읽기 시도 (Non-blocking)
        bool tryRecv();

        // [Subscriber] 소켓 쓰기 시도 (Non-blocking)
        bool trySend();
    };
}