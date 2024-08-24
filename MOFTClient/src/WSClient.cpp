#include "../include/WSClient.hpp"
#include "WSClientDetail.hpp"

namespace ogm
{
	WSClient::WSClient()
		: pImpl(std::make_shared<WSClientDetail>()) {}

	WSClient::~WSClient() {}

	void WSClient::onOpen(const std::function<void()>& callback) {
		pImpl->onOpen(callback);
	}
	void WSClient::onClose(const std::function<void()>& callback){
		pImpl->onClose(callback);
	}
	void WSClient::onError(const std::function<void()>& callback){
		pImpl->onError(callback);
	}
	void WSClient::onMessage(const std::function<void(const StringView)>& callback){
		pImpl->onMessage(callback);
	}

	bool WSClient::open(const StringView uri){
		return pImpl->open(uri);
	}

	[[nodiscard]]
	bool WSClient::isOpen() const{
		return pImpl->isOpen();
	}

	void WSClient::close(){
		pImpl->close();
	}

	bool WSClient::send(const StringView payload){
		return pImpl->send(payload);
	}

	[[nodiscard]]
	String WSClient::uri() const{
		return pImpl->uri();
	}

	[[nodiscard]]
	bool WSClient::hasError() const{
		return pImpl->hasError();
	}
}
