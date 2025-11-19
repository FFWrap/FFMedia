#include "network/srt/FFSrtStream.hpp"

namespace ff {
	FFSrtStream::FFSrtStream(char* data, int len) {
		this->insert(this->begin(), data, data + len);
	}

	FFSrtStream::~FFSrtStream() {

	}
}