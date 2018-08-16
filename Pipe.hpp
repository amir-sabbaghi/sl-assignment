#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class IOException : public std::exception {
};

template <class T> void sendPipe(HANDLE f, const T &v) {
	std::stringstream ss;
	{
		boost::archive::text_oarchive output(ss);
		output & v;
	}
	const std::string &s = ss.str();
	DWORD len = s.size();
	DWORD written;
	if (!WriteFile(f, &len, sizeof(len), &written, nullptr) || written != sizeof(len))
		throw IOException();
	if (!WriteFile(f, s.c_str(), s.size(), &written, nullptr) || written != s.size())
		throw IOException();
}

template <class T> void recvPipe(HANDLE f, T &v) {
	DWORD len;
	DWORD read;
	if (!ReadFile(f, &len, sizeof(len), &read, nullptr) || read != sizeof(len))
		throw IOException();
	std::unique_ptr<char> buffer(new char[len+1]);
	if (!ReadFile(f, buffer.get(), len, &read, nullptr) || read != len)
		throw IOException();
	buffer.get()[len] = 0;
	std::stringstream ss;
	ss << buffer.get();
	boost::archive::text_iarchive input(ss);
	input & v;
}
