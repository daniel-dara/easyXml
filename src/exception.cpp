#include "exception.h"
#include "functions.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdio.h>

namespace EASYXML_NAMESPACE
{
	EasyXmlException::EasyXmlException(const std::string& msg, int code, int line) :
		message(msg),
		errorCode(code),
		lineNumber(line)
	{
		if (lineNumber > 0)
		{
			// Convert lineNumber to string
			std::ostringstream ss;
			ss << lineNumber;

			replaceAll(message, "%d", ss.str());
		}
	}

	EasyXmlException::~EasyXmlException() throw ()
	{ }

	const char* EasyXmlException::what() const throw()
	{
		return message.c_str();
	}

	int EasyXmlException::getErrorCode() const
	{
		return errorCode;
	}

	int EasyXmlException::getLineNumber() const
	{
		return lineNumber;
	}
}
