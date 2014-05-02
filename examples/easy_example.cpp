#include <iostream>
#include <easyXml.h>

int main()
{
	// Loading an XML file is one function call. loadXml returns a pointer to the root node.
	xml::Node* root = xml::loadXml("example.xml");

	// You can easily search for nodes by name and use "/" as a hierarchical delimiter.
	// find() will return NULL if the given node was not found OR you can pass the boolean "true"
	// as the second argument and it will throw an exception instead.
	xml::Node* n2008 = root->find("history/2008");

	// You can access a node's name and value directly.
	std::cout << n2008->name << "=" << n2008->value << std::endl;

	// There is also a templated function called val() to parse the value for you.
	xml::Node* year = root->find("year");
	std::cout << year->name << "=" << (year->val<int>() - 10) << std::endl;

	// Saving is as easy as loading.
	xml::saveXml(root, "example_output.xml");

	// When finished with an XML tree, don't forget to free the memory.
	// deleteTree will recursively delete a node and all of its children.
	xml::deleteTree(root);

	return 0;
}
