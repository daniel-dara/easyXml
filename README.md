# Description

I got sick of the myriad of XML parsers that are overly-complex to use and don't provide the basic features I expected an XML parser to have. easyXml is designed to be the easiest to use XML parser. I also plan for it to be simple to setup.

# Installation

Not yet available.

# Features Implemented
  * class "Node" that provides access to an XML elements name, value, and children.
    * Has function "printTree" to print the XML tree starting from the given node
    * Has function "findElement" to find an element with the given name
  * Parse an XML file with the following:
    * elements with values
    * elements with children
    * require top-level element
    * error handling for:
      * mismatched tags
      * missing tags

# TODO
  * Add support for:
    * elements with mixed content
    * element attributes
    * element naming rules
    * empty elements
  
