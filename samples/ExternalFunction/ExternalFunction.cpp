// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XercesParserLiaison/XercesDOMSupport.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



// This class defines a function that will return the square root
// of its argument.
class FunctionSquareRoot : public Function
{
public:

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param opPos            current op position
	 * @param args             vector of pointers to XObject arguments
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		if (args.size() != 1)
		{
			executionContext.error("The square-root() function takes one argument!", context);
		}

		assert(args[0].null() == false);

		return executionContext.getXObjectFactory().createNumber(sqrt(args[0]->num()));
	}

	/**
	 * Create a copy of the function object.
	 *
	 * @return pointer to the new object
	 */
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionSquareRoot*
#endif
	clone() const
	{
		return new FunctionSquareRoot(*this);
	}

private:

	// Not implemented...
	FunctionSquareRoot&
	operator=(const FunctionSquareRoot&);

	bool
	operator==(const FunctionSquareRoot&) const;
};



// This class defines a function that will return the cube
// of its argument.
class FunctionCube : public Function
{
public:

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param opPos            current op position
	 * @param args             vector of pointers to XObject arguments
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		if (args.size() != 1)
		{
			executionContext.error("The cube() function takes one argument!", context);
		}

		assert(args[0].null() == false);

		return executionContext.getXObjectFactory().createNumber(pow(args[0]->num(), 3));
	}

	/**
	 * Create a copy of the function object.
	 *
	 * @return pointer to the new object
	 */
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionCube*
#endif
	clone() const
	{
		return new FunctionCube(*this);
	}

private:

	// Not implemented...
	FunctionCube&
	operator=(const FunctionCube&);

	bool
	operator==(const FunctionCube&) const;
};



// This class defines a function that runs the C function
// asctime() using the current system time.
class FunctionAsctime : public Function
{
public:

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has no parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node	 
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context)
	{
		time_t	theTime;

		time(&theTime);

		char* const	theTimeString = asctime(localtime(&theTime));

		// The resulting string has a newline character at the end,
		// so get rid of it.
		theTimeString[strlen(theTimeString) - 1] = '\0';

		return executionContext.getXObjectFactory().createString(XalanDOMString(theTimeString));
	}

	/**
	 * Create a copy of the function object.
	 *
	 * @return pointer to the new object
	 */
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionAsctime*
#endif
	clone() const
	{
		return new FunctionAsctime(*this);
	}

	const XalanDOMString
	getError() const
	{
		return XalanDOMString("The asctime() function takes no arguments!");
	}

private:

	// Not implemented...
	FunctionAsctime&
	operator=(const FunctionAsctime&);

	bool
	operator==(const FunctionAsctime&) const;
};



int
main(
			int				argc,
			const char*		/* argv */[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
	using std::ofstream;
#endif

	if (argc != 1)
	{
		cerr << "Usage: ExternalFunction"
			 << endl
			 << endl;
	}
	else
	{
		try
		{
			// Call the static initializer for Xerces...
			XMLPlatformUtils::Initialize();

			{
				// Initialize the Xalan XSLT subsystem...
				XSLTInit						theInit;

				// Create the support objects that are necessary for running the processor...
				XercesDOMSupport				theDOMSupport;
				XercesParserLiaison				theParserLiaison(theDOMSupport);
				XPathSupportDefault				theXPathSupport(theDOMSupport);
				XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
				XObjectFactoryDefault			theXObjectFactory;
				XPathFactoryDefault				theXPathFactory;

				// Create a processor...
				XSLTEngineImpl	theProcessor(
						theParserLiaison,
						theXPathSupport,
						theXSLTProcessorEnvSupport,
						theDOMSupport,
						theXObjectFactory,
						theXPathFactory);

				// Connect the processor to the support object...
				theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

				// Create a stylesheet construction context, and a stylesheet
				// execution context...
				StylesheetConstructionContextDefault	theConstructionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theXPathFactory);

				StylesheetExecutionContextDefault		theExecutionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theXPathSupport,
							theXObjectFactory);

				// Our input files...
				// WARNING!!! You may need to modify these absolute paths depending on where
				// you've put the Xalan sources.
				const XalanDOMString		theXMLFileName("foo.xml");
				const XalanDOMString		theXSLFileName("foo.xsl");

				// Our input sources...
				XSLTInputSource		theInputSource(c_wstr(theXMLFileName));
				XSLTInputSource		theStylesheetSource(c_wstr(theXSLFileName));

				// Our output target...
				const XalanDOMString	theOutputFile("foo.out");
				XSLTResultTarget		theResultTarget(theOutputFile);

				// Install the function directly into the XPath
				// function table.  We don't recommend doing this,
				// but you can if you want to be evil! ;-)  Since
				// the function is in the XPath table, the XPath
				// parser will treat it like any other XPath
				// function.  Therefore, it cannot have a namespace
				// prefix.  It will also be available to every
				// processor in the executable's process, since there
				// is one static XPath function table.
				XPath::installFunction(
					XalanDOMString("asctime"),
					FunctionAsctime());

				// The namespace for our functions...
				const XalanDOMString	theNamespace("http://ExternalFunction.xalan-c++.xml.apache.org");

				// Install the function in the global space.  All
				// instances will know about the function, so all
				// processors which use an instance of this the
				// class XPathEnvSupportDefault will be able to
				// use the function.
				XSLTProcessorEnvSupportDefault::installExternalFunctionGlobal(
					theNamespace,
					XalanDOMString("square-root"),
					FunctionSquareRoot());

				// Install the function in the local space.  It will only
				// be installed in this instance, so no other instances
				// will know about the function...
				theXSLTProcessorEnvSupport.installExternalFunctionLocal(
					theNamespace,
					XalanDOMString("cube"),
					FunctionCube());

				theProcessor.process(
							theInputSource,
							theStylesheetSource,
							theResultTarget,
							theConstructionContext,
							theExecutionContext);
			}

			// Call the static terminator for Xerces...
			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}
	}

	return 0;
}
