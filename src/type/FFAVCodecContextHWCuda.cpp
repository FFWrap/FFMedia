#include "type/FFAVCodecContextHWCuda.hpp"
#include "type/FFAVCodecContext.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

namespace ff {
    FFAVCodecContextHWCuda::FFAVCodecContextHWCuda()
    : FFAVCodecContextHWFormat(ENCODE_HW_TYPE::CUDA, AV_HWDEVICE_TYPE_CUDA)
    {

    }

    FFAVCodecContextHWCuda::~FFAVCodecContextHWCuda() {

    }

    bool FFAVCodecContextHWCuda::findHWFormat(FFAVCodecContext& codecContext) {
        AVCodecContext* context = codecContext.getImpl()->getRaw();
        const AVCodecHWConfig* codecHwConfig = nullptr;
        for (int i = 0; (codecHwConfig = avcodec_get_hw_config(context->codec, i)); i++) {
            if (codecHwConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
                codecHwConfig->device_type == AV_HWDEVICE_TYPE_CUDA) {
                codecContext.setPixFmt((int)codecHwConfig->pix_fmt);
                return true;
                }
        }
        return false;
    }

}