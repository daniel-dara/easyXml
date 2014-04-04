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
		EasyXmlException(const std::string& msg, int errorCode = 0, uint lineNumber = 0);
		~EasyXmlException() throw ();

		const char* what() const throw();

		int getErrorCode() const;
		uint getLineNumber() const;

	private:
		std::string message_;
		const int errorCode_;
		const uint lineNumber_;
	};
}

#endif
