#include <Siv3D.hpp>
#include "include/WSClient.hpp"

void Main() {
	ogm::WSClient client;
	client.open(U"localhost", U"/");

	while (System::Update())
	{
		if (SimpleGUI::Button(U"send", Vec2{ 100, 100 })) {
			client.send(U"hogehoge");
		}
		if (SimpleGUI::Button(U"close", Vec2{ 100, 300 })) {
			client.close();
		}
	}
}
