#include <iostream>
#include <fstream>

#include "XPathWrapper.hpp"

int main(int argc, const char* argv[])
{
	if (argc<4)
	{
		std::cerr << "Syntax: TestDriver XMLFilePath Context XPathExpression\n";
		return -1;
	}

	std::string theXML;

	std::ifstream in(argv[1]);

	// slow and dirty dump of the xml file into a buffer
	char c;
	while(in.get(c))
		theXML += c;

	///////////////////////////////////////////..

	// create your XPath helper object
	XPathWrapper helper;

	// call evaluate, passing in the XML string, the context string and the xpath string
	std::vector<std::string> result = helper.evaluate(theXML, argv[2], argv[3]);

	// take the resulting string vector	and do whatever you want with it:
	size_t len = result.size();

	std::cout<< "the result set has " << len << " strings\n";

	for (size_t i=0; i<len; i++)
		std::cout<< "item " << (i+1) << "= \"" << result[i] << "\"" << std::endl;


	return 0;
}