#pragma once

#include "../include/WSClient.hpp"
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

namespace ogm
{
	enum class NetworkStatus
	{
		NETWORK_OK,
		NETWORK_ERROR,
	};

	using client = websocketpp::client<websocketpp::config::asio_client>;
	using message_ptr = websocketpp::config::asio_client::message_type::ptr;

	class WSClient::WSClientDetail
	{
	public:
		WSClientDetail();
		~WSClientDetail();

		void onOpen(const std::function<void()>& callback);
		void onClose(const std::function<void()>& callback);
		void onError(const std::function<void()>& callback);
		void onMessage(const std::function<void(const StringView)>& callback);

		bool open(const StringView uri);

		[[nodiscard]]
		bool isOpen() const;

		void close();

		bool send(const StringView payload);

		[[nodiscard]]
		String uri() const;

		[[nodiscard]]
		bool hasError() const;

		enum class Status
		{
			NETWORK_OK,
			NETWORK_ERROR,
		};
	private:
		std::future<void> m_IOServiceThred;
		std::shared_ptr<client> m_client;

		String m_uri;
		client::connection_ptr m_connectionHdl;

		void _onOpen(websocketpp::connection_hdl hdl);
		void _onClose(websocketpp::connection_hdl hdl);
		void _onError(websocketpp::connection_hdl hdl);
		void _onMessage(websocketpp::connection_hdl hdl, message_ptr msg);

		std::function<void()> m_callbackOpen;
		std::function<void()> m_callbackClose;
		std::function<void()> m_callbackError;
		std::function<void(const StringView)> m_callbackMessage;

		bool m_isOpen = false;
		bool m_waitingConnection = false;
		Status m_status = Status::NETWORK_OK;
	};
}
