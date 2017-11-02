#include "TestCases.h"

MissingFileTest::MissingFileTest::MissingFileTest() : FunctionalTest(FOPEN_FAILED)
{ }

void MissingFileTest::run()
{
	xml::loadXml("/home/ddara/Downloads/dblp.xml");
	fail("This is a test message."); // TODO: Add support for messages.
}

String MissingFileTest::getName()
{
	return "MissingFileTest";
}
