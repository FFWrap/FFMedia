#include "type/FFAVInputContext.hpp"

#include "type/FFAVDecodeStream.hpp"
#include "type/impl/FFAVChannelLayoutImpl.hpp"
#include "type/impl/FFAVCodecParametersImpl.hpp"
#include "type/impl/FFAVFormatContextImpl.hpp"
#include "type/impl/FFAVPacketImpl.hpp"
#include "type/impl/FFAVStreamImpl.hpp"
#include "utils/FFAVDecodeCodecContext.hpp"
#include "error/AVDemuxException.hpp"
#include "type/FFAVCodecContextFactory.hpp"

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
}

#include <memory>

namespace ff {
    FFAVInputContext::FFAVInputContext() {
        this->formatContextImpl = FFAVFormatContextImpl::create();
    }

    FFAVInputContext::~FFAVInputContext() {
        this->close();
    }

    FFAVFormatContextImplPtr FFAVInputContext::getImpl() {
        return this->formatContextImpl;
    }

    AVError FFAVInputContext::open(const std::string& url, FFAVCodecContextHWFormatPtr hwFormat) {
        this->close();

        if (hwFormat == nullptr) {
            hwFormat = FFAVCodecContextFactory::createHWFormat(ENCODE_HW_TYPE::CPU);
		}

        // AVFormatContext alloc
        AVFormatContext* avFormatContext = avformat_alloc_context();
        if (avFormatContext == nullptr) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "avformat_alloc_context failed", -1, "avformat_alloc_context");
        }

        // File open
        int ret = avformat_open_input(&avFormatContext, url.c_str(), nullptr, nullptr);
        if (ret < 0) {
            avformat_free_context(avFormatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " open failed", ret, "avformat_open_input");
        }

        // Stream find
        ret = avformat_find_stream_info(avFormatContext, nullptr);
        if (ret < 0) {
            avformat_close_input(&avFormatContext);
            avformat_free_context(avFormatContext);
            return AVError(AV_ERROR_TYPE::AV_ERROR, url + " find stream info failed", ret, "avformat_find_stream_info");
        }

        this->formatContextImpl->setRaw(avFormatContext);
        this->parseStreamInfo(hwFormat);

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    bool FFAVInputContext::isOpened() {
        return this->formatContextImpl->getRaw() != nullptr;
    }

    void FFAVInputContext::close() {
        this->decodeStreamList = nullptr;

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();

        if (formatContext != nullptr) {
            avformat_close_input(&formatContext);
            avformat_free_context(formatContext);
        }

        this->formatContextImpl->setRaw(nullptr);
    }

    AVError FFAVInputContext::demux(FFAVPacket* packet) {
        if (!this->isOpened()) {
            return AVError(
                AV_ERROR_TYPE::AV_ERROR, "FFAVInputContext is not opened", -1, "FFAVInputContext::readFrame");
        }

        AVFormatContext* formatContext = this->formatContextImpl->getRaw();
        AVPacket* avPacket = packet->getImpl()->getRaw().get();

        // AVFormatContext ���� AVPacket�� �о��.
        int ret = av_read_frame(formatContext, avPacket);
        // ret�� eof �ϰ�� null packet�� ��ȯ
        if (ret == AVERROR_EOF) {
            packet->getImpl()->getRaw() = nullptr;
            return AVError(AV_ERROR_TYPE::AV_EOF);
        } else if (ret < 0) {
            return AVError(AV_ERROR_TYPE::AV_ERROR, "av_read_frame failed", ret, "av_read_frame");
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getDecodeStreamList() {
        return this->decodeStreamList;
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getVideoDecodeStreamList() {
        FFAVDecodeStreamListPtr videoDecodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        for (auto iter : *this->decodeStreamList) {
            if (iter->getType() == DATA_TYPE::VIDEO) {
                videoDecodeStreamList->emplace_back(iter);
            }
        }

        return videoDecodeStreamList;
    }

    FFAVDecodeStreamListPtr FFAVInputContext::getAudioDecodeStreamList() {
        FFAVDecodeStreamListPtr audioDecodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        for (auto iter : *this->decodeStreamList) {
            if (iter->getType() == DATA_TYPE::AUDIO) {
                audioDecodeStreamList->emplace_back(iter);
            }
        }

        return audioDecodeStreamList;
    }

    FFAVInputContextIterator FFAVInputContext::begin() {
        return FFAVInputContextIterator(this);
    }

    FFAVInputContextIterator FFAVInputContext::end() {
        return FFAVInputContextIterator();
    }

    AVError FFAVInputContext::parseStreamInfo(FFAVCodecContextHWFormatPtr hwFormat) {
        this->decodeStreamList = std::make_shared<FFAVDecodeStreamList>();

        AVFormatContext* avFormatContext = this->formatContextImpl->getRaw();

        AVError error;
        int streamIndex = 0;
        for (int i = 0; i < avFormatContext->nb_streams; i++) {
            FFAVDecodeStreamPtr decodeStream = FFAVDecodeStream::create(DATA_TYPE::UNKNOWN);

            decodeStream->getImpl()->setRaw(avFormatContext->streams[i]);
            decodeStream->setStreamIndex(streamIndex++);

            auto codecType = decodeStream->getImpl()->getRaw()->codecpar->codec_type;
            decodeStream->setType(DATA_TYPE_FROM_AV_CODEC_TYPE(codecType));

            decodeStream->setCodecContext(FFAVCodecContextFactory::createContext(decodeStream, hwFormat));

            this->decodeStreamList->emplace_back(decodeStream);
        }

        return AVError(AV_ERROR_TYPE::SUCCESS);
    }

    //////////////////////// Iterator
    FFAVInputContextIterator::FFAVInputContextIterator(FFAVInputContext* context) : context(context) {
        this->eofFlag = false;
        if (this->context && this->context->isOpened()) {
            // �ʱ� ��Ŷ �б� �õ�
            if (this->context->demux(&this->currentPacket).getType() != AV_ERROR_TYPE::SUCCESS) {
                // ������ ���, context�� nullptr�� �����Ͽ� �ݺ����� ���� ��Ÿ��
                this->context = nullptr;
            }
        } else {
            this->context = nullptr;
        }
    }

    FFAVPacket& FFAVInputContextIterator::operator*() {
        return this->currentPacket;
    }

    FFAVPacket& FFAVInputContextIterator::operator->() {
        return this->currentPacket;
    }

    FFAVInputContextIterator& FFAVInputContextIterator::operator++() {
        if (this->context != nullptr) {
            AVError error = this->context->demux(&this->currentPacket);
            if (error.getType() == AV_ERROR_TYPE::AV_EOF) {
                if (eofFlag == false) {
                    eofFlag = true;
                } else {
                    // �� �̻� ��Ŷ�� ���� �� ������ �ݺ��ڸ� ������ ����
                    this->context = nullptr;
                }
            } else if (error.getType() != AV_ERROR_TYPE::SUCCESS) {
                // ���� ��Ŷ�� �д� �� �����ϸ� �ݺ��ڸ� ������ ����
                throw AVDemuxException(error);
            }
        }
        return *this;
    }

    bool operator==(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b) {
        return a.context == b.context;
    }

    bool operator!=(const FFAVInputContextIterator& a, const FFAVInputContextIterator& b) {
        return a.context != b.context;
    }
};
