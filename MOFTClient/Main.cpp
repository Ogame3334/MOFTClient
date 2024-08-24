#include <Siv3D.hpp>
#include "include/WSClient.hpp"
#include <boost/beast/version.hpp>

void Main()
{
	ogm::WSClient wsClient;


	wsClient.onMessage([&](const StringView msg)
		{
			Print << msg;
		}
	);

	wsClient.onOpen([]() { Print(U"👍 connected!"); });
	wsClient.onClose([]() { Print(U"👼 disconnected"); });
	wsClient.onError([]() { Print(U"🙅‍♀ ERROR"); });

	wsClient.open(U"ws://localhost:8081");

	while (System::Update()) {
		if (SimpleGUI::Button(U"Send", Vec2(600, 20)))
		{
			//wsClient.send(U"いろはにほへと");
		}
	}
}
