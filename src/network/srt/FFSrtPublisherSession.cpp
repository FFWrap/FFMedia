#include "network/srt/FFSrtPublisherSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
	FFSrtPublisherSession::FFSrtPublisherSession(SRTSOCKET srtSocket, std::string& streamId, std::shared_ptr<FFSrtStreamChannel> srtStreamChannel)
	: FFSrtSocketBase(srtSocket)
	, streamId(streamId)
	, srtStreamChannel(srtStreamChannel)
	{

	}

	FFSrtPublisherSession::~FFSrtPublisherSession() {

	}

	void FFSrtPublisherSession::onStart() {

	}
	
	void FFSrtPublisherSession::onLoop() {

	}

	void FFSrtPublisherSession::onStop() {

	}
}