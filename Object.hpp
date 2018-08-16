#pragma once

#include <iostream>
#include <string>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class BasicObject {
public:
	virtual void addMethod(const std::string &, void (BasicObject::*)());
	virtual bool callMethod(const std::string &);
	virtual void serialize(boost::archive::text_iarchive &, const unsigned int) = 0;
	virtual void serialize(boost::archive::text_oarchive &, const unsigned int) = 0;
	virtual void print(std::ostream &) const = 0;

protected:
	std::map<std::string, void (BasicObject::*)()> methods;
};

class IntegerObject: public BasicObject {
public:
	IntegerObject(int = 0);
	int value;

	virtual void serialize(boost::archive::text_iarchive &, const unsigned int);
	virtual void serialize(boost::archive::text_oarchive &, const unsigned int);	

protected:
	virtual void print(std::ostream &) const;
	void inc();
	void dec();
};

class StringObject: public BasicObject {
public:
	StringObject(const std::string & = "");
	std::string value;

	virtual void serialize(boost::archive::text_iarchive &, const unsigned int);
	virtual void serialize(boost::archive::text_oarchive &, const unsigned int);	

protected:
	virtual void print(std::ostream &) const;
};

class GenericObject: public BasicObject {
public:
	enum {
		OBJECT_TYPE_INTEGER,
		OBJECT_TYPE_STRING,
	} type;
	std::shared_ptr<BasicObject> ptr;

	void put(int);
	void put(const std::string &);
	virtual bool callMethod(const std::string &);
	virtual void serialize(boost::archive::text_iarchive &, const unsigned int);
	virtual void serialize(boost::archive::text_oarchive &, const unsigned int);	

protected:
	virtual void print(std::ostream &) const;
};

std::ostream & operator << (std::ostream &, const BasicObject &);
