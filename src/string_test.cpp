#include <iostream>
#include <string>
#include "String.h"

void assert(bool condition)
{
	if (condition == true)
	{
		std::cout << "   PASS\n";
	}
	else
	{
		std::cout << "   FAIL***\n";
	}
}

int main()
{
	String s1;
	String s2(String("string2"));
	String s3("string3");
	String s4("string4asdf");
	String s4b("string4asdf", 7, 3);
	String s5(std::string("string5"));

	s1.debug();
	s2.debug();
	s3.debug();
	s4.debug();
	s4b.debug();
	s5.debug();

	int result;

	result = s2.compare(String("String2"));
	std::cout << "s2.compare(\"String2\") -> " << result;
	assert(result == 1);

	result = (s2 == String("String2"));
	std::cout << "s2 == String(\"String2\"): " << result;
	assert(result == 0);

	result = (s2 == String("string2"));
	std::cout << "s2 == String(\"string2\"): " << result;
	assert(result == 1);

	result = (s4 == "string4asdf");
	std::cout << "s4 == \"string4asdf\": " << result;
	assert(result == 1);

	result = s4.compare(7, 4, "test4asdf", 5, 4);
	std::cout << "s4.compare(7, 4, \"test4asdf\", 5, 4): " << result;
	assert(result == 0);

	result = String("a") == 'a';
	std::cout << "String(\"a\") == 'a': " << result;
	assert(result == 1);

	result = s5 != "string";
	std::cout << "s5 != \"string\": " << result;
	assert(result == 1);

	String temp("assign_test");
	temp = "arf!";
	std::cout << "temp = \"arf!\": " << temp;
	assert(temp == "arf!");

	temp = 'c';
	std::cout << "temp = 'c': " << temp;
	assert(temp == "c");

	temp += "HAR";
	std::cout << "temp += \"HAR\": " << temp;
	assert(temp == "cHAR");

	return 0;	
}