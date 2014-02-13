#include "easyXml.h"

int main()
{
	Node* root = Parser::loadXml("sample.xml");

	std::cout << "*** MAIN ***" << std::endl;
	std::cout << (root == NULL ? "root is null" : "root has something!") << std::endl;

	root->printTree(root);

	std::cout << root->findElement("year", true)->value << std::endl;

	return 0;
}
