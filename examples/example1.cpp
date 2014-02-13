#include "easyXml.h"

int main()
{
	xml::Node* root = xml::Parser::loadXml("sample.xml");

	std::cout << "*** MAIN ***" << std::endl;
	std::cout << (root == NULL ? "root is null" : "root has something!") << std::endl;

	root->printTree(root);

	std::cout << root->findElement("year", true)->value << std::endl;

	xml::Parser::deleteTree(root);

	return 0;
}
