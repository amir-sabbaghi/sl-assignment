#include <SDKDDKVer.h>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

int main() {
	LPCSTR pipeName = "\\\\.\\pipe\\sl-test";
	ofstream s(pipeName);
	boost::archive::text_oarchive a(s);
	a & "Hi\n";
	return 0;
}
