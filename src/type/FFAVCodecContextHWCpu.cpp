#include "type/FFAVCodecContextHWCpu.hpp"
#include "type/FFAVCodecContext.hpp"
#include "type/FFAVCodecContextHWCuda.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    FFAVCodecContextHWCpu::FFAVCodecContextHWCpu()
    : FFAVCodecContextHWFormat(ENCODE_HW_TYPE::CPU, AV_HWDEVICE_TYPE_NONE){

    }

    FFAVCodecContextHWCpu::~FFAVCodecContextHWCpu() {

    }

    bool FFAVCodecContextHWCpu::findHWFormat(FFAVCodecContext& codecContext) {
        return true;
    }


}