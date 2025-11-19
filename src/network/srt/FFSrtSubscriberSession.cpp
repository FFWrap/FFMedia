#include "network/srt/FFSrtSubscriberSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
	FFSrtSubscriberSession::FFSrtSubscriberSession(SRTSOCKET srtSocket, std::string& streamId, std::shared_ptr<FFSrtStreamChannel> srtStreamChannel)
	: FFSrtSocketBase(srtSocket)
	, streamId(streamId)
	, srtStreamChannel(srtStreamChannel) 
	{

	}

	FFSrtSubscriberSession::~FFSrtSubscriberSession() {

	}

	void FFSrtSubscriberSession::enqueuePacket(std::vector<char>& srtStream) {

	}

	void FFSrtSubscriberSession::onStart() {

	}

	void FFSrtSubscriberSession::onLoop() {

	}

	void FFSrtSubscriberSession::onStop() {

	}
}