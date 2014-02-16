# Description

I got sick of the myriad of XML parsers that are overly-complex to use and don't provide the basic features I expected an XML parser to have. "easyXml" is a C++ XML parser designed to be dead simple to install and use.

It is currently usable however it does not yet implement all common XML features.

# Installation

After downloading the repo, run "sudo make install". Use the include

    #include <easyXml.h>
    
in your code and compile it with the flag

    -lEasyXml
    
Ex:

    g++ your_source.cpp -lEasyXml

For now, see examples/example1.cpp for usage.

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

# Error Codes
0 - No error code set
1 - Multiple root nodes
2 - Mismatched tags
3 - No opening tag
4 - No closing tag
100 - unable to open file
101 - deleteTree on NULL pointer