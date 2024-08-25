#pragma once

#include "../../include/WSClient.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace ogm
{
	namespace beast = boost::beast;
	namespace http = boost::beast::http;
	namespace websocket = boost::beast::websocket;
	namespace net = boost::asio;]
	namespace ssl = boost::asio::ssl;
	using tcp = boost::asio::ip::tcp;

	class WSClient::WSClientDetail
	{
	public:
		WSClientDetail();

		~WSClientDetail();

		bool open(const StringView host, const StringView endpoint);

		void onOpen(const std::function<void()>& callback);
		void onClose(const std::function<void()>& callback);
		void onError(const std::function<void()>& callback);
		void onMessage(const std::function<void(const StringView)>& callback);

		[[nodiscard]]
		bool isOpen() const;

		void close();

		bool send(const StringView message);

		[[nodiscard]]
		String host() const;

		[[nodiscard]]
		bool hasError() const;

		void fail(beast::error_code ec, std::string what);

		void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
		void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
		void on_handshake(beast::error_code ec);
		void on_write(beast::error_code ec, std::size_t bytes_transferred);
		void on_read(beast::error_code ec, std::size_t bytestransferred);
		void on_close(beast::error_code ec);
	private:
		net::io_context m_ioc;
		AsyncTask<void> m_task;

		tcp::resolver m_resolver;
		websocket::stream<beast::tcp_stream> m_ws;
		beast::flat_buffer m_buffer;
		String m_host;
		String m_endpoint;
		String m_text;

		std::function<void()> m_callbackOpen;
		std::function<void()> m_callbackClose;
		std::function<void()> m_callbackError;
		std::function<void(const StringView)> m_callbackMessage;

	};
}
