#include <iostream>
#include <typeinfo>

#include "../../src/dev-easyXml.h"
#include "AssertionException.h"
#include "FunctionalTest.h"
#include "TestCases.h"
#include "TestRunner.h"

int main()
{
	// TODO: Add more tests!
	TestRunner::run(new MissingFileTest());

	std::cout << "Done!\n"; // TODO: Add test summary to TestRunner and most likely use instance instead of static methods.
}
