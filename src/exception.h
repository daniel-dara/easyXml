#ifndef EXCEPTION
#define EXCEPTION

#include <iostream>
#include <cmath>
#include <stdio.h>
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	class easyXmlException : public std::exception
	{
	public:
		easyXmlException(std::string msg, int line = 0);

		~easyXmlException() throw ();

		const char* what() const throw();

	private:
		std::string message;
		int lineNumber;
	};
}

#endif
