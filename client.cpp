#include <SDKDDKVer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#include "Command.hpp"

using namespace std;

void usage() {
	cout << "send VALUE         send VALUE to server" << endl;
	cout << "call METHOD        call method on the last object" << endl;
	cout << "q                  quit" << endl;
}

int main() {
	LPCSTR pipeName = "\\\\.\\pipe\\sl-test";
	fstream pios(pipeName);
	if (!pios.is_open()) {
		cerr << "Could not connect to the server" << endl;
		return 1;
	}

	for (;;) {
		cout << "> " << std::flush;
		string cmd;
		cin >> cmd;
		if (cmd == "send") {
			boost::archive::text_oarchive output(pios);
			Command com;
			com.cmd = COMMAND_TYPE_TRANSFER;
			cin >> com.transferArg;
			output << com;
		} else if (cmd == "q") {
			return 0;
		} else {
			usage();
		}
	}
	return 0;
}
