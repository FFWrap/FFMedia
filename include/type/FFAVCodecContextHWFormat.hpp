#pragma once

#include <memory>

#include "type/FFAVCodecContext.hpp"

namespace ff {
    enum ENCODE_HW_TYPE {
        CPU,
        CUDA,
    };

    class FFAVCodecContextHWFormat {
    public:
        explicit FFAVCodecContextHWFormat(ENCODE_HW_TYPE encodeHW, int ffmpegEncodeHW);
        virtual ~FFAVCodecContextHWFormat();

    public:
        ENCODE_HW_TYPE getEncodeHWType();
        int getFFmpegEncodeHW();

        virtual bool findHWFormat(FFAVCodecContext& codecContext) = 0;

    private:
    private:
        ENCODE_HW_TYPE encodeHW;
        int ffmpegEncodeHW; // FFmpeg HW 인코딩 파라미터.
    };

    using FFAVCodecContextHWFormatPtr = std::shared_ptr<FFAVCodecContextHWFormat>;
};