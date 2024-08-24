#include "WSClientDetail.hpp"

namespace ogm
{
	WSClient::WSClientDetail::WSClientDetail() {}

	WSClient::WSClientDetail::~WSClientDetail()
	{
		this->close();
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

	bool WSClient::WSClientDetail::open(const StringView uri)
	{
		m_uri = uri;

		if (m_isOpen or m_waitingConnection)
		{
			return false;
		}

		m_client = std::make_shared<client>();

		m_client->set_open_handler(std::bind(&WSClientDetail::_onOpen, this, std::placeholders::_1));
		m_client->set_close_handler(std::bind(&WSClientDetail::_onClose, this, std::placeholders::_1));
		m_client->set_fail_handler(std::bind(&WSClientDetail::_onError, this, std::placeholders::_1));
		m_client->set_message_handler(std::bind(&WSClientDetail::_onMessage, this, std::placeholders::_1, std::placeholders::_2));

		websocketpp::lib::error_code error;
		m_connectionHdl = m_client->get_connection(uri.toUTF8(), error);

		

		return true;
	}

	[[nodiscard]]
	bool WSClient::WSClientDetail::isOpen() const
	{
		return m_isOpen;
	}

	void WSClient::WSClientDetail::close()
	{
		if (m_client)
		{
			if (m_client->is_listening())
			{
				m_client->close(m_connectionHdl, websocketpp::close::status::normal, "close()");
			}

			m_waitingConnection = false;
			m_status = Status::NETWORK_OK;
			m_isOpen = false;

			m_client->stop_perpetual();
			m_client->stop();
			m_IOServiceThred.wait();
			m_client->reset();

			m_client.reset();

			Console << U"WebSocketClient: closed";
		}
	}

	bool WSClient::WSClientDetail::send(const StringView payload)
	{
		if (!m_isOpen)
		{
			return false;
		}

		m_client->send(m_connectionHdl, payload.toUTF8(), websocketpp::frame::opcode::TEXT);

		return true;
	}

	[[nodiscard]]
	String WSClient::WSClientDetail::uri() const
	{
		return m_uri;
	}

	[[nodiscard]]
	bool WSClient::WSClientDetail::hasError() const
	{
		return m_status != Status::NETWORK_ERROR;
	}


	void WSClient::WSClientDetail::_onOpen(websocketpp::connection_hdl hdl)
	{
		m_waitingConnection = false;
		m_isOpen = true;

		if (m_callbackOpen)
		{
			m_callbackOpen();
		}
	}
	void WSClient::WSClientDetail::_onClose(websocketpp::connection_hdl hdl)
	{
		if (m_callbackClose)
		{
			m_callbackClose();
		}
	}
	void WSClient::WSClientDetail::_onError(websocketpp::connection_hdl hdl)
	{
		if (m_callbackError)
		{
			m_callbackError();
		}
	}
	void WSClient::WSClientDetail::_onMessage(websocketpp::connection_hdl hdl, message_ptr msg)
	{
		if (m_callbackMessage)
		{
			m_callbackMessage(Unicode::FromUTF8(msg->get_payload()));
		}
	}
}
