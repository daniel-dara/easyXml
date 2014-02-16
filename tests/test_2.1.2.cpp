#include "../src/dev-easyXml.h"
#include "test.h"

#define EXPECTED_ERROR_CODE 1
#define XML_SECTION "2.1.2"

int main()
{
	xml::Node* root;

	try
	{
		root = xml::loadXml(testXmlPath + "test_2.1.2.xml");
	}
	catch (const xml::EasyXmlException& e)
	{
		if (e.errorCode == EXPECTED_ERROR_CODE)
		{
			std::cout << "PASS: " << XML_SECTION << std::endl;
			return 0;
		}
		else
		{
			std::cout << "FAIL: XML_SECTION - Expected " << EXPECTED_ERROR_CODE << ", Recieved " << e.errorCode << std::endl;
			return 1;
		}
	}

	if (root != NULL)
	{
		xml::deleteTree(root);
	}

	std::cout << "FAIL: " << XML_SECTION << " - Expected " << EXPECTED_ERROR_CODE << ", Recieved (no error)" << std::endl;

	return 1;
}
