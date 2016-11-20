#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include <algorithm>
#include "Exception.h"
#include "String.h"
#include "Stack.h"
#include "List.h"
#include "functions.h"

namespace EASYXML_NAMESPACE
{
	// Initial string capicites.
	const uint ELEMENT_NAME_SIZE      = 50;
	const uint ELEMENT_VALUE_SIZE     = 50;

	const std::string esc_sequences[] = {"&amp;", "&lt;", "&gt;", "&apos;", "&quot;"};
	const std::string esc_values[]    = {"&", "<", ">", "'", "\""};

	// Read the entire contents of a file and store the text as a char array in 'file_contents'. Also stores
	// the length of the file (in characters) in 'file_length'.
	static inline void readFile(const std::string& file_path, char*& file_contents, long int& file_length)
	{
		// Get a pointer to the file.
		FILE* fp = fopen(file_path.c_str(), "rb");

		// Verify the file was opened successfully.
		if (fp == NULL)
		{
			throw EasyXmlException("fopen() - Error opening file \"" + file_path + "\": " + strerror(errno),
			                       FOPEN_FAILED);
		}

		// Move the stream position indicator to the end of the file.
		if (fseek(fp, 0, SEEK_END))
		{
			throw EasyXmlException("fseek() - Error reading file \"" + file_path + "\": " + strerror(errno),
			                       FSEEK_END_FAILED);
		}

		// Get the stream position indicator's length. This is the length of the file (in bytes).
		file_length = ftell(fp);

		// Verify the file_length was successfully read.
		if (file_length == -1L)
		{
			throw EasyXmlException("ftell() - Error reading file \"" + file_path + "\": " + strerror(errno),
			                       FTELL_FAILED);
		}

		// Move the stream position indicator to the beginning of the file.
		if (fseek(fp, 0, SEEK_SET))
		{
			throw EasyXmlException("fseek() - Error reading file \"" + file_path + "\": " + strerror(errno),
			                       FSEEK_SET_FAILED);
		}

		try
		{
			// Attempt to allocate space for the file in memory.
			file_contents = new char[file_length];
		}
		catch (const std::bad_alloc& e)
		{
			throw EasyXmlException("Error allocating memory for file \"" + file_path + "\": " + e.what());
		}

		// Read the file into memory and verify the entire contents were read.
		if (file_length != static_cast<long int>(fread(file_contents, sizeof(char), file_length, fp)))
		{
			if (ferror(fp))
			{
				throw EasyXmlException("fread() - Error reading file \"" + file_path + "\": " +
				                       strerror(errno), FREAD_FAILED);
			}
			else if (feof(fp))
			{
				throw EasyXmlException("fread() - Reached early EOF in file \"" + file_path + "\": " +
				                       strerror(errno), FREAD_EOF);
			}
		}

		// Close the file pointer.
		if (fclose(fp) == EOF)
		{
			std::cerr << "EasyXML: [WARNING] Unable to close the input file.\n";
		}
	}

	Node* loadXml(const std::string& file_path)
	{
		char*    file_contents = NULL;
		long int file_length   = 0;

		// Read the input file into memory.
		// 'file_contents' and 'file_length' are passed by reference and set in readFile().
		readFile(file_path, file_contents, file_length);

		Input input(file_contents, file_length);

		Node* root = parseXml(input);

		delete[] file_contents;

		return root;
	}

	Node* parseXml(Input& input) {
		#if DEBUG
			printf("\n");
			printf("pointer size: %d\n", sizeof(void*));
			printf("uint size: %d\n", sizeof(uint));
			printf("Node size: %d\n", sizeof(Node));
			printf("String size: %d\n", sizeof(String));
			printf("std::string size: %d\n", sizeof(std::string));
			printf("set: %d\n", sizeof(std::set<Node*, bool (*)(const Node*, const Node*)>));
			printf("my List: %d\n", sizeof(List<Node*>));
			printf("\n");
		#endif

		// The top-level XML node.
		Node* root = NULL;

		// A stack of the currently open nodes. Used for assigning values and parents.
		Stack<Node*> ancestors;

		// Instantiate temporary String containers.
		String name, value, attr_name, attr_value;

		// Increase initial String capacity to reduce number of memory reallocations.
		value.reserve(ELEMENT_VALUE_SIZE);

		// Parse the input.
		while (!input.end())
		{
			// Check for an XML tag.
			if (input.peek() == '<')
			{
				input.consumeComment();
				input.consumeProlog();
				input.consumeDocType();

				if (input.consumeEndTag(name)) {
					// Validate that the name matches the name of the currently open node.
					if (name != ancestors.top()->name)
					{
						std::cout << "ancestor val: " << ancestors.top()->getFirstAttribute() << "\n";
						std::cout << "ancestor name: " << ancestors.top()->name << "\n";
						std::cout << "ancestor parent name: " << ancestors.top()->getParent()->name << "\n";
						std::cout << "ancestor parent name: " << ancestors.top()->getParent()->getFirstAttribute() << "\n";
						throw EasyXmlException("Mismatched closing tag \"" + name + "\". " \
						                       "Expected \"" + ancestors.top()->name + "\".",
						                       XML_MALFORMED_MISTMATCHED_CLOSING_TAG);
					}

					// Assign the accumulated value to the currently open node.
					value.shrink_to_fit();
					ancestors.top()->value.swap(value);

					name.clear();
					name.reserve(ELEMENT_NAME_SIZE);

					// Since the element is now closed, it will not have any more children.
					ancestors.pop();
				}

				Attribute* first_attr = NULL;
				bool is_self_closing;

				if (input.consumeStartTag(name, first_attr, is_self_closing)) {
					// Reset value (it should only contain the whitespace between this tag and the previous one).
					value.clear();
					value.reserve(ELEMENT_VALUE_SIZE);

					// Create a node for this element.
					Node* node = new Node();

					// Assign the name.
					name.shrink_to_fit();
					name.swap(node->name);

					// Assign the attribute.
					node->firstAttribute_ = first_attr;

					// Set the root node if it hasn't been found yet.
					if (root == NULL)
					{
						root = node;
					}
					// Otherwise, add the node to his parent's child list.
					else
					{
						ancestors.top()->addChild(node);
					}

					// Self-closing tags cannot have children and should not be put on the ancestor's stack.
					if (!is_self_closing)
					{
						ancestors.push(node);
					}
				}
			}
			else
			{
				// Accumulate the value for this element.
				char c;
				input.consumeChar(c);
				value += c;
			}
		}

		return root;
	}

	void saveXml(const Node* node, const std::string& file_path)
	{
		std::ofstream file;
		file.open(file_path.c_str());
		saveXml(node, file);
		file.close();
	}

	void saveXml(const Node* node, std::ostream& out, std::string indentation)
	{
		// silence warnings.
		out << indentation << node->name << "\n";
	// 	out << indentation << "<" << node->name;

	// 	if (node->children.size() > 0)
	// 	{
	// 		out << ">\n";

	// 		for (List<Node*>::iterator it = node->children.begin();
	// 		     it != node->children.end(); ++it)
	// 		{
	// 			saveXml(*it, out, indentation + "\t");
	// 		}

	// 		out << indentation << "</" << node->name << ">";
	// 	}
	// 	else if (node->value == "")
	// 	{
	// 		out << " />";
	// 	}
	// 	else
	// 	{
	// 		String value(node->value);

	// 		// Replace bad chars with XML escape sequences. Ampersands must be escaped first otherwise
	// 		// ampersands from other escape sequences will be double encoded.
	// 		for (int i = 0; i < 5; i++)
	// 		{
	// 			// TODO: write replacement function for class String
	// 			// replaceAll(value, esc_values[i], esc_sequences[i]);
	// 		}

	// 		out << ">" << value;
	// 		out << "</" << node->name << ">";
	// 	}

	// 	out << "\n";
	}

	void printTree(const Node* node, std::string indentation)
	{
		std::cout << indentation << node->name << ": " << node->value << std::endl;

		// if (node->attributes.size() != 0)
		// {
		// 	std::cout << "\t" + indentation + "attributes:";

		// 	std::set<Node::Attribute>::const_iterator it;
		// 	for (it = node->attributes.begin(); it != node->attributes.end(); ++it)
		// 	{
		// 		std::cout << " " << (*it).name << "=\"" << (*it).value << "\"";
		// 	}

		// 	std::cout << std::endl;
		// }

		// increase the indentation for the next level
		// indentation += "\t";

		// List<Node*>::iterator it;
		// for (it = node->children.begin(); it != node->children.end(); ++it)
		// {
		// 	printTree((*it), indentation);
		// }
	}

	// Recursively deletes the given node and all of its offspring.
	void deleteTree(Node* node)
	{
		if (node == NULL)
		{
			throw EasyXmlException("deleteTree() was called on a NULL pointer.", 102);
		}
		else
		{
			Node* child = node->getFirstChild();

			while (child != NULL)
			{
				Node* next_child = child->getNextSibling();
				deleteTree(child);
				child = next_child;
			}

			delete node;
		}
	}

	// The following 3 functions are public domain: http://stackoverflow.com/a/217605
	// They perform in-place trims.

	// trim from start
	std::string& ltrim(std::string& s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		        std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	std::string& rtrim(std::string& s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(),
		        std::not1(std::ptr_fun<int, int>(std::isspace))).base() ,s.end());
		return s;
	}

	// trim from end (copy)
	std::string rTrim(std::string s)
	{
		return rtrim(s);
	}

	// trim from both ends
	std::string& trim(std::string& s)
	{
		return ltrim(rtrim(s));
	}

	// trim from both ends (copy)
	std::string Trim(std::string s)
	{
		return trim(s);
	}

	// public domain: http://stackoverflow.com/a/17620909
	void replaceAll(std::string& str, const char* from, const std::string& to)
	{
		if (strlen(from) == 0)
		{
			return;
		}

		std::string newStr;
		newStr.reserve(str.length());

		size_t start_pos = 0, pos;
		while ((pos = str.find(from, start_pos)) != std::string::npos)
		{
			newStr += str.substr(start_pos, pos - start_pos);
			newStr += to;
			start_pos = pos + strlen(from);
		}
		newStr += str.substr(start_pos);
		str.swap(newStr); // faster than str = newStr;
	}

	void replaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		replaceAll(str, from.c_str(), to);
	}
}
