
#include "Attribute.h"

namespace EASYXML_NAMESPACE
{
	Attribute::Attribute(const String& name, const String& value) :
		name(name),
		value(value)
	{ }

	Attribute* Attribute::getNextSibling() {
		return next_sibling_;
	}
}
