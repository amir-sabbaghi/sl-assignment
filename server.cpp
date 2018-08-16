#include "Command.hpp"
#include "Pipe.hpp"

#include <iostream>

using namespace std;

int main() {
	LPCSTR pipeName = "\\\\.\\pipe\\sl-test";
	HANDLE pipe = CreateNamedPipeA(pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
		PIPE_UNLIMITED_INSTANCES,
		2048, 2048, 0, nullptr);
	if (!pipe) {
		cerr << "Could not create pipe" << endl;
		return 1;
	}
	cout << "Waiting for client to connect..." << endl;
	if (!ConnectNamedPipe(pipe, nullptr) && GetLastError() != ERROR_PIPE_CONNECTED) {
		cerr << "Connection failed" << GetLastError() << endl;
		return 1;
	}

	for (;;) {
		try {
			Command com;
			recv(pipe, com);
			switch (com.cmd) {
			case COMMAND_TYPE_TRANSFER:
				cout << com.transferArg << endl;
				break;
			case COMMAND_TYPE_CALL:
				break;
			}
		}
		catch (IOException &) {
			return 0;
		}
		catch (boost::archive::archive_exception &) {
			return 0;
		}
		catch (...) {
			cerr << "Unknown error occured" << endl;
			return 1;
		}
	}
	return 0;
}
