#include "Object.hpp"

void BasicObject::addMethod(const std::string &name, void (BasicObject::*m)()) {
	methods[name] = m;
}

bool BasicObject::callMethod(const std::string &name) {
	if (methods.find(name) == methods.end())
		return false;
	(this->*(methods[name]))();
	return true;
}

IntegerObject::IntegerObject(int v) : value(v) {
	addMethod("inc", (void(BasicObject::*)())&IntegerObject::inc);
	addMethod("dec", (void(BasicObject::*)())&IntegerObject::dec);
}

void IntegerObject::serialize(boost::archive::text_iarchive & ar, const unsigned int) {
	ar & value;
}

void IntegerObject::serialize(boost::archive::text_oarchive & ar, const unsigned int) {
	ar & value;
}

void IntegerObject::print(std::ostream &out) const {
	out << value;
}

void IntegerObject::inc() {
	value ++;
}

void IntegerObject::dec() {
	value --;
}

StringObject::StringObject(const std::string & v) : value(v) {
}

void StringObject::serialize(boost::archive::text_iarchive & ar, const unsigned int) {
	ar & value;
}

void StringObject::serialize(boost::archive::text_oarchive & ar, const unsigned int) {
	ar & value;
}

void StringObject::print(std::ostream &out) const {
	out << value;
}

void GenericObject::put(int v) {
	type = OBJECT_TYPE_INTEGER;
	std::shared_ptr<BasicObject> p(new IntegerObject(v));
	ptr.swap(p);
}

void GenericObject::put(const std::string &v) {
	type = OBJECT_TYPE_STRING;
	std::shared_ptr<BasicObject> p(new StringObject(v));
	ptr.swap(p);
}

bool GenericObject::callMethod(const std::string &name) {
	if (!ptr)
		return false;
	return ptr->callMethod(name);
}

void GenericObject::serialize(boost::archive::text_iarchive & ar, const unsigned int ver) {
	ar & type;
	if (type == OBJECT_TYPE_INTEGER) {
		std::shared_ptr<BasicObject> p(new IntegerObject);
		ptr.swap(p);
	} else if (type == OBJECT_TYPE_STRING) {
		std::shared_ptr<BasicObject> p(new StringObject);
		ptr.swap(p);
	} else
		throw boost::archive::archive_exception::invalid_signature;
	ptr->serialize(ar, ver);
}

void GenericObject::serialize(boost::archive::text_oarchive & ar, const unsigned int ver) {
	ar & type;
	if (ptr)
		ptr->serialize(ar, ver);
}

void GenericObject::print(std::ostream &out) const {
	if (ptr)
		ptr->print(out);
	else
		out << "null";
}

std::ostream &operator << (std::ostream & out, const BasicObject & ob) {
	ob.print(out);
	return out;
}
