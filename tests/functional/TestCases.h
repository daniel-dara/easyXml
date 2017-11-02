#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "FunctionalTest.h"

class MissingFileTest : public FunctionalTest
{
public:
	MissingFileTest();

	void run();

	String getName();
};

#endif
