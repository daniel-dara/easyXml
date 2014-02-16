#ifndef EXCEPTION
#define EXCEPTION

#include <iostream>
#include <cmath>
#include <stdio.h>
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	class EasyXmlException : public std::exception
	{
	public:
		const int errorCode;

		EasyXmlException(const std::string msg, int line = 0, int errorCode = 0);
		~EasyXmlException() throw ();

		const char* what() const throw();

	private:
		std::string message;
		const int lineNumber;
	};
}

#endif
