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

#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ifstream;
	using std::ios_base;
	using std::ostrstream;
	using std::string;
#endif

// XALAN HEADERS...
#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>
#include <XalanTransformer/XalanTransformer.hpp>
#include <XalanTransformer/XalanCompiledStylesheetDefault.hpp>

#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>

#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>

#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include <XalanSourceTree/XalanSourceTreeDocument.hpp>

// HARNESS HEADERS...
#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
	typedef std::map<XalanDOMString, XalanDOMString>  Hashtable;
#endif

// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

bool 
domCompare(const XalanNode& gold ,const XalanNode& doc);

void
printArgOptions()
{
	cerr << endl
		 << "Perf dirname [-out -category -i -iter]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl
		 << "-gold dirname	(base directory for gold files)"
		 << endl
		 << "-category dirname (run files only from a specific directory)"
		 << endl
		 << "-i                (include all testcases)"
		 << endl
		 << "-iter n           (specifies number of iterations; must be > 0)"
		 << endl;
}

bool
getParams(int argc, 
		  const char*	argv[],
		  FileUtility& f,
		  XalanDOMString& baseDir,
		  XalanDOMString& outDir,
		  XalanDOMString& goldRoot,
		  XalanDOMString& category)
{
	bool fSuccess = true;	// Used to continue argument loop
	bool fsetOut = true;	// Set default output directory, set to false if data is provided
	bool fsetGold = true;	// Set default gold directory, set to false if data is provided


	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		if (f.checkDir(pathSep + XalanDOMString(argv[1])))
		{
			assign(baseDir, XalanDOMString(argv[1]));
			insert(baseDir, 0, pathSep);
		}
		else
		{
			cout << endl << "Given base directory \"" << argv[1] << "\" does not exist" << endl;
			printArgOptions();
			return false;
		}
	}

	// Get the rest of the arguments in any order.
	for (int i = 2; i < argc && fSuccess == true; ++i)
	{
		if(!stricmp("-out", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(outDir, XalanDOMString(argv[i]));
				insert(outDir, 0, XalanDOMString("\\"));
				append(outDir, XalanDOMString("\\"));
				f.checkAndCreateDir(outDir);
				fsetOut = false;
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-category", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(category, XalanDOMString(argv[i]));
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-gold", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(goldRoot, XalanDOMString(argv[i]));
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}

		else
		{
			printArgOptions();
			fSuccess = false;
		}

	} // End of for-loop

	// Do we need to set the default output directory??
	if (fsetOut)
	{
		unsigned int ii = lastIndexOf(baseDir,charAt(pathSep,0));
		outDir = substring(baseDir, 0, ii+1);
		append(outDir,XalanDOMString("COMPARE-results"));
		f.checkAndCreateDir(outDir);
		append(outDir,pathSep);
	}
	if (fsetGold)
	{
		goldRoot = baseDir;
		append(goldRoot,XalanDOMString("\\xout"));
		f.checkAndCreateDir(goldRoot);
		append(goldRoot,pathSep);
	}
	
	// Add the path seperator to the end of the base directory 
	// here after we've finished using it for all directory creation.

	append(baseDir,pathSep);
	return fSuccess;
}


XalanDocument* getGoldDom(XalanSourceTreeParserLiaison& theParser, const XSLTInputSource&  goldInputSource)
{

	return theParser.parseXMLStream(goldInputSource);
}

/*
void
reportError(char* msg, int type, void& x)
{
	switch (type)
	{
	case 1:
		{
			cout << msg << "Expected: " << c_str(TranscodeToLocalCodePage(*x));
		}

	case 2:
		{
			cout << "hello" << endl;
		}
	case 3:
		{
			cout << "boo" << endl;
		}
	}
}

*/

bool diffATTR(const XalanNode* gAttr, const XalanNode* dAttr)
{
	//cout << "	Attr Name: " << c_str(TranscodeToLocalCodePage(goldData)) << endl;
						
	const XalanDOMString& goldAttrName = gAttr->getNodeValue();
	const XalanDOMString& docAttrName  = dAttr->getNodeValue();

	const XalanDOMString& goldAttrNsUri = gAttr->getNamespaceURI();
	const XalanDOMString& docAttrNsUri  = dAttr->getNamespaceURI();

	const XalanDOMString& goldAttrPrefix = gAttr->getPrefix();
	const XalanDOMString& docAttrPrefix = dAttr->getPrefix();

	const XalanDOMString& goldAttrLName = gAttr->getLocalName();
	const XalanDOMString& docAttrLName = dAttr->getLocalName();

	if (goldAttrName != docAttrName)
	{
		cout << "Error6a: Wrong Attribute Value. Expecting: " << c_str(TranscodeToLocalCodePage(goldAttrName)) << endl;							
		//return false;
	}

	if (goldAttrNsUri != docAttrNsUri)
	{
		cout << "Error7a: Wrong NamespaceURI. Expecting: " << c_str(TranscodeToLocalCodePage(goldAttrNsUri)) << endl;							
		//return false;
	}

	if (goldAttrPrefix != docAttrPrefix)
	{
		cout << "Error8a: Wrong Namespace Prefix. Expecting: " << c_str(TranscodeToLocalCodePage(goldAttrPrefix)) << endl;							
		//return false;
	}

	if (goldAttrLName != docAttrLName)
	{
		cout << "Error9a: Wrong LocalName. Expecting: " << c_str(TranscodeToLocalCodePage(goldAttrLName)) << endl;							
		//return false;
	}

	return true;
}

bool
diffElement(const XalanNode& gold, const XalanNode& doc)
{
	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&	docNsUri  = doc.getNamespaceURI();
	const XalanDOMString&	goldNsUri = gold.getNamespaceURI();

	cout << "Processing: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
		
	if (goldNodeName != docNodeName)
	{
		cout << "Error2: Element mismatch. Expected: " << c_str(TranscodeToLocalCodePage(goldNodeName)) << endl;		
		return false;
	}

	if ( goldNsUri != docNsUri)
	{
		cout << "Error3: NamespaceURI mismatch. Expected: " << c_str(TranscodeToLocalCodePage(goldNsUri)) << endl;
		return false;
	}

	// Get Attributes for each Element Node. 
	const XalanNamedNodeMap	*goldAttrs = gold.getAttributes();
	const XalanNamedNodeMap *docAttrs  = doc.getAttributes();
	
	// Get number of Attributes
	int numGoldAttr = goldAttrs->getLength();
	int numDomAttr  = docAttrs ->getLength();

	// Check that each Element has same number of Attributes. 
	if ( numGoldAttr == numDomAttr )
	{
		// Compare Attributes one at a time.
		for (int i=0; i < numGoldAttr; i++)
		{
			// Attribute order is irrelvant, so comparision is base on Attribute name.
			XalanNode *gAttr = goldAttrs->item(i);
			XalanDOMString goldAttrName = gAttr->getNodeName();

			XalanNode *dAttr = docAttrs->getNamedItem(goldAttrName);
			if (dAttr != 0)
			{
				if( ! (diffATTR(gAttr, dAttr)) )
					return false;
			}
			else
			{
				cout << "Error5: Named Attribute missing. Expected: " << c_str(TranscodeToLocalCodePage(goldAttrName)) << endl;
				return false;
			}
		}
	}
	else
	{
		cout << "Error 4: Number of attributes mismatch. Expecting: " << numGoldAttr << endl;
		return false;
	}

	const XalanNode	*goldNextNode;
	const XalanNode	*domNextNode;

	goldNextNode = gold.getFirstChild();
	domNextNode = doc.getFirstChild();

	if (0 != goldNextNode )
	{
		if (0 != domNextNode)
		{
			if ( ! domCompare(*goldNextNode, *domNextNode) )
				return false;
		}
		else
		{
			cout << "Error 10: Missing ChildNode. Expecting: " << c_str(TranscodeToLocalCodePage(goldNextNode->getNodeName())) << endl;
			return false;
		}
	}

	goldNextNode = gold.getNextSibling();
	domNextNode = doc.getNextSibling();

	if (0 != goldNextNode)
	{
		if (0 != domNextNode)
		{
			if ( ! domCompare(*goldNextNode, *domNextNode) )
				return false;
		}
		else
		{
			cout << "Error 11: Missing SiblingNode. Expecting: " << c_str(TranscodeToLocalCodePage(goldNextNode->getNodeName())) << endl;
			return false;
		}
	}

	return true;
}


bool 
domCompare(const XalanNode& gold ,const XalanNode& doc)
{
	const XalanNode::NodeType	docNodeType  = doc.getNodeType();
	const XalanNode::NodeType	goldNodeType = gold.getNodeType();

	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&	docNodeValue  = doc.getNodeValue();
	const XalanDOMString&	goldNodeValue = gold.getNodeValue();

	if (goldNodeType != docNodeType)
	{
		cout << "Error1: NodeType mismatch. Expected: " << goldNodeType << endl;				
		return false;
	}

	switch (goldNodeType)
	{
	case XalanNode::ELEMENT_NODE:	// ATTRIBUTE_NODE's are processed with diffElement().
	{ 

		if ( ! diffElement(gold, doc) ) 
		{
			return false;
		}

		break;
	}
	case XalanNode::TEXT_NODE:	
	{
		cout << "Processing: " << c_str(TranscodeToLocalCodePage(docNodeValue)) << endl;

		if(goldNodeValue != docNodeValue)
		{
			cout << "Error7: Text node mismatch. Expected: " << c_str(TranscodeToLocalCodePage(goldNodeValue)) << endl;
			return false;
		}

		break;
	}
	case XalanNode::CDATA_SECTION_NODE:
	case XalanNode::ENTITY_REFERENCE_NODE:
	case XalanNode::ENTITY_NODE:
	case XalanNode::PROCESSING_INSTRUCTION_NODE:
	case XalanNode::COMMENT_NODE:
	{
		break;
	}
	case XalanNode::DOCUMENT_NODE:
	{
		cout << "Processing: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
		if (goldNodeName != docNodeName)  
		{
			cout << "Error 8: Missing Document Node" << endl;
		}
		else
		{
			const XalanNode	*goldNextNode;
			const XalanNode	*domNextNode;

			goldNextNode = gold.getFirstChild();
			domNextNode = doc.getFirstChild();

			if (0 != goldNextNode)
			{
				if( ! domCompare(*goldNextNode,*domNextNode) )
					return false;
			}

		}

		break;
	}

	case XalanNode::DOCUMENT_TYPE_NODE:
	case XalanNode::DOCUMENT_FRAGMENT_NODE:
	case XalanNode::NOTATION_NODE:
	{
		break;
	}
	default:
		cout << "What are you doing? " << endl;
	}

	return true;
}

FormatterListener* 
getXMLFormatter(bool					shouldWriteXMLHeader,
				bool					stripCData,
				bool					escapeCData,
				PrintWriter&			resultWriter,
				int						indentAmount,
				const XalanDOMString&	mimeEncoding,
				const StylesheetRoot*	stylesheet)
{
	FormatterListener*	formatter = 0;

		XalanDOMString	version;
		bool			outputIndent= 0;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->m_version;

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
			outputIndent = stylesheet->m_indentResult;
		}

		FormatterToXML* const	fToXML =
			new FormatterToXML(
					resultWriter,
					version,
					outputIndent,
					indentAmount,
					mimeEncoding,
					mediatype,
					doctypeSystem,
					doctypePublic,
					true,	// xmlDecl
					standalone);

		fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
		fToXML->setStripCData(stripCData);
		fToXML->setEscapeCData(escapeCData);

		formatter = fToXML;
		return formatter;
}



int
main(
	 int			argc,
	 const char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	HarnessInit xmlPlatformUtils;
	XalanTransformer::initialize();

	{
		Hashtable runAttrs;		// Attribute list for perfdata element
		int transResult = 0;

		XalanDOMString  category;	// Test all of base dir by default
		XalanDOMString  baseDir, outputRoot, goldRoot;	


		FileUtility futil;

		if (getParams(argc, argv, futil, baseDir, outputRoot, goldRoot, category) == true)
		{
			//
			// Call the static initializers for xerces and xalan, and create a transformer
			//
			XalanTransformer xalan;

			XalanSourceTreeDOMSupport domSupport;
			XalanSourceTreeParserLiaison parserLiaison(domSupport);
			domSupport.setParserLiaison(&parserLiaison);


			// Generate Unique Run id and processor info
			const XalanDOMString UniqRunid = futil.GenerateUniqRunid();


			// Defined basic constants for file manipulation and open results file
			const XalanDOMString  resultFilePrefix("cpp");
			const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);


			XMLFileReporter	logFile(resultsFile);
			logFile.logTestFileInit("Comparison Testing:");


			// Create run entry that contains runid and number of iterations used for averages.
			runAttrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"), UniqRunid));
			runAttrs.insert(Hashtable::value_type(XalanDOMString("Xerces-Version "), futil.getXercesVersion()));
			logFile.logElement(10, "perfdata", runAttrs, "xxx");
				
			// Specify the "test" directory for both input and output.
			const XalanDOMString  xDir("simpletests");
			const XalanDOMString  theOutputDir = outputRoot + xDir;
			futil.checkAndCreateDir(theOutputDir);

			// Get the files found in the test directory
			const FileNameVectorType files = futil.getTestFileNames(baseDir, xDir, true);

				for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
				{

					Hashtable attrs;

					attrs.insert(Hashtable::value_type(XalanDOMString("idref"), files[i]));
					attrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"),UniqRunid));
					attrs.insert(Hashtable::value_type(XalanDOMString("processor"),processorType));

					const XalanDOMString  theXSLFile= baseDir + xDir + pathSep + files[i];
					const XalanDOMString  theXMLFile = futil.GenerateFileName(theXSLFile,"xml");
					XalanDOMString  theGoldFile = goldRoot +xDir + pathSep + files[i];
					theGoldFile = futil.GenerateFileName(theGoldFile, "out");

					const XalanDOMString  outbase =  outputRoot + xDir + pathSep + files[i]; 
					const XalanDOMString  theOutputFile = futil.GenerateFileName(outbase, "out");

					const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
					const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
					const XSLTInputSource	goldInputSource(c_wstr(theGoldFile));

					// Use a XalanSourceTreeDocument to create the XSLTResultTarget. 
					XalanSourceTreeDocument* dom = parserLiaison.createXalanSourceTreeDocument();
					FormatterToSourceTree domOut(dom); 
					XSLTResultTarget domResultTarget;
					domResultTarget.setDocumentHandler(&domOut);

					cout << endl << files[i] << endl;

					//XalanSourceTreeDocument* goldDom=getGoldDom(parserLiaison, goldInputSource);
					//XalanDocument* goldDom=getGoldDom(parserLiaison, goldInputSource);

					//
					// Parsing(compile) the XSL stylesheet and report the results..
					//
					const XalanCompiledStylesheet*	compiledSS = 0;
					xalan.compileStylesheet(xslInputSource, compiledSS);
					if (compiledSS == 0 )
					{
						continue;
					}

					//
					// Parsing the input XML and report the results..
					//
					const XalanParsedSource*	parsedSource = 0;
					xalan.parseSource(xmlInputSource, parsedSource);
					if (parsedSource == 0)
					{
						continue;
					}

					//
					// Perform One transform using parsed stylesheet and unparsed xml source, report results...
					// 
					transResult = xalan.transform(*parsedSource, compiledSS, domResultTarget);

					if(!transResult)
					{
						const XalanDOMString mimeEncoding("whatever");
						XalanFileOutputStream myOutput(theOutputFile);
						XalanOutputStreamPrintWriter myResultWriter(myOutput);
						FormatterListener* theFormatter = getXMLFormatter(true,true,true,
																		myResultWriter,0,
																		mimeEncoding,
																		compiledSS->getStylesheetRoot());

						FormatterTreeWalker theTreeWalker(*theFormatter);
						theTreeWalker.traverse(dom);
						delete theFormatter;

						XalanDocument* goldDom = parserLiaison.parseXMLStream(goldInputSource);
						domCompare(*goldDom, *dom);
					}
					else
					{
						cout << xalan.getLastError();
						return 0;
					}


					xalan.destroyParsedSource(parsedSource);
					xalan.destroyStylesheet(compiledSS);

				}//for files

		logFile.logTestFileClose("Performance", "Done");
		logFile.close();

		} //if getParams
	}

	XalanTransformer::terminate();


	return 0;
}
