#pragma once

#include "FFAVDecodeStream.hpp"
#include "type/FFAVStream.hpp"
#include "type/FFAVCodecContext.hpp"
#include "type/FFAVCodecContextHWFormat.hpp"

namespace ff {
    class FFAVCodecContextFactory {
    public:
        explicit FFAVCodecContextFactory() = delete;
        explicit FFAVCodecContextFactory(FFAVCodecContextFactory&&) = delete;
        explicit FFAVCodecContextFactory(FFAVCodecContextFactory&) = delete;

    public:
        static FFAVCodecContextPtr createContext(FFAVDecodeStreamPtr decodeStream, FFAVCodecContextHWFormatPtr hwFormat, AVError* error = nullptr);
        static FFAVCodecContextHWFormatPtr createHWFormat(ENCODE_HW_TYPE encodeType);

    private:
        static FFAVCodecContextPtr createDecodeContext(FFAVDecodeStreamPtr decodeStream, FFAVCodecContextHWFormatPtr hwFormat, AVError* error);
    };
}