#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <sstream>
#include "AssertionException.h"
#include "FunctionalTest.h"

class TestRunner
{
public:
	static void run(FunctionalTest* test);
};

#endif
