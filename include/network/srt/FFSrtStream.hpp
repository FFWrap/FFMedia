#pragma once

#include <vector>
#include <memory>


namespace ff {
	class FFSrtStream;
	using FFSrtStreamPtr = std::shared_ptr<FFSrtStream>;

	class FFSrtStream : public std::vector<char> {
	public:
		explicit FFSrtStream(char* data, int len);
		virtual ~FFSrtStream();

	public:
		static FFSrtStreamPtr create(char* data, int len) {
			FFSrtStreamPtr stream = std::make_shared<FFSrtStream>(data, len);
			return stream;
		}
	};
};