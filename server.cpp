#include <SDKDDKVer.h>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

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
	boost::archive::text_iarchive input(pios);
	char b[100];
	input >> b;
	cout << b;

	for (;;) {
		try {
			break;
		}
		catch (...) {
			cerr << "Unknown error occured" << endl;
			return 1;
		}
	}
	return 0;
}
