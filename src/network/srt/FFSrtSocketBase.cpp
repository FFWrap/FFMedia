#include "network/srt/FFSrtSocketBase.hpp"

#include <stop_token>

namespace ff {
	FFSrtSocketBase::FFSrtSocketBase(SRTSOCKET srtSocket)
	: srtSocket(srtSocket) 
	{

	}

	FFSrtSocketBase::~FFSrtSocketBase() {
		this->stop();
	}

	void FFSrtSocketBase::start() {
		this->thread = std::make_shared<std::jthread>([=](std::stop_token stopToken) {
			this->onStart();
			this->run(stopToken);
			this->onStop();
		});
	}

	void FFSrtSocketBase::stop() {
		if (this->thread == nullptr) {
			return;
		}

		this->thread->request_stop();
		this->thread = nullptr;
	}

	void FFSrtSocketBase::onStart() {

	}

	void FFSrtSocketBase::onStop() {

	}

	void FFSrtSocketBase::run(std::stop_token stopToken) {
		if (stopToken.stop_requested() == false) {
			this->onLoop();
		}
	}
};