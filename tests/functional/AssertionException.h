#include "../../src/dev-easyXml.h"

class AssertionException : public std::exception
{
public:
	AssertionException(const String& message);

	String generateFullMessage(const String& message);

	~AssertionException() throw ();

	virtual const char* what() const throw();

private:
	String message_;
};
