#pragma once

#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <iostream>

extern "C" {
#include <srt/srt.h>
}

namespace ff {
    // ---------------------------------------------------------------------------
    // 패킷 정의 (Zero-Copy)
    // ---------------------------------------------------------------------------
    struct FFSrtPacket {
        std::vector<char> data;
        uint64_t timestamp;

        FFSrtPacket(const char* buf, int size) : data(buf, buf + size) {
            timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
        }
    };

    using PacketPtr = std::shared_ptr<FFSrtPacket>;

    // ---------------------------------------------------------------------------
    // 세션 타입 정의
    // ---------------------------------------------------------------------------
    enum class SessionType {
        PUBLISHER,
        SUBSCRIBER
    };
}