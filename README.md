# easyXml

## Description

I got sick of the myriad of XML parsers that are overly-complex and aren't very user-friendly. "easyXml" is a DOM-Style XML parser for C++. It is designed to be extremely simple to install and use.

Currently, it only supports parsing a small subset of the XML specifications and manipulating XML nodes on a very basic level. Development is in progress and I will frequently be adding new features to make easyXml more practical.

## Installation

Note: The makefile currently uses clang++ as the compiler so you can either install that package or you can change the first line of the makefile to <code>CC=g++</code> to use the GNU compiler.

After downloading the repo, run:

    sudo make install
    
Use <code>#include &lt;easyXml.h&gt;</code> to include the easyXml library in your code. When compiling your program, use the flag <code>-lEasyXml</code>.
    
Ex:

    g++ your_source.cpp -lEasyXml
    
## Documentation

For now, this readme and the example program are the only documentation. I have written the example program, <code>examples/example1.cpp</code>, such that it shows simple uses for (nearly) all of easyXml's current features. I will add more code to the sample program as new features are implemented. Eventually I plan to produce formal documentation.

# Development Notes

## Features Implemented
A rough outline of features supported by easyXml.

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

## Todo List
  * High Priority:
    * Support/enforce XML 1.0 specifications
      * elements with mixed content
      * element attributes
      * element naming rules
      * empty element syntax
  * Medium Priority:
    * Allow writing a tree of nodes to an xml file
    * Consider creating xmlTree class to manage an XML tree rather than use global functions.
      * Will depend on amount of functionality "xmlTree"s need.
  * Low Priority:
    * Write alternative classes for STL set and stack to reduce dependencies and overhead for statically linked programs.
    * Offer option for "children" data structure:
      * A binary tree/set (current setup) offers logarithmic lookup
        * But does not maintain order of elements at the same tree level
      * A vector/linked list would take linear time to search but would maintain element order

## Unit Testing
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
