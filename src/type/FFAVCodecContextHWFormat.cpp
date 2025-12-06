#include "type/FFAVCodecContextHWFormat.hpp"


namespace ff {
    FFAVCodecContextHWFormat::FFAVCodecContextHWFormat(ENCODE_HW_TYPE encodingHW, int ffmpegEncodeHW) {
        this->encodeHW = encodingHW;
        this->ffmpegEncodeHW = ffmpegEncodeHW;
    }

    FFAVCodecContextHWFormat::~FFAVCodecContextHWFormat() {

    }

    ENCODE_HW_TYPE FFAVCodecContextHWFormat::getEncodeHWType() {
        return this->encodeHW;
    }

    int FFAVCodecContextHWFormat::getFFmpegEncodeHW() {
        return this->ffmpegEncodeHW;
    }

}