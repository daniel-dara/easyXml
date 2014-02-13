#include "parser.h"

Node* Parser::loadXml(const std::string filePath)
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

					Node* newNode = new Node(getElementName(line, openIndex + 1), ancestors.empty());

					if (!newNode->isRoot)
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
							throw easyXmlException("Malformed XML: There must be one top-level node." \
								" First top-level node is \"" + root->name + "\". Second is \"" + \
								newNode->name + "\" defined at line %d.", lineNumber);
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
						throw easyXmlException("Malformed XML: No opening tag for \"" + 
							elementName + "\", closing tag found at line %d.", lineNumber);
					}

					if (ancestors.top()->name.compare(elementName) != 0)
					{
						throw easyXmlException("Malformed XML: Mismatched closing tag at line %d." \
							" Expected \"" + ancestors.top()->name + "\" found \"" + elementName + "\"." \
							, lineNumber);
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

		if (!ancestors.empty())
		{
			throw easyXmlException("Unclosed XML element \"" + ancestors.top()->name + "\".");
		}

		reader.close();
std::cout << "root->val: " + root->value << std::endl;
		return root;
	}
	else
	{
		throw easyXmlException("Unable to find or open file \"" + filePath + "\".");
		return NULL;
	}
}

Parser::Parser()
{}

// public domain: http://stackoverflow.com/a/17620909
void Parser::replaceAll(std::string& str, const std::string& from, const std::string& to)
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

std::string Parser::getElementName(const std::string& data, int startIndex)
{
	return data.substr(startIndex, data.find(">", startIndex) - startIndex);
}
