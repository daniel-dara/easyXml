#include "exception.h"

namespace EASYXML_NAMESPACE
{
	easyXmlException::easyXmlException(std::string msg, int line) : message(msg), lineNumber(line)
	{}

	easyXmlException::~easyXmlException() throw () 
	{}

	const char* easyXmlException::what() const throw()
	{
		if (lineNumber > 0)
		{
			char* buf = new char[message.length() + static_cast<int>(log10(lineNumber)) + 1];
			sprintf(buf, message.c_str(), lineNumber);
			return buf;
		}
		else
		{
			return message.c_str();
		}
	}
}
