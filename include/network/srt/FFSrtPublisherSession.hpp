#pragma once

#include "network/srt/FFSrtSocketBase.hpp"

#include <memory>

namespace ff {
	class FFSrtPublisherSession;
	using FFSrtPublisherSessionPtr = std::shared_ptr<FFSrtPublisherSession>;
	class FFSrtStreamChannel;
	using FFSrtStreamChannelPtr = std::shared_ptr< FFSrtStreamChannel>;


	class FFSrtPublisherSession : public FFSrtSocketBase, std::enable_shared_from_this<FFSrtPublisherSession> {
	public:
		explicit FFSrtPublisherSession(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel);
		virtual ~FFSrtPublisherSession();

	public:
		static FFSrtPublisherSessionPtr create(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel);

	protected:
		virtual void onStart() override;
		virtual void onLoop() override;
		virtual void onStop() override;

	private:
		std::string streamId;
		std::weak_ptr<FFSrtStreamChannel> srtStreamChannel;
	};
};