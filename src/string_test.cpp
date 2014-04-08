#include <iostream>
#include "String.h"

int main()
{
	String str;

	std::cout << str << "\n";
	std::cout << str.length() << "\n";
	std::cout << str.capacity() << "\n";

	str.append("testasd");

	std::cout << str << "\n";
	std::cout << str.length() << "\n";
	std::cout << str.capacity() << "\n";

	str.append('a');

	std::cout << str << "\n";
	std::cout << str.length() << "\n";
	std::cout << str.capacity() << "\n";

	String* test_str = new String("swap test");
	str.swap(*test_str);
	delete test_str;
	std::cout << str << "\n";

	return 0;	
}