#ifndef FUNCTIONAL_TEST_H
#define FUNCTIONAL_TEST_H

#include "../../src/dev-easyXml.h"

class FunctionalTest
{
public:
	FunctionalTest();
	FunctionalTest(ERROR_CODE errorCode);
	virtual ~FunctionalTest();

	virtual void run() = 0;
	virtual String getName() = 0;

	virtual ERROR_CODE getExpectedErrorCode();

	bool errorIsExpected();

protected:
	// TODO: Consider moving to an Assertion class for SoC.
	void fail(const String& message);

private:
	const ERROR_CODE expectedErrorCode;
};

#endif
