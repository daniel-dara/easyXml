#include "../src/dev-easyXml.h"

int main()
{
	// Loading an XML file is easy, and it returns a pointer to the root node.
	xml::Node* root = xml::loadXml("example.xml");

	// You can print the XML structure in a readable format.
	std::cout << "Print the XML structure" << std::endl;
	xml::printTree(root);

	// You can search for nodes by value, using "/" as a node delimiter.
	// You can access a node's name and value members directly. This is so that in the future, you can make
	// changes to them and then write the XML tree back to a file.
	std::cout << "\nRetrieve value of node history/2008: " << root->findNode("history/2008", true)->value \
		<< std::endl;

	// When finished with an XML tree, don't forget to free the memory.
	// deleteTree will recursively delete all of its children for you.
	xml::deleteTree(root);

	return 0;
}
