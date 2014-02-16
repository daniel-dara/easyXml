#include "../src/dev-easyXml.h"
#include "test.h"

#define EXPECTED_ERROR_CODE 1

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
			std::cout << "PASS: 2.1.2" << std::endl;
			return 0;
		}
		else
		{
			std::cout << "FAIL: 2.1.2 - Expected " << EXPECTED_ERROR_CODE << " Recieved " << e.errorCode << std::endl;
			return 1;
		}
	}

	if (root != NULL)
	{
		xml::deleteTree(root);
	}
}
