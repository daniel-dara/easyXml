#include "exception.h"
#include "functions.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdio.h>

namespace EASYXML_NAMESPACE
{
	EasyXmlException::EasyXmlException(const std::string& message, int errorCode, uint lineNumber) :
		message_(message),
		errorCode_(errorCode),
		lineNumber_(lineNumber)
	{
		// If a line number was specified...
		if (lineNumber_ > 0)
		{
			// convert "lineNumber" to a string.
			std::ostringstream ss;
			ss << lineNumber_;

			replaceAll(message_, "%d", ss.str());

			message_ += "\n";
		}
	}

	EasyXmlException::~EasyXmlException() throw ()
	{ }

	const char* EasyXmlException::what() const throw()
	{
		return message_.c_str();
	}

	int EasyXmlException::getErrorCode() const
	{
		return errorCode_;
	}

	uint EasyXmlException::getLineNumber() const
	{
		return lineNumber_;
	}
}
