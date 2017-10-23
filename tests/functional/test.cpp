#include <iostream>
#include <sstream>
#include <typeinfo>

#include "../../src/dev-easyXml.h"

// TODO: Break out into separate files.
class AssertionException : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "AssertionException";
	}
};

class FunctionalTest
{
public:
	FunctionalTest() : expectedErrorCode(NO_ERROR) {}
	FunctionalTest(ERROR_CODE errorCode) : expectedErrorCode(errorCode) {}
	virtual ~FunctionalTest() {}

	virtual void run() = 0;

	virtual ERROR_CODE getExpectedErrorCode() {
		return expectedErrorCode;
	}

	bool errorIsExpected() {
		return expectedErrorCode != NO_ERROR;
	}

protected:
	// TODO: Consider moving to an Assertion class for SoC.
	void fail()
	{
		throw AssertionException(); // TODO: Add support for assertion types.
	}

private:
	const ERROR_CODE expectedErrorCode;
};


class MissingFileTest : public FunctionalTest
{
public:
	MissingFileTest() : FunctionalTest(FOPEN_FAILED)
	{ }

	void run()
	{
		xml::loadXml("/home/ddara/Downloads/dblp.xml");
		fail(); // TODO: Add support for messages.
	}
};

class TestRunner
{
public:
	static void run(FunctionalTest* test)
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

		std::cout << "[" << (didTestPass ? "PASS" : "FAIL") << "] " << typeid(*test).name() << ": " << message.rdbuf() << "\n";

		delete test;
	}
};

int main()
{
	// TODO: Add more tests!
	TestRunner::run(new MissingFileTest());

	std::cout << "Done!\n"; // TODO: Add test summary to TestRunner and most likely use instance instead of static methods.
}

