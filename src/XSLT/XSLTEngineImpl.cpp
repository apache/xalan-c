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
/**
 * $Id$
 * 
 * $State$
 * 
 * @author Scott Boag (scott_boag@lotus.com)
 	@@ Need to check this text based on newer implementation:
 * 
 * It is the responsibility of the XSLTEngineImpl class to traverse the 
 * source DOM tree according to the instructions in the style DOM tree, 
 * and output nodes into the result DOM tree.
 * 
 * Basic flow of control:
 * Starting from process(Document sourceTree, Document out, String mode),
 * 1)  init the Stylesheet instances (which hold the state for each imported 
 *	   XSL stylesheet) in the parseXSLRoot method, then 
 * 2)  locate the rootRule (all rules are found via Stylesheet.findTemplate), 
 *	   then pass the root node of the source tree to 
 * 3)  buildResultFromTemplate, which processes each of the template's 
 *	   child elements by calling 
 * 4)  processTemplateElement, which either processes the element 
 *	   by handling it as an XSL directive (if it is in the xsl: namespace), 
 *	   or by cloning the source node to the result tree.  
 *	   If it is an XSL instruction,
 * 5)  processXSLTemplateInstruction is called, which calls various 
 *	   subfunctions according to the tag name.
 *	   The XSL directives that effect the source traversal are xsl:apply-templates 
 *	   and xsl:for-each, which are handled in the 
 * 6)  handleApplyTemplatesInstruction, which selects the start node and 
 *	   calls the 
 * 7)  transformSelectedChildren method, which performs a query or just 
 *	   traverses the child list, then for each node in the selected list calls 
 * 8)  transformChild, which then calls 
 * 9)  Stylesheet.findTemplate (if it is not an xsl:for-each instruction)
 *	   to locate the corresponding template, and then calls 
 * 10) buildResultFromTemplate, recursing back in to step 3 and starting the 
 *	   traversal cycle all over again.
 * 
 * Glossory (abbreviations used in variable and method 
 * names in square brackets):
 * <DL>
 *	 <DT>XSL Instruction [instruction]</DT>
 *	   <DD>Any tag with an XSL namespace prefix.</DD>
 *	 <DT>XSL Template Instruction [instruction]</DT>
 *	   <DD>Any tag with an XSL namespace prefix
 *		   that occurs inside an xsl:template element.</DD>
 *	 <DT>Template Child [templateChild]</DT>
 *		 <DD>Any node that is a child of an xsl:template element.</DD>
 *	 <DT>Source Tree</DT>
 *	   <DD>The tree input to the XSL process.</DD>
 *	 <DT>Result Tree</DT>
 *	   <DD>The tree that is output by the XSL process.</DD>
 *	 <DT>Stylesheet Tree</DT>
 *	   <DD>The stylesheet tree produced from the XSL file.</DD>
 *	 <DT>Pattern List [patternList]</DT>
 *		 <DD>A parsed query or match pattern.</DD>
 * </DL>
 * 
 */



// Class header file.
#include "XSLTEngineImpl.hpp"



#include <dom/DOM_DOMException.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_ProcessingInstruction.hpp>
#include <sax/DocumentHandler.hpp>
#include <sax/SaxException.hpp>
#include <util/StdOut.hpp>
#include <util/PlatformUtils.hpp>
#include <framework/URLInputSource.hpp>

#include "Arg.hpp"
#include "ContextMarker.hpp"
#include "ElemWithParam.hpp"
#include "ElementMarker.hpp"
#include "FunctionCurrent.hpp"
#include "FunctionFormatNumber.hpp"
#include "FunctionKey.hpp"
#include "FunctionUnparsedEntityURI.hpp"
#include "FunctionSystemProperty.hpp"
#include "FunctionGenerateID.hpp"
#include "NodeSortKey.hpp"
#include "NodeSorter.hpp"
#include "ProblemListener.hpp"
#include "ProblemListenerDefault.hpp"
#include "Stylesheet.hpp"
#include "StylesheetHandler.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTProcessorException.hpp"

#include "GenerateEvent.hpp"
// @@ LATER #include "FunctionDocument.hpp"

// ?? #include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <Include/DOMHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/PrintWriter.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <XMLSupport/Formatter.hpp>
#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>

#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/ResultTreeFrag.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathSupport.hpp>
#include <XPath/XString.hpp>
#include <XPath/XResultTreeFrag.hpp>

#include <DOMSupport/UnimplementedNode.hpp>
#include <DOMSupport/UnimplementedElement.hpp>
#include <DOMSupport/UnimplementedDocumentFragment.hpp>


const double XSLTEngineImpl::s_XSLTVerSupported(1.0);
const DOMString	XSLTEngineImpl::s_DefaultXSLNameSpaceURL("http://www.w3.org/1999/XSL/Transform/1.0");
const DOMString XSLTEngineImpl::s_XSLNameSpaceURLPre("http://www.w3.org/1999/XSL/Transform");
const DOMString	XSLTEngineImpl::s_XSLT4JNameSpaceURL("http://xml.apache.org/xslt");

/**
 * Control if the xsl:variable is resolved early or 
 * late. Resolving the xsl:variable
 * early is a drag because it means that the fragment 
 * must be created into a DocumentFragment, and then 
 * cloned each time to the result tree.  If we can resolve 
 * late, that means we can evaluate directly into the 
 * result tree.  Right now this must be kept on 'early' 
 * because you would need to set the call stack back to 
 * the point of xsl:invoke... which I can't quite work out 
 * at the moment.  I don't think this is worth fixing 
 * until NodeList variables are implemented.
 */
const bool										XSLTEngineImpl::m_resolveContentsEarly = true;

const XSLTEngineImpl::AttributeKeysMapType		XSLTEngineImpl::s_attributeKeys =
					XSLTEngineImpl::InitializeAttributeKeysTable();

const XSLTEngineImpl::ElementKeysMapType		XSLTEngineImpl::s_elementKeys =
					XSLTEngineImpl::InitializeElementKeysTable();

const XSLTEngineImpl::ElementKeysMapType		XSLTEngineImpl::s_XSLT4JElementKeys =
					XSLTEngineImpl::InitializeXSLT4JElementKeys();


const XSLTEngineImpl::StaticInitializer			XSLTEngineImpl::s_staticInitializer;



//==========================================================
// SECTION: Constructors
//==========================================================

XSLTEngineImpl::XSLTEngineImpl(
							XMLParserLiaison &parserLiaison,
							XPathSupport&		xpathSupport,
							XPathEnvSupport&		xpathEnvSupport,
							XObjectFactory* xobjectFactory,
							XPathFactory *xpathFactory) :
	/* was:
XSLTEngineImpl::XSLTEngineImpl(XMLParserLiaison&	parserLiaison, XPathSupport&		xpathSupport)
	*/
	XSLTProcessor(),
	DocumentHandler(),
	m_stylesheetRoot(0),
	m_stylesheets(),
	m_sourceDocs(),
	m_rootDoc(),
	m_XSLNameSpaceURL(s_DefaultXSLNameSpaceURL),
	m_XSLDirectiveLookup(),
	m_quietConflictWarnings(false),
	m_traceTemplateChildren(false),
	m_traceTemplates(false),
	m_traceSelects(false),
	m_diagnosticsPrintWriter(0),
	m_durationsTable(),
	m_pendingElementName(),
	m_pendingAttributes(),
	m_resultNameSpaces(),
	m_emptyNamespace(),
	m_uniqueNSValue(0),
	m_useATVsInSelects(false),
	m_cssKeys(),
	m_translateCSS(false),
	m_stripWhiteSpace(false),
	m_outputCarriageReturns(false),
	m_outputLinefeeds(false),
	m_topLevelParams(),
	m_parserLiaison(parserLiaison),
	m_xpathSupport(xpathSupport),
	m_xpathEnvSupport(xpathEnvSupport),
	m_xpathFactory(xpathFactory),
	m_xobjectFactory(xobjectFactory),
	m_formatter(0),
	m_flistener(0),
	m_resultTreeFactory(),
	m_resultNameSpacePrefix(),
	m_resultNameSpaceURL(),
	m_contextNodeList(),
	m_keyDeclarations(),
	m_keys(),
	m_needToBuildKeysTable(false),
	m_currentNode(),

	m_namedTemplates(),
	m_topLevelVariables(),

	m_problemListener(new ProblemListenerDefault()),
	m_needToCheckForInfiniteLoops(false),
	m_stackGuard(*this),
	m_variableStacks(*this)
{
	// @@ what's this
	// FormatterToXML.initEncodings();

	// java: m_parserLiaison.setEnvSupport(this);
	
	 /*
	 // @@ JMD: Do we need this ??
	if(m_parserLiaison instanceof Formatter)
	{
		m_formatter = (Formatter)m_parserLiaison;
	}
	 */
	initXPath(m_xpathFactory);
}

/**
 * Init anything to do with XPath.
 */
void XSLTEngineImpl::initXPath(XPathFactory* xpathFactory)
{
	if(0 != xpathFactory)
		m_xpathFactory = xpathFactory;
			
	// @@ Not in parser liaison or friends
	// m_parserLiaison.setProcessorOwner(this);
	m_xpathProcessor = new XPathProcessorImpl(m_xpathEnvSupport, m_xpathSupport);
	m_xpath = m_xpathFactory->create();
	// @@ LATER m_xpath->installFunction("document", FunctionDocument());
}

/**
 * Reset the state.  This needs to be called after a process() call 
 * is invoked, if the processor is to be used again.
 */
void XSLTEngineImpl::reset()
{
	m_rootDoc = 0;
	m_XSLNameSpaceURL = s_DefaultXSLNameSpaceURL;
	m_durationsTable.clear();
	m_stylesheetLocatorStack.clear();
	m_pendingElementName = "";
	m_pendingAttributes.clear();
	m_cdataStack.clear();
	m_resultTreeFactory = 0;
	m_contextNodeList.clear();
	m_currentNode = 0;
	m_needToCheckForInfiniteLoops = false;
	m_variableStacks.reset();

	m_stackGuard.clear();
	m_xpathSupport.reset();
	m_parserLiaison.reset();
	m_xpathEnvSupport.reset();
	m_xpathFactory->reset();
	m_xobjectFactory->reset();

	delete m_stylesheetRoot;
	m_stylesheetRoot = 0;


}


XSLTEngineImpl::~XSLTEngineImpl()
{
	delete m_problemListener;
	delete m_xpathProcessor;

	reset();

	m_diagnosticsPrintWriter = 0;
	m_xpathFactory = 0;
	m_xobjectFactory = 0;
	m_xpath = 0;
	m_formatter = 0;
	m_flistener = 0;
}


//==========================================================
// SECTION: Main API Functions
//==========================================================

AttributeListImpl& 
XSLTEngineImpl::getPendingAttributes()
{
		return m_pendingAttributes;
}

DOMString
XSLTEngineImpl::getPendingElementName() const
{
		return m_pendingElementName;
}

void
XSLTEngineImpl::setPendingAttributes(const AttributeList&	pendingAttributes)
{
		m_pendingAttributes = pendingAttributes;
}	
void
XSLTEngineImpl::setPendingElementName(const DOMString&	elementName)
{
		m_pendingElementName = elementName;
}

void
XSLTEngineImpl::process(XSLTInputSource *inputSource, 
	                     XSLTInputSource *stylesheetSource,
	                     XSLTResultTarget* outputTarget)
{
	try
	{
		DOMString xslIdentifier(((0 == stylesheetSource) || 
										(0 == stylesheetSource->getSystemId())) 
										 ? DOMString("Input XSL") : stylesheetSource->getSystemId());
		bool totalTimeID = true;
		pushTime(&totalTimeID);
		DOM_Node sourceTree;
		if(0 != inputSource)
			sourceTree = getSourceTreeFromInput(inputSource);
		if(0 != stylesheetSource)
		{				
			m_stylesheetRoot = processStylesheet(stylesheetSource);
		}
		else if(0 != sourceTree)
		{
			DOMString stylesheetURI = 0;
			DOM_NodeList children = sourceTree.getChildNodes();
			int nNodes = children.getLength();
			Stack <DOMString> hrefs;
			for(int i = 0; i < nNodes; i++)
			{
				DOM_Node child = children.item(i);
				if(DOM_Node::PROCESSING_INSTRUCTION_NODE == child.getNodeType())
				{
				 /*DOM_ProcessingInstruction*/const DOM_Node& pi = child;
					if(pi.getNodeName().equals("xml-stylesheet") 
						 || pi.getNodeName().equals("xml:stylesheet"))
					{
						bool isOK = true;
						StringTokenizer 	tokenizer(pi.getNodeValue(), " \t=", true);
						while(tokenizer.hasMoreTokens())
						{
							if(tokenizer.nextToken().equals("type"))
							{
								DOMString typeVal = tokenizer.nextToken();
								typeVal = substring(typeVal, 1, typeVal.length()-1);
								if(!typeVal.equals("text/xsl"))
								{
									isOK = false;
								}
							}
						}	
						
						if(isOK)
						{
							StringTokenizer 	tokenizer(pi.getNodeValue(), " \t=", true);
							while(tokenizer.hasMoreTokens())
							{
								if(tokenizer.nextToken().equals("href"))
								{
									stylesheetURI = tokenizer.nextToken();
									stylesheetURI = substring(stylesheetURI, 1, stylesheetURI.length()-1);
									hrefs.push(stylesheetURI);
								}
							} 
							// break;
						}
					}
				}
			} // end for(int i = 0; i < nNodes; i++)
			bool isRoot = true;
			Stylesheet* prevStylesheet = 0;
			while(!hrefs.empty())
			{
				const XMLCh *pxch = (0 != inputSource) ?
					inputSource->getSystemId() : 0;
				DOMString sysid(pxch);
				DOMString ref =  hrefs.pop();
				Stylesheet* stylesheet =
					getStylesheetFromPIURL(ref, sourceTree, sysid, isRoot);
				if(false == isRoot)
				{
					prevStylesheet->getImports().push_back(stylesheet);
				}
				prevStylesheet = stylesheet;
				isRoot = false;
			}
		}
		else
		{
			error("Stylesheet input was not specified!");
		}
								
		if(0 == m_stylesheetRoot)
		{
			error("Failed to process stylesheet!");
		}
		
		if(0 != sourceTree)
		{
			m_stylesheetRoot->process(sourceTree, outputTarget);
			if(0 != m_diagnosticsPrintWriter)
			{
				displayDuration("Total time", &totalTimeID);
			}
		}
	}
/*
	catch(MalformedURLException mue)
	{
		error(mue.getMessage(), mue);
		// throw se;
	}
	catch(FileNotFoundException fnfe)
	{
		error(fnfe.getMessage(), fnfe);
		// throw se;
	}
	catch(IOException ioe)
	{
		error(ioe.getMessage(), ioe);
		// throw se;
	}
 */
	catch(SAXException& se)
	{
		message("SAX Exception");
		throw se;
	}
	catch (...)
	{
		assert(0);
	}
}

StylesheetRoot* XSLTEngineImpl::processStylesheet(const DOMString &xsldocURLString)
{
	try
	{
		std::auto_ptr<XMLURL> url(getURLFromString(xsldocURLString));

		XSLTInputSource input(url->getURLText(), 0);
		return processStylesheet(&input);
	}
	catch(SAXException& se)
	{
		message("processStylesheet not successful!");
		throw se;
	}
	return 0;
}

StylesheetRoot* XSLTEngineImpl::processStylesheet(XSLTInputSource* stylesheetSource)

{
	if(0 != m_stylesheetRoot)
		reset();
	DOMString xslIdentifier(((0 == stylesheetSource) || 
									(0 == stylesheetSource->getSystemId())) 
									 ? DOMString("Input XSL") : stylesheetSource->getSystemId());
	// In case we have a fragment identifier, go ahead and 
	// try to parse the XML here.
	try
	{
		m_stylesheetRoot = new StylesheetRoot(this, stylesheetSource->getSystemId());
		addTraceListenersToStylesheet();

		StylesheetHandler	stylesheetProcessor(*this, *m_stylesheetRoot);
		if(0 != stylesheetSource->getNode())
		{
			FormatterListener& flistener =
				dynamic_cast<FormatterListener&>(stylesheetProcessor);
			FormatterTreeWalker tw(flistener);
			tw.traverse(stylesheetSource->getNode());
		}
		else
		{
			diag("========= Parsing "+xslIdentifier+" ==========");
			pushTime(&xslIdentifier);
			m_parserLiaison.parseXMLStream(*stylesheetSource,
										   stylesheetProcessor);
			if(0 != m_diagnosticsPrintWriter)
				displayDuration("Parse of "+xslIdentifier, &xslIdentifier);
		}
	}
	catch(const XSLException&	e)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}

	catch(const SAXException&	e)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}


	catch(const XMLException&	e)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}



	return m_stylesheetRoot;
}
//==========================================================
// SECTION: XML Parsing Functions
//==========================================================

const DOM_Node XSLTEngineImpl::getSourceTreeFromInput(XSLTInputSource *inputSource)
{
	DOM_Node sourceTree;
	const DOMString ds("Input XML");
	DOMString xmlIdentifier = ((0 == inputSource) || 
										(0 == inputSource->getSystemId())) ?
										ds : inputSource->getSystemId();
	if(0 != inputSource->getNode())
	{
		sourceTree = inputSource->getNode();
	}
	else
	{
		// In case we have a fragment identifier, go ahead and 
		// try to parse the XML here.
		try
		{
			diag("========= Parsing "+xmlIdentifier+" ==========");
			pushTime(&xmlIdentifier);

			sourceTree = m_parserLiaison.parseXMLStream(*inputSource,
														xmlIdentifier);

			if(0 != m_diagnosticsPrintWriter)
				displayDuration("Parse of "+xmlIdentifier, &xmlIdentifier);
		}
		// catch(Exception e)
		// @@ Fix later
		catch(...)
		{
		/*
			java:
			// Unwrap exception
			if((e instanceof SAXException) && (null != ((SAXException)e).getException()))
			{
				// ((SAXException)e).getException().printStackTrace();
				e = ((SAXException)e).getException();
			}
			sourceTree = null; // shutup compiler
			error("Could not parse "+xmlIdentifier+" document!", e);
		*/
			error("Could not parse "+xmlIdentifier+" document!");
		}
	}
	return sourceTree;
}


DOM_Document XSLTEngineImpl::parseXML(const XMLURL& url, 
	                         DocumentHandler* docHandler, 
	                         const DOM_Document& docToRegister)
{
	// java: url.toExternalForm();
	const DOMString&	urlString = url.getURLText();
	DOM_Document	doc;
	const SourceDocumentsMapType::iterator	it = m_sourceDocs.find(urlString);
	if(it != m_sourceDocs.end())
	{
		doc = (*it).second;
		return doc;
	}
	 // java: url.toString()
	XSLTInputSource	inputSource(url.getURLText());
	if(0 != docHandler)
		m_parserLiaison.parseXMLStream(inputSource, *docHandler);
	else
		m_parserLiaison.parseXMLStream(inputSource);

	if(0 == docHandler)
	{
		// java:  doc = m_parserLiaison.getDocument();
		assert(0);	// @@ JMD: We don't handle this case right now
	}
	else
	{
		doc = docToRegister;
	}

	return doc;
}

Stylesheet* XSLTEngineImpl::getStylesheetFromPIURL(DOMString& xslURLString,
								const DOM_Node& fragBase,
								DOMString& xmlBaseIdent,
								bool isRoot)
{
	Stylesheet* stylesheet = 0;
	// ?? String[] stringHolder = { null};
	DOMString stringHolder;
	xslURLString = trim(xslURLString);
	int fragIndex = indexOf(xslURLString, '#');
	DOM_Document stylesheetDoc;
	if(fragIndex == 0)
	{
		diag("Locating stylesheet from fragment identifier...");
		DOMString fragID = substring(xslURLString, 1);
		DOM_Element nsNode;
		if (fragBase.getNodeType() == DOM_Node::DOCUMENT_NODE)
		{
			const DOM_Document& doc = static_cast<const DOM_Document&>(fragBase);
			nsNode = doc.getDocumentElement(); 
		}
		else if	(fragBase.getNodeType() == DOM_Node::ELEMENT_NODE) 
			nsNode = (static_cast<const DOM_Element&>(fragBase));
		else		
		{
			DOM_Node node = fragBase.getParentNode();
			if	(node.getNodeType() == DOM_Node::ELEMENT_NODE) 
				nsNode = (static_cast<const DOM_Element&>(node));
			else
				error("Could not identify fragment: "+fragID);
		}
		// Try a bunch of really ugly stuff to find the fragment.
		// What's the right way to do this?
		DOMString ds;
		ds = "id("; ds += fragID; ds += ")";

		ElementPrefixResolverProxy	theProxy(nsNode, m_xpathSupport);

		XObject* xobj = evalXPathStr(ds, fragBase, theProxy);

		// java: NodeList nl = xobj->nodeset();
		const NodeRefListBase* nl = &(xobj->nodeset());
		if(nl->getLength() == 0)
		{
			ds = "//*[@id='"; ds += fragID; ds += "']";
			xobj = evalXPathStr(ds, fragBase, theProxy);
			nl = &(xobj->nodeset());
			if(nl->getLength() == 0)
			{
				ds = "//*[@name='"; ds += fragID; ds += "']";
				xobj = evalXPathStr(ds, fragBase, theProxy);
				nl = &(xobj->nodeset());
				if(nl->getLength() == 0)
				{
					// Well, hell, maybe it's an XPath...
					xobj = evalXPathStr(fragID, fragBase, theProxy);
					nl = &(xobj->nodeset());
				}
			}
		}
		if(nl->getLength() == 0)
		{
			error("Could not find fragment: "+fragID);
		}
		
		DOM_Node frag = nl->item(0);
		if(DOM_Node::ELEMENT_NODE == frag.getNodeType())
		{
			pushTime(&frag);
			if(isRoot)
			{
				m_stylesheetRoot = new StylesheetRoot(this, stringHolder);
				stylesheet = m_stylesheetRoot;
			}
			else
			{
				stylesheet = new Stylesheet(*m_stylesheetRoot, this, stringHolder);
			}
			addTraceListenersToStylesheet();

			StylesheetHandler stylesheetProcessor(*this, *stylesheet);
			FormatterListener& flistener =
				dynamic_cast<FormatterListener&>(stylesheetProcessor);
			FormatterTreeWalker tw(flistener);
			tw.traverse(frag);
			
			displayDuration("Setup of "+xslURLString, &frag);
		}
		else
		{
			stylesheetDoc = 0;
			error("Node pointed to by fragment identifier was not an element: "+fragID);
		}
	}
	else
	{ 
		// hmmm.. for now I'll rely on the XML parser to handle 
		// fragment URLs.
		diag("========= Parsing and preparing "+xslURLString+" ==========");
		pushTime(&xslURLString);
		
		if(isRoot)
		{
			m_stylesheetRoot = new StylesheetRoot(this, xslURLString);
			stylesheet = m_stylesheetRoot;
		}
		else
		{
			stylesheet = new Stylesheet(*m_stylesheetRoot, this, xslURLString);
		}
		addTraceListenersToStylesheet();

		StylesheetHandler stylesheetProcessor(*this, *stylesheet);
		
		XMLURL* xslURL = getURLFromString(xslURLString, xmlBaseIdent);
		
		XSLTInputSource inputSource(xslURL->getURLText());
		// java: m_parserLiaison.setDocumentHandler(stylesheetProcessor);
		//       m_parserLiaison.parse(inputSource);
		m_parserLiaison.parseXMLStream(inputSource, stylesheetProcessor);

		displayDuration("Parsing and init of "+xslURLString, &xslURLString);
	}
	return stylesheet;
}


//==========================================================
// SECTION: Stylesheet Tables
//==========================================================


double XSLTEngineImpl::getXSLTVerSupported()
{
	return s_XSLTVerSupported;
}


//==========================================================
// SECTION: XSL directive handling functions
//==========================================================  

#if 0
void
XSLTEngineImpl::initXSLTKeys() 
{
	m_elementKeys.clear();

	m_elementKeys[Constants::ELEMNAME_APPLY_TEMPLATES_STRING] = Constants::ELEMNAME_APPLY_TEMPLATES;
	m_elementKeys[Constants::ELEMNAME_WITHPARAM_STRING] = Constants::ELEMNAME_WITHPARAM;
	m_elementKeys[Constants::ELEMNAME_CONSTRUCT_STRING] = Constants::ELEMNAME_CONSTRUCT;
	m_elementKeys[Constants::ELEMNAME_CONTENTS_STRING] = Constants::ELEMNAME_CONTENTS;
	m_elementKeys[Constants::ELEMNAME_COPY_STRING] = Constants::ELEMNAME_COPY;
	m_elementKeys[Constants::ELEMNAME_COPY_OF_STRING] = Constants::ELEMNAME_COPY_OF;

	m_elementKeys[Constants::ELEMNAME_ATTRIBUTESET_STRING] = Constants::ELEMNAME_DEFINEATTRIBUTESET;

	m_elementKeys[Constants::ELEMNAME_USE_STRING] = Constants::ELEMNAME_USE;

	m_elementKeys[Constants::ELEMNAME_VARIABLE_STRING] = Constants::ELEMNAME_VARIABLE;
	m_elementKeys[Constants::ELEMNAME_PARAMVARIABLE_STRING] = Constants::ELEMNAME_PARAMVARIABLE;

	m_elementKeys[Constants::ELEMNAME_DISPLAYIF_STRING] = Constants::ELEMNAME_DISPLAYIF;
	m_elementKeys[Constants::ELEMNAME_EMPTY_STRING] = Constants::ELEMNAME_EMPTY;
	m_elementKeys[Constants::ELEMNAME_EVAL_STRING] = Constants::ELEMNAME_EVAL;
	m_elementKeys[Constants::ELEMNAME_CALLTEMPLATE_STRING] = Constants::ELEMNAME_CALLTEMPLATE;
	m_elementKeys[Constants::ELEMNAME_TEMPLATE_STRING] = Constants::ELEMNAME_TEMPLATE;
	m_elementKeys[Constants::ELEMNAME_STYLESHEET_STRING] = Constants::ELEMNAME_STYLESHEET;
	m_elementKeys[Constants::ELEMNAME_TRANSFORM_STRING] = Constants::ELEMNAME_STYLESHEET;
	m_elementKeys[Constants::ELEMNAME_IMPORT_STRING] = Constants::ELEMNAME_IMPORT;
	m_elementKeys[Constants::ELEMNAME_INCLUDE_STRING] = Constants::ELEMNAME_INCLUDE;
	m_elementKeys[Constants::ELEMNAME_FOREACH_STRING] = Constants::ELEMNAME_FOREACH;
	m_elementKeys[Constants::ELEMNAME_VALUEOF_STRING] = Constants::ELEMNAME_VALUEOF;
	m_elementKeys[Constants::ELEMNAME_KEY_STRING] = Constants::ELEMNAME_KEY;
	m_elementKeys[Constants::ELEMNAME_STRIPSPACE_STRING] = Constants::ELEMNAME_STRIPSPACE;
	m_elementKeys[Constants::ELEMNAME_PRESERVESPACE_STRING] = Constants::ELEMNAME_PRESERVESPACE;
	m_elementKeys[Constants::ELEMNAME_NUMBER_STRING] = Constants::ELEMNAME_NUMBER;
	m_elementKeys[Constants::ELEMNAME_IF_STRING] = Constants::ELEMNAME_IF;
	m_elementKeys[Constants::ELEMNAME_CHOOSE_STRING] = Constants::ELEMNAME_CHOOSE;
	m_elementKeys[Constants::ELEMNAME_WHEN_STRING] = Constants::ELEMNAME_WHEN;
	m_elementKeys[Constants::ELEMNAME_OTHERWISE_STRING] = Constants::ELEMNAME_OTHERWISE;
	m_elementKeys[Constants::ELEMNAME_TEXT_STRING] = Constants::ELEMNAME_TEXT;
	m_elementKeys[Constants::ELEMNAME_ELEMENT_STRING] = Constants::ELEMNAME_ELEMENT;
	m_elementKeys[Constants::ELEMNAME_ATTRIBUTE_STRING] = Constants::ELEMNAME_ATTRIBUTE;
	m_elementKeys[Constants::ELEMNAME_SORT_STRING] = Constants::ELEMNAME_SORT;
	m_elementKeys[Constants::ELEMNAME_PI_STRING] = Constants::ELEMNAME_PI;
	m_elementKeys[Constants::ELEMNAME_COMMENT_STRING] = Constants::ELEMNAME_COMMENT;
   
	m_elementKeys[Constants::ELEMNAME_COUNTER_STRING] = Constants::ELEMNAME_COUNTER;
	m_elementKeys[Constants::ELEMNAME_COUNTERS_STRING] = Constants::ELEMNAME_COUNTERS;
	m_elementKeys[Constants::ELEMNAME_COUNTERINCREMENT_STRING] = Constants::ELEMNAME_COUNTERINCREMENT;
	m_elementKeys[Constants::ELEMNAME_COUNTERRESET_STRING] = Constants::ELEMNAME_COUNTERRESET;
	m_elementKeys[Constants::ELEMNAME_COUNTERSCOPE_STRING] = Constants::ELEMNAME_COUNTERSCOPE;
	
	m_elementKeys[Constants::ELEMNAME_APPLY_IMPORTS_STRING] = Constants::ELEMNAME_APPLY_IMPORTS;
	
	m_elementKeys[Constants::ELEMNAME_EXTENSION_STRING] = Constants::ELEMNAME_EXTENSION;

	m_elementKeys[Constants::ELEMNAME_MESSAGE_STRING] = Constants::ELEMNAME_MESSAGE;
	m_elementKeys[Constants::ELEMNAME_LOCALE_STRING] = Constants::ELEMNAME_LOCALE;
	m_elementKeys[Constants::ELEMNAME_FALLBACK_STRING] = Constants::ELEMNAME_FALLBACK;

	m_elementKeys[Constants::ELEMNAME_OUTPUT_STRING] = Constants::ELEMNAME_OUTPUT;

	m_XSLT4JElementKeys[Constants::ELEMNAME_CSSSTYLECONVERSION_STRING] = Constants::ELEMNAME_CSSSTYLECONVERSION;

	m_XSLT4JElementKeys[Constants::ELEMNAME_COMPONENT_STRING] = Constants::ELEMNAME_COMPONENT;
	m_XSLT4JElementKeys[Constants::ELEMNAME_SCRIPT_STRING] = Constants::ELEMNAME_SCRIPT;

	m_attributeKeys[Constants::ATTRNAME_OUTPUT_METHOD] = Constants::TATTRNAME_OUTPUT_METHOD;
	m_attributeKeys[Constants::ATTRNAME_AMOUNT] = Constants::TATTRNAME_AMOUNT;
	m_attributeKeys[Constants::ATTRNAME_ANCESTOR] = Constants::TATTRNAME_ANCESTOR;
	m_attributeKeys[Constants::ATTRNAME_ARCHIVE] = Constants::TATTRNAME_ARCHIVE;
	m_attributeKeys[Constants::ATTRNAME_ATTRIBUTE] = Constants::TATTRNAME_ATTRIBUTE;
	m_attributeKeys[Constants::ATTRNAME_ATTRIBUTE_SET] = Constants::TATTRNAME_ATTRIBUTE_SET;
	m_attributeKeys[Constants::ATTRNAME_CASEORDER] = Constants::TATTRNAME_CASEORDER;
	m_attributeKeys[Constants::ATTRNAME_CLASS] = Constants::TATTRNAME_CLASS;
	m_attributeKeys[Constants::ATTRNAME_CLASSID] = Constants::TATTRNAME_CLASSID;
	m_attributeKeys[Constants::ATTRNAME_CODEBASE] = Constants::TATTRNAME_CODEBASE;
	m_attributeKeys[Constants::ATTRNAME_CODETYPE] = Constants::TATTRNAME_CODETYPE;
	m_attributeKeys[Constants::ATTRNAME_CONDITION] = Constants::TATTRNAME_CONDITION;
	m_attributeKeys[Constants::ATTRNAME_COPYTYPE] = Constants::TATTRNAME_COPYTYPE;
	m_attributeKeys[Constants::ATTRNAME_COUNT] = Constants::TATTRNAME_COUNT;
	m_attributeKeys[Constants::ATTRNAME_DATATYPE] = Constants::TATTRNAME_DATATYPE;
	m_attributeKeys[Constants::ATTRNAME_DEFAULT] = Constants::TATTRNAME_DEFAULT;
	m_attributeKeys[Constants::ATTRNAME_DEFAULTSPACE] = Constants::TATTRNAME_DEFAULTSPACE;
	m_attributeKeys[Constants::ATTRNAME_DEPTH] = Constants::TATTRNAME_DEPTH;
	m_attributeKeys[Constants::ATTRNAME_DIGITGROUPSEP] = Constants::TATTRNAME_DIGITGROUPSEP;
	m_attributeKeys[Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING] = Constants::TATTRNAME_DISABLE_OUTPUT_ESCAPING;
	m_attributeKeys[Constants::ATTRNAME_ELEMENT] = Constants::TATTRNAME_ELEMENT;
	m_attributeKeys[Constants::ATTRNAME_ELEMENTS] = Constants::TATTRNAME_ELEMENTS;
	m_attributeKeys[Constants::ATTRNAME_EXPR] = Constants::TATTRNAME_EXPR;
	m_attributeKeys[Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES] = Constants::TATTRNAME_EXTENSIONELEMENTPREFIXES;
	m_attributeKeys[Constants::ATTRNAME_FORMAT] = Constants::TATTRNAME_FORMAT;
	m_attributeKeys[Constants::ATTRNAME_FROM] = Constants::TATTRNAME_FROM;
	m_attributeKeys[Constants::ATTRNAME_GROUPINGSEPARATOR] = Constants::TATTRNAME_GROUPINGSEPARATOR;
	m_attributeKeys[Constants::ATTRNAME_GROUPINGSIZE] = Constants::TATTRNAME_GROUPINGSIZE;
	m_attributeKeys[Constants::ATTRNAME_HREF] = Constants::TATTRNAME_HREF;
	m_attributeKeys[Constants::ATTRNAME_ID] = Constants::TATTRNAME_ID;
	m_attributeKeys[Constants::ATTRNAME_IMPORTANCE] = Constants::TATTRNAME_IMPORTANCE;
	m_attributeKeys[Constants::ATTRNAME_INDENTRESULT] = Constants::TATTRNAME_INDENTRESULT;
	m_attributeKeys[Constants::ATTRNAME_LANG] = Constants::TATTRNAME_LANG;
	m_attributeKeys[Constants::ATTRNAME_LETTERVALUE] = Constants::TATTRNAME_LETTERVALUE;
	m_attributeKeys[Constants::ATTRNAME_LEVEL] = Constants::TATTRNAME_LEVEL;
	m_attributeKeys[Constants::ATTRNAME_MATCH] = Constants::TATTRNAME_MATCH;
	m_attributeKeys[Constants::ATTRNAME_METHOD] = Constants::TATTRNAME_METHOD;
	m_attributeKeys[Constants::ATTRNAME_MODE] = Constants::TATTRNAME_MODE;
	m_attributeKeys[Constants::ATTRNAME_NAME] = Constants::TATTRNAME_NAME;
	m_attributeKeys[Constants::ATTRNAME_NAMESPACE] = Constants::TATTRNAME_NAMESPACE;
	m_attributeKeys[Constants::ATTRNAME_NDIGITSPERGROUP] = Constants::TATTRNAME_NDIGITSPERGROUP;
	m_attributeKeys[Constants::ATTRNAME_NS] = Constants::TATTRNAME_NS;
	m_attributeKeys[Constants::ATTRNAME_ONLY] = Constants::TATTRNAME_ONLY;
	m_attributeKeys[Constants::ATTRNAME_ORDER] = Constants::TATTRNAME_ORDER;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS] = Constants::TATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC] = Constants::TATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM] = Constants::TATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_ENCODING] = Constants::TATTRNAME_OUTPUT_ENCODING;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_INDENT] = Constants::TATTRNAME_OUTPUT_INDENT;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_MEDIATYPE] = Constants::TATTRNAME_OUTPUT_MEDIATYPE;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_STANDALONE] = Constants::TATTRNAME_OUTPUT_STANDALONE;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_VERSION] = Constants::TATTRNAME_OUTPUT_VERSION;
	m_attributeKeys[Constants::ATTRNAME_OUTPUT_XMLDECL] = Constants::TATTRNAME_OUTPUT_XMLDECL;
	m_attributeKeys[Constants::ATTRNAME_PRIORITY] = Constants::TATTRNAME_PRIORITY;
	m_attributeKeys[Constants::ATTRNAME_REFID] = Constants::TATTRNAME_REFID;
	m_attributeKeys[Constants::ATTRNAME_RESULTNS] = Constants::TATTRNAME_RESULTNS;
	m_attributeKeys[Constants::ATTRNAME_SELECT] = Constants::TATTRNAME_SELECT;
	m_attributeKeys[Constants::ATTRNAME_SEQUENCESRC] = Constants::TATTRNAME_SEQUENCESRC;
	m_attributeKeys[Constants::ATTRNAME_STYLE] = Constants::TATTRNAME_STYLE;
	m_attributeKeys[Constants::ATTRNAME_TEST] = Constants::TATTRNAME_TEST;
	m_attributeKeys[Constants::ATTRNAME_TOSTRING] = Constants::TATTRNAME_TOSTRING;
	m_attributeKeys[Constants::ATTRNAME_TYPE] = Constants::TATTRNAME_TYPE;
	m_attributeKeys[Constants::ATTRNAME_USE] = Constants::TATTRNAME_USE;
	m_attributeKeys[Constants::ATTRNAME_USEATTRIBUTESETS] = Constants::TATTRNAME_USEATTRIBUTESETS;
	m_attributeKeys[Constants::ATTRNAME_VALUE] = Constants::TATTRNAME_VALUE;

	m_attributeKeys[Constants::ATTRNAME_XMLNSDEF] = Constants::TATTRNAME_XMLNSDEF;
	m_attributeKeys[Constants::ATTRNAME_XMLNS] = Constants::TATTRNAME_XMLNS;
	m_attributeKeys[Constants::ATTRNAME_XMLSPACE] = Constants::TATTRNAME_XMLSPACE;
}
#endif



int
XSLTEngineImpl::getXSLToken(const DOM_Node&	node) const
{
	int 	tok = -2;

	if(DOM_Node::ELEMENT_NODE != node.getNodeType()) return tok;
	const DOMString 	ns =
			m_xpathSupport.getNamespaceOfNode(node);

	// was: toLowerCase
	if(equals(ns, m_XSLNameSpaceURL))
	{
		const DOMString 	localName =
			m_xpathSupport.getLocalNameOfNode(node);

		// $$$ ToDo: Why can't this iterator be a 
		// const_iterator?	VC++ pukes if it is.
		const ElementKeysMapType::const_iterator		j =
						s_elementKeys.find(localName);
		if(j != s_elementKeys.end())
		{
			tok = (*j).second;
		}
		else
			tok = -2;
	}
	else if(equals(ns, s_XSLT4JNameSpaceURL))
	{
		const DOMString localName = m_xpathSupport.getLocalNameOfNode(node);
		const ElementKeysMapType::const_iterator		j =
						s_XSLT4JElementKeys.find(localName);
		if(j != s_XSLT4JElementKeys.end())
		{
			tok = (*j).second;
		}
		else
			tok = -2;
	}
	else
			tok = -2;
	return tok;
}



bool
XSLTEngineImpl::isXSLTagOfType(const DOM_Node&	node,
							int 		tagType) const
{
	return getXSLToken(node) == tagType ? true : false;
}

void XSLTEngineImpl::outputToResultTree(const Stylesheet& stylesheetTree,
						XObject* value)
{
	 // java:
    // Make the return object into an XObject because it 
    // will be easier below.  One of the reasons to do this 
    // is to keep all the conversion functionality in the 
    // XObject classes.
	 // JMD: Has to be an XObject

	int type = value->getType();
	DOMString s;
	switch(type)
	{
	case XObject::eTypeBoolean:
	case XObject::eTypeNumber:
	case XObject::eTypeString:
		s = value->str();
		characters(toCharArray(s), 0, length(s));
		break;					
	case XObject::eTypeNodeSet:
		{
		const NodeRefListBase* nl = &(value->nodeset());
		int nChildren = nl->getLength();
		for(int i = 0; i < nChildren; i++)
		{
			DOM_Node		pos = nl->item(i);
			const DOM_Node&	top = pos;
			while(0 != pos)
			{
				flushPending();
				cloneToResultTree(stylesheetTree, pos, false, false, false, true);
				DOM_Node	nextNode = pos.getFirstChild();
				while(0 == nextNode)
				{
					if(DOM_Node::ELEMENT_NODE == pos.getNodeType())
					{
						endElement(c_wstr(pos.getNodeName()));
					}
					if(top == pos)
						break;
					nextNode = pos.getNextSibling();
					if(0 == nextNode)
					{
						pos = pos.getParentNode();
						if(top == pos)
						{
							if(DOM_Node::ELEMENT_NODE == pos.getNodeType())
							{
								endElement(c_wstr(pos.getNodeName()));
							}
							nextNode = 0;
							break;
						}
					}
				}
				pos = nextNode;
			}
		}
		}
		break;
		
	case XObject::eTypeResultTreeFrag:
		outputResultTreeFragment(value);
		break;
	case XObject::eTypeNull:
	case XObject::eTypeUnknown:
	case XObject::eUnknown:
	default:
		assert(0);
	}
}

/**
 * Handle an extension function.
 */
XObject*
XSLTEngineImpl::extFunction(
		XPathExecutionContext&			executionContext,
		const DOMString&				theNamespace,
		const DOMString&				extensionName, 
		const std::vector<XObject*>&	argVec) const
{
	return m_xpathEnvSupport.extFunction( executionContext,
		theNamespace, extensionName, argVec);
}

void
XSLTEngineImpl::handleFunctionsInstruction(DOM_Element	extensionElem)
{
	error("Calling external functions is not supported in the C++ version of Xalan!!!");
}

//==========================================================
// SECTION: Diagnostic functions
//==========================================================

/**
 * Add a trace listener for the purposes of debugging and diagnosis.
 * @param tl Trace listener to be added.
 */
void XSLTEngineImpl::addTraceListener(TraceListener* tl)
	// throws TooManyListenersException
{
	m_traceListeners.push_back(tl);
	if(0 != m_stylesheetRoot)
		m_stylesheetRoot->addTraceListener(tl);
}

/**
 * Add a trace listener for the purposes of debugging and diagnosis.
 * @param tl Trace listener to be added.
 */
void XSLTEngineImpl::addTraceListenersToStylesheet()
{
	if(0 == m_stylesheetRoot) return;
	try
	{
		int nListeners = m_traceListeners.size();
		for(int i = 0; i < nListeners; i++)
		{
			TraceListener* tl = m_traceListeners.at(i);
			m_stylesheetRoot->addTraceListener(tl);
		}
	}
	catch(...)
	// catch(TooManyListenersException tmle)
	{
		throw SAXException("addTraceListenersToStylesheet - TooManyListenersException");
	}
} 

/**
 * Remove a trace listener.
 * @param tl Trace listener to be removed.
 */
void XSLTEngineImpl::removeTraceListener(TraceListener* tl)
{
	int nListeners = m_traceListeners.size();
	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* tli = m_traceListeners.at(i);
		if (tli == tl) m_traceListeners.erase(m_traceListeners.begin()+i);
	}
	if(0 != m_stylesheetRoot)
		m_stylesheetRoot->removeTraceListener(tl);
}

/**
 * Fire a generate event.
 */
void XSLTEngineImpl::fireGenerateEvent(const GenerateEvent& te)
{
	int nListeners = m_traceListeners.size();
	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* tl = m_traceListeners.at(i);
		tl->generated(te);
	}
}


/**
* Warn the user of an problem.
* This is public for access by extensions.
* @exception XSLTProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
* the error condition is severe enough to halt processing.
*/
void XSLTEngineImpl::message(const DOMString& msg)
{
	DOM_Node node;
	message(node, node, msg);
}

void XSLTEngineImpl::message(	const DOM_Node& styleNode,
					const DOM_Node& sourceNode,
					const DOMString& msg)
{
	if (m_problemListener != 0)
	{
		const bool	shouldThrow =
			m_problemListener->problem(ProblemListener::eXSLPROCESSOR, 
									   ProblemListener::eMESSAGE,
									   styleNode, sourceNode,
									   c_wstr(msg), 0, 0, 0);
		if(shouldThrow == true)
		{
			throw XSLTProcessorException(msg);
		}
	}
}


void XSLTEngineImpl::problem(const DOM_Node& styleNode,
				const DOM_Node& sourceNode,
				const DOMString& msg, 
				ProblemListener::eClassification		classification) const
{
	if (m_problemListener == 0) return;
	Locator* locator = m_stylesheetLocatorStack.size() == 0 ? 0 : 
							  m_stylesheetLocatorStack.back();
	const XMLCh* id = (0 == locator) ?
						0 : (0 == locator->getPublicId()) ?
					 locator->getPublicId() : locator->getSystemId();
	const bool	shouldThrow =
	 m_problemListener->problem(ProblemListener::eXSLPROCESSOR, 
								  classification,
								  styleNode, sourceNode, msg, 
								  id, 
								  (0 == locator) ? 0: locator->getLineNumber(), 
								  (0 == locator) ? 0: locator->getColumnNumber());
	if(shouldThrow == true)
	{
		throw XSLTProcessorException(msg);
	}
}

void
XSLTEngineImpl::warn(const DOMString& msg) const
{
	warn(DOM_Node(), DOM_Node(), msg);
}



void
XSLTEngineImpl::warn(
			const DOM_Node&	styleNode,
			const DOM_Node&	sourceNode,
			const DOMString&	msg) const
{
	problem(styleNode, sourceNode, msg, ProblemListener::eWARNING);
}


void
XSLTEngineImpl::error(const DOMString&	msg) const
{
	error(DOM_Node(), DOM_Node(), msg);
}



void
XSLTEngineImpl::error(
			const DOM_Node&	styleNode,
			const DOM_Node&	sourceNode,
			const DOMString&	msg) const
{
	problem(styleNode, sourceNode, msg, ProblemListener::eERROR);
}



void
XSLTEngineImpl::pushTime(const void*	key) const
{
	if(0 != key)
	{
		m_durationsTable.insert(std::make_pair<const void* const,
										  clock_t>(key, clock()));
	}
}



clock_t
XSLTEngineImpl::popDuration(const void*	key) const
{
	clock_t 	clockTicksDuration = 0;
	if(0 != key)
	{
		const DurationsTableMapType::iterator	i =
				m_durationsTable.find(key);

		assert(i != m_durationsTable.end());

		if (i != m_durationsTable.end())
		{
			clockTicksDuration = clock() - (*i).second;
			m_durationsTable.erase(i);
		}
	}
	return clockTicksDuration;
}



void
XSLTEngineImpl::displayDuration(const DOMString&	info,
							  const void*		key) const
{
	if(0 != key)
	{
		const clock_t	theDuration = popDuration(key);
		double millis = 1000.0*theDuration/CLOCKS_PER_SEC;

		if(0 != m_diagnosticsPrintWriter)
		{
			DOMString	msg(info);

			msg += " took ";
			msg += DoubleToDOMString(millis);
			msg += " millis";

			m_diagnosticsPrintWriter->println(msg);
		}
	}
}


void
XSLTEngineImpl::setDiagnosticsOutput(PrintWriter*	pw)
{
	m_diagnosticsPrintWriter = pw;
	m_problemListener->setPrintWriter(pw);
}

void
XSLTEngineImpl::diag(const DOMString& 	s) const
{
	if (0 != m_diagnosticsPrintWriter)
	{
		m_diagnosticsPrintWriter->println(s);
	}
}

/**
 * If this is set to true, simple traces of 
 * template calls are made.
 */
void
XSLTEngineImpl::setTraceTemplates(bool	b)
{
	m_traceTemplates = b;
}

/**
 * If this is set to true, simple traces of 
 * template calls are made.
 */
void
XSLTEngineImpl::setTraceSelect(bool	b)
{
	m_traceSelects = b;
}

bool XSLTEngineImpl::isTraceSelect() const
{
	return	m_traceSelects;
}


/**
 * If this is set to true, debug diagnostics about 
 * template children as they are being constructed 
 * will be written to the m_diagnosticsPrintWriter 
 * stream.  diagnoseTemplateChildren is false by
 * default.
 */

void XSLTEngineImpl::setTraceTemplateChildren(bool	b)
{
	m_traceTemplateChildren = b;
}

void
XSLTEngineImpl::traceSelect(
			DOM_Element 	theTemplate,
			const NodeRefListBase& nl) const
{
	DOMString	msg = theTemplate.getNodeName() + DOMString(": ");
	DOM_Attr	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_SELECT);
	if(0 != attr)
	{
		msg += attr.getValue();
		msg += ", ";
		msg += LongToDOMString(nl.getLength());
		msg += " selected";
	}
	else
	{
		msg += "*|text(), (default select), ";
		msg += LongToDOMString(nl.getLength());
		msg += " selected";
	}
	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MODE);
	if(0 != attr)
	{
		msg += DOMString(", mode = ") + attr.getValue();
	}
	//System.out.println(msg);
}

  
/**
* Print a trace of a template that is being called, either by 
* a match, name, or as part of for-each.
*/
void
XSLTEngineImpl::traceTemplate(DOM_Element 	theTemplate) const
{
	DOMString	msg;
	DOM_Attr	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MATCH);
	if(0 != attr)
	{
		msg = DOMString("Calling template for: ") + attr.getValue();
	}
	else
	{
		attr = theTemplate.getAttributeNode(Constants::ATTRNAME_NAME);
		if(0 != attr) 
		{
			msg = DOMString("Calling named template, name = ") + attr.getValue();
		}
		else
		{
			const int	xslToken = getXSLToken(theTemplate);
			if(Constants::ELEMNAME_FOREACH == xslToken)
			{
				attr = theTemplate.getAttributeNode(Constants::ATTRNAME_SELECT);
				if(0 != attr)
				{
					msg = DOMString("Processing for-each, select = ") + attr.getValue();
				}
				else
				{
					error("for-each must have either a match or name attribute");
					msg = "";
				}
			}
			else
			{
				error("templates must have either a match or name attribute");

				msg = "";
			}
		}
	}

	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MODE);

	if(0 != attr)
	{
		msg += DOMString(", mode = ") + attr.getValue();
	}

	//System.out.println(msg);
}


void
XSLTEngineImpl::diagnoseTemplateChildren(
			const DOM_Node&	templateChild,
			const DOM_Node&	sourceNode) const
{
	if(m_traceTemplateChildren)
	{
		const DOMString 	templateChildTagName = templateChild.getNodeName();
		const DOMString 	xmlElemName = sourceNode.getNodeName();
		diag(DOMString("source node: ")
			 + xmlElemName
			 + DOMString(", template-node: ") 
			 + templateChildTagName);
	}
}

  

void
XSLTEngineImpl::setDocumentLocator(
			Locator* const	/* locator */)
{
	// Do nothing for now
}



void
XSLTEngineImpl::startDocument()
{
	assert(m_flistener != 0);
	m_flistener->startDocument();
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_STARTDOCUMENT);
		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::endDocument()
{
	assert(m_flistener != 0);
	flushPending();
	m_flistener->endDocument();
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_ENDDOCUMENT);
		fireGenerateEvent(ge);
	}
	m_variableStacks.popCurrentContext();
}

void XSLTEngineImpl::addResultNamespaceDecl(const DOMString& prefix, 
	                                   const DOMString& namespaceVal)
{
	NamespaceVectorType nsVector;
	NameSpace ns(prefix, namespaceVal);
	assert(m_resultNameSpaces.size() > 0);
	NamespaceVectorType nsOnStack = m_resultNameSpaces.back();
	// If the last vector contains only an empty namespace, replace it with a
	// new vector containing only this namespace
	if(isEmpty(nsOnStack.front().getURI()))
	{
		nsVector.push_back(ns);
		m_resultNameSpaces.pop_back();
		m_resultNameSpaces.push_back(nsVector);
	}
	// Otherwise, add the namespace at the end of the last vector
	else
	{
		nsOnStack.push_back(ns);
	}
}



void
XSLTEngineImpl::addResultAttribute(
			AttributeListImpl&	attList,
			const DOMString&	aname,
			const DOMString&	value)
{
	assert(length(value) > 0);

	const bool	isPrefix = startsWith(aname, "xmlns:");
	if (equals(aname, "xmlns") || isPrefix == true) 
	{
		const DOMString		p = isPrefix == true ? substring(aname, 6) : DOMString();
		addResultNamespaceDecl(p, value);
	}
	attList.removeAttribute(c_wstr(aname));
	attList.addAttribute(c_wstr(aname), c_wstr(DOMString("CDATA")), c_wstr(value));
}



void
XSLTEngineImpl::flushPending()
{
	if(0 != length(m_pendingElementName))
	{
		assert(m_flistener != 0);
		m_cdataStack.push(isCDataResultElem(m_pendingElementName)? true : false);
		m_flistener->startElement(c_wstr(m_pendingElementName), m_pendingAttributes);
		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_STARTELEMENT,
					m_pendingElementName, &m_pendingAttributes);
			fireGenerateEvent(ge);
		}
		m_pendingAttributes.clear();
		m_pendingElementName = "";
	}
}



void
XSLTEngineImpl::startElement(const XMLCh* const	name)
{
	assert(m_flistener != 0);
	assert(name != 0);
	flushPending();

	// Push a new container on the stack, then push an empty
	// result namespace on to that container.
	NamespaceVectorType nsVector;
	nsVector.push_back(m_emptyNamespace);
	m_resultNameSpaces.push_back(nsVector);
	m_pendingElementName = name;
}



void
XSLTEngineImpl::startElement(
			const XMLCh* const	name,
			AttributeList&		atts)
{
	assert(m_flistener != 0);
	assert(name != 0);
	flushPending();
	const int	nAtts = atts.getLength();
	m_pendingAttributes.clear();

	for(int i = 0; i < nAtts; i++)
	{
		m_pendingAttributes.addAttribute(atts.getName(i),
										 atts.getType(i),
										 atts.getValue(i));
	}

	// Push a new container on the stack, then push an empty
	// result namespace on to that container.
	NamespaceVectorType nsVector;
	nsVector.push_back(m_emptyNamespace);
	m_resultNameSpaces.push_back(nsVector);
	m_pendingElementName = name;
}



void
XSLTEngineImpl::endElement(const XMLCh* const 	name)
{
	assert(m_flistener != 0);
	assert(name != 0);
	flushPending();
	m_flistener->endElement(c_wstr(name));
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_ENDELEMENT, name, 0);
		fireGenerateEvent(ge);
	}
	m_resultNameSpaces.pop_back();
	Stylesheet::QNameVectorType cdataElems = m_stylesheetRoot->getCdataSectionElems();
	if(0 != cdataElems.size())
      m_cdataStack.pop();
}


void
XSLTEngineImpl::characters(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	characters(ch,
			   0,
			   length);
}



void
XSLTEngineImpl::characters(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int 	length)
{
	assert(m_flistener != 0);
	assert(ch != 0);
	flushPending();
	Stylesheet::QNameVectorType cdataElems = m_stylesheetRoot->getCdataSectionElems();
	// java:
	//	if((null != m_stylesheetRoot.m_cdataSectionElems) && 
	//		!m_cdataStack.isEmpty() && (m_cdataStack.peek() == TRUE))

	if(0 != cdataElems.size() && 0 != m_cdataStack.size())
	{
		//	java:	if(m_flistener instanceof FormatterListener)
		// @@ JMD: Need to add type member to DocumentHandler, etc ...
		// future: if (m_flistener.getType == eFormatterListener)
		if (true)		// @@ JMD: For now
		{
			FormatterListener* pfl =
				static_cast<FormatterListener*>(m_flistener);
			pfl->cdata(ch+start, length);
		}
		else
		{
			// Bad but I think it's better than dropping it.
			m_flistener->characters(ch, length);
		}

		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CDATA, ch, start, length);
			fireGenerateEvent(ge);
		}
	}
	else
	{
		m_flistener->characters(ch+start, length);
		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CHARACTERS, ch,
					start, length);
			fireGenerateEvent(ge);
		}
	}
}


/**
 * Bottleneck the charactersRaw event.
 */
void 
XSLTEngineImpl::charactersRaw (
			const XMLCh* const	ch, 
			const unsigned int	start, 
			const unsigned int	length)
    //throws SAXException
{
	flushPending();
	// java: if(m_flistener instanceof FormatterListener)
	FormatterListener* pFL = dynamic_cast<FormatterListener*>(m_flistener);
	if(0 != pFL)
	{
		pFL->charactersRaw(ch, length);
	}
	else
	{
		m_flistener->characters(ch, length);
	}
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CHARACTERS,
				ch, 0, length);
		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::resetDocument()
{
	assert(m_flistener != 0);

	flushPending();
	
	m_flistener->resetDocument();
}



void
XSLTEngineImpl::ignorableWhitespace(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	assert(m_flistener != 0);
	assert(ch != 0);
	flushPending();
	m_flistener->ignorableWhitespace(ch, length);
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_IGNORABLEWHITESPACE,
				ch, 0, length);
		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	assert(m_flistener != 0);
	assert(target != 0);
	assert(data != 0);
	flushPending();
	m_flistener->processingInstruction(target, data);
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_PI,
                                          target, data);
		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::comment(const XMLCh* const	data)
{
	assert(m_flistener != 0);
	assert(data != 0);
	flushPending();
	// future: assert (m_flistener.getType == eFormatterListener)
	FormatterListener* pfl =
		static_cast<FormatterListener*>(m_flistener);
	pfl->comment(data);
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_COMMENT,
                                          data);
		fireGenerateEvent(ge);
	}
}


void
XSLTEngineImpl::entityReference(const XMLCh* const	name)
{
	assert(m_flistener != 0);
	assert(name != 0);
	flushPending();
	// future: assert (m_flistener.getType == eFormatterListener)
	FormatterListener* pfl =
		static_cast<FormatterListener*>(m_flistener);
	pfl->entityReference(name);
	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_ENTITYREF,
                                          name);
		fireGenerateEvent(ge);
	}
}


void XSLTEngineImpl::cdata(
			const XMLCh* const	ch,
			const unsigned int 	start,
			const unsigned int 	length)
{
	assert(m_flistener != 0);
	assert(ch != 0);
	flushPending();
	Stylesheet::QNameVectorType cdataElems = m_stylesheetRoot->getCdataSectionElems();
	if(0 != cdataElems.size() && 0 != m_cdataStack.size())
	{
		// future: assert (m_flistener.getType == eFormatterListener)
		FormatterListener* pfl =
			static_cast<FormatterListener*>(m_flistener);
		pfl->cdata(ch, length);
		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CDATA, ch, start,
					length);
			fireGenerateEvent(ge);
		}
	}
	else
	{
		m_flistener->characters(ch, length);
		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CHARACTERS, ch,
					start, length);
			fireGenerateEvent(ge);
		}
	}
}




void
XSLTEngineImpl::

cloneToResultTree(
			const Stylesheet&	stylesheetTree,
			const DOM_Node&	node, 
			bool					isLiteral,
			bool				/* shouldCloneWithChildren */, 
			bool					overrideStrip,
			bool					shouldCloneAttributes)
{
	bool	stripWhiteSpace = false;
	switch(node.getNodeType())
	{
	case DOM_Node::TEXT_NODE:
		{
			// If stripWhiteSpace is false, then take this as an override and 
			// just preserve the space, otherwise use the XSL whitespace rules.
			if(!overrideStrip)
			{
				stripWhiteSpace = isLiteral ? true : false;
			  // was: stripWhiteSpace = isLiteral ? true : shouldStripSourceNode(node);
			}
			const DOM_Text& 	tx = static_cast<const DOM_Text&>(node);
			DOMString	data;
			// System.out.println("stripWhiteSpace = "+stripWhiteSpace+", "+tx.getData());
			if(stripWhiteSpace == true)
			{
				if(!m_xpathSupport.isIgnorableWhitespace(tx))
				{
					data = getNormalizedText(tx);
					if((0 != length(data)) && (0 == length(trim(data))))
					{
						data = "";
					}
				}
			}
			else 
			{
				const DOM_Node parent = node.getParentNode();
				if(parent==0 || DOM_Node::DOCUMENT_NODE != parent.getNodeType())
				{
					data = getNormalizedText(tx);
				}
			}
			

			if(0 != length(data))
			{
				// TODO: Hack around the issue of comments next to literals.
				// This would be, when a comment is present, the whitespace 
				// after the comment must be added to the literal.	The 
				// parser should do this, but XML4J doesn't seem to.
				// <foo>some lit text
				//	 <!-- comment -->	
				//	 </foo>
				// Loop through next siblings while they are comments, then, 
				// if the node after that is a ignorable text node, append 
				// it to the text node just added.
			  
				if(m_xpathSupport.isIgnorableWhitespace(tx))
				{
					ignorableWhitespace(toCharArray(data), length(data));
				}
				else
				{
					characters(toCharArray(data), 0, length(data));
				}
			}
		}
		break;

	case DOM_Node::ELEMENT_NODE:
		{
			if(shouldCloneAttributes == true)
			{
				copyAttributesToAttList(node,
										m_stylesheetRoot,
										static_cast<const DOM_Element&>(node),
										m_pendingAttributes);

				copyNamespaceAttributes(node,
										false,
										m_pendingAttributes);
			}

			startElement(c_wstr(node.getNodeName()));
		}
		break;

	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_CDATASection& 	theCDATA =
				static_cast<const DOM_CDATASection&>(node);
			const DOMString 	data = theCDATA.getData();
			cdata(toCharArray(data), 0, length(data));
		}
		break;
	
	case DOM_Node::ATTRIBUTE_NODE:
		{
			const DOM_Attr& 	attr =
				static_cast<const DOM_Attr&>(node);
			addResultAttribute(m_pendingAttributes,
							   attr.getName(),
							   attr.getValue());
		}
		break;

	case DOM_Node::COMMENT_NODE:
		{
			const DOM_Comment&	theComment =
				static_cast<const DOM_Comment&>(node);
			const DOMString 	theData = theComment.getData();
			comment(toCharArray(theData));
		}
		break;

	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
		{
			error(DOM_Node(), node, "No clone of a document fragment!");
		}
		break;
	
	case DOM_Node::ENTITY_REFERENCE_NODE:
		{
#if 0
			EntityReference er = (EntityReference)node;
			entityReference(toCharArray(er.getNodeName()));
#else
			const DOMString 	theName = node.getNodeName();
			entityReference(toCharArray(theName));
#endif
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		{
			const DOM_ProcessingInstruction&	pi =
				static_cast<const DOM_ProcessingInstruction&>(node);
			const DOMString 	theTarget = pi.getTarget();
			const DOMString 	theData = pi.getData();
			processingInstruction(toCharArray(theTarget),
								  toCharArray(theData));
		}
		break;

	default:
		{
			// $$$ ToDo: Fix this when DOMString::operator+() is const.
			DOMString	msg("Can not create item in result tree: ");
			msg +=node.getNodeName();
			error(msg);
//			error(DOMString("Can not create item in result tree: ") +
//				  node.getNodeName());
		}
	}
}


// @@ java: DocumentFragment
ResultTreeFragBase*
XSLTEngineImpl::createResultTreeFrag(
			const Stylesheet*			stylesheetTree, 
			ElemTemplateElement&		templateChild, 
			const DOM_Node&			sourceTree, 
			const DOM_Node&			sourceNode,
			const QName&				mode)
{
	DocumentHandler* const	savedFormatterListener = m_flistener;

	std::auto_ptr<ResultTreeFragBase> pfrag(createDocFrag());
		 
	DOMString savedPendingName = m_pendingElementName;
	m_pendingElementName = "";  // DOMString()
	AttributeListImpl savedPendingAttributes = m_pendingAttributes;
	m_pendingAttributes.clear();
	
	m_flistener = new FormatterToDOM(m_resultTreeFactory, 
		DOM_UnimplementedDocumentFragment(pfrag.get()));
	
	templateChild.executeChildren(*this, sourceTree, sourceNode, mode);
	
	// flushPending();
	delete m_flistener;
	m_flistener = savedFormatterListener;
	m_pendingElementName = savedPendingName;
	m_pendingAttributes = savedPendingAttributes;

	return pfrag.release();
}

void XSLTEngineImpl::writeChildren(
	                   FormatterListener* flistener,
	                   Stylesheet& stylesheetTree, 
	                   ElemTemplateElement& templateParent, 
	                   const DOM_Node& sourceTree, 
	                   const DOM_Node& sourceNode, QName mode)
{
    flushPending();

    DocumentHandler* savedFormatterListener = m_flistener;
    DOMString savedPendingName = m_pendingElementName;
    m_pendingElementName = 0;
    AttributeListImpl savedPendingAttributes = m_pendingAttributes;
    m_pendingAttributes.clear();
    m_flistener = flistener;
        
    templateParent.executeChildren(*this, sourceTree, sourceNode, mode);
    
    flushPending();
    m_flistener = savedFormatterListener;
    m_pendingElementName = savedPendingName;
    m_pendingAttributes = savedPendingAttributes;
}


void
XSLTEngineImpl::outputResultTreeFragment(XObject*		obj)
{
	ResultTreeFragBase&		docFrag = obj->rtree();
	const NodeRefListBase&	nl = docFrag.getChildNodesAsNodeRefList();
	const int			nChildren = nl.getLength();
	for(int i = 0; i < nChildren; i++)
	{
		DOM_Node		pos = nl.item(i);
		const DOM_Node&	top = pos;
		while(0 != pos)
		{
			flushPending();
			cloneToResultTree(*m_stylesheetRoot, pos, false, false, false, true );
			DOM_Node	nextNode = pos.getFirstChild();
			while(0 == nextNode)
			{
				if(DOM_Node::ELEMENT_NODE == pos.getNodeType())
				{
					endElement(c_wstr(pos.getNodeName()));
				}
				if(top == pos)
					break;
				nextNode = pos.getNextSibling();
				if(0 == nextNode)
				{
					pos = pos.getParentNode();
					if(top == pos || 0 == pos)
					{
						if (0 != pos)
						{
							if(DOM_Node::ELEMENT_NODE == pos.getNodeType())
							{
								endElement(c_wstr(pos.getNodeName()));
							}
						}

						nextNode = 0;
						break;
					}
				}
			}
			pos = nextNode;
		}
	}
}


/**
 * Tell if a given element name should output it's text as cdata.
 */
bool XSLTEngineImpl::isCDataResultElem(DOMString& elementName)
{
	bool is = false;
	Stylesheet::QNameVectorType cdataElems = m_stylesheetRoot->getCdataSectionElems();
	if(0 != cdataElems.size())
	{
		DOMString elemNS;
		DOMString elemLocalName;
		const int	indexOfNSSep = indexOf(elementName, ':');
		if(indexOfNSSep > 0)
		{
			DOMString prefix = substring(elementName, 0, indexOfNSSep);
			if(equals(prefix, "xml"))
			{
				elemNS = Constants::S_XMLNAMESPACEURI;
			}
			else
			{
				elemNS = getResultNamespaceForPrefix(prefix);
			}	
			if(0 == elemNS.length())
			{
				error(DOMString("Prefix must resolve to a namespace: ") + prefix);
			 // throw new RuntimeException(+prefix);
			}
		}
		elemLocalName = (indexOfNSSep < 0) ? elementName : substring(elementName, indexOfNSSep+1);
		const int n = cdataElems.size();
		for(int i = 0; i < n; i++)
		{
			QName qname = cdataElems.at(i);
			is = qname.equals(QName(elemNS, elemLocalName));
			if(is)
				break;
		}
	}
	return is;
}
	
	/**
	 * Tell if a qualified name equals the current result tree name.
	 */
bool XSLTEngineImpl::qnameEqualsResultElemName(QName& qname, DOMString& elementName)
{
	DOMString elemNS;
	DOMString elemLocalName;
	const int	indexOfNSSep = indexOf(elementName, ':');
	if(indexOfNSSep > 0)
	{
		DOMString prefix = substring(elementName, 0, indexOfNSSep);
		if(equals(prefix, "xml"))
		{
			elemNS = Constants::S_XMLNAMESPACEURI;
		}
		else
		{
			elemNS = getResultNamespaceForPrefix(prefix);
		}	
		if(0 == elemNS.length())
		{
			error(DOMString("Prefix must resolve to a namespace: ") + prefix);
		 // throw new RuntimeException(+prefix);
		}
	}
	elemLocalName = (indexOfNSSep < 0) ? elementName : substring(elementName, indexOfNSSep+1);
	return qname.equals(QName(elemNS, elemLocalName));
}

DOMString
XSLTEngineImpl::getResultNamespaceForPrefix(const DOMString&	prefix) const
{
	// Search vector from first element back
	return QName::getNamespaceForPrefix(m_resultNameSpaces, prefix, false);
}
  


DOMString
XSLTEngineImpl::getResultPrefixForNamespace(const DOMString&	theNamespace) const
{
	// Search vector from first element back
	return QName::getPrefixForNamespace(m_resultNameSpaces, theNamespace, false);
}



DOMString
XSLTEngineImpl::getPrefixForNamespace(
			const DOMString&	theNamespace,
			const DOM_Element&	namespaceContext) const
{
	int 		type;
	DOM_Node	parent = namespaceContext;
	DOMString	prefix;

	while ((0 != parent) && (0 == length(prefix))
		   && (((type = parent.getNodeType()) == DOM_Node::ELEMENT_NODE)
				|| (type == DOM_Node::ENTITY_REFERENCE_NODE))) 
	{
		if (type == DOM_Node::ELEMENT_NODE) 
		{
			DOM_NamedNodeMap	nnm = parent.getAttributes();
			for (int i = 0;  i < nnm.getLength();  i ++) 
			{
				const DOM_Node		attr = nnm.item(i);
				const DOMString 	aname = attr.getNodeName();
				const bool			isPrefix = startsWith(aname, "xmlns:");
				if (equals(aname, "xmlns") || isPrefix) 
				{
					const int			index = indexOf(aname, ':');
					const DOMString 	namespaceOfPrefix = attr.getNodeValue();
					if((0 != length(namespaceOfPrefix)) &&
						equals(namespaceOfPrefix, theNamespace))
					{
						prefix = isPrefix ? substring(aname, index + 1) : DOMString();
					}
				}
			}
		}
		parent = m_xpathSupport.getParentOfNode(parent);
	}
	return prefix;
}



void
XSLTEngineImpl::copyNamespaceAttributes(
			const DOM_Node&			src,
			bool				srcIsStylesheetTree,
			AttributeListImpl&	/* destination */) 
{
	int type;
	DOM_Node	parent = src;
	while (parent != 0
		   && ((type = parent.getNodeType()) == DOM_Node::ELEMENT_NODE
			   || (type == DOM_Node::ENTITY_REFERENCE_NODE))) 
	{
		if (type == DOM_Node::ELEMENT_NODE) 
		{
			// $$$ ToDo: This can become const when getLength()
			// is made const.
			DOM_NamedNodeMap	nnm = parent.getAttributes();
			const int			nAttrs = nnm.getLength();
			for (int i = 0;  i < nAttrs; i++) 
			{
				const DOM_Node		attr = nnm.item(i);
				const DOMString 	aname = attr.getNodeName();
				const bool		isPrefix = startsWith(aname, "xmlns:");
				if (equals(aname, "xmlns") || isPrefix) 
				{
					const DOMString 	prefix = isPrefix ? substring(aname, 6) : DOMString();
					const DOMString 	desturi = getResultNamespaceForPrefix(prefix);
					DOMString			srcURI = attr.getNodeValue();
					const bool			isXSLNS =
						(srcIsStylesheetTree && equalsIgnoreCase(srcURI, m_XSLNameSpaceURL)
					|| (0 != m_stylesheetRoot->lookupExtensionNSHandler(srcURI))
					|| (srcIsStylesheetTree && equalsIgnoreCase(srcURI, s_XSLT4JNameSpaceURL)));

					if(startsWith(srcURI, "quote:"))
					{
						srcURI = substring(srcURI, 6);
					}
					if(!equalsIgnoreCase(srcURI, desturi) && !isXSLNS)
					{
						addResultAttribute(m_pendingAttributes, aname, srcURI);
					}
				}
			}
		}
		parent = parent.getParentNode();
	}
}

XObject* XSLTEngineImpl::evalXPathStr(const DOMString& str,
						const DOM_Node& context,
						const PrefixResolver& resolver)
{
    m_xpathProcessor->initXPath(*m_xpath, str, resolver);
    return m_xpath->execute(context, resolver, m_contextNodeList);
}

	/**
	 * Evaluate an xpath string and return the result.
	 */
XPath* XSLTEngineImpl::createXPath(
		const DOMString&		str, 
		const PrefixResolver&	resolver)
{
	assert(m_xpathFactory);
	XPath* xpath = m_xpathFactory->create();
	m_xpathProcessor->initXPath(*xpath, str, resolver);
	xpath->shrink();
	return xpath;
}

/**
 * Evaluate an xpath string and return the result.
 */
double XSLTEngineImpl::evalMatchPatternStr(const DOMString& str,
						const DOM_Node& context,
						const PrefixResolver& resolver)
{
	// This needs to use a factory method of some sort.
	m_xpathProcessor->initMatchPattern(*m_xpath, str, resolver);
	return m_xpath->getMatchScore(context);
}

/**
 * Evaluate an xpath string and return the result.
 */
XPath* XSLTEngineImpl::createMatchPattern(const DOMString &str, const PrefixResolver& resolver)
{
	XPath* xpath = m_xpathFactory->create();
	m_xpathProcessor->initMatchPattern(*xpath, str, resolver);
	xpath->shrink();
	return xpath;
}


XPath* XSLTEngineImpl::getExpression(
					AttributeList& attrs,
					const DOMString& key,
					const PrefixResolver& resolver)
{
    const XMLCh* val = attrs.getValue(c_wstr(key));
    return (0 != val) ? createXPath(DOMString(val), resolver) : 0;
}



DOMString
XSLTEngineImpl::getAttrVal(
			const DOM_Element& 		el,
			const DOMString&	key,
			const DOM_Node&	/* contextNode */		)
{
	// @@ JMD: context not used
	return getAttrVal( el, key);
}



DOMString
XSLTEngineImpl::getAttrVal(
			const DOM_Element&	el,
			const DOMString&	key)
{
	const DOM_Attr	a = el.getAttributeNode(key);
	return 0 == a ? DOMString() : a.getValue();
}





DOMString
XSLTEngineImpl::evaluateAttrVal(
			const DOM_Node&	contextNode,
			const DOM_Element&	namespaceContext,
			const DOMString&	stringedValue)
{
	DOMString			expressedValue; // return value
	StringTokenizer 	tokenizer(stringedValue, "{}\"\'", true);
	const int			nTokens = tokenizer.countTokens();

	if(nTokens < 2)
	{
		expressedValue = stringedValue; // then do the simple thing
	}
	else
	{
		DOMString	buffer;
		DOMString	t; // base token
		DOMString	lookahead; // next token
		DOMString	error; // if not empty, break from loop

		ElementPrefixResolverProxy	theProxy(namespaceContext, m_xpathSupport);

		while(tokenizer.hasMoreTokens())
		{
			if(length(lookahead) != 0)
			{
				t = lookahead;
				lookahead = "";
			}
			else t = tokenizer.nextToken();

			if(length(t) == 1)
			{
				switch(charAt(t, 0))
				{
					case('\"'):
					case('\''):
					{
						// just keep on going, since we're not in an attribute template
						append(buffer, t);
						break;
					}
					case('{'):
					{
						// Attr template start
						lookahead = tokenizer.nextToken();
						if(equals(lookahead, "{"))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);
							lookahead = "";
							break; // from switch
						}
						/*
						else if(equals(lookahead, "\"") || equals(lookahead, "\'"))
						{
							// Error. Expressions can not begin with quotes.
							error = "Expressions can not begin with quotes.";
							break; // from switch
						}
						*/
						else
						{
							DOMString expression = lookahead; // Probably should make into StringBuffer
							while((0 != length(lookahead)) && (!equals(lookahead, "}")))
							{
								lookahead = tokenizer.nextToken();
								if(length(lookahead) == 1)
								{
									switch(charAt(lookahead, 0))
									{
										case '\'':
										case '\"':
										{
											// String start
											expression += lookahead;
											DOMString	quote = lookahead;
											// Consume stuff 'till next quote
											lookahead = tokenizer.nextToken();
											while(!equals(lookahead, quote))
											{
												expression += lookahead;
												lookahead = tokenizer.nextToken();
											}
											expression += lookahead;
											break;
										}
										case '{':
										{
											// What's another curly doing here?
											error = "Error: Can not have \"{\" within expression.";
											break;
										}
										case '}':
										{
											// Proper close of attribute template.
											// Evaluate the expression.
											XObject* xobj = evalXPathStr(expression, contextNode, theProxy);

											DOMString exprResult = xobj->str();
											append(buffer, exprResult);

											lookahead = ""; // breaks out of inner while loop
										break;
										}
										default:
										{
											// part of the template stuff, just add it.
											expression += lookahead;
										}
									} // end inner switch
								} // end if lookahead length == 1
								else
								{
									// part of the template stuff, just add it.
									expression += lookahead;
								}
							} // end while(!lookahead.equals("}"))

							if(length(error) != 0)
							{
								break; // from inner while loop
							}
						}
						break;
					}
					case('}'):
					{
						lookahead = tokenizer.nextToken();
						if(equals(lookahead, "}"))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);
							lookahead = ""; // swallow
						}
						else
						{
							// Illegal, I think...
							warn("Found \"}\" but no attribute template open!");
							append(buffer, "}");
							// leave the lookahead to be processed by the next round.
						}
						break;
					}
					default:
					{
						// Anything else just add to string.
						append(buffer, t);
					}
				} // end switch t
			} // end if length == 1
			else
			{
				// Anything else just add to string.
				append(buffer, t);
			}

			if(0 != length(error))
			{
				// $$$ ToDo: Fix this when DOMString::operator+() is const.
				DOMString	message("Attr Template, ");

				warn(message + error);
				break;
			}
		} // end while(tokenizer.hasMoreTokens())

		expressedValue = buffer;
	} // end else nTokens > 1

	return expressedValue;
}


void
XSLTEngineImpl::copyAttributeToTarget(
			const DOM_Attr&			attr,
			const DOM_Node&			contextNode,
			const Stylesheet* 		stylesheetTree,
			AttributeListImpl&		attrList, 
			const DOM_Element& 		namespaceContext)
{
	const DOMString 	attrName = trim(attr.getName());
	DOMString			stringedValue = attr.getValue();
	stringedValue = evaluateAttrVal(contextNode,
									namespaceContext,
									stringedValue);

	// evaluateAttrVal might return a null value if the template expression 
	// did not turn up a result, in which case I'm going to not add the 
	// attribute.
	// TODO: Find out about empty attribute template expression handling.
	if(0 != length(stringedValue))
	{
		if((equals(attrName, "xmlns") || startsWith(attrName, "xmlns:"))
		   && startsWith(stringedValue, "quote:"))
		{
			stringedValue = substring(stringedValue, 6);
		}
		addResultAttribute(attrList, attrName, stringedValue);
	}
}

void
XSLTEngineImpl::copyAttributesToAttList(
			const DOM_Node&			contextNode,
			const Stylesheet* 		stylesheetTree,
			const DOM_Element& 		templateChild,
			AttributeListImpl&		attList)
{
	assert(m_stylesheetRoot != 0);
	assert(stylesheetTree != 0);
	DOM_NamedNodeMap	attributes = templateChild.getAttributes();
	const int nAttributes = (0 != attributes) ? attributes.getLength() : 0;
	DOMString	attrSetUseVal;
	for(int i = 0; i < nAttributes; i++)  
	{	
		const DOM_Node		theNode = attributes.item(i);
		const DOM_Attr& 	attr = static_cast<const DOM_Attr&>(theNode);
		// $$$ ToDo: Fix this when DOMString::operator+() is const.
		DOMString			theTemp(m_XSLNameSpaceURL);
		theTemp += ":use";

		if(equalsIgnoreCase(m_parserLiaison.getExpandedAttributeName(attr), theTemp))
//		if(equalsIgnoreCase(m_parserLiaison.getExpandedAttributeName(attr), m_XSLNameSpaceURL + ":use"))
		{
			attrSetUseVal = attr.getValue();
		}
		else
		{
			copyAttributeToTarget(attr,
								  contextNode,
								  stylesheetTree, 
								  attList,
								  templateChild);
		}
	}
}


 
void
XSLTEngineImpl::translateCSSAttrsToStyleAttr(AttributeListImpl&		attList)
{
	if(m_translateCSS == true)
	{
		DOMString				styleAttrValueString;
		std::vector<const XMLCh*>	toBeRemoved;
		int nAttributes = attList.getLength();
		for(int i = 0; i < nAttributes; i++)
		{
			const XMLCh* const	attrName = attList.getName(i);
			if(isCSSAttribute(attrName) == true)
			{
				toBeRemoved.push_back(attrName);
				if(0 == length(styleAttrValueString))
				{
					// $$$ ToDo: Fix this when DOMString::operator+() is const.
					styleAttrValueString = attrName;
					styleAttrValueString += ":";
					styleAttrValueString += attList.getValue(i);
//					styleAttrValueString = attrName+":"+attList.getValue(i);
				}
				else
				{
					// $$$ ToDo: Fix this when DOMString::operator+() is const.
					styleAttrValueString += "; ";
					styleAttrValueString += attrName;
					styleAttrValueString += ":";
					styleAttrValueString += attList.getValue(i);
//					styleAttrValueString += "; "+attrName+":"+attList.getValue(i);
				}
			}
		}
		const int	nAttrsToRemove = toBeRemoved.size();
		for(int j = 0; j < nAttrsToRemove; j++)
		{
			attList.removeAttribute(toBeRemoved[j]);
		}
		if(0 != length(styleAttrValueString))
		{
			addResultAttribute(attList, "style", styleAttrValueString);
		}
	}
}



bool
XSLTEngineImpl::isCSSAttribute(const DOMString&	name) const
{
	if(m_cssKeys.size() == 0)
	{
		// Cast away const to initialize the table.
		const_cast<XSLTEngineImpl*>(this)->initCSS2Table();
	}
	return (m_cssKeys.find(name) != m_cssKeys.end() ? true : false);
}



void
XSLTEngineImpl::initCSS2Table()
{
	m_cssKeys.insert("azimuth");
	m_cssKeys.insert("background");
	m_cssKeys.insert("background-attachment");
	m_cssKeys.insert("background-color");
	m_cssKeys.insert("background-image");
	m_cssKeys.insert("background-position");
	m_cssKeys.insert("background-repeat");
	m_cssKeys.insert("border");
	m_cssKeys.insert("border-bottom");
	m_cssKeys.insert("border-bottom-color");
	m_cssKeys.insert("border-bottom-style");
	m_cssKeys.insert("border-bottom-width");
	m_cssKeys.insert("border-color");
	m_cssKeys.insert("border-left");
	m_cssKeys.insert("border-left-color");
	m_cssKeys.insert("border-left-style");
	m_cssKeys.insert("border-left-width");
	m_cssKeys.insert("border-right");
	m_cssKeys.insert("border-right-color");
	m_cssKeys.insert("border-right-style");
	m_cssKeys.insert("border-right-width");
	m_cssKeys.insert("border-spacing");
	m_cssKeys.insert("border-style");
	m_cssKeys.insert("border-top");
	m_cssKeys.insert("border-top-color");
	m_cssKeys.insert("border-top-style");
	m_cssKeys.insert("border-top-width");
	m_cssKeys.insert("border-width");
	m_cssKeys.insert("bottom");
	m_cssKeys.insert("caption-side");
	m_cssKeys.insert("clear");
	m_cssKeys.insert("clip");
	m_cssKeys.insert("color");
	m_cssKeys.insert("column-span");
	m_cssKeys.insert("content");
	m_cssKeys.insert("cue");
	m_cssKeys.insert("cue-after");
	m_cssKeys.insert("cue-before");
	m_cssKeys.insert("cursor");
	m_cssKeys.insert("direction");
	m_cssKeys.insert("display");
	m_cssKeys.insert("elevation");
	m_cssKeys.insert("float");
	m_cssKeys.insert("font");
	m_cssKeys.insert("font-family");
	m_cssKeys.insert("font-size");
	m_cssKeys.insert("font-size-adjust");
	m_cssKeys.insert("font-style");
	m_cssKeys.insert("font-variant");
	m_cssKeys.insert("font-weight");
	m_cssKeys.insert("height");
	m_cssKeys.insert("left");
	m_cssKeys.insert("letter-spacing");
	m_cssKeys.insert("line-height");
	m_cssKeys.insert("list-style");
	m_cssKeys.insert("list-style-image");
	m_cssKeys.insert("list-style-position");
	m_cssKeys.insert("list-style-type");
	m_cssKeys.insert("margin");
	m_cssKeys.insert("margin-bottom");
	m_cssKeys.insert("margin-left");
	m_cssKeys.insert("margin-right");
	m_cssKeys.insert("margin-top");
	m_cssKeys.insert("marks");
	m_cssKeys.insert("max-height");
	m_cssKeys.insert("max-width");
	m_cssKeys.insert("min-height");
	m_cssKeys.insert("min-width");
	m_cssKeys.insert("orphans");
	m_cssKeys.insert("overflow");
	m_cssKeys.insert("padding");
	m_cssKeys.insert("padding-bottom");
	m_cssKeys.insert("padding-left");
	m_cssKeys.insert("padding-right");
	m_cssKeys.insert("padding-top");
	m_cssKeys.insert("page-break-after");
	m_cssKeys.insert("page-break-before");
	m_cssKeys.insert("pause");
	m_cssKeys.insert("pause-after");
	m_cssKeys.insert("pause-before");
	m_cssKeys.insert("pitch");
	m_cssKeys.insert("pitch-range");
	m_cssKeys.insert("play-during");
	m_cssKeys.insert("position");
	m_cssKeys.insert("quotes");
	m_cssKeys.insert("richness");
	m_cssKeys.insert("right");
	m_cssKeys.insert("row-span");
	m_cssKeys.insert("size");
	m_cssKeys.insert("speak");
	m_cssKeys.insert("speak-date");
	m_cssKeys.insert("speak-header");
	m_cssKeys.insert("speak-numeral");
	m_cssKeys.insert("speak-punctuation");
	m_cssKeys.insert("speak-time");
	m_cssKeys.insert("speech-rate");
	m_cssKeys.insert("stress");
	m_cssKeys.insert("table-layout");
	m_cssKeys.insert("text-align");
	m_cssKeys.insert("text-decoration");
	m_cssKeys.insert("text-indent");
	m_cssKeys.insert("text-shadow");
	m_cssKeys.insert("text-transform");
	m_cssKeys.insert("top");
	m_cssKeys.insert("unicode-bidi");
	m_cssKeys.insert("vertical-align");
	m_cssKeys.insert("visibility");
	m_cssKeys.insert("voice-family");
	m_cssKeys.insert("volume");
	m_cssKeys.insert("white-space");
	m_cssKeys.insert("widows");
	m_cssKeys.insert("width");
	m_cssKeys.insert("word-spacing");
	m_cssKeys.insert("z-index");
}



const NodeRefListBase*
XSLTEngineImpl::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver) const
{
	// Should this call the root or the current stylesheet?
	const NodeRefListBase*	nl = 0;

	if (m_stylesheetRoot != 0)
	{
		nl = m_stylesheetRoot->getNodeSetByKey(doc, name, ref, resolver);
	}

	if(0 == nl)
	{
		error(DOMString("There is no xsl:key declaration for '") + name + DOMString("'!"));
	}

    return nl;
}



DOM_Element
XSLTEngineImpl::getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const
{
	return m_xpathSupport.getElementByID(id, doc);
}


bool
XSLTEngineImpl::shouldStripSourceNode(const DOM_Node&	textNode) const
{
	bool		strip = false; // return value
	int type = textNode.getNodeType();
	if((DOM_Node::TEXT_NODE == type) || (DOM_Node::CDATA_SECTION_NODE == type))
	{
		const DOM_Text& 	theTextNode =
				static_cast<const DOM_Text&>(textNode);
		if(!m_xpathSupport.isIgnorableWhitespace(theTextNode))
		{
			DOMString data = theTextNode.getData();
			if(0 == length(data))
			{
				return true;
			}
			else if(!isWhiteSpace(data))
			{
				return false;
			}
		}
		DOM_Node	parent = m_xpathSupport.getParentOfNode(textNode);
		while(0 != parent)
		{
			if(parent.getNodeType() == DOM_Node::ELEMENT_NODE)
			{
				const DOM_Element&	parentElem =
					static_cast<const DOM_Element&>(parent);
				const DOM_Attr	attr = parentElem.getAttributeNode("xml:space");
				if(0 != attr)
				{
					const DOMString 	xmlSpaceVal = attr.getValue();
					if(equals(xmlSpaceVal, "preserve"))
					{
						strip = false;
					}
					else if(equals(xmlSpaceVal, "default"))
					{
						strip = true;
					}
					else
					{
						error("xml:space in the source XML has an illegal value: "+xmlSpaceVal);
					}
					break;
				}

				double highPreserveScore = XPath::s_MatchScoreNone;
				double highStripScore = XPath::s_MatchScoreNone;
				{
				int nTests = m_stylesheetRoot->m_whitespacePreservingElements.size();
				for(int i = 0; i < nTests; i++)
				{
					XPath* matchPat = m_stylesheetRoot->m_whitespacePreservingElements.at(i);
					double score = matchPat->getMatchScore(parent);
					if(score > highPreserveScore)
						highPreserveScore = score;
				}
				}
				{
				int nTests = m_stylesheetRoot->m_whitespaceStrippingElements.size();
				for(int i = 0; i < nTests; i++)
				{
					XPath* matchPat = m_stylesheetRoot->m_whitespaceStrippingElements.at(i);
					double score = matchPat->getMatchScore(parent);
					if(score > highStripScore)
						highStripScore = score;
				}
				}
				if((highPreserveScore > XPath::s_MatchScoreNone) ||
				(highStripScore > XPath::s_MatchScoreNone))
				{
					if(highPreserveScore > highStripScore)
					{
						strip = false;
					}
					else if(highStripScore > highPreserveScore)
					{
						strip = true;
					}
					else
					{
						warn("Match conflict between xsl:strip-space and xsl:preserve-space");
					}
					break;
				}
			}
			parent = parent.getParentNode();
		}
	}
	return strip;
}



DOMString
XSLTEngineImpl::fixWhiteSpace(
			const DOMString&	string, 
			bool				trimHead, 
			bool				trimTail, 
			bool				doublePunctuationSpaces) 
{
	const XMLCh* const	theStringData = c_wstr(string);
	std::vector<XMLCh>		buf(theStringData,
							theStringData + length(string));
	const int			len = buf.size();
	bool				edit = false;
	int 				s;
	for(s = 0;	s < len;  s++) 
	{
		if(isSpace(buf[s]) == true) 
		{
			break;
		}
	}
	/* replace S to ' '. and ' '+ -> single ' '. */
	int d = s;
	bool	pres = false;
	for ( ;  s < len;  s ++) 
	{
		const XMLCh 	c = buf[s];
		if (isSpace(c) == true) 
		{
			if (!pres) 
			{
				if (' ' != c)  
				{
					edit = true;
				}
				buf[d++] = ' ';
				if(doublePunctuationSpaces == true && (s != 0))
				{
					const XMLCh 	prevChar = buf[s-1];
					if(!((prevChar == '.') || (prevChar == '!') || (prevChar == '?')))
					{
						pres = true;
					}
				}
				else
				{
					pres = true;
				}
			}
			else
			{
				edit = true;
				pres = true;
			}
		}
		else 
		{
			buf[d++] = c;
			pres = false;
		}
	}

	if (trimTail == true && 1 <= d && ' ' == buf[d-1]) 
	{
		edit = true;
		d --;
	}

	std::vector<XMLCh>::const_iterator	start = buf.begin();
	if (trimHead  == true && 0 < d && ' ' == buf[0]) 
	{
		edit = true;
		start++;
	}

	if (edit == false)
	{
		// If we haven't changed the string, just return a copy of the
		// input string.
		return string;
	}
	else
	{
		// OK, we have to calculate the length of the string,
		// taking into account that we may have moved up the
		// start because we're trimming the from of the string.
		const int	theLength = d - (start - buf.begin());
		return DOMString(start, theLength);
	}
}



DOMString
XSLTEngineImpl::getNormalizedText(const DOM_Text&	tx) const
{
	DOMString	theResult;
	if(m_outputCarriageReturns == false && m_outputLinefeeds == false)
	{
		return tx.getData();
	}
	const DOMString 	src = tx.getData();
	const int			nSrcChars = src.length();
	std::vector<XMLCh>		sb;
	XMLCh				prevChar = 0x00;
	for(int i = 0; i < nSrcChars; i++)
	{
		const XMLCh c = charAt(src, i);
		if(0x0A == c)
		{
			if(0x0D != prevChar)
			{
				if(m_outputCarriageReturns == true)
					sb.push_back(0x0D);
				if(m_outputLinefeeds == true)
					sb.push_back(0x0A);
			}
		}
		else if(0x0D == c)
		{
			if(m_outputCarriageReturns == true)
				sb.push_back(0x0D);
			if(m_outputLinefeeds == true)
				sb.push_back(0x0A);
		}
		else
		{
			sb.push_back(c);
		}
		prevChar = c;
	}
	sb.push_back(0);	// Null terminate
	theResult = DOMString(sb.begin());
	return theResult;
}


/**
 * Convenience function to create an XString.
 * @param s A valid string.
 * @return An XString object.
 */
XObject* XSLTEngineImpl::createXString(const DOMString& s)
{
	return m_xobjectFactory->createString(s);
}

/**
 * Convenience function to create an XObject.
 * @param o Any java object.
 * @return An XObject object.
 */
 // @@ JMD: how do we do this ?
XObject* XSLTEngineImpl::createXObject(void* o)
{
	assert(0);
//		return new XObject(o, m_parserLiaison);
	return 0;
}

/**
 * Convenience function to create an XNumber.
 * @param d Any double number.
 * @return An XNumber object.
 */
XObject* XSLTEngineImpl::createXNumber(double d)
{
	return m_xobjectFactory->createNumber(d);
}

/**
 * Convenience function to create an XBoolean.
 * @param b bool value.
 * @return An XBoolean object.
 */
XObject* XSLTEngineImpl::createXBoolean(bool b)
{
	return m_xobjectFactory->createBoolean(b);
}

/**
 * Convenience function to create an XNodeSet.
 * @param nl A NodeList object.
 * @return An XNodeSet object.
 */
XObject* XSLTEngineImpl::createXNodeSet(const NodeRefListBase& nl)
{
	return m_xobjectFactory->createNodeSet(nl);
}

/**
 * Convenience function to create an XRTreeFrag.
 * @return An XRTreeFrag object.
 */
XObject* XSLTEngineImpl::createXResultTreeFrag(const ResultTreeFragBase& r)
{
	return m_xobjectFactory->createResultTreeFrag(r);
}

/**
 * Convenience function to create an XNodeSet from a node.
 * @param n A DOM node.
 * @return An XNodeSet object.
 */
XObject* XSLTEngineImpl::createXNodeSet(const DOM_Node& n)
{
	return m_xobjectFactory->createNodeSet(n);
}

/**
 * Convenience function to create an XNull.
 * @return An XNull object.
 */
XObject* XSLTEngineImpl::createXNull()
{
	return m_xobjectFactory->createNull();
}


/**
 * Given a name, locate a variable in the current context, and return 
 * the Object.
 */
XObject* XSLTEngineImpl::getVariable(const QName& qname) const
{
	XObject* obj = m_variableStacks.getXObjectVariable(qname);
	// @@ JMD: in java the object may not already be an xobject, if so a new one is created
	// from it, this is not implemented in the C++ version, i.e., the variable stack contains
	// ONLY XObjects
	return obj;
}

/**
 * Create a document fragment.  This function may return null.
 */
ResultTreeFragBase* XSLTEngineImpl::createDocFrag() const
{
	if(0 == m_resultTreeFactory)
	{
		m_resultTreeFactory = m_parserLiaison.createDocument();
	}
	ResultTreeFrag* pfrag = new
		ResultTreeFrag(m_resultTreeFactory, m_xpathSupport);

	return pfrag;
}
  


XObject*
XSLTEngineImpl::getXObjectVariable(const DOMString&	name) const
{
	assert(m_stylesheetRoot != 0);

    XObject*	theResult = m_variableStacks.getXObjectVariable(name);

    if(0 == theResult)
    {
		theResult = m_stylesheetRoot->getTopLevelVariable(name);
    }

    return theResult;
}





XString*
XSLTEngineImpl::getTopLevelVariable(const DOMString&	theName) const
{
	TopLevelVariablesMapType::const_iterator	i =
		m_topLevelVariables.find(theName);

	if (i == m_topLevelVariables.end())
	{
		return 0;
	}
	else
	{
		assert((*i).second != 0);
		return (*i).second;
	}
}



ResultTreeFragBase*
XSLTEngineImpl::createResultTreeFrag() const
{
	if(0 == m_resultTreeFactory)
	{
		m_resultTreeFactory = m_parserLiaison.createDocument();
	}
	ResultTreeFrag* pfrag = new
		ResultTreeFrag(m_resultTreeFactory, m_xpathSupport);

	return pfrag;
}


  /**
   * Given a document, get the default stylesheet URI from the 
   * xsl:stylesheet PI.
   */

DOMString
XSLTEngineImpl::getStyleSheetURIfromDoc(const DOM_Node&	sourceTree)
// was:	getStyleSheetURIfromDoc(DOM_Document	sourceTree)
{
	DOMString		stylesheetURI;
	// $$$ ToDo: Make this const when getLength() is const.
	DOM_NodeList	children = sourceTree.getChildNodes();
	const int		nNodes = children.getLength();
	for(int i = 0; i < nNodes; i++)
	{
		const DOM_Node	child = children.item(i);
		if(DOM_Node::PROCESSING_INSTRUCTION_NODE == child.getNodeType())
		{
			 /*DOM_ProcessingInstruction*/ const DOM_Node& pi = child;
			if(equals(pi.getNodeName(), "xml-stylesheet") ||
				equals(pi.getNodeName(), "xml:stylesheet"))
			{
				bool	isOK = true;
				StringTokenizer 	tokenizer(pi.getNodeValue(), " \t=");
				while(tokenizer.hasMoreTokens() == true)
				{
					if(equals(tokenizer.nextToken(), "type") == true)
					{
						DOMString	typeVal = tokenizer.nextToken();
						typeVal = substring(typeVal, 1, typeVal.length() - 1);
						if(equals(typeVal, "text/xsl") == false)
						{
							isOK = false;
						}
					}
				}  
				if(isOK == true)
				{
					StringTokenizer 	tokenizer(pi.getNodeValue(), " \t=");
					while(tokenizer.hasMoreTokens() == true)
					{
						if(tokenizer.nextToken().equals("href"))
						{
							stylesheetURI = tokenizer.nextToken();
							stylesheetURI =
								substring(stylesheetURI, 1, stylesheetURI.length() - 1);
						}
					} 
					break;
				}
			}
		}
	}
	return stylesheetURI;
}



//bool
void
XSLTEngineImpl::setStylesheetParam(
			const DOMString&	theName,
			const DOMString&	expression)
{
	// java:     QName qname = new QName(key, null, m_parserLiaison);
	QName qname(theName, DOM_Element(), m_xpathSupport);
	Arg arg(qname, expression, true);
	m_topLevelParams.push_back(arg);
}


void
XSLTEngineImpl::setStylesheetParam(
			const DOMString&	theName,
			const XObject*	theValue)
{
	// java:     QName qname = new QName(key, null, m_parserLiaison);
	QName qname(theName, DOM_Element(), m_xpathSupport);
	Arg arg(qname, theValue);
	m_topLevelParams.push_back(arg);
}


void
XSLTEngineImpl::resolveTopLevelParams()
{
	m_stylesheetRoot->pushTopLevelVariables(m_topLevelParams);
}



void
XSLTEngineImpl::resetCurrentState(
			const DOM_Node&		/* sourceTree */,
			const DOM_Node&		xmlNode)
{
	if(0 != xmlNode)
	{
		//===============================================
		// This will be used with callbacks from script, 
		// in places like getAttributeCallback.
		m_currentNode = xmlNode;
	}
}



void
XSLTEngineImpl::registerExtensionHandlerByName(
			const DOMString&	/* classid */,
			const DOMString&	/* codetype */)
{
#if 1
	error("XSL4C does not support extensions at this time!");
#else
	try
	{
	  Class classObj = Class.forName(classid);
	  try
	  {
		Object[] args = new Object[1];
		Class[] argTypes = new Class[1];
		args[0] = codetype;
		argTypes[0] = codetype.getClass();
		Method m = classObj.getMethod ("register", argTypes);
		DispatcherFactory factory = (DispatcherFactory)m.invoke (0, args);
		if(0 != factory)
		{
		  registerExtensionHandler(codetype, factory);
		}
	  }
	  catch(NoSuchMethodException& nsme)
	  {
		warn("Failed to get registration method for extension handler: "+classid);
	  }
	  catch(InvocationTargetException& invocationTarget)
	  {
		warn("Invocation Target Error when trying to register extension handler: "+codetype);
	  }
	  catch(IllegalAccessException& illegalAccess)
	  {
		warn("Illegal Access Error when trying to register extension handler: "+codetype);
	  }
	}
	catch(ClassNotFoundException& classNotFound)
	{
	  warn("Class Not Found Error when trying to register extension handler: "+codetype);
	}
#endif
}
  


void
XSLTEngineImpl::registerExtensionHandler(
			const DOMString&	/* mimeType */,
			DispatcherFactory*	/* factory */)
{
#if 1
	error("XSL4C does not support extensions at this time!");
#else
	if(0 != m_diagnosticsPrintWriter)
	{
		diag(DOMString("Registering dispatcher factory for: ") + mimeType);
	}

	m_dispatcherFactories.put(mimeType, factory);
#endif
}



  

DOM_Element
XSLTEngineImpl::findElementByAttribute(
			const DOM_Element& 	elem,
			const DOMString&	targetElementName, 
			const DOMString&	targetAttributeName,
			const DOMString&	targetAttributeValue)
{
	DOM_Element 		theFoundElement;
	const DOMString 	tagName = elem.getTagName();
	if((0 == length(targetElementName)) || tagName.equals(targetElementName))
	{
		// $$$ Todo: Make this const when getLength is const.
		DOM_NamedNodeMap	attributes = elem.getAttributes();

		try
		{
			const int	nAttributes = (0 != attributes) ? attributes.getLength() : 0;
			for(int i = 0; i < nAttributes; i++)  
			{
				DOM_Node			theNode = attributes.item(i);
				const DOM_Attr& 	attr = 
						  static_cast<DOM_Attr&>(theNode);
				const DOMString 	attrName = attr.getName();
				if(attrName.equals(targetAttributeName))
				{
					DOMString	attrVal = attr.getValue();
					if(attrVal.equals(targetAttributeValue))
					{
						theFoundElement = elem;
						break;
					}
				}
			}
		}
		catch(const DOM_DOMException&)
		{
		}
	}
	if(0 == theFoundElement)
	{
		// $$$ Todo: Make this const when getLength is const.
		DOM_NodeList	children = elem.getChildNodes();	
		const int		nNodes = children.getLength();
		for(int i = 0; i < nNodes; i++) 
		{
			const DOM_Node	childNode = children.item(i);
			if (childNode.getNodeType() == DOM_Node::ELEMENT_NODE) 
			{
				const DOM_Element&	child = 
						  static_cast<const DOM_Element&>(childNode);
				const DOMString 	childName = child.getTagName();
				if(0 != length(childName))
				{
					theFoundElement = findElementByAttribute(
													 child,
													 targetElementName, 
													 targetAttributeName,
													 targetAttributeValue);

					if(0 != theFoundElement)
					{
						break;
					}
				}
			}
		}
	}
	return theFoundElement;
}



XMLURL* XSLTEngineImpl::getURLFromString (const DOMString&	urlString)
	// throws MalformedURLException
/*
 *  Create an URL as a file: protocol by constructing fully qualified name
 *  from 'urlstring'
 *  NOTE: caller owns memory
 */
{
	XMLURL* url = new XMLURL();
	try 
	{
		url->setURL(c_wstr(urlString));
	}
	// 'urlString' not a valid url, try to construct a file url
	catch (const MalformedURLException&)
	{
		DOMString fullpath("file:///");
		try 
		{
//			XMLCh* lastPart = XMLPlatformUtils::getBasePath(c_wstr(urlString));
//			fullpath += lastPart;
			fullpath += urlString;
			url->setURL(c_wstr(fullpath));
		}
		catch (MalformedURLException& e2)
		{
			diag("Error! Cannot create url for: " + fullpath);
			throw e2;
		}
	}
	return url;
}

XMLURL* XSLTEngineImpl::getURLFromString(const DOMString&	urlString, const DOMString& base)
{
	if (isEmpty(base))
		return getURLFromString(urlString);

	// We'll only do the really simple case for now:
	// base is a complete file URL and urlString is a forward relative path, i.e. 
	// in the same directory as the urlString or a subdirectory

	DOMString context;

	// just to be robust, we'll accept a forward or back slash
	int i1 = lastIndexOf(base,'/');
	int i2 = lastIndexOf(base,'\\');

	int i = i1>i2 ? i1: i2;

	if (i != -1)
		context = substring(base,0,i+1);

	context += urlString;

	return getURLFromString(context);
}	


void
XSLTEngineImpl::setFormatter(Formatter*	formatter)
{
	flushPending();
	m_formatter = formatter;
	if(0 != formatter)
	{
		m_flistener = formatter->getFormatterListener();
	}
	else
	{
		m_flistener = 0;
	}
}

	/**
	 * Given an element, return an attribute value in 
	 * the form of a string, processing attributes as 
	 * need be.
	 * @param el The element from where to get the attribute.
	 * @param key The name of the attribute.
	 * @param contextNode The context to evaluate the 
	 * attribute value template.
	 * @return Attribute value.
	 */
DOMString
XSLTEngineImpl::getProcessedAttrVal(
			const DOM_Element&	el,
			const DOMString&	key, 
			const DOM_Node&		contextNode)
{
	DOMString	val;
	const DOM_Attr	attr = el.getAttributeNode(key);

	if(0 != attr)
	{
		const DOMString 	stringedValue = attr.getValue();
		if(stringedValue.length() != 0)
		{
			val = evaluateAttrVal(contextNode, el, stringedValue);
		}
	}
	return val;
}

DOMString
XSLTEngineImpl::getNodeData(const DOM_Node&	node)
{
	DOMString	data;
	switch(node.getNodeType())
	{
	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
	case DOM_Node::DOCUMENT_NODE:
	case DOM_Node::ELEMENT_NODE:
		{
			DOM_NodeList	children = node.getChildNodes();
			int nNodes = children.getLength();
			for(int i = 0; i < nNodes; i++)
			{
				DOMString	nodeData = getNodeData(children.item(i));
				if(length(nodeData) > 0)
				{
					data += nodeData;
				}
			}
		}
		break;

	case DOM_Node::TEXT_NODE:
	case DOM_Node::CDATA_SECTION_NODE:
		{
			// Apply the same whitespace rules that you apply to 
			// fixing the whitespace when m_stripWhiteSpace == true.
			// Don't know if this is OK.
			const DOM_Text& 	theTextNode =
					static_cast<const DOM_Text&>(node);
			if(shouldStripSourceNode(node))
			{
				if(!m_xpathSupport.isIgnorableWhitespace(theTextNode))
				{
					// data = fixWhiteSpace(((Text)node).getData(), false, false, true);
					data = getNormalizedText(theTextNode);
				}
			}
			else
			{
				data = theTextNode.getData();
			}
		}
		break;

	case DOM_Node::ATTRIBUTE_NODE:
		data = node.getNodeValue();
		break;

	default:
		// ignore
		break;
	}
	return data;
}


//@@ JMD: NOTE: java implementation of these classes does not pass reference
//to processor

//////////////////////////////////////////////////////////////////////////////
// CLASS XSLTEngineImpl::StackGuard
//////////////////////////////////////////////////////////////////////////////

XSLTEngineImpl::StackGuard::StackGuard(
			XSLTEngineImpl&		processor,
			const DOM_Element&	xslTemplate,
			const DOM_Node&		sourceXML) :
	m_processor(&processor),
	m_xslRule(xslTemplate),
	m_sourceXML(sourceXML),
	m_stack()
{
}



XSLTEngineImpl::StackGuard::~StackGuard()
{
	m_processor = 0;
}



void
XSLTEngineImpl::StackGuard::print(PrintWriter&	pw) const
{
	// for the moment, these diagnostics are really bad...
	const int	theType = m_sourceXML.getNodeType();

	if(theType == DOM_Node::TEXT_NODE)
	{
		const DOM_Text&		tx =
			static_cast<const DOM_Text&>(m_sourceXML);

		pw.println(tx.getData());
	}
	else if(theType == DOM_Node::ELEMENT_NODE)
	{
		const DOM_Element&		elem =
			static_cast<const DOM_Element&>(m_sourceXML);

		pw.println(elem.getNodeName());
	}
}



void
XSLTEngineImpl::StackGuard::checkForInfiniteLoop(const StackGuard&	guard) const
{
	const int	nRules = m_stack.size();

	int loopCount = 0;

	for(int i = (nRules - 1); i >= 0; i--)
	{
		if(m_stack[i] == (guard))
		{
			loopCount++;
		}

		if(loopCount >= 4)
		{
			DOMStringPrintWriter	pw;
			pw.println(DOMString("Infinite loop diagnosed!  Stack trace:"));
			for(int k = 0; k < nRules; k++)
			{
				pw.println(DOMString("Source Elem #") +
								LongToDOMString(k) +
								DOMString(" "));
				const StackGuard&	guardOnStack = m_stack[i];
				guardOnStack.print(pw);
			}
			pw.println(DOMString("Source Elem #") +
							LongToDOMString(k) +
							DOMString(" "));
			guard.print(pw);
			pw.println(DOMString("End of infinite loop diagnosis."));
			m_processor->diag(pw.getString());
			throw XSLTEngineImpl::XSLInfiniteLoopException();
		}
	}
}



void
XSLTEngineImpl::StackGuard::push(
				const DOM_Element&	xslTemplate,
				const DOM_Node&		sourceXML)
{
	const StackGuard	guard(*m_processor, xslTemplate, sourceXML);
	checkForInfiniteLoop(guard);
	m_stack.push_back(guard);
}

void
XSLTEngineImpl::StackGuard::pop()
{
	m_stack.pop_back();
}

//////////////////////////////////////////////////////////////////////////////
// CLASS XSLTEngineImpl::VariableStack
//////////////////////////////////////////////////////////////////////////////


XSLTEngineImpl::VariableStack::VariableStack(XSLTEngineImpl&	theProcessor) :
	m_caller(),
	m_stack(),
	m_processor(theProcessor)
{
	pushContextMarker(DOM_Node(), DOM_Node());	
}



XSLTEngineImpl::VariableStack::~VariableStack()
{
	reset();
}



void
XSLTEngineImpl::VariableStack::reset()
{
	// Delete all entries left on the stack
	std::for_each(m_stack.begin(),
			 m_stack.end(),
			 DeleteFunctor<StackEntry>());
	m_stack.clear();
	pushContextMarker(DOM_Node(), DOM_Node());	
}



void
XSLTEngineImpl::VariableStack::pushElementMarker(const DOM_Node&	elem)
{
	m_stack.push_back(new ElementMarker(elem));
}



void
XSLTEngineImpl::VariableStack::popElementMarker(const DOM_Node&	elem)
{
	if(elementMarkerAlreadyPushed(elem) == true)
	{
		int nElems = m_stack.size();
		bool		fFound = false;
		// Sub 1 extra for the context marker.
		for(int i = (nElems - 1); i >= 0 && fFound == false; i--)
		{
			const StackEntry* const		theEntry = m_stack[i];
			assert(theEntry != 0);
			if(theEntry->getType() == StackEntry::eElementMarker)
			{
				m_stack.pop_back();
				fFound = true;
			}
			else
			{
				m_stack.pop_back();
			}
			delete theEntry;
		}
	}
}



bool
XSLTEngineImpl::VariableStack::elementMarkerAlreadyPushed(const DOM_Node&	elem) const
{
	const int	nElems = m_stack.size();
	// Sub 1 extra for the context marker.
	for(int i = (nElems - 1); i >= 0; i--)
	{
		const StackEntry* const		theEntry = m_stack[i];
		assert(theEntry != 0);
		if(theEntry->getType() == StackEntry::eElementMarker)
		{
			const ElementMarker* const	theElementMarkerEntry =
					static_cast<const ElementMarker*>(theEntry);

			if(&theElementMarkerEntry->getElement() == &elem)
			{
				return true;
			}
		}
	}
	return false;
}


	
void
XSLTEngineImpl::VariableStack::pushContextMarker(
			const DOM_Node&	caller,
			const DOM_Node&	sourceNode)
{
	m_stack.push_back(new ContextMarker(caller, sourceNode));
}



void
XSLTEngineImpl::VariableStack::popCurrentContext()
{
	const int	nElems = m_stack.size();
	bool		fFound = false;
	// Sub 1 extra for the context marker.
	for(int i = (nElems - 1); i >= 0 && fFound == false; i--)
	{
		const StackEntry* const		theEntry = m_stack[i];
		assert(theEntry != 0);
		int type = theEntry->getType();
		assert(type <4 && type >= 0);
		fFound  = (type == StackEntry::eContextMarker);
		m_stack.pop_back();
		delete theEntry;
	}
}


void
XSLTEngineImpl::VariableStack::pushParams(
				const Stylesheet*			stylesheetTree,
				const ElemTemplateElement&	xslCallTemplateElement,
				const DOM_Node&					sourceTree, 
				const DOM_Node&					sourceNode,
				const QName&			mode,
				const DOM_Node&	targetTemplate)
{
	StackEntry* const		theStackEntry = m_stack.back();
	if (theStackEntry->getType() != StackEntry::eContextMarker)
	{
		// @@ $$$ ToDo: Fix this!!!
		// throw InvalidStackContext();
		return;
	}
	VariableStackStackType		tempStack;
	ContextMarker* const	cm =
			dynamic_cast<ContextMarker*>(theStackEntry);
	try
	{
		try
		{
			m_stack.pop_back();
			NodeImpl* child =
				(const_cast<ElemTemplateElement&>(xslCallTemplateElement)).getFirstChild();
			while(0 != child)
			{
				if(Constants::ELEMNAME_WITHPARAM ==
					(dynamic_cast<ElemTemplateElement*>(child))->getXSLToken())
				{
					ElemWithParam* xslParamElement =
						dynamic_cast<ElemWithParam*>(child);
					Arg*	theArg = 0;
					XPath* pxpath = xslParamElement->getSelectPattern();
					if(0 != pxpath)
					{
						XObject* const	theXObject =
							pxpath->execute(sourceNode,
									*xslParamElement,
									m_processor.getContextNodeList());
						theArg = new Arg(*xslParamElement->getQName(), theXObject);
					}
					else
					{
						ResultTreeFragBase* const	theDocFragment =
							m_processor.createResultTreeFrag(stylesheetTree,
									*xslParamElement,
									sourceTree,
									sourceNode,
									mode);
						assert(theDocFragment != 0);
						// Cast to result tree fragment
						ResultTreeFrag* const	theResultTreeFrag =
							static_cast<ResultTreeFrag* const>(theDocFragment);
						XObject* var = m_processor.createXResultTreeFrag(*theResultTreeFrag);
						theArg = new Arg(*xslParamElement->getQName(), var);
					}
					assert(theArg != 0);
					tempStack.push_back(theArg);
				}
				child = child->getNextSibling();
			} // while
		}
		catch(...)
		{
			m_stack.push_back(cm);
			throw;
		}
		m_stack.push_back(cm);
		try
		{
			pushElementMarker(targetTemplate);
			const int	nParams = tempStack.size();
			for(int i = 0; i < nParams; i++)
			{
				m_stack.push_back(tempStack.back());
				tempStack.pop_back();
			}
		}
		catch(...)
		{
			popElementMarker(targetTemplate);
			throw;
		}
	} 
	catch(...)
	{
		// Delete all temp entries not yet transferred.
		// Any transferred ones will have been deleted
		// by popElementMarker();
		std::for_each(tempStack.begin(),
				 tempStack.end(),
				 DeleteFunctor<StackEntry>());

		throw;
	}
}

bool XSLTEngineImpl::VariableStack::hasParamVariable(QName& qname)
{
	bool hasit = false;
	const int	nElems = m_stack.size();
	// Sub 1 extra for the context marker.
	for(int i = (nElems - 1); i >= 0; i--)
	{
		const StackEntry* const		theEntry = m_stack[i];
		assert(theEntry != 0);
		if(theEntry->getType() == StackEntry::eArgument)
		{
			const Arg* const	theArg = dynamic_cast<const Arg*>(theEntry); 
			if((theArg->getName()).equals(qname))
			{
				hasit = true;
				break;
			}
		}
		else if(theEntry->getType() == StackEntry::eContextMarker)
		{
			break;
		}
	}
	return hasit;
}


void
XSLTEngineImpl::VariableStack::pushVariable(
			const QName&			name,
			XObject*					val,
			const DOM_Node&	e)
{
	if(elementMarkerAlreadyPushed(e) == false)
	{
		pushElementMarker(e);
	}
	m_stack.push_back(new Arg(name, val));
}




XObject*
XSLTEngineImpl::VariableStack::findXObject(
			const QName&	name,
			bool				fSearchGlobalSpace) const
{
	XObject*			theXObject = 0;

	const Arg* const	theArg = findArg(name, fSearchGlobalSpace);

	if (theArg != 0)
	{
		if (theArg->getArgType() == Arg::eXObject)
		{
			theXObject = const_cast<XObject*>(theArg->getXObjectPtr());
		}
	}

	return theXObject;
}





const Arg*
XSLTEngineImpl::VariableStack::findArg(
			const QName&	qname,
			bool				fSearchGlobalSpace) const
{
	const Arg*	theResult = 0;

	const int	nElems = m_stack.size();

	// Sub 1 extra for the context marker.
	for(int i = (nElems - 1); i >= 0; i--)
	{
		const StackEntry* const		theEntry =
			m_stack[i];
		assert(theEntry != 0);

		if(theEntry->getType() == StackEntry::eArgument)
		{
			const Arg* const	theArg =
				dynamic_cast<const Arg*>(theEntry);

			if((theArg->getName()).equals(qname))
			{
				theResult = theArg;
				break;
			}
		}
		else if(theEntry->getType() == StackEntry::eContextMarker)
		{
			break;
		}
	}

	if(0 == theResult && true == fSearchGlobalSpace)
	{
		// Look in the global space
		for(int i = 2; i < (nElems - 1); i++)
		{
			const StackEntry* const		theEntry = m_stack[i];
			assert(theEntry != 0);
			if(theEntry->getType() == StackEntry::eArgument)
			{
				const Arg* const	theArg =
					dynamic_cast<const Arg*>(theEntry);
				if((theArg->getName()).equals(qname))
				{
					theResult = theArg;
					break;
				}
			}
			else if(theEntry->getType() == StackEntry::eContextMarker)
			{
				break;
			}
		}
	}

	return theResult;
}



void
XSLTEngineImpl::InstallFunctions()
{
	XPath::installFunction("current", FunctionCurrent());
	XPath::installFunction("format-number", FunctionFormatNumber());
	XPath::installFunction("key", FunctionKey());
	XPath::installFunction("unparsed-entity-uri", FunctionUnparsedEntityURI());
	XPath::installFunction("system-property", FunctionSystemProperty());
	XPath::installFunction("generate-id", FunctionGenerateID());
}



XSLTEngineImpl::AttributeKeysMapType
XSLTEngineImpl::InitializeAttributeKeysTable()
{
	AttributeKeysMapType	theAttributeKeys;

	theAttributeKeys[Constants::ATTRNAME_OUTPUT_METHOD] = Constants::TATTRNAME_OUTPUT_METHOD;
	theAttributeKeys[Constants::ATTRNAME_AMOUNT] = Constants::TATTRNAME_AMOUNT;
	theAttributeKeys[Constants::ATTRNAME_ANCESTOR] = Constants::TATTRNAME_ANCESTOR;
	theAttributeKeys[Constants::ATTRNAME_ARCHIVE] = Constants::TATTRNAME_ARCHIVE;
	theAttributeKeys[Constants::ATTRNAME_ATTRIBUTE] = Constants::TATTRNAME_ATTRIBUTE;
	theAttributeKeys[Constants::ATTRNAME_ATTRIBUTE_SET] = Constants::TATTRNAME_ATTRIBUTE_SET;
	theAttributeKeys[Constants::ATTRNAME_CASEORDER] = Constants::TATTRNAME_CASEORDER;
	theAttributeKeys[Constants::ATTRNAME_CLASS] = Constants::TATTRNAME_CLASS;
	theAttributeKeys[Constants::ATTRNAME_CLASSID] = Constants::TATTRNAME_CLASSID;
	theAttributeKeys[Constants::ATTRNAME_CODEBASE] = Constants::TATTRNAME_CODEBASE;
	theAttributeKeys[Constants::ATTRNAME_CODETYPE] = Constants::TATTRNAME_CODETYPE;
	theAttributeKeys[Constants::ATTRNAME_CONDITION] = Constants::TATTRNAME_CONDITION;
	theAttributeKeys[Constants::ATTRNAME_COPYTYPE] = Constants::TATTRNAME_COPYTYPE;
	theAttributeKeys[Constants::ATTRNAME_COUNT] = Constants::TATTRNAME_COUNT;
	theAttributeKeys[Constants::ATTRNAME_DATATYPE] = Constants::TATTRNAME_DATATYPE;
	theAttributeKeys[Constants::ATTRNAME_DEFAULT] = Constants::TATTRNAME_DEFAULT;
	theAttributeKeys[Constants::ATTRNAME_DEFAULTSPACE] = Constants::TATTRNAME_DEFAULTSPACE;
	theAttributeKeys[Constants::ATTRNAME_DEPTH] = Constants::TATTRNAME_DEPTH;
	theAttributeKeys[Constants::ATTRNAME_DIGITGROUPSEP] = Constants::TATTRNAME_DIGITGROUPSEP;
	theAttributeKeys[Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING] = Constants::TATTRNAME_DISABLE_OUTPUT_ESCAPING;
	theAttributeKeys[Constants::ATTRNAME_ELEMENT] = Constants::TATTRNAME_ELEMENT;
	theAttributeKeys[Constants::ATTRNAME_ELEMENTS] = Constants::TATTRNAME_ELEMENTS;
	theAttributeKeys[Constants::ATTRNAME_EXPR] = Constants::TATTRNAME_EXPR;
	theAttributeKeys[Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES] = Constants::TATTRNAME_EXTENSIONELEMENTPREFIXES;
	theAttributeKeys[Constants::ATTRNAME_FORMAT] = Constants::TATTRNAME_FORMAT;
	theAttributeKeys[Constants::ATTRNAME_FROM] = Constants::TATTRNAME_FROM;
	theAttributeKeys[Constants::ATTRNAME_GROUPINGSEPARATOR] = Constants::TATTRNAME_GROUPINGSEPARATOR;
	theAttributeKeys[Constants::ATTRNAME_GROUPINGSIZE] = Constants::TATTRNAME_GROUPINGSIZE;
	theAttributeKeys[Constants::ATTRNAME_HREF] = Constants::TATTRNAME_HREF;
	theAttributeKeys[Constants::ATTRNAME_ID] = Constants::TATTRNAME_ID;
	theAttributeKeys[Constants::ATTRNAME_IMPORTANCE] = Constants::TATTRNAME_IMPORTANCE;
	theAttributeKeys[Constants::ATTRNAME_INDENTRESULT] = Constants::TATTRNAME_INDENTRESULT;
	theAttributeKeys[Constants::ATTRNAME_LANG] = Constants::TATTRNAME_LANG;
	theAttributeKeys[Constants::ATTRNAME_LETTERVALUE] = Constants::TATTRNAME_LETTERVALUE;
	theAttributeKeys[Constants::ATTRNAME_LEVEL] = Constants::TATTRNAME_LEVEL;
	theAttributeKeys[Constants::ATTRNAME_MATCH] = Constants::TATTRNAME_MATCH;
	theAttributeKeys[Constants::ATTRNAME_METHOD] = Constants::TATTRNAME_METHOD;
	theAttributeKeys[Constants::ATTRNAME_MODE] = Constants::TATTRNAME_MODE;
	theAttributeKeys[Constants::ATTRNAME_NAME] = Constants::TATTRNAME_NAME;
	theAttributeKeys[Constants::ATTRNAME_NAMESPACE] = Constants::TATTRNAME_NAMESPACE;
	theAttributeKeys[Constants::ATTRNAME_NDIGITSPERGROUP] = Constants::TATTRNAME_NDIGITSPERGROUP;
	theAttributeKeys[Constants::ATTRNAME_NS] = Constants::TATTRNAME_NS;
	theAttributeKeys[Constants::ATTRNAME_ONLY] = Constants::TATTRNAME_ONLY;
	theAttributeKeys[Constants::ATTRNAME_ORDER] = Constants::TATTRNAME_ORDER;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS] = Constants::TATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC] = Constants::TATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM] = Constants::TATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_ENCODING] = Constants::TATTRNAME_OUTPUT_ENCODING;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_INDENT] = Constants::TATTRNAME_OUTPUT_INDENT;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_MEDIATYPE] = Constants::TATTRNAME_OUTPUT_MEDIATYPE;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_STANDALONE] = Constants::TATTRNAME_OUTPUT_STANDALONE;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_VERSION] = Constants::TATTRNAME_OUTPUT_VERSION;
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_XMLDECL] = Constants::TATTRNAME_OUTPUT_XMLDECL;
	theAttributeKeys[Constants::ATTRNAME_PRIORITY] = Constants::TATTRNAME_PRIORITY;
	theAttributeKeys[Constants::ATTRNAME_REFID] = Constants::TATTRNAME_REFID;
	theAttributeKeys[Constants::ATTRNAME_RESULTNS] = Constants::TATTRNAME_RESULTNS;
	theAttributeKeys[Constants::ATTRNAME_SELECT] = Constants::TATTRNAME_SELECT;
	theAttributeKeys[Constants::ATTRNAME_SEQUENCESRC] = Constants::TATTRNAME_SEQUENCESRC;
	theAttributeKeys[Constants::ATTRNAME_STYLE] = Constants::TATTRNAME_STYLE;
	theAttributeKeys[Constants::ATTRNAME_TEST] = Constants::TATTRNAME_TEST;
	theAttributeKeys[Constants::ATTRNAME_TOSTRING] = Constants::TATTRNAME_TOSTRING;
	theAttributeKeys[Constants::ATTRNAME_TYPE] = Constants::TATTRNAME_TYPE;
	theAttributeKeys[Constants::ATTRNAME_USE] = Constants::TATTRNAME_USE;
	theAttributeKeys[Constants::ATTRNAME_USEATTRIBUTESETS] = Constants::TATTRNAME_USEATTRIBUTESETS;
	theAttributeKeys[Constants::ATTRNAME_VALUE] = Constants::TATTRNAME_VALUE;

	theAttributeKeys[Constants::ATTRNAME_XMLNSDEF] = Constants::TATTRNAME_XMLNSDEF;
	theAttributeKeys[Constants::ATTRNAME_XMLNS] = Constants::TATTRNAME_XMLNS;
	theAttributeKeys[Constants::ATTRNAME_XMLSPACE] = Constants::TATTRNAME_XMLSPACE;

	return theAttributeKeys;
}



XSLTEngineImpl::AttributeKeysMapType
XSLTEngineImpl::InitializeElementKeysTable()
{
	AttributeKeysMapType	theElementKeys;

	theElementKeys[Constants::ELEMNAME_APPLY_TEMPLATES_STRING] = Constants::ELEMNAME_APPLY_TEMPLATES;
	theElementKeys[Constants::ELEMNAME_WITHPARAM_STRING] = Constants::ELEMNAME_WITHPARAM;
	theElementKeys[Constants::ELEMNAME_CONSTRUCT_STRING] = Constants::ELEMNAME_CONSTRUCT;
	theElementKeys[Constants::ELEMNAME_CONTENTS_STRING] = Constants::ELEMNAME_CONTENTS;
	theElementKeys[Constants::ELEMNAME_COPY_STRING] = Constants::ELEMNAME_COPY;
	theElementKeys[Constants::ELEMNAME_COPY_OF_STRING] = Constants::ELEMNAME_COPY_OF;

	theElementKeys[Constants::ELEMNAME_ATTRIBUTESET_STRING] = Constants::ELEMNAME_DEFINEATTRIBUTESET;

	theElementKeys[Constants::ELEMNAME_USE_STRING] = Constants::ELEMNAME_USE;

	theElementKeys[Constants::ELEMNAME_VARIABLE_STRING] = Constants::ELEMNAME_VARIABLE;
	theElementKeys[Constants::ELEMNAME_PARAMVARIABLE_STRING] = Constants::ELEMNAME_PARAMVARIABLE;

	theElementKeys[Constants::ELEMNAME_DISPLAYIF_STRING] = Constants::ELEMNAME_DISPLAYIF;
	theElementKeys[Constants::ELEMNAME_EMPTY_STRING] = Constants::ELEMNAME_EMPTY;
	theElementKeys[Constants::ELEMNAME_EVAL_STRING] = Constants::ELEMNAME_EVAL;
	theElementKeys[Constants::ELEMNAME_CALLTEMPLATE_STRING] = Constants::ELEMNAME_CALLTEMPLATE;
	theElementKeys[Constants::ELEMNAME_TEMPLATE_STRING] = Constants::ELEMNAME_TEMPLATE;
	theElementKeys[Constants::ELEMNAME_STYLESHEET_STRING] = Constants::ELEMNAME_STYLESHEET;
	theElementKeys[Constants::ELEMNAME_TRANSFORM_STRING] = Constants::ELEMNAME_STYLESHEET;
	theElementKeys[Constants::ELEMNAME_IMPORT_STRING] = Constants::ELEMNAME_IMPORT;
	theElementKeys[Constants::ELEMNAME_INCLUDE_STRING] = Constants::ELEMNAME_INCLUDE;
	theElementKeys[Constants::ELEMNAME_FOREACH_STRING] = Constants::ELEMNAME_FOREACH;
	theElementKeys[Constants::ELEMNAME_VALUEOF_STRING] = Constants::ELEMNAME_VALUEOF;
	theElementKeys[Constants::ELEMNAME_KEY_STRING] = Constants::ELEMNAME_KEY;
	theElementKeys[Constants::ELEMNAME_STRIPSPACE_STRING] = Constants::ELEMNAME_STRIPSPACE;
	theElementKeys[Constants::ELEMNAME_PRESERVESPACE_STRING] = Constants::ELEMNAME_PRESERVESPACE;
	theElementKeys[Constants::ELEMNAME_NUMBER_STRING] = Constants::ELEMNAME_NUMBER;
	theElementKeys[Constants::ELEMNAME_IF_STRING] = Constants::ELEMNAME_IF;
	theElementKeys[Constants::ELEMNAME_CHOOSE_STRING] = Constants::ELEMNAME_CHOOSE;
	theElementKeys[Constants::ELEMNAME_WHEN_STRING] = Constants::ELEMNAME_WHEN;
	theElementKeys[Constants::ELEMNAME_OTHERWISE_STRING] = Constants::ELEMNAME_OTHERWISE;
	theElementKeys[Constants::ELEMNAME_TEXT_STRING] = Constants::ELEMNAME_TEXT;
	theElementKeys[Constants::ELEMNAME_ELEMENT_STRING] = Constants::ELEMNAME_ELEMENT;
	theElementKeys[Constants::ELEMNAME_ATTRIBUTE_STRING] = Constants::ELEMNAME_ATTRIBUTE;
	theElementKeys[Constants::ELEMNAME_SORT_STRING] = Constants::ELEMNAME_SORT;
	theElementKeys[Constants::ELEMNAME_PI_STRING] = Constants::ELEMNAME_PI;
	theElementKeys[Constants::ELEMNAME_COMMENT_STRING] = Constants::ELEMNAME_COMMENT;
   
	theElementKeys[Constants::ELEMNAME_COUNTER_STRING] = Constants::ELEMNAME_COUNTER;
	theElementKeys[Constants::ELEMNAME_COUNTERS_STRING] = Constants::ELEMNAME_COUNTERS;
	theElementKeys[Constants::ELEMNAME_COUNTERINCREMENT_STRING] = Constants::ELEMNAME_COUNTERINCREMENT;
	theElementKeys[Constants::ELEMNAME_COUNTERRESET_STRING] = Constants::ELEMNAME_COUNTERRESET;
	theElementKeys[Constants::ELEMNAME_COUNTERSCOPE_STRING] = Constants::ELEMNAME_COUNTERSCOPE;
	
	theElementKeys[Constants::ELEMNAME_APPLY_IMPORTS_STRING] = Constants::ELEMNAME_APPLY_IMPORTS;
	
	theElementKeys[Constants::ELEMNAME_EXTENSION_STRING] = Constants::ELEMNAME_EXTENSION;

	theElementKeys[Constants::ELEMNAME_MESSAGE_STRING] = Constants::ELEMNAME_MESSAGE;
	theElementKeys[Constants::ELEMNAME_LOCALE_STRING] = Constants::ELEMNAME_LOCALE;
	theElementKeys[Constants::ELEMNAME_FALLBACK_STRING] = Constants::ELEMNAME_FALLBACK;

	theElementKeys[Constants::ELEMNAME_OUTPUT_STRING] = Constants::ELEMNAME_OUTPUT;

	return theElementKeys;
}



XSLTEngineImpl::ElementKeysMapType
XSLTEngineImpl::InitializeXSLT4JElementKeys()
{
	ElementKeysMapType	theElementKeys;

	theElementKeys[Constants::ELEMNAME_CSSSTYLECONVERSION_STRING] = Constants::ELEMNAME_CSSSTYLECONVERSION;

	theElementKeys[Constants::ELEMNAME_COMPONENT_STRING] = Constants::ELEMNAME_COMPONENT;
	theElementKeys[Constants::ELEMNAME_SCRIPT_STRING] = Constants::ELEMNAME_SCRIPT;

	return theElementKeys;
}



XSLTEngineImpl::StaticInitializer::StaticInitializer()
{
	XSLTEngineImpl::InstallFunctions();
}



XSLTEngineImpl::StaticInitializer::~StaticInitializer()
{
}



//////////////////////////////////////////////////////////////////////////////
