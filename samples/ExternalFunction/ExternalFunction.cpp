// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cmath>
#include <ctime>
#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <util/PlatformUtils.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#include <XPath/Function.hpp>
#include <XPath/XObjectFactory.hpp>



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
		time_t	theTime;

		time(&theTime);

		char* const	theTimeString = asctime(localtime(&theTime));
		assert(theTimeString != 0);

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
#endif

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: ExternalFunction"
			 << endl
			 << endl;
	}
	else
	{
		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		// Create a XalanTransformer.
		XalanTransformer theXalanTransformer;

		// The namespace for our functions...
		const XalanDOMString	theNamespace("http://ExternalFunction.xalan-c++.xml.apache.org");

		// Install the functions in the local space.  They will only
		// be installed in this instance, so no other instances
		// will know about them...
		theXalanTransformer.installExternalFunction(
			theNamespace,
			XalanDOMString("asctime"),
			FunctionAsctime());

		theXalanTransformer.installExternalFunction(
			theNamespace,
			XalanDOMString("square-root"),
			FunctionSquareRoot());

		theXalanTransformer.installExternalFunction(
			theNamespace,
			XalanDOMString("cube"),
			FunctionCube());

		// Do the transform.
		theResult = theXalanTransformer.transform("foo.xml", "foo.xsl", "foo.out");
    
		if(theResult != 0)
		{
			cerr << "ExternalFunction Error: \n" << theXalanTransformer.getLastError()
				 << endl
				 << endl;
		}

		// Terminate Xalan.
		XalanTransformer::terminate();

		// Call the static terminator for Xerces.
		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
