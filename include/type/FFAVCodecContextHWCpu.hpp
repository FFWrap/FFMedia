#pragma once

#include "type/FFAVCodecContextHWFormat.hpp"

namespace ff {
    class FFAVCodecContextHWCpu : public FFAVCodecContextHWFormat {
    public:
        explicit FFAVCodecContextHWCpu();
        virtual ~FFAVCodecContextHWCpu();

    public:
        virtual bool findHWFormat(FFAVCodecContext& codecContext) override;
    };
}