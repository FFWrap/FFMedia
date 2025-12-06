#include "FFMedia.hpp"

#include <iostream>

#define ERROR_CHECK(msg, averror) \
    if (averror.getType() != ff::AV_ERROR_TYPE::SUCCESS) { \
        std::cout << msg << ": " << averror.getAllErrorMessage() << std::endl; \
        return -1; \
    }

int main(void) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4", ff::FFAVCodecContextFactory::createHWFormat(ff::ENCODE_HW_TYPE::CUDA));
    ERROR_CHECK("Failed to open input file", error);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();

    if (videoStreams->empty() || audioStreams->empty()) {
        std::cout << "No video or audio streams found in the input file." << std::endl;
        return -1;
    }

    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    ff::FFAVPacket packet;
    try {
        for (auto& packet : inputContext) {
            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) {
                auto videoFrameList = videoStream->decode(packet, &error);
            }
            else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {
                auto audioFrameList = audioStream->decode(packet, &error);
            }

            ERROR_CHECK("Decoding error", error);
        }
    }
    catch (ff::AVDemuxException& e) {
        std::cout << "all error message: " << e.getAVError().getAllErrorMessage() << std::endl;
    }

    return 0;
}