#include "Command.hpp"
#include "Pipe.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void usage() {
	cout << "send VALUE         send VALUE to server" << endl;
	cout << "l                  list the list of stored values on the server" << endl;
	cout << "call METHOD        call method on the last object" << endl;
	cout << "q                  quit" << endl;
}

int main() {
	LPCSTR pipeName = "\\\\.\\pipe\\sl-test";
	HANDLE pipe = CreateFileA(pipeName,
		GENERIC_READ | GENERIC_WRITE,
		0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (!pipe) {
		cerr << "Could not connect to the server" << endl;
		return 1;
	}

	for (;;) {
		cout << "> " << std::flush;
		string cmd;
		cin >> cmd;
		Command com;
		if (cmd == "send") {
			com.cmd = COMMAND_TYPE_TRANSFER;
			cin >> com.transferArg;
			send(pipe, com);
		} else if (cmd == "l") {
			com.cmd = COMMAND_TYPE_LIST;
			send(pipe, com);
			vector<int> list;
			recv(pipe, list);
			for (auto i: list)
				cout << i << endl;
		} else if (cmd == "q") {
			return 0;
		} else {
			usage();
		}
	}
	return 0;
}
