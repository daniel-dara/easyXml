#include "functions.h"

namespace EASYXML_NAMESPACE
{
	std::string esc_sequences[] = {"&amp;", "&lt;", "&gt;", "&apos;", "&quot;"};
	std::string esc_values[] = {"&", "<", ">", "'", "\""};

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
				// line += '\n';

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
					// Check if this is an opening tag.
					else if (openIndex < closeIndex || closeIndex == -1)
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
							if (name.length() < 5 || name.substr(name.length() - 2) != "--")
							{
								throw EasyXmlException("XML Comment must end with \"-->\" at line %d.", \
								                       7, lineNumber);
							}

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

							// TODO: Parse xml Prolog for document information.
						}
						// It must be a node.
						else
						{
							bool isSelfClosing = (name[name.length() - 1] == '/');

							Node* newNode = new Node(name);

							if (isSelfClosing)
							{
								// Trim the '/' and any whitespace in between the name and closing bracket.
								// We must save the original name so we know how far to advance the index
								// pointer used for pasring.
								newNode->name = name.substr(0, name.length() - 1);
								newNode->name = rtrim(newNode->name);
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
						}

						// Use the original name since the new name may have been self-closing.
						index = openIndex + 1 + name.length() + 1;
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

						// Replace XML escape sequences. Ampersands must be replaced last or else they may
						// cause other escape sequences to be replaced that were not in the original string.
						for (int i = 4; i >= 0; i--)
						{
							replaceAll(value, esc_sequences[i], esc_values[i]);
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
		out << indentation << "<" << node->getName();

		if (node->children.size() > 0)
		{
			out << ">\n";

			std::set<Node*>::const_iterator it;
			for (it = node->children.begin(); it != node->children.end(); ++it)
			{
				saveXml(*it, out, indentation + "\t");
			}

			out << indentation << "</" << node->getName() << ">";
		}
		else if (node->val() == "")
		{
			out << " />";
		}
		else
		{
			std::string value(node->val());

			// Replace bad chars with XML escape sequences. Ampersands must be escaped first otherwise
			// ampersands from other escape sequences will be double encoded.
			for (int i = 0; i < 5; i++)
			{
				replaceAll(value, esc_values[i], esc_sequences[i]);
			}

			out << ">" << value;
			out << "</" << node->getName() << ">";
		}

		out << "\n";
	}

	void printTree(const Node* node, std::string indentation)
	{
		std::cout << indentation << node->name << ": " << node->value << std::endl;

		// increase the indentation for the next level
		indentation += "\t";

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
			throw EasyXmlException("deleteTree was called on a NULL pointer.", 102);
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

		std::string newStr;
		newStr.reserve(str.length());

		size_t start_pos = 0, pos;
		while ((pos = str.find(from, start_pos)) != std::string::npos)
		{
			newStr += str.substr(start_pos, pos - start_pos);
			newStr += to;
			start_pos = pos + from.length();
		}
		newStr += str.substr(start_pos);
		str.swap(newStr); // faster than str = newStr;
	}
}
