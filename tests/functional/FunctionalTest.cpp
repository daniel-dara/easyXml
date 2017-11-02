#include "FunctionalTest.h"
#include "AssertionException.h"

FunctionalTest::FunctionalTest() : expectedErrorCode(NO_ERROR)
{ }

FunctionalTest::FunctionalTest(ERROR_CODE errorCode) : expectedErrorCode(errorCode)
{ }

FunctionalTest::~FunctionalTest()
{ }

ERROR_CODE FunctionalTest::getExpectedErrorCode() {
	return expectedErrorCode;
}

bool FunctionalTest::errorIsExpected() {
	return expectedErrorCode != NO_ERROR;
}

void FunctionalTest::fail(const String& message)
{
	throw AssertionException(message); // TODO: Add support for assertion types.
}
