#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <algorithm>

// The following 3 trim functions are public domain: http://stackoverflow.com/a/217605

// trim from start
inline std::string& ltrim(std::string &s);

// trim from end
inline std::string& rtrim(std::string &s);

// trim from both ends
inline std::string& trim(std::string &s);

#endif
