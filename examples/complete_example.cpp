#include <iostream>

// I only use this include so I do not have to re-install the library for each change when I am developing.
#include "../src/dev-easyXml.h"

// In your code you would use the following include (uncommented of course).
// #include <easyXml.h>

int main()
{
	/*** LOADING FILES  *************************************************************************************/

	// Loading an XML file is easy, and it returns a pointer to the root node.
	xml::Node* root = xml::loadXml("example.xml");

	/*** PRINTING THE STRUCTURE *****************************************************************************/

	// You can print the XML structure in a readable format.
	std::cout << "Printing the XML structure..." << std::endl;
	xml::printTree(root);
	std::cout << std::endl;

	/*** ACCESSING NODES ************************************************************************************/

	// You can search for nodes by name, using "/" as a hierarchical delimiter.
	// findNode() will throw an exception if the given node was not found OR you pass the boolean "true"
	// as the second argument and it will return NULL instead.
	xml::Node* n2008 = root->findNode("history/2008");

	// You can access a node's name and value  through its respective "get" and "set" methods.
	std::cout << "The value of \"" << n2008->getName() << "\" is \"" << n2008->val() << "\""
	          << std::endl;

	xml::Node* year = root->findNode("year", true);

	// There is also a templated getValue() method that allows you to choose between std::string, int, or
	// double as the return type.
	std::cout << "The integer value of \"" + year->getName() + "\" + 1 is " << year->val<int>() + 1 \
	          << std::endl;

	xml::saveXml(root, "example_output.xml");

	/*** CLEANUP ********************************************************************************************/
	          
	// When finished with an XML tree, don't forget to free the memory.
	// deleteTree will recursively delete a node and its children for you.
	xml::deleteTree(root);

	return 0;
}
