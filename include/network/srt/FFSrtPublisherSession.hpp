#pragma once

#include "network/srt/FFSrtSocketBase.hpp"

#include <memory>

namespace ff {
	class FFSrtStreamChannel;

	class FFSrtPublisherSession : public FFSrtSocketBase, std::enable_shared_from_this<FFSrtPublisherSession> {
	public:
		explicit FFSrtPublisherSession(SRTSOCKET srtSocket, std::string& streamId, std::shared_ptr<FFSrtStreamChannel> srtStreamChannel);
		virtual ~FFSrtPublisherSession();

	protected:
		virtual void onStart() override;
		virtual void onLoop() override;
		virtual void onStop() override;

	private:
		std::string streamId;
		std::weak_ptr<FFSrtStreamChannel> srtStreamChannel;
	};
};