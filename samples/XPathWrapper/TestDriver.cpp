#include <iostream>
#include <fstream>

#include "XPathWrapper.hpp"



class XMLException;



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
	using std::ifstream;
	using std::string;
	using std::vector;
#endif

	if (argc < 4)
	{
		cerr << "Usage: TestDriver XMLFilePath Context XPathExpression" << endl;

		return -1;
	}

	string		theXML;

	ifstream	in(argv[1]);

	// slow and dirty dump of the xml file into a buffer
	char c;
	while(in.get(c))
		theXML += c;

	///////////////////////////////////////////..

	// create your XPath helper object
	XPathWrapper helper;

	try
	{
		// call evaluate, passing in the XML string, the context string and the xpath string
		const vector<string>	result = helper.evaluate(theXML, argv[2], argv[3]);

		// take the resulting string vector	and do whatever you want with it:
		size_t len = result.size();

		cout<< "the result set has " << len << " strings\n";

		for (size_t i=0; i<len; i++)
			cout<< "item " << (i+1) << "= \"" << result[i] << "\"" << endl;
	}
	catch(const XMLException&)
	{
		cerr << "Exception caught!  Exiting..." << endl;
	}

	return 0;
}
