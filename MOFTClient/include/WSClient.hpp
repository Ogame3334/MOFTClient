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

		bool open(const StringView host, const StringView endpoint);
		bool open(const URLView url);

		void onOpen(const std::function<void()>& callback);
		void onClose(const std::function<void()>& callback);
		void onError(const std::function<void()>& callback);
		void onMessage(const std::function<void(const StringView)>& callback);

		[[nodiscard]] bool isOpen() const;

		void close();

		bool send(const StringView message);

		[[nodiscard]] String host() const;

		[[nodiscard]] bool hasError() const;
	private:
		class WSClientDetail;

		std::shared_ptr<WSClientDetail> pImpl;
	};
}
