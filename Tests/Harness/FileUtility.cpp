/*
*
* FileUtility.cpp
*
* Copyright 2000 Lotus Development Corporation. All rights reserved.
* This software is subject to the Lotus Software Agreement, Restricted
* Rights for U.S. government users and applicable export regulations.
*/

#include <stdlib.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <string.h>
#include <time.h>

// Added for directory creation 
#include <strstream>
#include <stdio.h>
#include <direct.h>


#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

// XERCES HEADERS...
//	Are included by HarnessInit.hpp
#include <sax/SAXException.hpp>

// XALAN HEADERS...
//	Are included by FileUtility.hpp

#include "FileUtility.hpp"
#include "XMLFileReporter.hpp"

char *xalanNodeTypes[]=
	{"UNKNOWN_NODE",
	"ELEMENT_NODE",
	"ATTRIBUTE_NODE",
	"TEXT_NODE",
	"CDATA_SECTION_NODE",
	"ENTITY_REFERENCE_NODE",
	"ENTITY_NODE",
	"PROCESSING_INSTRUCTION_NODE",
	"COMMENT_NODE",
	"DOCUMENT_NODE",
	"DOCUMENT_TYPE_NODE",
	"DOCUMENT_FRAGMENT_NODE",
	"NOTATION_NODE"};

/*	This routine retrieves test file names from specified directories.
//	Inputs: baseDir:	typically "conf" or "perf"
//			relDir:		sub-directory to search.
//
//	Notes:	It builds the searchSpecification by concatenating all the 
//			necessary components.
//																			*/	
FileNameVectorType FileUtility::getTestFileNames(XalanDOMString baseDir, XalanDOMString relDir, bool useDirPrefix)
{
	const XalanDOMString	searchSuffix(XALAN_STATIC_UCODE_STRING("*.xsl"));
	XalanDOMString	searchSpecification;

	// Allow directory search w/o mandating files start with directory name. Required for files
	// garnered from XSLTMARK performance directory exm.
	if (useDirPrefix)
	{
		assign(searchSpecification, baseDir + relDir + pathSep + relDir + searchSuffix);
	}
	else
	{
		assign(searchSpecification, baseDir + relDir + pathSep + searchSuffix); 
	}


	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString>	theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(searchSpecification, theFiles);

	return theFiles;
}

/*	This routine retrieves all sub-directories from the specified directories.
//	Inputs: rootDirectory:	typically "conf" or "perf"
//
//	Notes:	The searchSpecification in this case is just "*". 
//																			*/	
FileNameVectorType FileUtility::getDirectoryNames(XalanDOMString rootDirectory)
{
	const XalanDOMString	dirSpec(XALAN_STATIC_UCODE_STRING("*"));

	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString, DirectoryFilterPredicate> theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(XalanDOMString(rootDirectory), XalanDOMString(dirSpec), theFiles);

	return theFiles;
}

bool FileUtility::checkDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );

	if ( _chdir(c_str(TranscodeToLocalCodePage(directory))) )
	{
		_chdir(buffer);
		return false;
	}
	else
	{
		return true;
	}
}


void FileUtility::checkAndCreateDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );


	if ( (_chdir(c_str(TranscodeToLocalCodePage(directory)))) )
	{
		//cout << "Couldn't change to " << directory << ", will create it." << endl;
		if ( !(_mkdir(c_str(TranscodeToLocalCodePage(directory)))))
		{
			cout << directory << " created." << endl;
		}
	}

	_chdir(buffer);
}

/*	This routine generates file names based on the provide suffix
//	Inputs: theXMLFileName:	typically "conf" or "perf"
//			suffix:			typically "xsl" or "out".
//
//	Notes:	
*/	

XalanDOMString FileUtility::GenerateFileName(const XalanDOMString&  theXMLFileName, char* suffix)
{
	XalanDOMString	theResult;
	int				thePeriodIndex = -1;
	const int		theLength = length(theXMLFileName);

	for (int i = theLength - 1; i > 0; i--)
	{
		if (charAt(theXMLFileName, i) == XalanUnicode::charFullStop)
		{
			thePeriodIndex = i;        // charFullStop is the dot (x2E)
			break;
		}
	}

	if (thePeriodIndex != -1)
	{
		theResult = substring(theXMLFileName,
							  0,
							  thePeriodIndex + 1);


		theResult += XalanDOMString(suffix);
	}

	return theResult;
}


/*	This routine generates a Unique Runid. 
//	Inputs: None
//			
//	Notes: The format is mmddhhmm. For example
//		   03151046 is "Mar 15 10:46"	
*/

XalanDOMString FileUtility::GenerateUniqRunid()
{

		struct tm *newtime;
		time_t long_time;
		char tmpbuf[10];

		time( &long_time );                /* Get time as long integer. */
		newtime = localtime( &long_time ); /* Convert to local time. */

		strftime( tmpbuf, 10,"%m%d%H%M",newtime );

		return(XalanDOMString(tmpbuf));

}

//	This routine gets Xerces Version number. It's used to put the Xerces Version
//	into the output xml results file as an attribute of 'PerfData' element.
//	Inputs: None
//				

XalanDOMString FileUtility::getXercesVersion()
{

	return(XalanDOMString(gXercesFullVersionStr));

}

/*	This routine creates a FormatterToXML FormatterListener. This is used to format
//	the output DOM so a comparision can be done with the expected GOLD file. 
//	Inputs: None
//				
*/


FormatterListener* 
FileUtility::getXMLFormatter(bool		shouldWriteXMLHeader,
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

//		fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
//		fToXML->setStripCData(stripCData);
//		fToXML->setEscapeCData(escapeCData);

		formatter = fToXML;
		return formatter;
}

/*	This routine is used to compares the results of a transform and report the results.
//  When a failure is detected the 'data' structure used to report detailed info about 
//  a failure is filled in.
//	Inputs: 
//		goldFile    - Name of gold file
//		outputFile  - Name of result file.
//		logfile	    - Name of log file reporter.
//		
//	Returns: 
//		Void
*/
void
FileUtility::checkResults(const XalanDOMString& outputFile, 
						  const XalanDOMString& goldFile, 
						  XMLFileReporter& logfile)
{
	int ambgFlag = data.nogold;

	if(compareSerializedResults(outputFile, goldFile))
	{
		cout << "Passed: " << data.testOrFile << endl;
		logfile.logCheckPass(data.testOrFile);
		data.pass += 1;
	}
	else
	{
		Hashtable attrs;
		Hashtable actexp;
		reportDOMError();

		attrs.insert(Hashtable::value_type(XalanDOMString("reason"), XalanDOMString(data.msg)));
		attrs.insert(Hashtable::value_type(XalanDOMString("atNode"), data.currentNode));
		actexp.insert(Hashtable::value_type(XalanDOMString("exp"), data.expected));
		actexp.insert(Hashtable::value_type(XalanDOMString("act"), data.actual));

		if (ambgFlag < data.nogold)
		{
			logfile.logCheckAmbiguous(data.testOrFile);
		}
		else
		{
			logfile.logCheckFail(data.testOrFile, attrs, actexp);
		}
	}

}

/*	This routine compares the results of a transform with the gold file.
//	It in turn call the domCompare routine to do the actual comparision. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filename
//		
//	Returns: 
//		Void
//		
*/
bool
FileUtility::compareDOMResults(const XalanDOMString& theOutputFile, 
			   const XalanCompiledStylesheet* compiledSS, 
			   XalanSourceTreeDocument* dom,
			   const XSLTInputSource& goldInputSource)
{
	const XalanDOMString mimeEncoding("");
	XalanFileOutputStream myOutput(theOutputFile);
	XalanOutputStreamPrintWriter myResultWriter(myOutput);
	FormatterListener* theFormatter = getXMLFormatter(true,true,true,
															myResultWriter,0,
															mimeEncoding,
															compiledSS->getStylesheetRoot());

	FormatterTreeWalker theTreeWalker(*theFormatter);
	theTreeWalker.traverse(dom);
	delete theFormatter;


	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);
	
	XalanDocument* goldDom = parserLiaison.parseXMLStream(goldInputSource);

	return domCompare(*goldDom, *dom);
}

/*	This routine takes the result file and gold file and parses them.
//  If either of the files fails to parse and a SAXException is throw,
//  then the files are compared using a char by char file compare,
//  otherwise the domCompare routine is used.
//	Inputs: 
//		outputFile:  Name of result file
//		goldFile:	 Name of gold file
//		
//	Returns: 
//		True or False
//		
*/
bool
FileUtility::compareSerializedResults(const XalanDOMString& outputFile,
									const XalanDOMString& goldFile)
{

	const XSLTInputSource resultInputSource(c_wstr(outputFile));
	const XSLTInputSource goldInputSource(c_wstr(goldFile));

	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);

	XalanDocument* goldDom = 0;
	XalanDocument* transformDom = 0;

	try
	{
		transformDom = parserLiaison.parseXMLStream(resultInputSource);
		goldDom = parserLiaison.parseXMLStream(goldInputSource);
		return domCompare(*goldDom, *transformDom);
	}

	// This exception is being reported prior to this Catch, however, however, I clarify that it's a SAX exception.
	// It's a good indication that the Gold file is not a valid XML.  When this happens the transform result needs
	// to be compared with the Gold,  with a character by character basis,  not via the DOM compair. 
	catch (SAXException& e)
	{
		cout << "SAXException: Using fileCompare to check output.\n";
		return fileCompare(c_str(TranscodeToLocalCodePage(goldFile)), c_str(TranscodeToLocalCodePage(outputFile)));
	}
 
}

/*	This routine is used to compare the results against the gold when one or both of 
//  fails to parse without throwing a SAXException. When a failure is detected the 'data' 
//  structure used to report detailed info about a failure is filled in.
//	Inputs: 
//		outputFile:  Name of result file
//		goldFile:	 Name of gold file
//		
//	Returns: 
//		True or False
//		
*/
bool
FileUtility::fileCompare(const char* goldFile,
						const char* outputFile)
{
	FILE *result, *gold;			// declare files
	char rline[132] = {'/n'};		// declare buffers to hold single line from file
	char gline[132] = {'/n'};	
	char temp[10];					// buffer to hold line number
	char lineNum = 1;

	// Set fail data incase there are i/o problems with the files to compare.
	data.expected = XalanDOMString(" ");
	data.actual = XalanDOMString(" ");
	data.currentNode = XalanDOMString("Line: 0");

	result = fopen(outputFile, "r");
	gold   = fopen(goldFile, "r");

	if (!gold || !result)
	{
		data.msg = "Failed to open either Gold or Result file";
		data.nogold += 1;
		return false;
	}

	while( !feof(result) && !feof(gold))
	{
		fgets(gline, sizeof(gline), gold );
		fgets(rline, sizeof(rline), result );
		sprintf(temp,"%d",lineNum);

		if (ferror(gold) || ferror(result))
		{
			data.msg = "Read Error on either Gold or Result file";
			data.currentNode = XalanDOMString("Line: ") + XalanDOMString(temp);
			return false;
		}

		int i = 0;
		while(i < sizeof(gline)) 
		{
			if (gline[i] == rline[i]) 
			{
				i++;
				continue;
			}
			else 
			{
				data.msg = "Error: Text based comparison failure";
				data.expected = XalanDOMString("<![CDATA[") + XalanDOMString(gline) + XalanDOMString("]]>");
				data.actual = XalanDOMString("<![CDATA[") + XalanDOMString(rline) + XalanDOMString("]]>");
				data.currentNode = XalanDOMString("Line: ") + XalanDOMString(temp);
				data.fail += 1;
				return false;
			}
		}

		lineNum += 1;
	}

	return true;
}



/*	This routine performs a DOM Comparision. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filename
//		
//	Returns: 
//		True or False
//		
*/
bool 
FileUtility::domCompare(const XalanNode& gold ,const XalanNode& doc)
{
	const XalanNode::NodeType	docNodeType  = doc.getNodeType();
	const XalanNode::NodeType	goldNodeType = gold.getNodeType();

	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&	docNodeValue  = doc.getNodeValue();
	const XalanDOMString&	goldNodeValue = gold.getNodeValue();


	if (goldNodeType != docNodeType)
	{
		collectData("Error: NodeType mismatch.",
					docNodeName,
					XalanDOMString(xalanNodeTypes[docNodeType]),
					XalanDOMString(xalanNodeTypes[goldNodeType]));
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

//#if !defined(NDEBUG) && defined(_MSC_VER)
//		cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeValue)) << endl;
//#endif
		
		if(goldNodeValue != docNodeValue)
		{
			collectData("Error: Text node mismatch. ", 
						 docNodeName,
						 goldNodeValue,
						 docNodeValue);
			return false;
		}

		// Need to process textnode siblings.  Note that text nodes do not have child nodes.
		const XalanNode	*goldNextNode;
		const XalanNode	*domNextNode;
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
				collectData("Error: Element missing SiblingNode. ", 
						 docNodeName,
						 goldNextNode->getNodeName(),
						 goldNextNode->getNodeName());
				return false;
			}
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

//#if !defined(NDEBUG) && defined(_MSC_VER)
//		cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
//#endif
		// We should never reach this code path.  The parser should flag the fact that there
		// is no Document_Node.
		if (goldNodeName != docNodeName)  
		{
			assert(goldNodeName != docNodeName);
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

/*	This routine compares two element nodes. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filenam
//		
//	Returns: 
//		True or False
//						
*/

bool
FileUtility::diffElement(const XalanNode& gold, const XalanNode& doc)
{
	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&  docNsUri  = doc.getNamespaceURI();
	const XalanDOMString&  goldNsUri = gold.getNamespaceURI();

//#if !defined(NDEBUG) && defined(_MSC_VER)
//	cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
//#endif

	// This essentially checks 2 things, that the prefix and localname are the
	// same.  So specific checks of these items are not necessary.
	if (goldNodeName != docNodeName)
	{
		
		collectData("Error: Element mismatch. ", 
						 docNodeName,
						 goldNodeName,
						 docNodeName);
		return false;
	}

	if ( goldNsUri != docNsUri)
	{

		collectData("Error: Element NamespaceURI mismatch. ",
						 docNodeName,
						 goldNsUri,
						 docNsUri);
		return false;
	}

	// Get Attributes for each Element Node. 
	const XalanNamedNodeMap	*goldAttrs = gold.getAttributes();
	const XalanNamedNodeMap *docAttrs  = doc.getAttributes();
	
	// Get number of Attributes
	int numGoldAttr = goldAttrs->getLength();
	int numDomAttr  = docAttrs ->getLength();

	// Check that each Element has same number of Attributes. If they don't report error 
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
				if( ! (diffAttr(gAttr, dAttr)) )
					return false;
			}
			else
			{
				collectData("Error: Element missing named Attribute. ",
						 docNodeName,
						 goldAttrName,
						 XalanDOMString("NOTHING"));
				return false;
			}
		}
	}
	else
	{
		char  buf1[2], buf2[2];
		sprintf(buf1, "%d", numGoldAttr);
		sprintf(buf2, "%d", numDomAttr);
		collectData("Error: Elements don't have same number of attributes. ",
						 docNodeName,
						 XalanDOMString(buf1),
						 XalanDOMString(buf2));
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
			collectData("Error: Element missing ChildNode. ", 
						 docNodeName,
						 XalanDOMString(goldNextNode->getNodeName()),
						 XalanDOMString("NOTHING"));
			return false;
		}
	}
	else if (domNextNode)
	{
		// The result doc has additional Children. If the additional node is a text node
		// then gather up the text and print it out.
		if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
		{
			collectData("Error: Transformed Doc has additional Child nodes: ", 
					docNodeName,
					XalanDOMString("NOTHING"),		 
					XalanDOMString(domNextNode->getNodeName()) + XalanDOMString("  \"") +
					XalanDOMString(domNextNode->getNodeValue()) + XalanDOMString("\""));
		}
		// Additional node is NOT text, so just print it's Name.
		else
		{
			collectData("Error: Transformed Doc has additional Child node: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()));

		}
		return false;

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
		{	// domcomtest10 used to fail here,  now it is caught above, with the error
			// "Transformed Doc has additional Child nodes:"
			collectData("Error: Element missing SiblingNode. ", 
						 docNodeName,
						 XalanDOMString(goldNextNode->getNodeName()),
						 XalanDOMString("NOTHING"));
			return false;
		}
	}
	else if ( domNextNode)
	{	
			// The result doc has additional siblings. If the additional node is a text node
			// then gather up the text and print it out.
			if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
			{
				collectData("Error: Transformed Doc has additional sibling nodes: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()) + XalanDOMString("  \"") +
						XalanDOMString(domNextNode->getNodeValue()) + XalanDOMString("\""));
			}
			// Additional node is NOT text, so just print it's Name.
			else
			{
				collectData("Error: Transformed Doc has additional sibling node: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()));

			}
			return false;

	}

	return true;
}


/*	This routine compares two attribute nodes. 
//	Inputs: 
//		gAttr - attribute from Gold dom tree 
//		dAttr - attribute from Dom tree created during transformation
//		fileName - Current filenam
//		
//	Returns: 
//		True or False
//				
*/

bool FileUtility::diffAttr(const XalanNode* gAttr, const XalanNode* dAttr)
{

	const XalanDOMString& docAttrName  = dAttr->getNodeName();

//#if !defined(NDEBUG) && defined(_MSC_VER)
//	const XalanDOMString& goldAttrName = gAttr->getNodeName();
//	cout << "	Attribute is: " << c_str(TranscodeToLocalCodePage(goldAttrName)) << endl;
//#endif

	const XalanDOMString& goldAttrNsUri = gAttr->getNamespaceURI();
	const XalanDOMString& docAttrNsUri	= dAttr->getNamespaceURI();

	const XalanDOMString& goldAttrValue = gAttr->getNodeValue();
	const XalanDOMString& docAttrValue	= dAttr->getNodeValue();

	if (goldAttrValue != docAttrValue)
	{
		collectData("Error: Attribute Value mismatch. ",
						 docAttrName,
						 goldAttrValue,
						 docAttrValue);
		return false;
	}

	if (goldAttrNsUri != docAttrNsUri)
	{
		collectData("Error: Attribute NamespaceURI mismatch. ", 
						 docAttrName,
						 goldAttrNsUri,
						 docAttrNsUri);
		return false;
	}

	return true;
}

/*	This routine reports DOM comparison errors. 
//	Inputs: 
//		file	-	Name of current file
//		node	-	Current node that fails
//		msg		-	Failure message
//				
*/
void
FileUtility::reportDOMError()
{

	cout << endl << "* Failed "<< data.testOrFile 
		 << "  " << data.msg << endl
		 << "	" << "Processing Node: " << data.currentNode << endl
		 << "	Expected:	" << data.expected << endl
		 << "	Actual:		" << data.actual << endl;

}

/*	This routine collects up data pertinent to a dom comparison failure. 
//	Inputs: 
//		errmsg:			Reason for the failure.
//		currentnode:	Node in the dom tree where the mismatch occured
//		expdata:		Expected data based on the Gold file.
//		actdata:		Actual data returned in the result file.
//	Returns: 
//		Void
//						
*/
void 
FileUtility::collectData(char* errmsg, XalanDOMString currentnode, XalanDOMString expdata, XalanDOMString actdata)
{

	data.msg = errmsg;
	data.currentNode = currentnode;
	data.expected = expdata;
	data.actual = actdata;
	data.fail += 1;
}

void
FileUtility::reportPassFail(XMLFileReporter& logfile)
{
	Hashtable runResults;
	char temp[5];

	// Create entrys that contain numbers for Pass and Fail.
	sprintf(temp, "%d", data.pass);
	runResults.insert(Hashtable::value_type(XalanDOMString("Passed"), XalanDOMString(temp)));
	
	sprintf(temp, "%d", data.fail);
	runResults.insert(Hashtable::value_type(XalanDOMString("Failed"), XalanDOMString(temp)));

	logfile.logElementWAttrs(10, "RunResults", runResults, "xxx");	

	cout << "\nPassed " << data.pass;
	cout << "\nFailed " << data.fail;
	cout << "\nMissing Gold " << data.nogold << endl;

}
