# Description

I got sick of the myriad of XML parsers that are overly-complex to use and don't provide the basic features I expected an XML parser to have. "easyXml" is designed to be the easiest to use and setup XML parser.

It is currently usable however it does not yet implement all common  XML features.

# Installation

Not yet available.

# Features Implemented
  * class "Node" that provides access to an XML element's name, value, and children.
    * contains function "findElement" to find an element with the given name (or path)
    * global functions for printing and deleting a tree starting from the given node.
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
    * Writing a tree of class Node back to an xml file.