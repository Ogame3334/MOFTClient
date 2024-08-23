#pragma once

#include <Siv3D.hpp>

namespace ogm
{
	class WSClient
	{
	public:
		SIV3D_NODISCARD_CXX20
		WSClient();

		~WSClient();

		//bool connect(const IPv4Address& ip, uint16 port);
		void connect(const URLView url, const StringView text);
	private:
		class WSClientDetail;

		std::shared_ptr<WSClientDetail> pImpl;
	};
}
