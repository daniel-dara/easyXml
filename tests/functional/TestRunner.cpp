#include "TestRunner.h"

void TestRunner::run(FunctionalTest* test)
{
	bool didTestPass;
	std::stringstream message;

	try
	{
		test->run();
		didTestPass = true;
		message << "The test successfully completed.";
	}
	catch (xml::EasyXmlException exception)
	{
		if (test->errorIsExpected()) {
			if (test->getExpectedErrorCode() == exception.getErrorCode())
			{
				didTestPass = true;
				message << "The correct exception was thrown.";
			}
			else
			{
				didTestPass = false;
				message << "The wrong exception was thrown. " <<
					"Expected \"" << test->getExpectedErrorCode() << "\" but received \"" << exception.getErrorCode() << "\".";
			}
		}
		else
		{
			didTestPass = false;
			message << "An unexpected exception was thrown: " << exception.what();
		}
	}
	catch (AssertionException exception)
	{
		didTestPass = false;
		message << exception.what();
	}

	std::cout << "[" << (didTestPass ? "PASS" : "FAIL") << "] [" << test->getName() << "] " << message.rdbuf() << "\n";

	delete test;
}
