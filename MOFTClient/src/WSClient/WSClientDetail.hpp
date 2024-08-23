#pragma once

#include "../../include/WSClient.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace ogm
{
	class WSClient::WSClientDetail
	{
	public:
		WSClientDetail();
		~WSClientDetail();

		//bool connect(const IPv4Address& ip, uint16 port);
		void connect(const URLView url, const StringView text);
	private:
		void fail(beast::error_code ec, char const* what);
		void onResolve(beast::error_code ec, tcp::resolver::results_type results);
		void onConnect(beast::error_code ec, tcp::resolver::endpoint_type ep);
		void onHandShake(beast::error_code ec);
		void onWrite(beast::error_code ec, std::size_t bytes_transferred);
		void onRead(beast::error_code ec, std::size_t bytes_transferred);
		void onClose(beast::error_code ec);

		net::io_context m_ioc;
		tcp::resolver m_resolver;
		websocket::stream<beast::tcp_stream> m_ws;
		beast::flat_buffer m_buffer;
		std::string m_host;
		std::string m_text;
		AsyncTask<net::io_context::count_type> task;
	};
}
