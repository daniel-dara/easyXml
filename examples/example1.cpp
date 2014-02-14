#include "../src/dev-easyXml.h"

int main()
{
	xml::Node* root = xml::loadXml("sample.xml");

	std::cout << "*** MAIN ***" << std::endl;
	std::cout << (root == NULL ? "root is null" : "root has something!") << std::endl;

	xml::printTree(root);

	std::cout << "history/2008: " << root->findElement("history/2008", true)->value << std::endl;

	xml::deleteTree(root);

	return 0;
}
