#pragma once

#include <Siv3D.hpp>

namespace ogm
{
	class WSClient
	{
	public:
		WSClient();
		~WSClient();

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
	private:
		class WSClientDetail;

		std::shared_ptr<WSClientDetail> pImpl;
	};
}
