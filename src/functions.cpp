#include "functions.h"

namespace EASYXML_NAMESPACE
{
	Node* loadXml(const std::string filePath)
	{
		std::ifstream reader(filePath.c_str());

		if (reader.is_open())
		{
			std::stack<Node*> ancestors;
			Node* root = NULL;

			std::string line;
			std::string value = "";
			int lineNumber = 0;

			// read file line by line
			while (getline(reader, line))
			{
				lineNumber++;
				unsigned int index = 0;

				// mixed content should retain new lines between actual values
				line += '\n';

				while (index < line.length())
				{
					int openIndex = static_cast<int>(line.find('<', index));
					int closeIndex = static_cast<int>(line.find("</" , index, 2));

					if (openIndex == -1 && closeIndex == -1)
					{ 
						// the rest of this line is probably a value so save it
						value += line.substr(index);
						index = line.length();
					}
					else if (openIndex < closeIndex || closeIndex == -1)
					{ 
						// opening tag found

						Node* newNode = new Node(getElementName(line, openIndex + 1));

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
								throw EasyXmlException("Malformed XML: There must be only one root node." \
									" First root node is \"" + root->name + "\", second is \"" + \
									newNode->name + "\" defined at line %d.", 1, lineNumber);
							}

							root = newNode;
						}

						ancestors.push(newNode);
						index = openIndex + 1 + newNode->name.length() + 1;
					}
					else
					{
						// closing tag found

						std::string elementName = getElementName(line, closeIndex + 2);

						if (ancestors.top()->name.compare("root") == 0)
						{
							throw EasyXmlException("Malformed XML: No opening tag for \"" + 
								elementName + "\", closing tag found at line %d.", 3, lineNumber);
						}

						if (ancestors.top()->name.compare(elementName) != 0)
						{
							throw EasyXmlException("Malformed XML: Mismatched closing tag at line %d." \
								" Expected \"" + ancestors.top()->name + "\" found \"" + elementName + "\"." \
								, 2, lineNumber);
						}


						value += line.substr(index, openIndex - index);

						// Replace XML escape sequences.
						std::string sequences[] = {"&lt;", "&gt;", "&amp;", "&apos;", "&quot;"};
						std::string values[] = {"<", ">", "&", "'", "\""};

						for (int i = 0; i < 5; i++)
						{
							replaceAll(value, sequences[i], values[i]);
						}

						ancestors.top()->value += value;
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
			throw EasyXmlException("Unable to find or open file \"" + filePath + "\".", 101);
			return NULL;
		}
	}

	void printTree(const Node* node, std::string indentation)
	{
		std::cout << (indentation += "\t") + node->name << ": " << node->value << std::endl;

		std::set<Node*, Node::node_ptr_compare>::iterator it;
		for (it = node->children.begin(); it != node->children.end(); ++it)
		{
			printTree((*it), indentation);
		}
	}

	void deleteTree(Node* node)
	{
		if (node == NULL)
		{
			throw EasyXmlException("deleteTree was called on a NULL pointer.", 101);
		}
		else
		{
			std::set<Node*, Node::node_ptr_compare>::iterator it;
			for (it = node->children.begin(); it != node->children.end(); ++it)
			{
				deleteTree(*it);
			}

			delete node;
		}
	}

	std::string getElementName(const std::string& data, int startIndex)
	{
		return data.substr(startIndex, data.find(">", startIndex) - startIndex);
	}

	// The following 3 functions are public domain: http://stackoverflow.com/a/217605

	// trim from start
	std::string& ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), \
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	std::string& rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), \
			std::not1(std::ptr_fun<int, int>(std::isspace))).base() ,s.end());
		return s;
	}

	// trim from both ends
	std::string& trim(std::string &s)
	{
		return ltrim(rtrim(s));
	}

	// public domain: http://stackoverflow.com/a/17620909
	void replaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return;
		}

		std::string wsRet;
		wsRet.reserve(str.length());

		size_t start_pos = 0, pos;
		while ((pos = str.find(from, start_pos)) != std::string::npos)
		{
			wsRet += str.substr(start_pos, pos - start_pos);
			wsRet += to;
			pos += from.length();
			start_pos = pos;
		}
		wsRet += str.substr(start_pos);
		str.swap(wsRet); // faster than str = wsRet;
	}
}
