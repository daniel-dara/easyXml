// Most common includes come from just including easyXml
#include <cstdlib>
#include "../src/dev-easyXml.h"

const std::string testXmlPath("xml/");

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Error: incorrect number of arguments" << std::endl;
		std::cout << "Usage: unit_test <test identifier> <expected error code>" << std::endl;
		return 1;
	}

	const std::string XML_SECTION_NUM(argv[1]);
	const int EXPECTED_ERROR_CODE = atoi(argv[2]);
	xml::Node* root;

	try
	{
		root = xml::loadXml(testXmlPath + "test_" + XML_SECTION_NUM + ".xml");
	}
	catch (const xml::EasyXmlException& e)
	{
		if (e.errorCode == EXPECTED_ERROR_CODE)
		{
			std::cout << "PASS: " << XML_SECTION_NUM << " - Received " << EXPECTED_ERROR_CODE << std::endl;
			return 0;
		}
		else
		{
			std::cout << "FAIL: " << XML_SECTION_NUM << " - Expected " << EXPECTED_ERROR_CODE << ", Recieved " << e.errorCode << std::endl;
			return 1;
		}
	}

	if (root != NULL)
	{
		xml::deleteTree(root);
	}

	std::cout << "FAIL: " << XML_SECTION_NUM << " - Expected " << EXPECTED_ERROR_CODE << ", Recieved (no error)" << std::endl;
	return 1;
}
