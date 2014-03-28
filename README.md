# easyXml

## Description

easyXml is a DOM-Style XML parser written in C++. It is designed to be extremely simple to install and use.

I created easyXml because during my endeavors to find a decent XML parser, I got frustrated with the lack of intuitive interface design, missing sample code, and poor documentation of other libraries. I wrote easyXml to meet my own needs and to help others who are tired of overly complex parsers.

This is a young project so the documentation aspect is still to come, however, easyXml is so simple I recommend you learn it from the sample code anyway! I found that when I was looking for an XML parser myself, some decent sample code with proper explanations would have been worth a thousand pages of documentation.

Currently, easyXml only supports a small subset of the XML specifications and features. However, development is in progress and I plan to add these important features in the near future.

easyXml is offered "as is" so use at your own risk. I have not stress tested this library and it has no guaranteed performance level. It works fine for my needs of reading a basic, hundred line XML file and I hope you find it useful.

I love feedback so feel free to contact me about bugs, feature requests, or any comments you might have.

## Installation

### Linux / OSX

Note: The makefile currently uses clang++ as the compiler so you can either install clang or you can change the first line of the makefile to <code>CC=g++</code> to use the GNU compiler.

Download the repo with either

    git clone https://github.com/danielsd/easyXml.git

or

    wget https://github.com/danielsd/easyXml/archive/master.zip
    unzip master.zip

Then navigate into the directory

    cd easyXml # if you used "git clone"

or

    cd easyXml-master # if you used "wget"

Once in the repo directory, run

    sudo make install
    
Use <code>#include &lt;easyXml.h&gt;</code> to include the easyXml library in your code. When compiling your program, use the flag <code>-lEasyXml</code>.
    
Ex:

    g++ your_source.cpp -lEasyXml

### Windows

I currently do not support installing this library on Windows however easyXML has no Unix dependencies. You could either compile the code yourself into a static library to link with your projects or directly compile your code with the files in the <code>src</code> folder.
    
## Documentation

For now, this readme and the example programs are the only documentation. I have written the example program <code>examples/easy_example.cpp</code> to explain the most simple usage of easyXml. The source <code>examples/complete_example.cpp</code> provides a more in depth look at easyXml's functionality. I will add more code to the complete example program as new features are implemented. Eventually I plan to produce formal documentation.

## Development Notes

### Features Implemented
A rough outline of features supported by easyXml.

  * class "Node" that provides access to an XML element's name, value, and children.
    * contains function "findNode" to find an element with the given name (or path)
    * stand-alone functions for printing and deleting a tree starting from the given node.
  * Parse an XML file with the following:
    * elements with values
    * elements with children
    * empty elements (self-closing)
    * comments
    * a prolog (ignored)
    * basic xml validation (not complete)
    * error handling for:
      * mismatched tags
      * missing tags
  * Write node data to a file
    * Does not include comments or prologs

### Todo List
  * High Priority:
    * Support/enforce XML 1.0 specifications
      * elements with mixed content
      * element attributes
      * element naming rules
  * Medium Priority:
    * Consider creating xmlTree class to manage an XML tree rather than use global functions.
      * Will depend on amount of functionality "xmlTree"s need.
  * Low Priority:
    * Modify Node class to use an STL set for lookup efficiency but an STL list to preserve original order.
    * Write alternative classes for STL set and stack to reduce dependencies and overhead for statically linked programs.

### Unit Testing
The "tests" directory is dedicated to unit testing. Within that directory, there is a file named "test_cases.txt". Each row in the file represents a set of arguments to pass to the unit_test program. Each column of the is a single argument: the first being the test identifier and the second being the exepected error code. The unit test program will use these arguments to read in the appropriate xml file from the "xml" folder and check the results against the expected error code. "make test" will compile the unit_test program (if necessary) and run all tests. Each test will output "PASS" or "FAIL" along with the test identifier, the received error code, and on failure, the expected error code.

All tests should be run together to ensure that modifications to the library for one particular feature do not break another. My initial unit tests use identifiers that correspond to the section of the XML 1.0 Specifications (5th edition) they test for. I will use a different identifier format for bugs and additional features that I have yet to decide on.

### Error Codes:
0-99 are XML specification violations, 100-199 are other errors

    0 - No error code set
    1 - No elements in xml file
    2 - Multiple root nodes
    3 - No opening tag
    4 - Unclosed tag
    5 - Mismatched closing tag
    6 - XML Prolog not on first line
    7 - Malformed Prolog
    8 - Malformed Comment (end tag)
    9 - No opening tag name
    101 - unable to open file
    102 - deleteTree called on NULL pointer
    103 - Child element not found (when using Node.find() method)
