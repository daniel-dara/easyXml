#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "namespace.h"
#include "String.h"

namespace EASYXML_NAMESPACE
{
	class Attribute
	{
	public:
		friend class Input;

		Attribute(const String& name, const String& value);

		String name;
		String value;

		Attribute* getNextSibling();

	private:
		Attribute* next_sibling_;
	};
}

#endif