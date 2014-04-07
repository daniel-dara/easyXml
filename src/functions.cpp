#include "functions.h"
#include "exception.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stack>
#include <cstring>

namespace EASYXML_NAMESPACE
{
	const std::string esc_sequences[] = {"&amp;", "&lt;", "&gt;", "&apos;", "&quot;"};
	const std::string esc_values[] = {"&", "<", ">", "'", "\""};

	Node* loadXml2(const std::string& filePath)
	{
		std::ifstream reader(filePath.c_str());

		Node* root = NULL;

		if (reader.is_open())
		{
			std::stack<Node*> ancestors;

			std::string file, line;
			// uint lineNumber = 0;

			// read file line by line
			while (getline(reader, line))
			{
				file += line;
			}

			int index = 0, len = file.length();

			// for values
			std::string value;
			value.reserve(500);

			while (index < len)
			{
				if (file[index] == '<')
				{
					index++; // skip '<'

					if (file[index] == '/') // parse closing tag
					{
						value.resize(value.length());
						ancestors.top()->value.swap(value);

						while (file[index++] != '>')
							; // skip end tag

						ancestors.pop();
					}
					else if (file[index] == '!' && file[index + 1] == '-' && file[index + 2] == '-')
					{
						while (!(file[index] == '-' && file[index + 1] == '-' && file[index + 2] == '>'))
						{
							index++;
							while (file[index] != '-') // shorter condition
							{
								index++;
							}
						}

						index += 2;
					}
					else if (file[index] == '?')
					{
						while (file[index++] != '?')
							;
					}
					else // opening tag
					{
						value = "";

						std::string name;
						name.reserve(100);

						while (file[index] != '>' && file[index] != '/')
						{
							// name[i++] = file[index++];
							name += file[index++];
						}

						// create node
						name.resize(name.length());
						Node* node = new Node(name);
						if (root == NULL)
						{
							root = node;
						}
						else
						{
							ancestors.top()->children.insert(node);
						}

						if (file[index] == '/')
						{
							while (file[index++] != '>')
								; // whitespace
						}
						else
						{
							ancestors.push(node);
						}
					}
				}
				else
				{
					// parse value
					value += file[index];
				}

				index++; // next char
			}

			reader.close();
		}
		else
		{
			throw EasyXmlException("Unable to open file \"" + filePath + "\".", 101);
		}

		return root;
	}

	Node* loadXml(const std::string& filePath)
	{
		std::ifstream reader(filePath.c_str());

		if (reader.is_open())
		{
			std::stack<Node*> ancestors;
			Node* root = NULL;

			std::string line, line2;
			std::string value;
			uint lineNumber = 0;

			// read file line by line
			while (getline(reader, line2))
			{
				// lineNumber++;
				line += line2;
			}
				size_t index = 0;
				lineNumber = 1;

			{
				// mixed content should retain new lines between actual values
				// line += '\n';

				while (index < line.length())
				{
					size_t openIndex = line.find('<', index);
					size_t closeIndex = line.find("</" , index, 2);
					const size_t notFound = std::string::npos;

					if (openIndex == notFound && closeIndex == notFound)
					{ 
						// the rest of this line is probably a value so save it
						value += line.substr(index);
						index = line.length();
					}
					// Check if this is an opening tag.
					else if (openIndex < closeIndex || closeIndex == notFound)
					{ 
						std::string name = getElementName(line, openIndex + 1);

						// Make sure the tag is not empty.
						if (name.length() == 0)
						{
							throw EasyXmlException("No name in opening tag on line %d.", 9, lineNumber);
						}
						// Check if the tag is a comment.
						else if (name.length() >= 3 && name.substr(0, 3) == "!--")
						{
							std::string comment;

							size_t endIndex = openIndex + 4; // Index of comment close.
							size_t startIndex = endIndex; // Index to start search (after "!--")
							const uint origLineNumber = lineNumber; // Save the line number of the opening tag.

							// If the end of the comment is not on this line, we must keep searching.
							while ( (endIndex = line.find("-->", startIndex)) == notFound )
							{
								// Store comment contents, for use in future versions of easyXml.
								comment += line.substr(startIndex) + "\n";

								// Reset startIndex to the beginning of the new line.
								startIndex = 0; 

								if (!getline(reader, line))
								{
									throw EasyXmlException("Unclosed comment at line %d.", 8, origLineNumber);
								}

								lineNumber++;
							}

							comment += line.substr(startIndex, endIndex - startIndex);

							index = endIndex + 3;

							// TODO: Possibly store comments in the xml tree somehow, useful for preserving
							// them if data is to be written back to a file.
						}
						// Check if the tag is an xml Prolog.
						else if (name.length() >= 4 && name.substr(0, 4) == "?xml")
						{
							if (lineNumber != 1)
							{
								throw EasyXmlException("XML Prolog must appear on the first line." \
								                       " Prolog found on line %d.", 6, lineNumber);
							}

							if (name[name.length() - 1] != '?')
							{
								throw EasyXmlException("Malformed XML Prolog. Must end with \"?>\" instead" \
								                       " of \">\" at line %d.", 7, lineNumber);
							}

							// Use the original name since the new name may have been self-closing.
							index = openIndex + 1 + name.length() + 1;

							// TODO: Parse xml Prolog for document information.
						}
						// It must be a node.
						else
						{
							int tagLength = name.length();

							bool isSelfClosing = (name[name.length() - 1] == '/');

							// Check for attributes
							std::string attributes = "";
							size_t attrIndex = name.find(" ");

							// Fix the name if there were attributes
							if (attrIndex != notFound)
							{
								attributes.swap(name); // quicker than copy
								name = attributes.substr(0, attrIndex);
							}

							Node* newNode = new Node(name);

							// Parse attributes
							while (attrIndex != notFound && Trim(attributes.substr(attrIndex)) != "/")
							{
								size_t equalsIndex = attributes.find("=", attrIndex + 1);

								if (equalsIndex == notFound)
								{
									throw EasyXmlException("Equals sign not found after attribute name" \
									                       " at line %d.", 10, lineNumber);
								}

								size_t firstQuoteIndex = attributes.find("\"", equalsIndex + 1);

								if (firstQuoteIndex == notFound)
								{
									throw EasyXmlException("Missing double quote after equals sign in" \
									                     " element attribute at line %d.", 11, lineNumber);
								}

								size_t secondQuoteIndex = attributes.find("\"", firstQuoteIndex + 1);

								if (secondQuoteIndex == notFound)
								{
									throw EasyXmlException("Missing double quote after the value in" \
									                     " element attribute at line %d.", 12, lineNumber);
								}

								int attrNameLength = equalsIndex - attrIndex;
								int attrValLength = secondQuoteIndex - firstQuoteIndex - 1;

								// Parse attribute name and remove whitespace
								std::string attrName = attributes.substr(attrIndex, attrNameLength);
								attrName = trim(attrName);

								// Parse attribute value
								std::string attrVal = attributes.substr(firstQuoteIndex + 1, attrValLength);

								newNode->attributes.insert(Node::Attribute(attrName, attrVal));

								attrIndex = attributes.find(" ", secondQuoteIndex + 1);
							}

							if (isSelfClosing)
							{
								// Trim the '/' and any whitespace in between the name and closing bracket.
								// We must save the original name so we know how far to advance the index
								// pointer used for pasring.
								std::string temp = name.substr(0, name.length() - 1);
								newNode->name = rtrim(temp);
							}

							if (!ancestors.empty())
							{
								// To support mixed content, store any previously found values into parent
								// if (ancestors.top()->name == "car")
								// 	std::cout << "val add to: " + value << std::endl;

								ancestors.top()->value += value;

								// if (ancestors.top()->name == "car")
								// 	std::cout << "car val after: " + root->value << std::endl;
								value = "";
								ancestors.top()->children.insert(newNode);
							}
							else
							{
								if (root != NULL)
								{
									// 2.1.2a
									throw EasyXmlException("Malformed XML: Multiple root nodes found." \
										" First root node is \"" + root->name + "\", second node is \"" + \
										newNode->name + "\" defined at line %d.", 2, lineNumber);
								}

								root = newNode;
							}

							// If the tag is self closing, do not push it since it will have no closing tag.
							if (!isSelfClosing)
							{
								ancestors.push(newNode);
							}

							// Use the length of the original name since the new name may be shortened.
							index = openIndex + 1 + tagLength + 1;
						}
					}
					// Closing tag.
					else
					{
						std::string elementName = getElementName(line, closeIndex + 2);

						if (ancestors.top() == root && root->name != elementName)
						{
							// 2.1.2d
							throw EasyXmlException("Malformed XML: No opening tag for \"" + 
								elementName + "\", closing tag found at line %d.", 3, lineNumber);
						}

						if (ancestors.top()->name != elementName)
						{
							// 2.1.2b
							throw EasyXmlException("Malformed XML: Mismatched closing tag at line %d." \
								" Expected \"" + ancestors.top()->name + "\" found \"" + elementName + "\"." \
								, 5, lineNumber);
						}


						value += line.substr(index, openIndex - index);

						std::string temp = value;

						// Replace XML escape sequences. Ampersands must be replaced last or else they may
						// cause other escape sequences to be replaced that were not in the original string.
						for (int i = 4; i >= 0; i--)
						{
							replaceAll(temp, esc_sequences[i], esc_values[i]);
						}

						ancestors.top()->value += temp;
						value = "";

						index = closeIndex + 2 + ancestors.top()->name.length() + 1;
						ancestors.pop();
					}
				}
			}

			if (root == NULL)
			{
				// 2.1 Rule 1
				throw EasyXmlException("No XML elements found in file \"" + filePath + "\".", 1);
			}

			if (!ancestors.empty())
			{
				// 2.1 Rule 2
				throw EasyXmlException("Unclosed XML element \"" + ancestors.top()->name + "\".", 4);
			}

			reader.close();

			return root;
		}
		else
		{
			throw EasyXmlException("Unable to open file \"" + filePath + "\".", 101);
			return NULL;
		}
	}

	void saveXml(const Node* node, const std::string& filePath)
	{
		std::ofstream file;
		file.open(filePath.c_str());
		saveXml(node, file);
		file.close();
	}

	void saveXml(const Node* node, std::ostream& out, std::string indentation)
	{
		out << indentation << "<" << node->name;

		if (node->children.size() > 0)
		{
			out << ">\n";

			for (std::set<Node*>::const_iterator it = node->children.begin(); \
			     it != node->children.end(); ++it)
			{
				saveXml(*it, out, indentation + "\t");
			}

			out << indentation << "</" << node->name << ">";
		}
		else if (node->value == "")
		{
			out << " />";
		}
		else
		{
			std::string value(node->value);

			// Replace bad chars with XML escape sequences. Ampersands must be escaped first otherwise
			// ampersands from other escape sequences will be double encoded.
			for (int i = 0; i < 5; i++)
			{
				replaceAll(value, esc_values[i], esc_sequences[i]);
			}

			out << ">" << value;
			out << "</" << node->name << ">";
		}

		out << "\n";
	}

	void printTree(const Node* node, std::string indentation)
	{
		std::cout << indentation << node->name << ": " << node->value << std::endl;

		if (node->attributes.size() != 0)
		{
			std::cout << "\t" + indentation + "attributes:";

			std::set<Node::Attribute>::const_iterator it;
			for (it = node->attributes.begin(); it != node->attributes.end(); ++it)
			{
				std::cout << " " << (*it).name << "=\"" << (*it).value << "\"";
			}

			std::cout << std::endl;
		}

		// increase the indentation for the next level
		indentation += "\t";

		std::set<Node*>::const_iterator it;
		for (it = node->children.begin(); it != node->children.end(); ++it)
		{
			printTree((*it), indentation);
		}
	}

	void deleteTree(Node* node)
	{
		if (node == NULL)
		{
			throw EasyXmlException("deleteTree was called on a NULL pointer.", 102);
		}
		else
		{
			std::set<Node*>::iterator it;
			for (it = node->children.begin(); it != node->children.end(); ++it)
			{
				deleteTree(*it);
			}

			delete node;
		}
	}

	std::string getElementName(const std::string& data, size_t startIndex)
	{
		return data.substr(startIndex, data.find(">", startIndex) - startIndex);
	}

	// The following 3 functions are public domain: http://stackoverflow.com/a/217605
	// They perform in-place trims.

	// trim from start
	std::string& ltrim(std::string& s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), \
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	std::string& rtrim(std::string& s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), \
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
