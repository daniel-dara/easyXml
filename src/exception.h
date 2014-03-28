#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "namespace.h"
#include <exception>
#include <string>

namespace EASYXML_NAMESPACE
{
	class EasyXmlException : std::exception
	{
	public:
		EasyXmlException(const std::string& msg, int line = 0, int errorCode = 0);
		~EasyXmlException() throw ();

		const char* what() const throw();

		int getErrorCode() const;
		int getLineNumber() const;

	private:
		std::string message;
		const int errorCode;
		const int lineNumber;
	};
}

#endif
