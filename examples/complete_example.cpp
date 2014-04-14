#include <iostream>

// I only use this include so I do not have to re-install the library for each change when I am developing.
#include "../src/dev-easyXml.h"

// In your code you would use the following include (uncommented of course).
// #include <easyXml.h>

unsigned int count = 0;

void explore(xml::Node* parent)
{
	count++;
	List<xml::Node*>::iterator ite = parent->children.begin();
	while (ite != parent->children.end())
	{
		explore(*ite);
		ite++;
	}
}

int main()
{
	/*** LOADING FILES  *************************************************************************************/

	xml::Node* root = NULL;

	// Malformed XML can cause an exception to be thrown when loadXml is called.
	try
	{
		// Loading an XML file is easy, and it returns a pointer to the root node.
		root = xml::loadXml2("../../Downloads/nasa.xml");
		// root = xml::loadXml2("example.xml");

		if (root != NULL)
		{
			/*** PRINTING THE STRUCTURE *************************************************************************/

			// You can print the XML structure in a readable format.
			//std::cout << "Printing the XML structure..." << std::endl;
			// xml::printTree(root);
			//std::cout << std::endl;

			/*** ACCESSING NODES ********************************************************************************/

			// You can search for nodes by name, using "/" as a hierarchical delimiter.
			// findNode() will throw an exception if the given node was not found OR you pass the boolean "true"
			// as the second argument and it will return NULL instead.
			// xml::Node* n2008 = root->findNode("history/2008");

			// You can access a node's name and value directly so you can read or make changes to the XML structure
			// std::cout << "The value of \"" << n2008->name << "\" is \"" << n2008->value << "\""
			//           << std::endl;

			// xml::Node* year = root->findNode("year", true);

			// There is also a templated "getter" method for value that allows you to choose between std::string, int,
			// float, or double as the return type.
			// std::cout << "The integer value of \"" + year->name + "\" + 1 is " << year->val<int>() + 1 
			//           << std::endl;

			//xml::saveXml(root, "example_output.xml");

			explore(root);

			std::cout << "total nodes: " << count << "\n";

			/*** CLEANUP ****************************************************************************************/
			          
			// When finished with an XML tree, don't forget to free the memory.
			// deleteTree will recursively delete a node and its children for you.
			xml::deleteTree(root);
		}
	}
	catch (const xml::EasyXmlException& e)
	{
		// Print out the error message.
		std::cout << "error code: " << e.getErrorCode() << "\n";
		std::cout << e.what();
	}

	return 0;
}
