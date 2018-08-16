#pragma once

#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

enum CommandType {
	COMMAND_TYPE_TRANSFER,
	COMMAND_TYPE_LIST,
	COMMAND_TYPE_CALL,
};

class Command {
public:
	CommandType cmd;
	int transferArg;
	std::string method;
protected:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & cmd;
		switch (cmd) {
		case COMMAND_TYPE_TRANSFER:
			ar & transferArg;
			break;
		case COMMAND_TYPE_CALL:
			ar & method;
			break;
		case COMMAND_TYPE_LIST:
			break;
		default:
			throw std::exception();
		}
	}
};
