#include "type/FFAVCodecContext.hpp"
#include "type/FFAVCodecContextHWFormat.hpp"
#include "type/impl/FFAVCodecContextImpl.hpp"

#include "error/ffav.hpp"

extern "C" {
#include "libavutil/opt.h"
}

namespace ff {
    FFAVCodecContextPtr FFAVCodecContext::create() {
        return std::make_shared<FFAVCodecContext>();
    }

    FFAVCodecContext::FFAVCodecContext() {
        this->codecContextImpl = FFAVCodecContextImpl::create();

        this->isCodecOpenFlag = false;
    }

    FFAVCodecContext::~FFAVCodecContext() {}

    FFAVCodecContextImplPtr FFAVCodecContext::getImpl() {
        return this->codecContextImpl;
    }

    void FFAVCodecContext::setPixFmt(int pixFmt) {
        this->pixFmt = pixFmt;
    }

    int FFAVCodecContext::getPixFmt() {
        return this->pixFmt;
    }

    FFAVCodecContextHWFormatPtr FFAVCodecContext::getHWFormat() {
        return this->hwFormat;
    }

    bool FFAVCodecContext::findHWFormat() {
        return this->hwFormat->findHWFormat(*this);
    }

    std::string FFAVCodecContext::getCodecName() {
        return this->codecName;
    }

    void FFAVCodecContext::setCodecName(const std::string& codecName) {
        this->codecName = codecName;
    }

    void FFAVCodecContext::setHWFormat(FFAVCodecContextHWFormatPtr hwFormat) {
        this->hwFormat = hwFormat;
    }

    bool FFAVCodecContext::isHWCodec() {
        if (this->hwFormat->getEncodeHWType() == ENCODE_HW_TYPE::CPU) {
            return false;
        }
        return true;
    }

    bool FFAVCodecContext::isCodecOpen() {
		return this->isCodecOpenFlag;
	}

    AVError FFAVCodecContext::setOpt(const std::string& key, const std::string& value) {
        AVCodecContext* codecContext = this->codecContextImpl->getRaw();
        int ret = av_opt_set(codecContext->priv_data, key.c_str(), value.c_str(), 0);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_opt_set failed", ret, "av_opt_set");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    AVError FFAVCodecContext::openCodec() {
        const AVCodec* avCodec = avcodec_find_encoder_by_name(this->codecName.c_str());
        if (avCodec == nullptr) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_find_encoder_by_name failed", -1, "avcodec_find_encoder_by_name");
        }

        int ret = avcodec_open2(this->codecContextImpl->getRaw(), avCodec, nullptr);
        if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avcodec_open2 failed", ret, "avcodec_open2");
        }

        this->isCodecOpenFlag = true;

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }
};
