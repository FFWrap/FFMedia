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

    auto streams = inputContext.getDecodeStreamList();

    ff::FFAVOutputContext outputContext;
    error = outputContext.open("transmux.ts");
	ERROR_CHECK("Failed to open output file", error);

    outputContext.copyStreams(streams);
    error = outputContext.writeHeader();
	ERROR_CHECK("Failed to write header", error);

    ff::FFAVPacket packet;
    try {
        for (auto& packet : inputContext) {
            error = outputContext.writePacket(packet);
			ERROR_CHECK("Failed to write packet", error);
        }
    }
    catch (ff::AVDemuxException& e) {
        ff::AVError error = e.getAVError();
        std::cout << "all error message: " << error.getAllErrorMessage() << std::endl;
    }


    outputContext.close();
    inputContext.close();
}