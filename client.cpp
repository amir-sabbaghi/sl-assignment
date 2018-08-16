#include "Client.hpp"
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
	Client c(pipeName);

	for (;;) {
		cout << "> " << std::flush;
		string cmd;
		cin >> cmd;
		if (cmd == "send") {
			int v;
			cin >> v;
			c.send(v);
		} else if (cmd == "l") {
			for (auto i: c.list())
				cout << i << endl;
		} else if (cmd == "q") {
			return 0;
		} else {
			usage();
		}
	}
	return 0;
}

Client::Client(const std::string &pipeName) {
	pipe = CreateFileA(pipeName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (!pipe) {
		cerr << "Could not connect to the server" << endl;
		throw std::exception();
	}
}

Client::~Client() {
	CloseHandle(pipe);
}

void Client::send(int v) {
	lock_guard<mutex> lock(mut);
	Command com;
	com.cmd = COMMAND_TYPE_TRANSFER;
	com.transferArg = v;
	sendPipe(pipe, com);
}

vector<int> Client::list() {
	lock_guard<mutex> lock(mut);
	Command com;
	com.cmd = COMMAND_TYPE_LIST;
	sendPipe(pipe, com);
	vector<int> list;
	recvPipe(pipe, list);
	return list;
}
