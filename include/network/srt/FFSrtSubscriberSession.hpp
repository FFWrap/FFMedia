#pragma once

#include "network/srt/FFSrtSocketBase.hpp"
#include "network/srt/FFSrtStream.hpp"

#include <memory>
#include <vector>

namespace ff {
	class FFSrtSubscriberSession;
	using FFSrtSubscriberSessionPtr = std::shared_ptr<FFSrtSubscriberSession>;
	class FFSrtStreamChannel;
	using FFSrtStreamChannelPtr = std::shared_ptr<FFSrtStreamChannel>;

	class FFSrtSubscriberSession : public FFSrtSocketBase, std::enable_shared_from_this<FFSrtSubscriberSession> {
	public:
		explicit FFSrtSubscriberSession(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel);
		virtual ~FFSrtSubscriberSession();

	public:
		static FFSrtSubscriberSessionPtr create(SRTSOCKET, FFSrtStreamChannelPtr srtStreamChannel);

	public:
		void enqueuePacket(FFSrtStreamPtr srtStream);

	protected:
		virtual void onStart() override;
		virtual void onLoop() override;
		virtual void onStop() override;

	private:
		std::string streamId;
		std::weak_ptr<FFSrtStreamChannel> srtStreamChannel;
	};
}