#include "exception.h"
#include "functions.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdio.h>

namespace EASYXML_NAMESPACE
{
	EasyXmlException::EasyXmlException(const String& message, ERROR_CODE errorCode, uint lineNumber) :
		message_("EasyXmlException: "),
		errorCode_(errorCode),
		lineNumber_(lineNumber)
	{
		message_ += message + '\n';

		if (lineNumber > 0)
		{
			std::ostringstream ss;
			ss << lineNumber_;
			message_ += "At line " + ss.str() + "\n.";
		}
	}
				                       
	EasyXmlException::EasyXmlException(const String& message, int errorCode, uint lineNumber) :
		message_("EasyXmlException: "),
		errorCode_(static_cast<ERROR_CODE>(errorCode)),
		lineNumber_(lineNumber)
	{
		message_ += message + '\n';

		if (lineNumber > 0)
		{
			std::ostringstream ss;
			ss << lineNumber_;
			message_ += "At line " + ss.str() + "\n.";
		}
	}

	EasyXmlException::~EasyXmlException() throw ()
	{ }

	const char* EasyXmlException::what() const throw()
	{
		return message_.c_str();
	}

	ERROR_CODE EasyXmlException::getErrorCode() const
	{
		return errorCode_;
	}

	uint EasyXmlException::getLineNumber() const
	{
		return lineNumber_;
	}
}
