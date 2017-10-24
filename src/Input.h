#ifndef INPUT_H
#define INPUT_H

#include "namespace.h"
#include "String.h"
#include "Attribute.h"
#include "Exception.h"

namespace EASYXML_NAMESPACE
{
	class Attribute;

	class Input
	{
		public:
			Input(const char* input_string, long int input_length);

			bool end();
			bool consumeChar();
			bool consumeChar(char& c);
			bool consume(char c);
			bool consume(const char* s, uint length);
			bool consumeComment();
			bool consumeNodeName(String& name);
			bool consumeEndTag(String& name);
			bool consumeStartTag(String& name, Attribute* first_attr, bool &is_self_closing);
			bool consumeWhitespace();
			bool consumeProlog();
			bool consumeDocType();
			char peek();

		private:
			const String input_;
			const long int input_length_;
			long int index_;

			int strcmp2(const char* lhs, const char* rhs, uint len);
			bool isWhitespace(char c);
	};
}

#endif
