
#include "Input.h"
// #include "functions.h"

namespace EASYXML_NAMESPACE
{
	Input::Input(const char* input_string, long int input_length) :
		input_(input_string),
		input_length_(input_length),
		index_(0)
	{ }

	bool Input::end()
	{
		return index_ >= input_length_;
	}

	bool Input::consumeChar()
	{
		// TODO: ADD EOF VALIDATION
		index_++;
		return true;
	}

	bool Input::consumeChar(char& c)
	{
		// TODO: ADD EOF VALIDATION
		c = input_[index_++];
		return true;
	}

	bool Input::consume(char c)
	{
		if (input_[index_] == c)
		{
			consumeChar();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Input::consume(const char* s, uint length)
	{
		// TODO: EOF VALIDATION in strcmp2
		if (strcmp2(&input_[index_], s, length) == 0)
		{
			index_ += length;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Input::consumeComment()
	{
		bool foundComment = false;

		if (consume("<!--", 4))
		{
			foundComment = true;

			while (!consume("-->", 3))
			{
				consumeChar();
			}
		}

		return foundComment;
	}

	bool Input::consumeNodeName(String& name)
	{
		bool foundName = false;

		while (input_[index_] != '>' && input_[index_] != '/' && !isWhitespace(input_[index_]))
		{
			foundName = true;
			name += input_[index_];
			consumeChar();
		}

		return foundName;
	}

	bool Input::consumeEndTag(String& name)
	{
		// TODO: XML VALIDATION
		return consume("</", 2) && consumeNodeName(name) && consume('>');
	}

	bool Input::consumeStartTag(String& name, Attribute* first_attr, bool &is_self_closing)
	{
		first_attr = NULL;

		// TODO: XML VALIDATION
		if (index_ < input_length_ + 1 &&
			(consume('<')  ||
		    consume("</", 2) ||
		    consume("<?", 2) ||
		    consume("<!", 2)))
		{
			consumeNodeName(name);
			consumeWhitespace();

			Attribute* last_attr = NULL;

			while (!consume('>') && !(is_self_closing = consume("/>", 2)))
			{
				String attribute_name;
				String attribute_value;

				consumeNodeName(attribute_name);

				consumeWhitespace();
				consume('=');
				consumeWhitespace();

				consume('"');

				char c;
				while (consumeChar(c) && c != '"')
				{
					attribute_value += c;
				}

				Attribute* attr = new Attribute(attribute_name, attribute_value);

				if (first_attr == NULL)
				{
					last_attr = first_attr = attr;
				}
				else
				{
					last_attr->next_sibling_ = attr;
					last_attr = attr;
				}

				consumeWhitespace();
			}

			return true;
		}

		return false;
	}

	bool Input::consumeWhitespace()
	{
		int original_index = index_;

		while (index_ < input_length_ && isWhitespace(input_[index_]))
		{
			index_++;
		}

		return original_index != index_;
	}

	bool Input::consumeProlog()
	{
		// TODO: XML VALIDATION (of prolog)
		if (consume("<?", 2))
		{
			while (!consume("?>", 2))
			{
				consumeChar();
			}

			return true;
		}

		return false;
	}

	bool Input::consumeDocType()
	{
		// TODO: XML VALIDATION - Validate (or more likely ignore) DOCTYPEs.
		if (consume("<!DOCTYPE", 9))
		{
			throw EasyXmlException("DOCTYPEs are not currently supported and can't be ignored.",
			                       UNSUPPORTED_FEATURE);
		}

		return false;
	}

	char Input::peek()
	{
		// TODO: EOF VALIDATION
		return input_[index_];
	}

	// This function requires that each string be at least (len - 1) characters.
	// Compare the first 'len' characters between two strings returning:
	// -1 if lhs < rhs
	// 0  if lhs = rhs
	// 1  if lhs > rhs
	int Input::strcmp2(const char* lhs, const char* rhs, uint len)
	{
		for (uint i = 0; i < len; i++)
		{
			if (lhs[i] != rhs[i])
			{
				return (lhs[i] > rhs[i]) - (lhs[i] < rhs[i]);
			}
		}

		return 0;
	}

	bool Input::isWhitespace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}
}
