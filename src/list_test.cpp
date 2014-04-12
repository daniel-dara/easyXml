#include <iostream>
#include <typeinfo>
#include "List.h"
#include "Set.h"

// #include <set>
// #include <list>

using namespace std;

int main()
{
	List<int> numbers;

	numbers.push_back(3);
	numbers.push_back(2);
	numbers.push_back(1);
	numbers.push_back(5);
	numbers.push_back(10);
	numbers.push_back(8);

	List<int>::iterator ite;
	Set<int>::iterator ite2;
	// ite2 = numbers.end();
	for (ite = numbers.begin(); ite != numbers.end(); ite++)
	{
		std::cout << *ite << "\n";
	}

	std::cout << "removing...\n";

	numbers.erase(numbers.begin());
	numbers.erase(numbers.begin());
	numbers.erase(numbers.begin());

	for (ite = numbers.begin(); ite != numbers.end(); ite++)
	{
		std::cout << *ite << "\n";
	}

	std::cout << "sizeof: " << sizeof(List<int>) << "\n";

	std::cout << "done\n";

	return 0;
}