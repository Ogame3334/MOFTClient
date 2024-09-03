//#include "../../include/WinNT.hpp"

#include "WSClientDetail.hpp"
#include <boost/asio/strand.hpp>
#include <openssl/err.h>

namespace ogm
{
	WSClient::WSClientDetail::WSClientDetail() :
		m_ioc(),
		m_sslc(ssl::context::tlsv12_client),
		m_resolver(net::make_strand(m_ioc)),
		m_ws(net::make_strand(m_ioc), m_sslc)
	{}

	WSClient::WSClientDetail::~WSClientDetail()
	{
		
	}

	bool WSClient::WSClientDetail::open(const StringView host, const StringView endpoint)
	{
		m_host = host;
		m_endpoint = endpoint;

		m_resolver.async_resolve(
			m_host.toUTF8(),
			"8081",
			beast::bind_front_handler(
				&WSClientDetail::on_resolve,
				this
			)
		);

		Console << U"opened";

		m_task = Async([&] {m_ioc.run(); });

		return true;
	}

	void WSClient::WSClientDetail::onOpen(const std::function<void()>& callback)
	{
		m_callbackOpen = callback;
	}
	void WSClient::WSClientDetail::onClose(const std::function<void()>& callback)
	{
		m_callbackClose = callback;
	}
	void WSClient::WSClientDetail::onError(const std::function<void()>& callback)
	{
		m_callbackError = callback;
	}
	void WSClient::WSClientDetail::onMessage(const std::function<void(const StringView)>& callback)
	{
		m_callbackMessage = callback;
	}

	[[nodiscard]]
	bool WSClient::WSClientDetail::isOpen() const
	{
		return true;
	}

	void WSClient::WSClientDetail::close()
	{
		m_ws.async_close(
			websocket::close_code::normal,
			beast::bind_front_handler(
				&WSClientDetail::on_close,
				this
			)
		);
	}

	bool WSClient::WSClientDetail::send(const StringView message)
	{
		m_ws.async_write(
			net::buffer(message.toUTF8()),
			beast::bind_front_handler(
				&WSClientDetail::on_write,
				this
			)
		);
		return true;
	}

	[[nodiscard]]
	String WSClient::WSClientDetail::host() const
	{
		return U"hoge";
	}

	[[nodiscard]]
	bool WSClient::WSClientDetail::hasError() const
	{
		return true;
	}




	void WSClient::WSClientDetail::fail(beast::error_code ec, std::string what)
	{
		//Console << U"{}: {}"_fmt(what, ec.message());
		Console << Unicode::Widen(what) << U": " << Unicode::Widen(ec.message());
	}

	void WSClient::WSClientDetail::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
	{
		if (ec) return this->fail(ec, "resolve");

		beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(30));

		beast::get_lowest_layer(m_ws).async_connect(
			results,
			beast::bind_front_handler(
				&WSClientDetail::on_connect,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
	{
		if (ec) return this->fail(ec, "connect");

		beast::get_lowest_layer(m_ws).expires_after(std::chrono::seconds(30));

		if (not SSL_set_tlsext_host_name(
			m_ws.next_layer().native_handle(),
			m_host.c_str()
		))
		{
			ec = beast::error_code(
				static_cast<int>(::ERR_get_error()),
				net::error::get_ssl_category()
			);

			return fail(ec, "connect");
		}

		m_host += U": {}"_fmt(ep.port());

		m_ws.next_layer().async_handshake(
			ssl::stream_base::client,
			beast::bind_front_handler(
				&WSClientDetail::on_ssl_handshake,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_ssl_handshake(beast::error_code ec)
	{
		if (ec) return fail(ec, "ssl_handshake");

		beast::get_lowest_layer(m_ws).expires_never();

		m_ws.set_option(
			websocket::stream_base::timeout::suggested(
				beast::role_type::client));

		m_ws.set_option(websocket::stream_base::decorator(
			[](websocket::request_type& req)
			{
				req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
				" websocket-client-async-ssl");
			}));
		m_ws.async_handshake(
			m_host.toUTF8(),
			m_endpoint.toUTF8(),
			beast::bind_front_handler(
				&WSClientDetail::on_handshake,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_handshake(beast::error_code ec)
	{
		if (ec) return fail(ec, "handshake");

		m_ws.async_read(
			m_buffer,
			beast::bind_front_handler(
				&WSClientDetail::on_read,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_write(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if (ec) return fail(ec, "fail");

		m_ws.async_read(
			m_buffer,
			beast::bind_front_handler(
				&WSClientDetail::on_read,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_read(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);


		if(ec) return fail(ec, "read");

		std::ostringstream oss;
		oss << beast::make_printable(m_buffer.data());
		Print << Unicode::Widen(oss.str());
		m_buffer.clear();

		//m_ws.async_close(websocket::close_code::normal,
		//	beast::bind_front_handler(
		//		&WSClientDetail::on_close,
		//		this));
		m_ws.async_read(
			m_buffer,
			beast::bind_front_handler(
				&WSClientDetail::on_read,
				this
			)
		);
	}

	void WSClient::WSClientDetail::on_close(beast::error_code ec)
	{
		if (ec) return fail(ec, "close");

		//Console << U"buffer: {}"_fmt(m_buffer.data());
		Console << U"Connection closed";
	}


}
