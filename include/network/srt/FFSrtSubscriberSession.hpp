#pragma once

#include "network/srt/FFSrtSocketBase.hpp"

#include <memory>
#include <vector>

namespace ff {
	class FFSrtStreamChannel;

	class FFSrtSubscriberSession : public FFSrtSocketBase, std::enable_shared_from_this<FFSrtSubscriberSession> {
	public:
		explicit FFSrtSubscriberSession(SRTSOCKET srtSocket, std::string& streamId, std::shared_ptr<FFSrtStreamChannel> srtStreamChannel);
		virtual ~FFSrtSubscriberSession();

	public:
		void enqueuePacket(std::vector<char>& srtStream);

	protected:
		virtual void onStart() override;
		virtual void onLoop() override;
		virtual void onStop() override;

	private:
		std::string streamId;
		std::weak_ptr<FFSrtStreamChannel> srtStreamChannel;
	};
}