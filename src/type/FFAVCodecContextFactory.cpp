#include "type/FFAVCodecContextFactory.hpp"

#include "type/FFAVCodecContextHWCpu.hpp"
#include "type/FFAVCodecContextHWCuda.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace ff {
    FFAVCodecContextPtr FFAVCodecContextFactory::createContext(FFAVDecodeStreamPtr decodeStream, FFAVCodecContextHWFormatPtr hwFormat, AVError* error) {
        return FFAVCodecContextFactory::createDecodeContext(decodeStream, hwFormat, error);
    }

    FFAVCodecContextHWFormatPtr FFAVCodecContextFactory::createHWFormat(ENCODE_HW_TYPE encodeType) {
        switch (encodeType) {
            case ENCODE_HW_TYPE::CPU:
                return std::make_shared<FFAVCodecContextHWCpu>();
            case ENCODE_HW_TYPE::CUDA:
                return std::make_shared<FFAVCodecContextHWCuda>();
            default:
                return std::make_shared<FFAVCodecContextHWCpu>();
        }
    }


    FFAVCodecContextPtr FFAVCodecContextFactory::createDecodeContext(FFAVDecodeStreamPtr decodeStream, FFAVCodecContextHWFormatPtr hwFormat, AVError* error) {
        AVError tempError;
        AVError* referError = error ? error : &tempError;

        if (decodeStream == nullptr || hwFormat == nullptr) {
            *referError = AVError(AV_ERROR_TYPE::INVALID_ARGUMENT);
            return nullptr;
        }

        FFAVCodecContextPtr codecContext = FFAVCodecContext::create();
        AVCodecParameters* codecParameters = decodeStream->getImpl()->getRaw()->codecpar;
        if (codecParameters == nullptr) {
            referError->setError(AV_ERROR_TYPE::FAILED,"codecParameters is nullptr",-1,"");
            return nullptr;
        }

        const AVCodec* decodeCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (decodeCodec == nullptr) {
            referError->setError(AV_ERROR_TYPE::AV_ERROR,"avcodec_find_decoder failed",AVERROR_DECODER_NOT_FOUND, "avcodec_find_decoder");
            return nullptr;
        }

        AVCodecContext* decodeCodecContext = avcodec_alloc_context3(decodeCodec);
        if (decodeCodecContext == nullptr) {
            referError->setError(AV_ERROR_TYPE::AV_ERROR,"avcodec_alloc_context3 failed",AVERROR(ENOMEM),"avcodec_alloc_context3");
            return nullptr;
        }
        codecContext->getImpl()->setRaw(decodeCodecContext);
        codecContext->setHWFormat(hwFormat);

        int ret = 0;
        if (codecContext->getHWFormat()->getEncodeHWType() != ENCODE_HW_TYPE::CPU 
            && decodeStream->getType() == DATA_TYPE::VIDEO) {
            ret = av_hwdevice_ctx_create(&decodeCodecContext->hw_device_ctx, (enum AVHWDeviceType)hwFormat->getFFmpegEncodeHW(), nullptr, nullptr, 0);
            if (ret < 0) {
                referError->setError(AV_ERROR_TYPE::AV_ERROR,"av_hwdevice_ctx_create failed",ret,"av_hwdevice_ctx_create");
                return nullptr;
            }

            if (codecContext->findHWFormat() == false) {
                referError->setError(AV_ERROR_TYPE::AV_ERROR,"hwFormat->findHWFormat(codecContext) failed",AVERROR_DECODER_NOT_FOUND,"hwFormat->findHWFormat(codecContext)");
                return nullptr;
            }
        }

        ret = avcodec_parameters_to_context(decodeCodecContext, codecParameters);
        if (ret < 0) {
            referError->setError(AV_ERROR_TYPE::AV_ERROR,"avcodec_parameters_to_context failed",ret, "avcodec_parameters_to_context");
            return nullptr;
        }

        ret = avcodec_open2(decodeCodecContext, decodeCodec, nullptr);
        if (ret < 0) {
            referError->setError(AV_ERROR_TYPE::AV_ERROR,"avcodec_open2 failed",ret,"avcodec_open2");
            return nullptr;
        }

        *referError = AVError(AV_ERROR_TYPE::SUCCESS);
        return codecContext;
    }
}