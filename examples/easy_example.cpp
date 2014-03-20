#include <iostream>
#include <easyXml.h>

int main()
{
	// Loading an XML file is one function call. loadXml returns a pointer to the root node.
	xml::Node* root = xml::loadXml("example.xml");

	// You can search for nodes by name, using "/" as a hierarchical delimiter.
	// findNode() will throw an exception if the given node was not found OR you can pass the boolean "true"
	// as the second argument and it will return NULL instead.
	xml::Node* n2008 = root->findNode("history/2008");

	// You can access a node's name and value  through its respective "get" and "set" methods.
	// val() is templated so val<int>() will return the value parsed as an integer.
	std::cout << n2008->getName() << "=" << n2008->val() << std::endl;

	// Saving is as easy as loading
	xml::saveXml(root, "example_output.xml");

	// When finished with an XML tree, don't forget to free the memory.
	// deleteTree will recursively delete a node and all of its children for you.
	xml::deleteTree(root);

	return 0;
}
