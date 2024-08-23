#include <Siv3D.hpp>
#include "include/WSClient.hpp"
#include <boost/asio.hpp>


boost::asio::io_context::count_type hoge(int32 n)
{
	System::Sleep(n * 1s);

	return 1;
}

void Main() {
	//ogm::WSClient wsClient;

	//wsClient.connect(U"localhost", U"hogehoge");
	//wsClient.connect(U"api-realtime-sandbox.p2pquake.net", U"");

	AsyncTask<boost::asio::io_context::count_type> task;
	task = Async(hoge, 5);

	while (System::Update()) {
		if (task.isReady()) {
			Print << U"done";
		}
	}
}
