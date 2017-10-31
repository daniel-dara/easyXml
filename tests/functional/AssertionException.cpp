#include "AssertionException.h"

AssertionException::AssertionException(const String& message) : message_(generateFullMessage(message))
{ }

String AssertionException::generateFullMessage(const String& message)
{
	return "AssertionException: " + message;
}

AssertionException::~AssertionException() throw () {}

const char* AssertionException::what() const throw()
{
	return message_.c_str();
}
