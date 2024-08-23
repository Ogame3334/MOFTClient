#include "../../include/WSClient.hpp"
#include "WSClientDetail.hpp"

namespace ogm
{
	WSClient::WSClient()
		: pImpl{ std::make_shared<WSClientDetail>() } {}

	WSClient::~WSClient() {}

	//bool WSClient::connect(const IPv4Address& ip, uint16 port)
	//{
	//	return pImpl->connect(ip, port);
	//}
	bool WSClient::connect(const URLView url, const StringView text) {
		return pImpl->connect(url, text);
	}
}
