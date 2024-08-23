#include "WSClientDetail.hpp"
#include <boost/asio/strand.hpp>
#include <cstdlib>

namespace ogm
{
	net::io_context::count_type RunIOContext(net::io_context& ioc)
	{
		return ioc.run();
	}

	WSClient::WSClientDetail::WSClientDetail()
		: m_resolver{net::make_strand(m_ioc)}, m_ws(net::make_strand(m_ioc)){}

	WSClient::WSClientDetail::~WSClientDetail(){}

	//bool WSClient::WSClientDetail::connect(const IPv4Address& ip, uint16 port)
	//{
	//	return true;
	//}

	void WSClient::WSClientDetail::connect(const URLView url, const StringView text)
	{
		m_host = url.toUTF8();
		m_text = text.toUTF8();

		m_resolver.async_resolve(
			url.toUTF8(),
			"8081",
			beast::bind_front_handler(
				&WSClientDetail::onResolve,
				this
			)
		);

		//task = Async(RunIOContext, m_ioc);
		//m_ioc.run();
		
	}

	void WSClient::WSClientDetail::fail(beast::error_code ec, char const* what)
	{
		//std::cerr << what << ": " << ec.message() << std::endl;
		Console << Unicode::FromUTF8(what) << U": " << Unicode::Widen(ec.message());
	}

	void WSClient::WSClientDetail::onResolve(beast::error_code ec, tcp::resolver::results_type results)
	{
		if (ec) return this->fail(ec, "resolve");

		beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(30));

		beast::get_lowest_layer(m_ws).async_connect(
			results,
			beast::bind_front_handler(
				&WSClientDetail::onConnect,
				this
			)
		);
	}

	void WSClient::WSClientDetail::onConnect(beast::error_code ec, tcp::resolver::endpoint_type ep)
	{
		if (ec)	return this->fail(ec, "connect");

		beast::get_lowest_layer(m_ws).expires_never();

		m_ws.set_option(websocket::stream_base::timeout::suggested(
			beast::role_type::client
		));

		m_ws.set_option(
			websocket::stream_base::decorator(
				[](websocket::request_type& req)
				{
					req.set(http::field::user_agent,
					std::string{ BOOST_BEAST_VERSION_STRING }
						+ " websocket-client-async");
				}
			)
		);

		m_host += ':' + std::to_string(ep.port());

		m_ws.async_handshake(m_host, "/v2/ws",
			beast::bind_front_handler(
				&WSClientDetail::onHandShake,
				this
			)
		);
	}
	void WSClient::WSClientDetail::onHandShake(beast::error_code ec)
	{
		if (ec) return this->fail(ec, "handshake");

		m_ws.async_write(
			net::buffer(m_text),
			beast::bind_front_handler(
				&WSClientDetail::onWrite,
				this
			)
		);
 	}

	void WSClient::WSClientDetail::onWrite(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if (ec) return this->fail(ec, "write");

		m_ws.async_read(
			m_buffer,
			beast::bind_front_handler(
				&WSClientDetail::onRead,
				this
			)
		);
	}

	void WSClient::WSClientDetail::onRead(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if (ec) return this->fail(ec, "read");

		m_ws.async_write(
			net::buffer(m_text),
			beast::bind_front_handler(
				&WSClientDetail::onWrite,
				this
			)
		);
		//m_ws.async_close(
		//	websocket::close_code::normal,
		//	beast::bind_front_handler(
		//		&WSClientDetail::onClose,
		//		this
		//	)
		//);
	}

	void WSClient::WSClientDetail::onClose(beast::error_code ec)
	{
		if (ec) return this->fail(ec, "close");

		//std::cout << beast::make_printable(m_buffer.data()) << std::endl;
		Print << U"Connection Closed";
	}
}
