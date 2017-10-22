# easyXml

## Description

easyXml is an XML Library for C++. It started as an ambitious side project during my college years that inevitably lost steam after a few months. In that short time, I was able to accomplish rudimentary XML parsing and traversal of the XML tree with a basic Node entity. It was functional enough to use for loading configurations in some of my school side projects.

In order to optimize performance, I refactored the parsing implementation away from std::string and indexOf to instead use a character by character technique. This implementation currently lives in the dev branch. It is nearly complete but also has a few bugs that are yet to be worked out. 

I am not actively developing on either branch but some day would like to finish the parsing logic in the dev branch and merge it into master.

## Installation

### Linux / OSX

Use of the makefile requires the packages <code>make</code>, <code>g++</code> or <code>clang++</code> (depending on OS), and <code>ar</code>. These packages are typically pre-installed.

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

The library can be uninstalled likewise

    sudo make install

### Windows

I haven't tried to compile this project on Windows but there are no Unix-specific dependencies. You should be able to compile easyXml by downloading and using GNU Make for Windows and the MinGW compiler. You can then place the generated library file in MinGW's lib folder (in the MinGW install directory).
    
## Documentation

This readme, the example programs, and code comments are currently the only documentation. I have written the example program <code>examples/easy_example.cpp</code> to explain the most simple usage of easyXml. The source <code>examples/complete_example.cpp</code> provides a more in depth look at easyXml's functionality. I will add more code to the complete example program as new features are implemented. I plan to eventually produce formal documentation.

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

### Testing
The "tests" directory is dedicated to testing. Within that directory, there is a file named "test_cases.txt". Each row in the file represents a set of arguments to pass to the unit_test program. Each column is a single argument: the first being the test identifier and the second being the exepected error code. The test program will use these arguments to read in the appropriate xml file from the "xml" folder and check the results against the expected error code. "make test" will compile the unit_test program (if necessary) and run all tests. Each test will output "PASS" or "FAIL" along with the test identifier, the received error code, and on failure, the expected error code.

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
