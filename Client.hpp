#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <mutex>
#include <vector>

class Client {
public:
	Client(const std::string &pipeName);
	~Client();

	void send(int v);
	std::vector<int> list();

protected:
	HANDLE pipe;
	std::mutex mut;
};
