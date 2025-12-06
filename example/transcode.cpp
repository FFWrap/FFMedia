#include "FFMedia.hpp"

#include <iostream>

#define ERROR_CHECK(msg, averror) \
    if (averror.getType() != ff::AV_ERROR_TYPE::SUCCESS) { \
        std::cout << msg << ": " << averror.getAllErrorMessage() << std::endl; \
        return -1; \
    }

int main(void) {
    ff::FFAVInputContext inputContext;
    ff::AVError error = inputContext.open("sample.mp4");  // GPU Decode
	ERROR_CHECK("Failed to open input file", error);

    auto videoStreams = inputContext.getVideoDecodeStreamList();
    auto audioStreams = inputContext.getAudioDecodeStreamList();
	if (videoStreams->empty() || audioStreams->empty()) {
        std::cout << "No video or audio streams found in the input file." << std::endl;
        return -1;
    }

    auto videoStream = videoStreams->at(0);
    auto audioStream = audioStreams->at(0);

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("transcode.ts");
	ERROR_CHECK("Failed to open output file", error);

    
    /* if (outputFileName.find(".m3u8") != std::string::npos) {
        outputContext.setOpt("hls_time", "3");
        outputContext.setOpt("hls_list_size", "0");
        outputContext.setOpt("hls_flags", "delete_segments+append_list");
    } */

    auto encodeVideoStream = outputContext.addStream(ff::VIDEO_CODEC::H264, videoStream);   // GPU Encode
    auto encodeAudioStream = outputContext.addStream(ff::AUDIO_CODEC::AAC, audioStream);
	if (encodeVideoStream == nullptr || encodeAudioStream == nullptr) {
        std::cout << "Failed to create encode streams." << std::endl;
        return -1;
    }

    // Configure Video Stream
    encodeVideoStream->setBitrate(9999);

    ERROR_CHECK("Failed to open video codec.", encodeVideoStream->openCodec());
    ERROR_CHECK("Failed to open audio codec.", encodeAudioStream->openCodec());

    error = outputContext.writeHeader();
    ERROR_CHECK("Failed write header.", error);

    ff::FFAVPacket packet;
    ff::FFAVPacketListPtr encodedPacketList;
    try {
        for (auto& packet : inputContext) {
            // Decode
            if (videoStream->getStreamIndex() == packet.getStreamIndex()) {  // Video Stream Index
                auto videoFrameList = videoStream->decode(packet, &error);
                ERROR_CHECK("[VIDEO] Decoding error", error);

                encodedPacketList = encodeVideoStream->encode(videoFrameList, &error);
                ERROR_CHECK("[VIDEO] Encoding error", error);
            } else if (audioStream->getStreamIndex() == packet.getStreamIndex()) {  // Audio Stream Index
                auto audioFrameList = audioStream->decode(packet, &error);
                ERROR_CHECK("[AUDIO] Decoding error", error);

                encodedPacketList = encodeAudioStream->encode(audioFrameList, &error);
                ERROR_CHECK("[AUDIO] Encoding error", error);
            }

            error = outputContext.writePacket(encodedPacketList);
            ERROR_CHECK("Failed output packet", error);
        }
    }
    catch (ff::AVDemuxException& e) {
        ff::AVError error = e.getAVError();
        std::cout << "libav error message: " << error.getAVErrorMessage() << std::endl;
        std::cout << "error message: " << error.getMessage() << std::endl;
    }

    auto videoFrameList = encodeVideoStream->flush(&error);
    ERROR_CHECK("[VIDEO] Failed flush", error);
    auto audioFrameList = encodeAudioStream->flush(&error);
    ERROR_CHECK("[AUDIO] Failed flush", error);

    error = outputContext.writePacket(videoFrameList);
    ERROR_CHECK("[VIDEO] Failed output packet (FLUSH)", error);
    error = outputContext.writePacket(audioFrameList);
    ERROR_CHECK("[AUDIO] Failed output packet (FLUSH)", error);

    outputContext.close();

	return 0;
}