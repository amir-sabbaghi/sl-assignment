#pragma once

#include "Object.hpp"

#include <SDKDDKVer.h>
#include <Windows.h>
#include <mutex>
#include <vector>

class Client {
public:
	Client(const std::string &pipeName);
	~Client();

	void send(const GenericObject &v);
	std::vector<GenericObject> list();
	void call(const std::string &);

protected:
	HANDLE pipe;
	std::mutex mut;
};
