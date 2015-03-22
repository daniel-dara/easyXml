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
	const uint ELEMENT_NAME_SIZE = 50;
	const uint ELEMENT_VALUE_SIZE = 50;
	const std::string esc_sequences[] = {"&amp;", "&lt;", "&gt;", "&apos;", "&quot;"};
	const std::string esc_values[] = {"&", "<", ">", "'", "\""};

	static inline int strcmp2(const char* rhs, const char* lhs, uint len = -1)
	{
		uint i = 0;

		// while (buf_char - buf_ < len) could also be used but I'm not sure if the arithmetic would be
		// properly optimized out. The only variable changing in that equation is buf_char so its more
		// efficient to just use a counter.
		while (i < len)
		{
			if (rhs[i] != lhs[i])
			{
				return (rhs[i] > lhs[i]) - (rhs[i] < lhs[i]);
			}
			else if (rhs[i] == '\0')
			{
				return -1;
			}
			else if (lhs[i] == '\0')
			{
				return 1;
			}
			i++;
		}

		return 0;
	}

	static inline bool isWhitespace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}

	// Read the entire contents of a file and store the text as a char array in 'file_contents' and store
	// the length of the file (in characters) in 'file_length' (both arguments are by reference).
	static inline void readFile(const std::string& file_path, char*& file_contents, long int& file_length)
	{
		// Create an open pointer to the file.
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
		// Note: file_contents and file_length are passed by reference and are initialized in readFile().
		readFile(file_path, file_contents, file_length);

		const char* input = file_contents;

		// DEBUG
		// printf("\n");
		// printf("pointer size: %d\n", sizeof(void*));
		// printf("uint size: %d\n", sizeof(uint));
		// printf("Node size: %d\n", sizeof(Node));
		// printf("String size: %d\n", sizeof(String));
		// printf("std::string size: %d\n", sizeof(std::string));
		// printf("set: %d\n", sizeof(std::set<Node*, bool (*)(const Node*, const Node*)>));
		// printf("my List: %d\n", sizeof(List<Node*>));
		// printf("\n");

		// The first, top-level XML node.
		Node* root = NULL;

		// A stack of open nodes for keeping track of children and value assignment.
		Stack<Node*> ancestors;

		// Character index into "file".
		long int index = 0;

		// Instantiate temporary String containers.
		String name, value, attr_name, attr_value;
		value.reserve(ELEMENT_VALUE_SIZE);

		// Parse the file.
		while (index < file_length)
		{
			// Check for an XML tag.
			if (input[index] == '<')
			{
				index++; // Skip over the character '<'.

				// Check if closing tag.
				if (input[index] == '/')
				{
					index++; // Skip the '/'

					// Assign the accumulated value to the currently open node.
					value.shrink_to_fit();
					ancestors.top()->value.swap(value);

					// String name;
					name.clear();
					name.reserve(ELEMENT_NAME_SIZE);

					// Parse the element's name.
					while (input[index] != '>' && !isWhitespace(input[index]))
					{
						name += input[index];
						index++;
					}

					// Check that the tag is in order
					if (name != ancestors.top()->name)
					{
						throw EasyXmlException("Mismatched closing tag \"" + name + "\". " \
						                       "Expected \"" + ancestors.top()->name + "\".",
						                       XML_MALFORMED_MISTMATCHED_CLOSING_TAG);
					}

					// Skip optional whitespace at the end of the element name.
					while (isWhitespace(input[index]))
					{
						index++;
					}

					// Since the element is now closed, it will not have any more children.
					ancestors.pop();
				}
				// Check if comment
				else if (strcmp2(&input[index], "!--", 3) == 0)
				{
					// Ignore comments
					while (!(input[index] == '-' && input[index + 1] == '-' && input[index + 2] == '>'))
					{
						index++;
					}

					index += 2; // Skip the "--" at the end of the comment.
				}
				// Check if prolog
				else if (input[index] == '?')
				{
					// Ignore prologs
					while (input[++index] != '?')
						;

					index++; // Skip the '?' at the end of the prolog.
				}
				// Check if DOCTYPE
				else if (strcmp2(&input[index], "!DOCTYPE", 8) == 0)
				{
					// Ignore DOCTYPES
					while (input[++index] != '>')
						;
				}
				// It must be an opening tag.
				else
				{
					// Reset value (should only contain the whitespace between this tag and the previous one).
					value.clear();
					value.reserve(ELEMENT_VALUE_SIZE);

					// String name;
					name.clear();
					name.reserve(ELEMENT_NAME_SIZE);

					bool found_whitespace = false;

					// Parse the start tag contents for the element name and any attributes.
					while (input[index] != '>' && input[index] != '/')
					{
						// If no whitespace has been found yet, the character is part of the element's name
						if (!found_whitespace)
						{
							name += input[index++];
						}
						// Otherwise it is an attribute.
						else
						{
							// String attr_name;
							attr_name.clear();
							attr_name.reserve(50);

							// Parse attribute name.
							while (input[index] != '=')
							{
								attr_name += input[index++];
							}

							// printf("parsed attr_name: %s\n", attr_name.c_str());

							index++; // Skip the equals sign.

							// Attributes can start with either a single or double quote
							char start_quote = input[index++];
							// String attr_value;
							attr_value.clear();
							attr_value.reserve(50);

							// Parse attribute value
							while (input[index] != start_quote)
							{
								attr_value += input[index++];
							}

							// printf("parsed attr_name: %s\n", attr_value.c_str());

							index++; // Skip the last quote.
						}

						// Skip whitespace
						while (isWhitespace(input[index]))
						{
							found_whitespace = true;
							index++;
						}
					}

					bool is_self_closing = false;

					// Skip the slash in a self-closing tag.
					if (input[index] == '/')
					{
						is_self_closing = true;
						index++;
					}

					// std::cout << "parsed tag name: " << name << "\n";

					// Create a node for this element.
					Node* node = new Node();
					name.shrink_to_fit();
					name.swap(node->name);

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
				value += input[index];
			}

			index++; // Increment to next character.
		}

		delete[] file_contents;

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
