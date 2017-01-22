#include "./ExampleSelectorState.h"
#include <iostream>

#ifdef OPIFEX_OPTION_ASIO
#include "asio.hpp"

using asio::ip::tcp;
#endif

void ExampleAsioEnter(OPgameState* last) {
}

void ExampleAsioEnter2(OPgameState* last) {

}

OPint ExampleAsioUpdate(OPtimer* timer) {

#ifdef OPIFEX_OPTION_ASIO
	if (OPkeyboardWasPressed(OPkeyboardKey::S)) {
		try {
			asio::io_service io_service;
			asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1337));

			asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1337);
			for (;;) {
				asio::ip::tcp::socket socket(io_service);
				acceptor.accept(socket, endpoint);

				std::string message = "RAWR";

				asio::error_code ignored_error;
				asio::write(socket, asio::buffer(message), ignored_error);
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::C)) {
		try {
			asio::io_service io_service;
			asio::ip::tcp::resolver resolver(io_service);
			asio::ip::tcp::resolver::query query("localhost", "1337");
			asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			asio::ip::tcp::socket socket(io_service);
			asio::connect(socket, endpoint_iterator);

			char buffer[1024];

			for (;;) {
				asio::error_code error;
				size_t len = socket.read_some(asio::buffer(buffer), error);
				if (error == asio::error::eof) {
					break;
				}
				else if (error) {
					OPlog("Error %d", error);
				}

				std::cout.write(buffer, len);
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
#endif
	OPrenderClear(0, 0, 0);
	OPrenderPresent();

	return 0;
}


void ExampleAsioRender(OPfloat delta) {

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleAsioExit(OPgameState* next) {

	return 0;
}

#ifdef OPIFEX_OPTION_ASIO
OPint GS_EXAMPLE_ASIO_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_ASIO_AVAILABLE = 0;
#endif

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_ASIO = NULL;