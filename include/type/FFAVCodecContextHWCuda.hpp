#pragma once

#include "FFAVCodecContextHWFormat.hpp"
#include "type/FFAVCodecContextHWCuda.hpp"

namespace ff {
    class FFAVCodecContextHWCuda : public FFAVCodecContextHWFormat {
    public:
        explicit FFAVCodecContextHWCuda();
        virtual ~FFAVCodecContextHWCuda();

    public:
        virtual bool findHWFormat(FFAVCodecContext& codecContext) override;
    };
}