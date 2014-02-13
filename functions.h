#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <algorithm>
#include "namespace.h"

namespace EASYXML_NAMESPACE
{
	// The following 3 trim functions are public domain: http://stackoverflow.com/a/217605

	// trim from start
	inline std::string& ltrim(std::string &s);

	// trim from end
	inline std::string& rtrim(std::string &s);

	// trim from both ends
	inline std::string& trim(std::string &s);

	// public domain: http://stackoverflow.com/a/17620909
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
}

#endif
