/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include <cstdio>
#include <vector>



#include <util/StdOut.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NodeList.hpp>
#include <framework/URLInputSource.hpp>



#include <PlatformSupport/DirectoryEnumerator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <DOMSupport/DOMSupportDefault.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



namespace
{


bool
TestNumericResult(
			XPathSupport&			theXPathSupport,
			XPathProcessorImpl&		theXPathProcessor,
			XPath&					theXPath,
			const DOMString&		theXPathString,
			PrintWriter&			thePrintWriter,
			double					theExpectedResult,
			const DOM_Node& 		theContextNode,
			const DOM_Element&		theNamespaceContext,
			const DOM_NodeList& 	theContextNodeList)
{
	bool	fError = false;

	theXPathProcessor.initXPath(theXPath,
								theXPathString,
								ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport));

	MutableNodeRefList	theList;

	theList = theContextNodeList;

	const XObject* const	theResult =
		theXPath.execute(theContextNode,
						 ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport),
						 theList);

	thePrintWriter.print("Execution of XPath ");
	thePrintWriter.print(theXPathString);

	if (theResult->num() == theExpectedResult)
	{
		thePrintWriter.println(" succeeded.");
		thePrintWriter.print("The result was ");
		thePrintWriter.println(theResult->num());
	}
	else
	{
		fError = true;

		thePrintWriter.println(" failed!");
		thePrintWriter.print("The expected result was ");
		thePrintWriter.println(theExpectedResult);
		thePrintWriter.print("The actual result was ");
		thePrintWriter.println(theResult->num());
	}

	return fError;
}



bool
TestStringResult(
			XPathSupport&			theXPathSupport,
			XPathProcessorImpl&		theXPathProcessor,
			XPath&					theXPath,
			const DOMString&		theXPathString,
			PrintWriter&			thePrintWriter,
			const DOMString&		theExpectedResult,
			const DOM_Node& 		theContextNode,
			const DOM_Element&		theNamespaceContext,
			const DOM_NodeList& 	theContextNodeList)
{
	bool	fError = false;

	theXPathProcessor.initXPath(theXPath,
								theXPathString,
								ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport));

	MutableNodeRefList	theList;

	theList = theContextNodeList;

	const XObject* const	theResult =
		theXPath.execute(theContextNode,
						 ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport),
						 theList);

	thePrintWriter.print("Execution of XPath ");
	thePrintWriter.print(theXPathString);

	if (equals(theResult->str(), theExpectedResult) == true)
	{
		thePrintWriter.println(" succeeded.");
		thePrintWriter.print("The result was \"");
		thePrintWriter.print(theResult->str());
		thePrintWriter.println("\".");
	}
	else
	{
		fError = true;

		thePrintWriter.println(" failed!");
		thePrintWriter.print("The expected result was \"");
		thePrintWriter.print(theExpectedResult);
		thePrintWriter.println("\".");
		thePrintWriter.print("The actual result was \"");
		thePrintWriter.print(theResult->str());
		thePrintWriter.println("\".");
	}

	return fError;
}



DOM_Document
ParseXML(
			XMLParserLiaison&	theLiaison,
			const DOMString&	theFileName)
{
	URLInputSource	theURLInputSource(c_wstr(theFileName));

	return theLiaison.parseXMLStream(theURLInputSource);
}



bool
GetXSLInput(
			const DOMString&	theXSLFileURL,
			DOMString&			theContextNodeMatchPattern,
			DOMString&			theXPathString)
{
	bool	fResult = false;

	if (endsWith(theXSLFileURL, "axes01.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
//		theXPathString = "ancestor::*";
		theXPathString = "node()|@*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes02.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "ancestor-or-self::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes03.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "attribute::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes04.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "child::*";

		fResult = true;
	}
	else
	if (endsWith(theXSLFileURL, "axes05.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes06.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant-or-self::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes07.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes08.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes09.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following-sibling::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes10.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding-sibling::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes11.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "parent::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes12.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "self::*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes16.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "ancestor::*[3]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes17.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "ancestor-or-self::*[1]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes18.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "attribute::*[2]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes19.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "@*";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes20.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "@*[2]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes21.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "child::*[2]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes22.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "child::near-south-west";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes23.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant::*[3]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes24.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant::far-south";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes25.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant-or-self::*[3]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes26.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant-or-self::far-south";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes27.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "descendant-or-self::center";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes28.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following::*[4]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes29.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following::out-yonder-east";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes30.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding::*[4]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes31.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding::out-yonder-west";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes32.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following-sibling::*[2]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes33.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "following-sibling::east";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes34.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding-sibling::*[2]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes35.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding-sibling::west";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes35.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "preceding-sibling::west";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes36.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "parent::near-north";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes37.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "parent::*[1]";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes38.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "parent::foo";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes39.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "..";

		fResult = true;
	}
	else if (endsWith(theXSLFileURL, "axes40.xsl") == true)
	{
		theContextNodeMatchPattern = "//center";
		theXPathString = "self::center";

		fResult = true;
	}

	return fResult;
}



DOM_Node
FindContextNode(
			XPathSupport&			theXPathSupport,
			XPathProcessorImpl&		theXPathProcessor,
			XPathFactory&			theXPathFactory,
			const DOM_Document&		theDocument,
			const DOMString&		theContextNodeMatchPattern,
			PrintWriter&			thePrintWriter)
{
	DOM_Node							theResult;

	FactoryObjectAutoPointer<XPath>		theXPath(&theXPathFactory,
												 theXPathFactory.create());

	DOM_Element							theNamespaceContext;

	theXPathProcessor.initXPath(*theXPath.get(),
								theContextNodeMatchPattern,
								ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport));

	DOM_NodeList		theContextNodeList;
	MutableNodeRefList	theList;

	theList = theContextNodeList;

	const XObject* const	theXObject =
					theXPath->execute(theDocument,
									  ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport),
									  theList);

	try
	{
		assert(theXObject != 0);

		const MutableNodeRefList&	theResultList =
						theXObject->mutableNodeset();

		assert(theResultList.getLength() == 1);

		theResult = theResultList.item(0);
	}
	catch(...)
	{
		thePrintWriter.print("FindContextNode: Error executing match pattern \"");
		thePrintWriter.print(theContextNodeMatchPattern);
		thePrintWriter.println("\".");
	}

	return theResult;
}



bool
TestAxisResult(
			XPathSupport&			theXPathSupport,
			XPathProcessorImpl&		theXPathProcessor,
			XMLParserLiaison&		theLiaison,
			XPathFactory&			theXPathFactory,
			const DOMString&		theXMLFileURL,
			const DOMString&		theXSLFileURL,
			PrintWriter&			thePrintWriter)
{
	bool	fError = false;

	const DOM_Document	theXMLDocument = ParseXML(theLiaison,
												  theXMLFileURL);

	if (theXMLDocument != 0)
	{
		DOMString		theContextNodeMatchPattern;
		DOMString		theXPathString;

		if (GetXSLInput(theXSLFileURL,
						theContextNodeMatchPattern,
						theXPathString) == true)
		{
			DOM_Node		theContextNode = FindContextNode(theXPathSupport,
															 theXPathProcessor,
															 theXPathFactory,
															 theXMLDocument,
															 theContextNodeMatchPattern,
															 thePrintWriter);

			if (theContextNode != 0)
			{
				DOM_Element		theNamespaceContext;
				DOM_NodeList	theContextNodeList;

				FactoryObjectAutoPointer<XPath>		theXPath(&theXPathFactory,
															 theXPathFactory.create());

				theXPathProcessor.initXPath(*theXPath.get(),
											theXPathString,
											ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport));

				bool	fDump = false;

				if (fDump == true)
				{
					thePrintWriter.println();
					thePrintWriter.println();
					theXPath->getExpression().dumpOpCodeMap(thePrintWriter);
					thePrintWriter.println();
					theXPath->getExpression().dumpTokenQueue(thePrintWriter);
					thePrintWriter.println();
					thePrintWriter.println();
				}

				MutableNodeRefList	theList;
				
				theList = theContextNodeList;

				const XObject* const	theResult =
					theXPath->execute(theContextNode,
									  ElementPrefixResolverProxy(theNamespaceContext,theXPathSupport),
									  theList);

				try
				{
					assert(theResult != 0);

					const MutableNodeRefList&	theResultList =
						theResult->mutableNodeset();

					const int	theLength = theResultList.getLength();

					if (theLength > 0)
					{
						thePrintWriter.print("<out>");

						for (int i = 0; i < theLength; i++)
						{
							thePrintWriter.print(theResultList.item(i).getNodeName());
							thePrintWriter.print(" ");
						}
					}

					thePrintWriter.println("</out>");
				}
				catch(...)
				{
					thePrintWriter.print("Execution of XPath ");
					thePrintWriter.print(theXPathString);
					thePrintWriter.println(" failed!");
				}
			}
		}
	}

	return fError;
}



const char* const	theNumericTestInput[] =
{
	"9 + 7 - 6 * 42 + 36 div 3 + 100 - 2 * 6",
	"9 + 7 - 6 + 3 + 24 - 32 - 89 + 123 - 12256 + 89957",
	"0 + 0",
	"1 div 2",
	"1 * 0.25",
	0
};


const double	theNumericTestExpectedOutput[] =
{
	-136.0,
	77740.0,
	0,
	0.5,
	0.25,
	0	// This is a dummy entry and should be last.
};



void
TestNumericResults(
			XPathSupport&			theXPathSupport,
			XPathFactory&			theXPathFactory,
			XObjectFactory&			/* theXObjectFactory */,
			XPathProcessorImpl&		theXPathProcessor,
			PrintWriter&			thePrintWriter)
{
	assert(sizeof(theNumericTestInput) / sizeof(theNumericTestInput[0]) ==
			sizeof(theNumericTestExpectedOutput) / sizeof(theNumericTestExpectedOutput[0]));

	for(int i = 0; theNumericTestInput[i] != 0; i++)
	{
		FactoryObjectAutoPointer<XPath>		theXPath(&theXPathFactory,
													 theXPathFactory.create());

		TestNumericResult(theXPathSupport,
						  theXPathProcessor,
						  *theXPath.get(),
						  theNumericTestInput[i],
						  thePrintWriter,
						  theNumericTestExpectedOutput[i],
						  DOM_Node(),
						  DOM_Element(),
						  DOM_NodeList());
	}
}


const char* const	theStringTestInput[] =
{
	"string(0)",
	"string(1.5)",
	"string(-1.5)",
	"string(0.5)",
	"string(-0.5)",
	"string(9)",
	"string(-9)",
	"string(\"9\")",
	"string(0.25)",

	"concat(\"foo\", \"bar\")",
	"concat(\"foo\", \"bar\", \"sky\")",

	"contains(\"foobar\", \"oba\")",
	"contains(\"Xalan is great\", \"boag\")",

	"starts-with(\"foobar\", \"foo\")",
	"starts-with(\"Xalan is great\", \"boag\")",

	"substring-after(\"1999/04/01\", \"/\")",
	"substring-after(\"1999/04/01\", \"19\")",

	"substring-before(\"1999/04/01\", \"/\")",
	"substring-before(\"1999/04/01\", \"01\")",

	"substring(\"12345\", 1.5, 2.6)",
	"substring(\"12345\", 0, 3)",
	"substring(\"12345\", 5, 29)",
	"substring(\"12345\", -1, 2)",
	"substring(\"12345\", -2, -1)",
	"substring(\"12345\", -2)",

	"normalize(\"   aa   a  \")",

	"translate(\"---aaa--\", \"abc-\", \"ABC\")",

	"5 = 0",
	"5 = 5",
	0
};


const char* const	theStringTestExpectedOutput[] =
{
	"0",
	"1.5",
	"-1.5",
	"0.5",
	"-0.5",
	"9",
	"-9",
	"9",
	"0.25",

	"foobar",
	"foobarsky",

	"true",
	"false",

	"true",
	"false",

	"04/01",
	"99/04/01",

	"1999",
	"1999/04/",

	"234",
	"12",
	"5",
	"",
	"",
	"12345",

	"aa a",

	"AAA",

	"false",
	"true",
	0
};



void
TestStringResults(
			XPathSupport&			theXPathSupport,
			XPathFactory&			theXPathFactory,
			XObjectFactory&			/* theXObjectFactory */,
			XPathProcessorImpl&		theXPathProcessor,
			PrintWriter&			thePrintWriter)
{
	assert(sizeof(theStringTestInput) == sizeof(theStringTestExpectedOutput));

	for(int i = 0; theStringTestInput[i] != 0; i++)
	{
		FactoryObjectAutoPointer<XPath>		theXPath(&theXPathFactory,
													 theXPathFactory.create());

		TestStringResult(theXPathSupport,
						theXPathProcessor,
						 *theXPath.get(),
						 theStringTestInput[i],
						 thePrintWriter,
						 theStringTestExpectedOutput[i],
						 DOM_Node(),
						 DOM_Element(),
						 DOM_NodeList());
	}
}



DOMString
GetXSLFileName(const DOMString&		theXMLFileName)
{
	DOMString	theResult;

	int			thePeriodIndex = -1;

	const int	theLength = length(theXMLFileName);

	for (int i = theLength - 1; i > 0; i--)
	{
		if (charAt(theXMLFileName, i) == '.')
		{
			thePeriodIndex = i;

			break;
		}
	}

	if (thePeriodIndex != -1)
	{
		theResult = substring(theXMLFileName,
							  0,
							  thePeriodIndex + 1);

		theResult += "xsl";
	}

	return theResult;
}



void
TestAxes(
			XPathSupport&			theXPathSupport,
			XPathFactory&			theXPathFactory,
			XObjectFactory&			/* theXObjectFactory */,
			XPathProcessorImpl&		theXPathProcessor,
			XMLParserLiaison&		theLiaison,
			PrintWriter&			thePrintWriter)
{
	const char* const	theBaseURL = "file:///testsuite/conf/Axes/";
	const char* const	theSearchSpecification = "/testsuite/conf/Axes";
	const char* const	theXMLSuffix = ".xml";

	typedef std::vector<DOMString>	FileNameVectorType;

	DirectoryEnumeratorFunctor<FileNameVectorType>	theEnumerator;

	FileNameVectorType	theFiles;

	theEnumerator(theSearchSpecification,
				  theFiles);

	thePrintWriter.println();

	for(FileNameVectorType::size_type	i = 0; i < theFiles.size(); i++)
	{
		if (endsWith(theFiles[i], theXMLSuffix) == true)
		{
			// Create a fully qualified URL specification...
			const DOMString		theXMLFileName = theBaseURL + theFiles[i];

			// Get the name of the corresponding XSL file...
			const DOMString		theXSLFileName = GetXSLFileName(theXMLFileName);

			FactoryObjectAutoPointer<XPath>		theXPath(&theXPathFactory,
														 theXPathFactory.create());

			TestAxisResult(theXPathSupport,
							theXPathProcessor,
						   theLiaison,
						   theXPathFactory,
						   theXMLFileName,
						   theXSLFileName,
						   thePrintWriter);
		}
	}
}



void
RunTests(
			XPathSupport&			theXPathSupport,
			XPathFactory&			theXPathFactory,
			XObjectFactory&			theXObjectFactory,
			XPathProcessorImpl&		theXPathProcessor,
			XMLParserLiaison&		theLiaison,
			PrintWriter&			thePrintWriter)
{
	TestNumericResults(theXPathSupport,
					   theXPathFactory,
					   theXObjectFactory,
					   theXPathProcessor,
					   thePrintWriter);

	TestStringResults(theXPathSupport,
					  theXPathFactory,
					  theXObjectFactory,
					  theXPathProcessor,
					  thePrintWriter);

	TestAxes(theXPathSupport,
		     theXPathFactory,
			 theXObjectFactory,
			 theXPathProcessor,
			 theLiaison,
			 thePrintWriter);
}

};



int
main(int			/* argc */,
	 const char*	/* argv[] */)
{
	XPathEnvSupportDefault	theEnvSupport;
	DOMSupportDefault		theDOMSupport;
	XPathSupportDefault		theSupport(theDOMSupport);
	XObjectFactoryDefault	theXObjectFactory(theEnvSupport, theSupport);
	XPathFactoryDefault		theXPathFactory(theXObjectFactory, theEnvSupport, theSupport);
	XPathProcessorImpl		theXPathProcessor(theEnvSupport, theSupport);
	XMLStdOut				theStdOut;
	XercesDOMPrintWriter		thePrintWriter(theStdOut);
	XercesParserLiaison		theLiaison(theDOMSupport);

	RunTests(theSupport,
			theXPathFactory,
			theXObjectFactory,
			theXPathProcessor,
			theLiaison,
			thePrintWriter);

	return 0;
}
