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

// XALAN HEADERS...
//	Are included by FileUtility.hpp

#include "FileUtility.hpp"

bool diffElement(const XalanNode& gold, const XalanNode& doc, const XalanDOMString& fileName);
bool diffATTR(const XalanNode* gAttr, const XalanNode* dAttr, const XalanDOMString& fileName);
void reportDOMError( XalanDOMString file, XalanDOMString node, char* msg);

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
//											*/	

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
void
FileUtility::compareResults(const XalanDOMString& theOutputFile, 
			   const XalanCompiledStylesheet* compiledSS, 
			   XalanSourceTreeDocument* dom,
			   XalanDOMString fileName,
			   const XSLTInputSource& goldInputSource)
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


	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);
	
	XalanDocument* goldDom = parserLiaison.parseXMLStream(goldInputSource);
	if ( domCompare(*goldDom, *dom, fileName) )
	{
		cout << endl << "Passed: " << c_str(TranscodeToLocalCodePage(fileName));
	}

}

void
FileUtility::compareSerializedResults(const XSLTInputSource& transformResult,
									const XSLTInputSource& goldInputSource,
									XalanDOMString fileName, const char* testCase)
{
	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);
	
	XalanDocument* goldDom = parserLiaison.parseXMLStream(goldInputSource);
	XalanDocument* transformDom = parserLiaison.parseXMLStream(transformResult);
	if ( domCompare(*goldDom, *transformDom, fileName) )
	{
		cout << endl << "Passed: " << testCase;
	}

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
FileUtility::domCompare(const XalanNode& gold ,const XalanNode& doc,  const XalanDOMString& fileName)
{
	const XalanNode::NodeType	docNodeType  = doc.getNodeType();
	const XalanNode::NodeType	goldNodeType = gold.getNodeType();

	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&	docNodeValue  = doc.getNodeValue();
	const XalanDOMString&	goldNodeValue = gold.getNodeValue();


	if (goldNodeType != docNodeType)
	{
		reportDOMError(fileName, docNodeName, "Error: NodeType mismatch. Expected: ");
		cout << xalanNodeTypes[goldNodeType];				
		return false;
	}

	switch (goldNodeType)
	{
	case XalanNode::ELEMENT_NODE:	// ATTRIBUTE_NODE's are processed with diffElement().
	{ 

		if ( ! diffElement(gold, doc, fileName) ) 
		{
			return false;
		}

		break;
	}
	case XalanNode::TEXT_NODE:	
	{

#if !defined(NDEBUG) && defined(_MSC_VER)
		cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeValue)) << endl;
#endif
		
		if(goldNodeValue != docNodeValue)
		{
			reportDOMError(fileName, docNodeName, "Error: Text node mismatch. Expected: ");
			cout << c_str(TranscodeToLocalCodePage(goldNodeValue));
			cout << c_str(TranscodeToLocalCodePage(docNodeValue));
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
				if ( ! domCompare(*goldNextNode, *domNextNode, fileName) )
					return false;
			}
			else
			{
				reportDOMError(fileName, docNodeName, "Error: Element missing SiblingNode. Expected: ");
				cout << c_str(TranscodeToLocalCodePage(goldNextNode->getNodeName()));
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

#if !defined(NDEBUG) && defined(_MSC_VER)
		cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
#endif

		if (goldNodeName != docNodeName)  
		{
			reportDOMError(fileName, docNodeName, "Error: Missing Document Node");
		}
		else
		{
			const XalanNode	*goldNextNode;
			const XalanNode	*domNextNode;

			goldNextNode = gold.getFirstChild();
			domNextNode = doc.getFirstChild();

			if (0 != goldNextNode)
			{
				if( ! domCompare(*goldNextNode,*domNextNode, fileName) )
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
FileUtility::diffElement(const XalanNode& gold, const XalanNode& doc, const XalanDOMString& fileName)
{
	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&  docNsUri  = doc.getNamespaceURI();
	const XalanDOMString&  goldNsUri = gold.getNamespaceURI();

	//const XalanDOMString&  docPrefix = doc.getPrefix();
	//const XalanDOMString&  goldPrefix = gold.getPrefix();

	//const XalanDOMString& docLName = doc.getLocalName();
	//const XalanDOMString& goldLName = gold.getLocalName();

#if !defined(NDEBUG) && defined(_MSC_VER)
	cout << "Node is: " << c_str(TranscodeToLocalCodePage(docNodeName)) << endl;
#endif

	// This essentially checks 2 things, that the prefix and localname are the
	// same.  So specific checks of these items are not necessary.
	if (goldNodeName != docNodeName)
	{
		reportDOMError(fileName, docNodeName, "Error: Element mismatch. Expected: ");
		cout << c_str(TranscodeToLocalCodePage(goldNodeName));		
		return false;
	}

	if ( goldNsUri != docNsUri)
	{
		reportDOMError(fileName, docNodeName, "Error: Element NamespaceURI mismatch. Expected: ");
		cout << c_str(TranscodeToLocalCodePage(goldNsUri));
		return false;
	}

	if ( goldNsUri != docNsUri)
	{
		reportDOMError(fileName, docNodeName, "Error: Element NamespaceURI mismatch. Expected: ");
		cout << c_str(TranscodeToLocalCodePage(goldNsUri));
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
				if( ! (diffATTR(gAttr, dAttr, fileName)) )
					return false;
			}
			else
			{
				reportDOMError(fileName, docNodeName, "Error: Element missing named Attribute. Expected: ");
				cout << c_str(TranscodeToLocalCodePage(goldAttrName));
				return false;
			}
		}
	}
	else
	{
		reportDOMError( fileName, docNodeName, "Error: Elements don't have same number of attributes. Expected: ");
		cout << numGoldAttr;;
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
			if ( ! domCompare(*goldNextNode, *domNextNode, fileName) )
				return false;
		}
		else
		{
			reportDOMError(fileName, docNodeName, "Error: Element missing ChildNode. Expected: ");
			cout <<  c_str(TranscodeToLocalCodePage(goldNextNode->getNodeName()));
			return false;
		}
	}

	goldNextNode = gold.getNextSibling();
	domNextNode = doc.getNextSibling();

	if (0 != goldNextNode)
	{
		if (0 != domNextNode)
		{
			if ( ! domCompare(*goldNextNode, *domNextNode, fileName) )
				return false;
		}
		else
		{
			reportDOMError(fileName, docNodeName, "Error: Element missing SiblingNode. Expected: ");
			cout << c_str(TranscodeToLocalCodePage(goldNextNode->getNodeName()));
			return false;
		}
	}
	else if ( domNextNode)
	{
			reportDOMError(fileName, docNodeName, "Error: Transformed Doc has additional sibling nodes: ");
			cout << c_str(TranscodeToLocalCodePage(domNextNode->getNodeName())) << endl; 
			if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
			{
				cout << "	"
					 << "\"" << c_str(TranscodeToLocalCodePage(domNextNode->getNodeValue())) << "\"" << endl;
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

bool FileUtility::diffATTR(const XalanNode* gAttr, const XalanNode* dAttr, const XalanDOMString& fileName)
{

	const XalanDOMString& docAttrName  = dAttr->getNodeName();

#if !defined(NDEBUG) && defined(_MSC_VER)
	const XalanDOMString& goldAttrName = gAttr->getNodeName();
	cout << "	Attribute is: " << c_str(TranscodeToLocalCodePage(goldAttrName)) << endl;
#endif

	const XalanDOMString& goldAttrNsUri = gAttr->getNamespaceURI();
	const XalanDOMString& docAttrNsUri	= dAttr->getNamespaceURI();

	//const XalanDOMString& goldAttrPrefix = gAttr->getPrefix();
	//const XalanDOMString& docAttrPrefix = dAttr->getPrefix();

	//const XalanDOMString& goldAttrLName = gAttr->getLocalName();
	//const XalanDOMString& docAttrLName = dAttr->getLocalName();
	
	const XalanDOMString& goldAttrValue = gAttr->getNodeValue();
	const XalanDOMString& docAttrValue	= dAttr->getNodeValue();

	if (goldAttrValue != docAttrValue)
	{
		reportDOMError(fileName, docAttrName, "Error: Attribute Value mismatch. Expected: ");
		cout << c_str(TranscodeToLocalCodePage(goldAttrValue));							
		return false;
	}

	if (goldAttrNsUri != docAttrNsUri)
	{
		reportDOMError(fileName, docAttrName, "Error: Attribute NamespaceURI mismatch. Expected: ");
		cout << c_str(TranscodeToLocalCodePage(goldAttrNsUri)); 						
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
FileUtility::reportDOMError( XalanDOMString file, XalanDOMString node, char* msg)
{

	cout << endl << "Failed "<< file << endl
		 << "	Processing node : " << node << endl
		 << "	" << msg ;
}

