#include "XPathWrapper.hpp"



#include <util/XMLException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <fstream.h>
#include <iostream.h>
#else
#include <fstream>
#include <iostream>
#endif



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
	using std::vector;
#endif

	if (argc < 4)
	{
		cerr << "Usage: TestDriver XMLFilePath Context XPathExpression" << endl;
		return -1;
	}

	CharVectorType		theXML;

	ifstream	in(argv[1]);

	// slow and dirty dump of the xml file into a buffer
	char c;
	while(in.get(c))
		theXML.push_back(c);
	theXML.push_back('\0');

	///////////////////////////////////////////..

	// create your XPath helper object
	XPathWrapper helper;

	try
	{
		// call evaluate, passing in the XML string, the context string and the xpath string
		const XPathWrapper::CharVectorTypeVectorType	result = helper.evaluate(&*theXML.begin(), argv[2], argv[3]);

		// take the resulting string vector	and do whatever you want with it:
		size_t len = result.size();

		cout << "the result set has " << len << " strings\n";

		for (size_t i=0; i<len; i++)
			cout << "item " << (i+1) << "= \"" << result[i] << "\"" << endl;
	}
	catch(const XMLException&)
	{
		cerr << "Exception caught!  Exiting..." << endl;
	}

	return 0;
}
