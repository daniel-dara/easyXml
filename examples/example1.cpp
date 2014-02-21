#include "../src/dev-easyXml.h"

int main()
{
	// Loading an XML file is easy, and it returns a pointer to the root node.
	xml::Node* root = xml::loadXml("example.xml");

	// You can print the XML structure in a readable format.
	std::cout << "Print the XML structure" << std::endl;
	xml::printTree(root);

	// You can search for nodes by name, using "/" as a hierarchical delimiter.
	xml::Node* history2008 = root->findNode("history/2008", true);

	// You can access a node's name and value members through their respective "get" and "set" methods.
	std::cout << "\nValue of the node \"history\" which is in \"2008\": " \
	          << history2008->getValue();

	// The getValue() method also accepts a type template allowing you to choose std::string, int, or
	// double as the return type.
	std::cout << "\nRetrieve value of node \"year\" (and add one to it): " \
	          << root->findNode("year", true)->getValue<int>() + 1 \
	          << std::endl;

	// When finished with an XML tree, don't forget to free the memory.
	// deleteTree will recursively delete a node and its children for you.
	xml::deleteTree(root);

	return 0;
}
