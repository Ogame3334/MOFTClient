//#include "../../stdafx.h"

#include "../../include/WSClient.hpp"
#include "WSClientDetail.hpp"

namespace ogm
{
	WSClient::WSClient()
		: pImpl(std::make_shared<WSClientDetail>()) {}

	WSClient::~WSClient() {}

	bool WSClient::open(const StringView host, const StringView endpoint)
	{
		return pImpl->open(host, endpoint);
	}

	void WSClient::onOpen(const std::function<void()>& callback)
	{
		pImpl->onOpen(callback);
	}
	void WSClient::onClose(const std::function<void()>& callback)
	{
		pImpl->onClose(callback);
	}
	void WSClient::onError(const std::function<void()>& callback)
	{
		pImpl->onError(callback);
	}
	void WSClient::onMessage(const std::function<void(const StringView)>& callback)
	{
		pImpl->onMessage(callback);
	}

	[[nodiscard]]
	bool WSClient::isOpen() const
	{
		return pImpl->isOpen();
	}

	void WSClient::close()
	{
		pImpl->close();
	}

	bool WSClient::send(const StringView message)
	{
		return pImpl->send(message);
	}

	[[nodiscard]]
	String WSClient::host() const
	{
		return pImpl->host();
	}

	[[nodiscard]]
	bool WSClient::hasError() const
	{
		return pImpl->hasError();
	}
}
