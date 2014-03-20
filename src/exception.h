#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "namespace.h"
#include <exception>
#include <string>

namespace EASYXML_NAMESPACE
{
	class EasyXmlException : public std::exception
	{
	public:
		std::string message;
		const int errorCode;
		const int lineNumber;

		EasyXmlException(const std::string msg, int line = 0, int errorCode = 0);
		~EasyXmlException() throw ();

		const char* what() const throw();
	};
}

#endif
