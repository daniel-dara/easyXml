#include <iostream>
#include "Set.h"

int main()
{
	Set<int> numbers;

	numbers.insert(3);
	numbers.insert(2);
	numbers.insert(1);
	numbers.insert(5);
	numbers.insert(10);
	numbers.insert(8);

	Set<int>::iterator ite;
	for (ite = numbers.begin(); ite != numbers.end(); ite++)
	{
		std::cout << *ite << "\n";
	}

	std::cout << "done\n";

	return 0;
}