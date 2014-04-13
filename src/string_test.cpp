#include <iostream>
#include "String.h"

void test_minimize(String& str)
{
	String temp(str);
	temp += "!";
	str.swap(temp);
}

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

	str.append("asdf");

	std::cout << str << "\n";
	std::cout << str.length() << "\n";
	std::cout << str.capacity() << "\n";

	test_minimize(str);
	str.minimize();

	std::cout << str << "\n";

	// String* test_str = new String("swap test");
	// str.swap(*test_str);
	// delete test_str;
	// std::cout << str << "\n";

	return 0;	
}