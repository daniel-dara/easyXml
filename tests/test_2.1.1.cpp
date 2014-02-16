#include "../src/dev-easyXml.h"
#include "test.h"

#define EXPECTED_ERROR_CODE 1
#define XML_SECTION_NUM "2.1.1"

int main()
{
	xml::Node* root;

	try
	{
		root = xml::loadXml(testXmlPath + "test_" + XML_SECTION_NUM + ".xml");
	}
	catch (const xml::EasyXmlException& e)
	{
		if (e.errorCode == EXPECTED_ERROR_CODE)
		{
			std::cout << "PASS: " << XML_SECTION_NUM << std::endl;
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
