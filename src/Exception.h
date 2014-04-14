#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "namespace.h"
#include "String.h"
#include <exception>
#include <string>

enum ERROR_CODE
{
	FOPEN_FAILED = 1,
	FSEEK_END_FAILED = 2,
	FTELL_FAILED = 3,
	FSEEK_SET_FAILED = 4,
	FREAD_FAILED = 5,
	FREAD_EOF = 6,
	XML_MALFORMED_MISTMATCHED_CLOSING_TAG = 101
};

namespace EASYXML_NAMESPACE
{
	class EasyXmlException : std::exception
	{
	public:
		EasyXmlException(const String& message, ERROR_CODE errorCode, uint lineNumber = 0);
		EasyXmlException(const String& msg, int errorCode = 0, uint lineNumber = 0);
		~EasyXmlException() throw ();

		const char* what() const throw();

		ERROR_CODE getErrorCode() const;
		uint getLineNumber() const;

	private:
		String message_;
		const ERROR_CODE errorCode_;
		const uint lineNumber_;
	};
}

#endif
