#include <SDKDDKVer.h>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

#include "Command.hpp"

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

	boost::iostreams::file_descriptor pfdes(pipe, boost::iostreams::close_handle);
	boost::iostreams::stream_buffer<boost::iostreams::file_descriptor> psb(pfdes);
	std::iostream pios(&psb);

	for (;;) {
		try {
			boost::archive::text_iarchive input(pios);
			Command com;
			input >> com;
			switch (com.cmd) {
			case COMMAND_TYPE_TRANSFER:
				cout << com.transferArg << endl;
				break;
			case COMMAND_TYPE_CALL:
				break;
			}
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
