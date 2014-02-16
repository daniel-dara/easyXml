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

# Unit Testing
The "tests" directory is dedicated to unit testing. Within that directory, there is a file named "test_cases.txt". Each row in the file represents a set of arguments to pass to the unit_test program. Each column of the is a single argument: the first being the test identifier and the second being the exepected error code. The unit test program will use these arguments to read in the appropriate xml file from the "xml" folder and check the results against the expected error code. "make test" will compile the unit_test program (if necessary) and run all tests. Each test will output "PASS" or "FAIL" along with the test identifier, the received error code, and on failure, the expected error code.

All tests should be run together to ensure that modifications to the library for one particular feature do not break another. My initial unit tests use identifiers that correspond to the section of the XML 1.0 Specifications (5th edition) they test for. I will use a different identifier format for bugs and additional features that I have yet to decide on.

# Error Codes:
0-99 are XML specification violations, 100-199 are other errors

0 - No error code set
1 - No elements in xml file
2 - Multiple root nodes
3 - No opening tag
4 - Unclosed tag
5 - Mismatched closing tag
101 - unable to open file
102 - deleteTree called on NULL pointer
103 - Child element not found (when using Node.find() method)