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



// Class header file.
#include "XSLTEngineImpl.hpp"


#include <memory>



#include <XalanDOM/XalanDOMException.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanComment.hpp>
#include <XalanDOM/XalanCDATASection.hpp>
#include <XalanDOM/XalanNodeList.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanProcessingInstruction.hpp>
#include <XalanDOM/XalanText.hpp>



#include <sax/DocumentHandler.hpp>
#include <sax/Locator.hpp>
#include <sax/SAXException.hpp>
#include <util/PlatformUtils.hpp>
#include <framework/URLInputSource.hpp>



#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/PrintWriter.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XMLSupport/Formatter.hpp>
#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/ResultTreeFrag.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathSupport.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XResultTreeFrag.hpp>



#include "Arg.hpp"
#include "Constants.hpp"
#include "ContextMarker.hpp"
#include "ElemWithParam.hpp"
#include "ElementMarker.hpp"
#include "FunctionCurrent.hpp"
#include "FunctionDocument.hpp"
#include "FunctionElementAvailable.hpp"
#include "FunctionFunctionAvailable.hpp"
#include "FunctionFormatNumber.hpp"
#include "FunctionGenerateID.hpp"
#include "FunctionKey.hpp"
#include "FunctionSystemProperty.hpp"
#include "FunctionUnparsedEntityURI.hpp"
#include "GenerateEvent.hpp"
#include "NodeSorter.hpp"
#include "ProblemListener.hpp"
#include "ProblemListenerDefault.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetHandler.hpp"
#include "StylesheetRoot.hpp"
#include "TraceListener.hpp"
#include "XSLTInputSource.hpp"
#include "XSLTProcessorException.hpp"



const double			XSLTEngineImpl::s_XSLTVerSupported(1.0);
const XalanDOMString	XSLTEngineImpl::s_XSLNameSpaceURL(XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform"));
const XalanDOMString	XSLTEngineImpl::s_XSLT4JNameSpaceURL(XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xslt"));

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
const bool								XSLTEngineImpl::s_resolveContentsEarly = true;

XSLTEngineImpl::AttributeKeysMapType	XSLTEngineImpl::s_attributeKeys;

XSLTEngineImpl::ElementKeysMapType		XSLTEngineImpl::s_elementKeys;

XSLTEngineImpl::ElementKeysMapType		XSLTEngineImpl::s_XSLT4JElementKeys;



//==========================================================
// SECTION: Constructors
//==========================================================

XSLTEngineImpl::XSLTEngineImpl(
			XMLParserLiaison&	parserLiaison,
			XPathSupport&		xpathSupport,
			XPathEnvSupport&	xpathEnvSupport,
			XObjectFactory&		xobjectFactory,
			XPathFactory&		xpathFactory) :
	XSLTProcessor(),
	DocumentHandler(),
	m_rootDoc(),
	m_outputCarriageReturns(false),
	m_outputLinefeeds(false),
	m_formatter(0),
	m_resultTreeFactory(0),
	m_resultNameSpacePrefix(),
	m_resultNameSpaceURL(),
	m_stylesheets(),
	m_currentNode(),
	m_pendingElementName(),
	m_pendingAttributes(),
	m_resultNameSpaces(),
	m_emptyNamespace(),
	m_xpathFactory(xpathFactory),
	m_xobjectFactory(xobjectFactory),
	m_xpathProcessor(new XPathProcessorImpl),
	m_cdataStack(),
	m_stylesheetLocatorStack(),
	m_variableStacks(*this),
	m_problemListener(new ProblemListenerDefault()),
	m_stylesheetRoot(0),
	m_stylesheetExecutionContext(0),
	m_XSLDirectiveLookup(),
	m_quietConflictWarnings(false),
	m_traceTemplateChildren(false),
	m_traceTemplates(false),
	m_traceSelects(false),
	m_diagnosticsPrintWriter(0),
	m_durationsTable(),
	m_traceListeners(),
	m_outputFileName(),
	m_uniqueNSValue(0),
	m_useATVsInSelects(false),
	m_stripWhiteSpace(false),
	m_topLevelParams(),
	m_parserLiaison(parserLiaison),
	m_xpathSupport(xpathSupport),
	m_xpathEnvSupport(xpathEnvSupport),
	m_flistener(0),
	m_contextNodeList(&xpathSupport),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_needToCheckForInfiniteLoops(false),
	m_stackGuard(*this),
	m_attrSetStack()
{
}



void
XSLTEngineImpl::Initialize()
{
	InstallFunctions();

	InitializeAttributeKeysTable();
	InitializeElementKeysTable();
	InitializeXSLT4JElementKeys();
}



/**
 * Reset the state.  This needs to be called after a process() call 
 * is invoked, if the processor is to be used again.
 */
void
XSLTEngineImpl::reset()
{
	m_rootDoc = 0;

	m_durationsTable.clear();
	m_stylesheetLocatorStack.clear();
	clear(m_pendingElementName);
	m_pendingAttributes.clear();
	m_cdataStack.clear();
	m_resultTreeFactory = 0;
	m_contextNodeList.clear();
	m_currentNode = 0;
	m_needToCheckForInfiniteLoops = false;
	m_variableStacks.reset();

	m_stackGuard.clear();
	m_xpathSupport.reset();
	m_xpathEnvSupport.reset();
	m_xpathFactory.reset();
	m_xobjectFactory.reset();

	m_stylesheetExecutionContext = 0;
}



XSLTEngineImpl::~XSLTEngineImpl()
{
	delete m_problemListener;

	reset();
}



//==========================================================
// SECTION: Main API Functions
//==========================================================



XalanDocument*
XSLTEngineImpl::getRootDoc() const
{
	return m_rootDoc;
}



void
XSLTEngineImpl::setRootDoc(XalanDocument*	doc)
{
	m_rootDoc = doc;
}



AttributeListImpl& 
XSLTEngineImpl::getPendingAttributes()
{
	return m_pendingAttributes;
}



const XalanDOMString
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
XSLTEngineImpl::setPendingElementName(const XalanDOMString&	elementName)
{
	m_pendingElementName = elementName;
}



void
XSLTEngineImpl::process(
			XSLTInputSource&				inputSource, 
	        XSLTInputSource*				stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext)
{
	try
	{
		XalanDOMString xslIdentifier(((0 == stylesheetSource) || 
										(0 == stylesheetSource->getSystemId())) 
										 ? XalanDOMString(XALAN_STATIC_UCODE_STRING("Input XSL")) : stylesheetSource->getSystemId());
		bool totalTimeID = true;
		pushTime(&totalTimeID);

		XalanNode*	sourceTree = 0;

		sourceTree = getSourceTreeFromInput(inputSource);

		if(0 != stylesheetSource)
		{
			m_stylesheetRoot = processStylesheet(*stylesheetSource, constructionContext);
		}
		else if(0 != sourceTree)
		{
			XalanDOMString			stylesheetURI = 0;
			XalanNode*				child = sourceTree->getFirstChild();

#if !defined(XALAN_NO_NAMESPACES)
			using std::vector;
#endif

			vector<XalanDOMString>	hrefs;

			// $$$ ToDo: is this first one style valid?
			const XalanDOMString	stylesheetNodeName1(XALAN_STATIC_UCODE_STRING("xml-stylesheet"));
			const XalanDOMString	stylesheetNodeName2(XALAN_STATIC_UCODE_STRING("xml:stylesheet"));

			// $$$ ToDo: This code is much like that in getStyleSheetURIFromDoc().
			// Why is it repeated???
			// $$$ ToDo: Move these embedded strings from inside these loops
			// out here...
			// $$$ ToDo: These loops are a mess of repeated use of the
			// same data values.
			while(child != 0)
			{
				if(XalanNode::PROCESSING_INSTRUCTION_NODE == child->getNodeType())
				{
					const XalanDOMString	nodeName(child->getNodeName());

					if(equals(nodeName, stylesheetNodeName1) ||
					   equals(nodeName, stylesheetNodeName2))
					{
						bool isOK = true;

						StringTokenizer 	tokenizer(child->getNodeValue(), XALAN_STATIC_UCODE_STRING(" \t="), true);

						while(tokenizer.hasMoreTokens())
						{
							if(equals(tokenizer.nextToken(), XALAN_STATIC_UCODE_STRING("type")))
							{
								XalanDOMString	typeVal = tokenizer.nextToken();

								typeVal = substring(typeVal, 1, length(typeVal) - 1);

								if(!equals(typeVal, XALAN_STATIC_UCODE_STRING("text/xsl")))
								{
									isOK = false;
								}
							}
						}	
						
						if(isOK)
						{
							StringTokenizer 	tokenizer(child->getNodeValue(), XALAN_STATIC_UCODE_STRING(" \t="), true);
							while(tokenizer.hasMoreTokens())
							{
								if(equals(tokenizer.nextToken(), XALAN_STATIC_UCODE_STRING("href")))
								{
									stylesheetURI = tokenizer.nextToken();
									stylesheetURI = substring(stylesheetURI, 1, stylesheetURI.length()-1);
									hrefs.push_back(stylesheetURI);
								}
							}
							// break;
						}
					}
				}

				child = child->getNextSibling();
			}

			bool isRoot = true;
			Stylesheet* prevStylesheet = 0;
			while(!hrefs.empty())
			{
				const XMLCh* const		pxch = inputSource.getSystemId();
				const XalanDOMString	sysid(pxch);
				const XalanDOMString&	ref =  hrefs.back();

				Stylesheet* stylesheet =
					getStylesheetFromPIURL(ref, *sourceTree, sysid, isRoot, constructionContext);

				if(false == isRoot)
				{
					prevStylesheet->getImports().push_back(stylesheet);
				}

				prevStylesheet = stylesheet;
				isRoot = false;
				hrefs.pop_back();
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
			m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);

			if(0 != m_diagnosticsPrintWriter)
			{
				displayDuration(XALAN_STATIC_UCODE_STRING("Total time"), &totalTimeID);
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



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
			const XalanDOMString&				xsldocURLString,
			StylesheetConstructionContext&	constructionContext)
{
	try
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

		auto_ptr<XMLURL> url(getURLFromString(xsldocURLString));
		assert(url.get() != 0);

		XSLTInputSource		input(url->getURLText(), 0);

		return processStylesheet(input, constructionContext);
	}
	catch(SAXException& se)
	{
		message("processStylesheet not successful!");

		throw se;
	}
	return 0;
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
  			XSLTInputSource&				stylesheetSource,
			StylesheetConstructionContext&	constructionContext)
{
	const XalanDOMString	ds(XALAN_STATIC_UCODE_STRING("Input XSL"));
	const XalanDOMString	xslIdentifier(0 == stylesheetSource.getSystemId() ? 
									 ds : stylesheetSource.getSystemId());

	// In case we have a fragment identifier, go ahead and 
	// try to parse the XML here.
	try
	{
		m_stylesheetRoot = constructionContext.create(stylesheetSource);

		addTraceListenersToStylesheet();

		StylesheetHandler	stylesheetProcessor(*this, *m_stylesheetRoot, constructionContext);

		if(0 != stylesheetSource.getNode())
		{
			FormatterTreeWalker tw(stylesheetProcessor);

			tw.traverse(stylesheetSource.getNode());
		}
		else
		{
			diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") + xslIdentifier + XALAN_STATIC_UCODE_STRING(" =========="));
			pushTime(&xslIdentifier);
			m_parserLiaison.parseXMLStream(stylesheetSource,
										   stylesheetProcessor);
			if(0 != m_diagnosticsPrintWriter)
				displayDuration(XALAN_STATIC_UCODE_STRING("Parse of ") + xslIdentifier, &xslIdentifier);
		}
	}
	catch(const XSLException&)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}

	catch(const SAXException&)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}


	catch(const XMLException&)
	{
		message("Error parsing " + xslIdentifier);

		throw;
	}

	return m_stylesheetRoot;
}



//==========================================================
// SECTION: XML Parsing Functions
//==========================================================

XalanNode*
XSLTEngineImpl::getSourceTreeFromInput(XSLTInputSource&		inputSource)
{
	XalanNode*		sourceTree = 0;

	XalanDOMString	xmlIdentifier = 0 != inputSource.getSystemId() ?
											inputSource.getSystemId() :
											XALAN_STATIC_UCODE_STRING("Input XML");

	if(0 != inputSource.getNode())
	{
		sourceTree = inputSource.getNode();
	}
	else
	{
		// In case we have a fragment identifier, go ahead and 
		// try to parse the XML here.
		try
		{
			diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") +
					xmlIdentifier +
					XALAN_STATIC_UCODE_STRING(" =========="));

			pushTime(&xmlIdentifier);

			XalanDocument* const	theDocument =
						m_parserLiaison.parseXMLStream(inputSource,
													   xmlIdentifier);
			assert(theDocument != 0);

			if(0 != m_diagnosticsPrintWriter)
				displayDuration(XALAN_STATIC_UCODE_STRING("Parse of ") +
									xmlIdentifier,
								&xmlIdentifier);

			m_xpathEnvSupport.setSourceDocument(xmlIdentifier, theDocument);

			sourceTree = theDocument;
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



XalanDocument*
XSLTEngineImpl::parseXML(
			const XMLURL&		url,
			DocumentHandler*	docHandler,
			XalanDocument*		docToRegister)
{
	// java: url.toExternalForm();
	const XalanDOMString&	urlString = url.getURLText();

	XalanDocument*			doc =
			m_xpathEnvSupport.getSourceDocument(urlString);

	if(doc == 0)
	{
		 // java: url.toString()
		XSLTInputSource		inputSource(url.getURLText());

		if(0 != docHandler)
		{
			m_parserLiaison.parseXMLStream(inputSource, *docHandler);

			doc = docToRegister;
		}
		else
		{
			doc = m_parserLiaison.parseXMLStream(inputSource);
		}

		m_xpathEnvSupport.setSourceDocument(urlString, doc);
	}

	return doc;
}



Stylesheet*
XSLTEngineImpl::getStylesheetFromPIURL(
			const XalanDOMString&			xslURLString,
			XalanNode&						fragBase,
			const XalanDOMString&			xmlBaseIdent,
			bool							isRoot,
			StylesheetConstructionContext&	constructionContext)
{
	Stylesheet*			stylesheet = 0;

	XalanDOMString			stringHolder;
	const XalanDOMString	localXSLURLString = clone(trim(xslURLString));

	const unsigned int	fragIndex = indexOf(localXSLURLString, '#');

	XalanDocument*		stylesheetDoc = 0;

	if(fragIndex == 0)
	{
		diag("Locating stylesheet from fragment identifier...");

		const XalanDOMString	fragID = substring(localXSLURLString, 1);

		XalanElement*			nsNode = 0;

		if (fragBase.getNodeType() == XalanNode::DOCUMENT_NODE)
		{
			const XalanDocument&	doc =
				static_cast<const XalanDocument&>(fragBase);

			nsNode = doc.getDocumentElement(); 
		}
		else if	(fragBase.getNodeType() == XalanNode::ELEMENT_NODE)
		{
			nsNode = static_cast<XalanElement*>(&fragBase);
		}
		else
		{
			XalanNode* const	node = fragBase.getParentNode();

			if	(node->getNodeType() == XalanNode::ELEMENT_NODE) 
			{
				nsNode = static_cast<XalanElement*>(node);
			}
			else
			{
				error("Could not identify fragment: " + fragID);
			}
		}

		// Try a bunch of really ugly stuff to find the fragment.
		// What's the right way to do this?
		XalanDOMString	ds(XALAN_STATIC_UCODE_STRING("id("));

		ds += fragID;
		ds += XALAN_STATIC_UCODE_STRING(")");

		assert(nsNode != 0);

		ElementPrefixResolverProxy		theProxy(nsNode, m_xpathEnvSupport, m_xpathSupport);

		XPathExecutionContextDefault	theExecutionContext(m_xpathEnvSupport,
															m_xpathSupport,
															m_xobjectFactory,
															&fragBase,
															NodeRefList(),
															&theProxy);

		const XObject*	xobj = evalXPathStr(ds, theExecutionContext);

		const NodeRefListBase* nl = &xobj->nodeset();

		if(nl->getLength() == 0)
		{
			NodeRefList		theEmptyList;

			ds = XALAN_STATIC_UCODE_STRING("//*[@id='");
			ds += fragID;
			ds += XALAN_STATIC_UCODE_STRING("']");

			theExecutionContext.setContextNodeList(theEmptyList);

			xobj = evalXPathStr(ds, theExecutionContext);

			nl = &xobj->nodeset();

			if(nl->getLength() == 0)
			{
				ds = XALAN_STATIC_UCODE_STRING("//*[@name='");
				ds += fragID;
				ds += XALAN_STATIC_UCODE_STRING("']");

				theExecutionContext.setContextNodeList(theEmptyList);

				xobj = evalXPathStr(ds, theExecutionContext);

				nl = &xobj->nodeset();

				if(nl->getLength() == 0)
				{
					// Well, hell, maybe it's an XPath...
					theExecutionContext.setContextNodeList(theEmptyList);

					xobj = evalXPathStr(fragID, theExecutionContext);

					nl = &xobj->nodeset();
				}
			}
		}

		if(nl->getLength() == 0)
		{
			error("Could not find fragment: " + fragID);
		}

		XalanNode* const	frag = nl->item(0);

		if(XalanNode::ELEMENT_NODE == frag->getNodeType())
		{
			pushTime(frag);

			if(isRoot)
			{
				m_stylesheetRoot = constructionContext.create(stringHolder);
				stylesheet = m_stylesheetRoot;
			}
			else
			{
				stylesheet = new Stylesheet(*m_stylesheetRoot, stringHolder, constructionContext);
			}

			addTraceListenersToStylesheet();

			StylesheetHandler stylesheetProcessor(*this, *stylesheet, constructionContext);

			FormatterTreeWalker tw(stylesheetProcessor);

			tw.traverse(frag);

			displayDuration(XalanDOMString(XALAN_STATIC_UCODE_STRING("Setup of ")) +
								localXSLURLString,
								&frag);
		}
		else
		{
			stylesheetDoc = 0;
			error("Node pointed to by fragment identifier was not an element: " + fragID);
		}
	}
	else
	{ 
		// hmmm.. for now I'll rely on the XML parser to handle 
		// fragment URLs.
		diag(XalanDOMString(XALAN_STATIC_UCODE_STRING("========= Parsing and preparing ")) +
				localXSLURLString +
				XALAN_STATIC_UCODE_STRING(" =========="));
		pushTime(&localXSLURLString);

		if(isRoot)
		{
			m_stylesheetRoot = constructionContext.create(localXSLURLString);
			stylesheet = m_stylesheetRoot;
		}
		else
		{
			stylesheet = new Stylesheet(*m_stylesheetRoot, localXSLURLString, constructionContext);
		}

		addTraceListenersToStylesheet();

		StylesheetHandler stylesheetProcessor(*this, *stylesheet, constructionContext);

#if !defined(XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

		auto_ptr<XMLURL>	xslURL(getURLFromString(localXSLURLString, xmlBaseIdent));

		XSLTInputSource		inputSource(xslURL->getURLText());

		m_parserLiaison.parseXMLStream(inputSource, stylesheetProcessor);

		displayDuration("Parsing and init of " + localXSLURLString, &localXSLURLString);
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



int
XSLTEngineImpl::getXSLToken(const XalanNode&	node) const
{
	int 	tok = -2;

	if(XalanNode::ELEMENT_NODE != node.getNodeType()) return tok;
	const XalanDOMString 	ns =
			m_xpathSupport.getNamespaceOfNode(node);

	if(equals(ns, s_XSLNameSpaceURL))
	{
		const XalanDOMString 	localName =
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
		const XalanDOMString localName = m_xpathSupport.getLocalNameOfNode(node);
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
XSLTEngineImpl::isXSLTagOfType(const XalanNode&	node,
							int 		tagType) const
{
	return getXSLToken(node) == tagType ? true : false;
}



void
XSLTEngineImpl::outputToResultTree(
			const XObject&		value)
{
	 // java:
    // Make the return object into an XObject because it 
    // will be easier below.  One of the reasons to do this 
    // is to keep all the conversion functionality in the 
    // XObject classes.
	 // JMD: Has to be an XObject

	const int	type = value.getType();

	XalanDOMString s;

	switch(type)
	{
	case XObject::eTypeBoolean:
	case XObject::eTypeNumber:
	case XObject::eTypeString:
		s = value.str();
		characters(toCharArray(s), 0, length(s));
		break;				

	case XObject::eTypeNodeSet:
		{
			const NodeRefListBase&	nl = value.nodeset();
			const unsigned int		nChildren = nl.getLength();

			for(unsigned int i = 0; i < nChildren; i++)
			{
				XalanNode*			pos = nl.item(i);
				XalanNode* const	top = pos;

				while(0 != pos)
				{
					flushPending();

					cloneToResultTree(*pos, false, false, true);

					XalanNode*	nextNode = pos->getFirstChild();

					while(0 == nextNode)
					{
						if(XalanNode::ELEMENT_NODE == pos->getNodeType())
						{
							endElement(c_wstr(pos->getNodeName()));
						}

						if(top == pos)
							break;

						nextNode = pos->getNextSibling();

						if(0 == nextNode)
						{
							pos = pos->getParentNode();

							if(top == pos)
							{
								if(XalanNode::ELEMENT_NODE == pos->getNodeType())
								{
									endElement(c_wstr(pos->getNodeName()));
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



bool
XSLTEngineImpl::functionAvailable(
			XalanDOMString&	theNamespace, 
			XalanDOMString&	extensionName) const
{
	return m_xpathEnvSupport.functionAvailable(theNamespace, extensionName);
}



/**
 * Handle an extension function.
 */
XObject*
XSLTEngineImpl::extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			extensionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) const
{
	return m_xpathEnvSupport.extFunction(
			executionContext,
			theNamespace,
			extensionName,
			context,
			argVec);
}



void
XSLTEngineImpl::handleFunctionsInstruction(XalanElement&	/* extensionElem */)
{
	error("Calling external functions is not supported in the C++ version of Xalan!!!");
}



StylesheetRoot*
XSLTEngineImpl::getStylesheetRoot() const
{
	return m_stylesheetRoot;
}



void
XSLTEngineImpl::setStylesheetRoot(StylesheetRoot*	theStylesheet)
{
	m_stylesheetRoot = theStylesheet;
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
			TraceListener* tl = m_traceListeners[i];
			m_stylesheetRoot->addTraceListener(tl);
		}
	}
	catch(...)
	// catch(TooManyListenersException tmle)
	{
		throw SAXException("addTraceListenersToStylesheet - TooManyListenersException");
	}
} 


void XSLTEngineImpl::removeTraceListener(TraceListener* tl)
{
	int nListeners = m_traceListeners.size();
	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* tli = m_traceListeners[i];
		if (tli == tl) m_traceListeners.erase(m_traceListeners.begin()+i);
	}
	if(0 != m_stylesheetRoot)
		m_stylesheetRoot->removeTraceListener(tl);
}


void XSLTEngineImpl::fireGenerateEvent(const GenerateEvent& te)
{
	const int	nListeners = m_traceListeners.size();

	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* const	tl = m_traceListeners[i];

		tl->generated(te);
	}
}


void
XSLTEngineImpl::message(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode) const
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


void
XSLTEngineImpl::problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const XalanNode*					styleNode,
			const XalanNode*					sourceNode) const
{
	if (m_problemListener == 0) return;

	const Locator* const	locator = getLocatorFromStack();

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
XSLTEngineImpl::warn(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode) const
{
	problem(msg, ProblemListener::eWARNING, styleNode, sourceNode);
}


void
XSLTEngineImpl::error(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode) const
{
	problem(msg, ProblemListener::eERROR, styleNode, sourceNode);
}



void
XSLTEngineImpl::pushTime(const void*	key) const
{
	if(0 != key)
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::make_pair;
#endif

		m_durationsTable.insert(make_pair(key, clock()));
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
XSLTEngineImpl::displayDuration(const XalanDOMString&	info,
							  const void*		key) const
{
	if(0 != key)
	{
		const clock_t	theDuration = popDuration(key);
		double millis = 1000.0*theDuration/CLOCKS_PER_SEC;

		if(0 != m_diagnosticsPrintWriter)
		{
			XalanDOMString	msg(info);

			msg += XALAN_STATIC_UCODE_STRING(" took ");
			msg += DoubleToDOMString(millis);
			msg += XALAN_STATIC_UCODE_STRING(" milliseconds");

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
XSLTEngineImpl::diag(const XalanDOMString& 	s) const
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



void
XSLTEngineImpl::setTraceSelect(bool	b)
{
	m_traceSelects = b;
}



bool XSLTEngineImpl::isTraceSelect() const
{
	return	m_traceSelects;
}



void
XSLTEngineImpl::setTraceTemplateChildren(bool	b)
{
	m_traceTemplateChildren = b;
}


void
XSLTEngineImpl::setQuietConflictWarnings(bool	b)
{
	m_quietConflictWarnings = b;
}



void
XSLTEngineImpl::traceSelect(
			const XalanElement& 	theTemplate,
			const NodeRefListBase&	nl) const
{
	XalanDOMString	msg = theTemplate.getNodeName() + XalanDOMString(XALAN_STATIC_UCODE_STRING(": "));

	XalanAttr*		attr = theTemplate.getAttributeNode(Constants::ATTRNAME_SELECT);

	if(0 != attr)
	{
		msg += attr->getValue();
		msg += XALAN_STATIC_UCODE_STRING(", ");
		msg += LongToDOMString(nl.getLength());
		msg += XALAN_STATIC_UCODE_STRING(" selected");
	}
	else
	{
		msg += XALAN_STATIC_UCODE_STRING("*|text(), (default select), ");
		msg += LongToDOMString(nl.getLength());
		msg += XALAN_STATIC_UCODE_STRING(" selected");
	}

	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MODE);

	if(0 != attr)
	{
		msg += XalanDOMString(XALAN_STATIC_UCODE_STRING(", mode = ")) + attr->getValue();
	}

	// $$$ ToDo: We do all of this work, and don't output the message.
	//System.out.println(msg);
}



void
XSLTEngineImpl::traceTemplate(const XalanElement& 	theTemplate) const
{
	XalanDOMString	msg;

	XalanAttr*	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MATCH);

	if(0 != attr)
	{
		msg = XalanDOMString(XALAN_STATIC_UCODE_STRING("Calling template for: ")) + attr->getValue();
	}
	else
	{
		attr = theTemplate.getAttributeNode(Constants::ATTRNAME_NAME);

		if(0 != attr) 
		{
			msg = XalanDOMString(XALAN_STATIC_UCODE_STRING("Calling named template, name = ")) + attr->getValue();
		}
		else
		{
			const int	xslToken = getXSLToken(theTemplate);

			if(Constants::ELEMNAME_FOREACH == xslToken)
			{
				attr = theTemplate.getAttributeNode(Constants::ATTRNAME_SELECT);

				if(0 != attr)
				{
					msg = XalanDOMString(XALAN_STATIC_UCODE_STRING("Processing for-each, select = ")) + attr->getValue();
				}
				else
				{
					error("for-each must have either a match or name attribute");
					clear(msg);
				}
			}
			else
			{
				error("templates must have either a match or name attribute");

				clear(msg);
			}
		}
	}

	attr = theTemplate.getAttributeNode(Constants::ATTRNAME_MODE);

	if(0 != attr)
	{
		msg += XalanDOMString(XALAN_STATIC_UCODE_STRING(", mode = ")) + attr->getValue();
	}

	// $$$ ToDo: We do all of this work, and don't output the message.
	//System.out.println(msg);
}


void
XSLTEngineImpl::diagnoseTemplateChildren(
			const XalanNode&	templateChild,
			const XalanNode&	sourceNode) const
{
	if(m_traceTemplateChildren)
	{
		const XalanDOMString 	templateChildTagName = templateChild.getNodeName();
		const XalanDOMString 	xmlElemName = sourceNode.getNodeName();
		diag(XalanDOMString(XALAN_STATIC_UCODE_STRING("source node: "))
			 + xmlElemName
			 + XalanDOMString(XALAN_STATIC_UCODE_STRING(", template-node: ")) 
			 + templateChildTagName);
	}
}

  

void
XSLTEngineImpl::setDocumentLocator(const Locator* const		/* locator */)
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

void XSLTEngineImpl::addResultNamespaceDecl(const XalanDOMString& prefix, 
	                                   const XalanDOMString& namespaceVal)
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
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
{
	assert(length(value) > 0);

	const bool	isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);
	if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix == true) 
	{
		const XalanDOMString		p = isPrefix == true ? substring(aname, 6) : XalanDOMString();
		addResultNamespaceDecl(p, value);
	}
	attList.removeAttribute(c_wstr(aname));
	attList.addAttribute(c_wstr(aname), c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")), c_wstr(value));
}



void
XSLTEngineImpl::flushPending()
{
	if(0 != length(m_pendingElementName))
	{
		assert(m_flistener != 0);
		m_cdataStack.push_back(isCDataResultElem(m_pendingElementName)? true : false);
		m_flistener->startElement(c_wstr(m_pendingElementName), m_pendingAttributes);
		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_STARTELEMENT,
					m_pendingElementName, &m_pendingAttributes);
			fireGenerateEvent(ge);
		}
		m_pendingAttributes.clear();
		clear(m_pendingElementName);
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

	m_flistener->endElement(name);

	if(m_traceListeners.size() > 0)
	{
		GenerateEvent ge(this, GenerateEvent::EVENTTYPE_ENDELEMENT, name, 0);
		fireGenerateEvent(ge);
	}

	m_resultNameSpaces.pop_back();

	const Stylesheet::QNameVectorType&	cdataElems =
		m_stylesheetRoot->getCdataSectionElems();

	if(0 != cdataElems.size())
	{
		m_cdataStack.pop_back();
	}
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

	const Stylesheet::QNameVectorType&	cdataElems =
		m_stylesheetRoot->getCdataSectionElems();

	if(0 != cdataElems.size() && 0 != m_cdataStack.size())
	{
		m_flistener->cdata(ch + start, length);

		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CDATA, ch, start, length);
			fireGenerateEvent(ge);
		}
	}
	else
	{
		m_flistener->characters(ch + start, length);

		if(m_traceListeners.size() > 0)
		{
			GenerateEvent ge(this, GenerateEvent::EVENTTYPE_CHARACTERS, ch,
					start, length);
			fireGenerateEvent(ge);
		}
	}
}



void 
XSLTEngineImpl::charactersRaw (
			const XMLCh* const	ch,
			const unsigned int	/* start */,
			const unsigned int	length)
{
	flushPending();

	FormatterListener* const	pFL =
		dynamic_cast<FormatterListener*>(m_flistener);

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
XSLTEngineImpl::cloneToResultTree(
			XalanNode&			node, 
			bool				isLiteral,
			bool				overrideStrip,
			bool				shouldCloneAttributes)
{
	bool	stripWhiteSpace = false;

	switch(node.getNodeType())
	{
	case XalanNode::TEXT_NODE:
		{
			// If stripWhiteSpace is false, then take this as an override and 
			// just preserve the space, otherwise use the XSL whitespace rules.
			if(!overrideStrip)
			{
				stripWhiteSpace = isLiteral ? true : false;
			  // was: stripWhiteSpace = isLiteral ? true : shouldStripSourceNode(node);
			}

			XalanText& 	tx = static_cast<XalanText&>(node);

			XalanDOMString		data;

			if(stripWhiteSpace == true)
			{
				if(!m_xpathSupport.isIgnorableWhitespace(tx))
				{
					data = getNormalizedText(tx);
					if((0 != length(data)) && (0 == length(trim(data))))
					{
						clear(data);
					}
				}
			}
			else 
			{
				XalanNode*	parent = node.getParentNode();

				if(parent == 0 || XalanNode::DOCUMENT_NODE != parent->getNodeType())
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

	case XalanNode::ELEMENT_NODE:
		{
			if(shouldCloneAttributes == true)
			{
				copyAttributesToAttList(&node,
										m_stylesheetRoot,
										static_cast<const XalanElement&>(node),
										m_pendingAttributes);

				copyNamespaceAttributes(node,
										false);
			}

			startElement(c_wstr(node.getNodeName()));
		}
		break;

	case XalanNode::CDATA_SECTION_NODE:
		{
			const XalanCDATASection& 	theCDATA =
				static_cast<const XalanCDATASection&>(node);

			const XalanDOMString 	data = theCDATA.getData();

			cdata(toCharArray(data), 0, length(data));
		}
		break;
	
	case XalanNode::ATTRIBUTE_NODE:
		{
			const XalanAttr& 	attr =
				static_cast<const XalanAttr&>(node);

			addResultAttribute(m_pendingAttributes,
							   attr.getName(),
							   attr.getValue());
		}
		break;

	case XalanNode::COMMENT_NODE:
		{
			const XalanComment&		theComment =
				static_cast<const XalanComment&>(node);

			const XalanDOMString 	theData = theComment.getData();

			comment(toCharArray(theData));
		}
		break;

	case XalanNode::DOCUMENT_FRAGMENT_NODE:
		{
			error("No clone of a document fragment!");
		}
		break;
	
	case XalanNode::ENTITY_REFERENCE_NODE:
		{
			const XalanDOMString 	theName = node.getNodeName();
			entityReference(toCharArray(theName));
		}
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		{
			const XalanProcessingInstruction&	pi =
				static_cast<const XalanProcessingInstruction&>(node);

			const XalanDOMString 	theTarget = pi.getTarget();
			const XalanDOMString 	theData = pi.getData();

			processingInstruction(toCharArray(theTarget),
								  toCharArray(theData));
		}
		break;

	default:
		{
			// $$$ ToDo: Fix this when XalanDOMString::operator+() is const.
			XalanDOMString	msg("Can not create item in result tree: ");
			msg +=node.getNodeName();
			error(msg);
//			error(XalanDOMString("Can not create item in result tree: ") +
//				  node.getNodeName());
		}
	}
}



class StatePushPop
{
public:

	StatePushPop(
		FormatterListener*&		theCurrentListener,
		FormatterListener*		theNewListener,
		DOMString&				thePendingElementName,
		AttributeListImpl&		thePendingAttributes) :
			m_listener(theCurrentListener),
			m_savedListener(theCurrentListener),
			m_pendingElementName(thePendingElementName),
			m_savedPendingElementName(thePendingElementName),
			m_pendingAttributes(thePendingAttributes),
			m_savedPendingAttributes(thePendingAttributes)
	{
		theCurrentListener = theNewListener;

		clear(m_pendingElementName);

		m_pendingAttributes.clear();
	}

	~StatePushPop()
	{
		m_listener = m_savedListener;
		m_pendingElementName = m_savedPendingElementName;
		m_pendingAttributes = m_savedPendingAttributes;
	}

private:

	FormatterListener*&			m_listener;
	FormatterListener* const	m_savedListener;

	DOMString&					m_pendingElementName;
	const DOMString				m_savedPendingElementName;

	AttributeListImpl&			m_pendingAttributes;
	const AttributeListImpl		m_savedPendingAttributes;
};




// @@ java: DocumentFragment
ResultTreeFragBase*
XSLTEngineImpl::createResultTreeFrag(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		templateChild, 
			XalanNode*						sourceTree, 
			XalanNode*						sourceNode,
			const QName&					mode)
{
#if !defined(XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

	auto_ptr<ResultTreeFragBase> pfrag(createDocFrag());

	FormatterToDOM	tempFormatter(m_resultTreeFactory, 
								  pfrag.get());

	StatePushPop	theStateSaver(
						m_flistener,
						&tempFormatter,
						m_pendingElementName,
						m_pendingAttributes);

	templateChild.executeChildren(executionContext, sourceTree, sourceNode, mode);

	return pfrag.release();
}



// $$$ ToDo: This is not called anywhere, can it be removed?
void
XSLTEngineImpl::writeChildren(
			FormatterListener*				flistener,
			StylesheetExecutionContext&		executionContext,
	        const ElemTemplateElement&		templateParent,
	        XalanNode&						sourceTree,
	        XalanNode&						sourceNode,
			const QName&					mode)
{
    flushPending();

	StatePushPop	theStateSaver(
						m_flistener,
						flistener,
						m_pendingElementName,
						m_pendingAttributes);

    templateParent.executeChildren(executionContext, &sourceTree, &sourceNode, mode);
    
    flushPending();
}



void
XSLTEngineImpl::outputResultTreeFragment(const XObject&		theTree)
{
	const ResultTreeFragBase&	docFrag = theTree.rtree();

	// $$$ ToDo: We should optimize this so that we don't have
	// a node list.
	const XalanNodeList*		nl = docFrag.getChildNodes();
	assert(nl != 0);

	const unsigned int			nChildren = nl->getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		XalanNode*			pos = nl->item(i);
		XalanNode* const	top = pos;

		while(0 != pos)
		{
			flushPending();

			cloneToResultTree(*pos, false, false, true);

			XalanNode*	nextNode = pos->getFirstChild();

			while(0 == nextNode)
			{
				if(XalanNode::ELEMENT_NODE == pos->getNodeType())
				{
					endElement(c_wstr(pos->getNodeName()));
				}

				if(top == pos)
					break;

				nextNode = pos->getNextSibling();

				if(0 == nextNode)
				{
					pos = pos->getParentNode();

					if(top == pos || 0 == pos)
					{
						if (0 != pos)
						{
							if(XalanNode::ELEMENT_NODE == pos->getNodeType())
							{
								endElement(c_wstr(pos->getNodeName()));
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



bool
XSLTEngineImpl::isCDataResultElem(const XalanDOMString&		elementName)
{
	typedef Stylesheet::QNameVectorType		QNameVectorType;

	bool is = false;
	const QNameVectorType&				cdataElems = m_stylesheetRoot->getCdataSectionElems();
	const QNameVectorType::size_type	theSize = cdataElems.size();

	if(0 != theSize)
	{
		XalanDOMString		elemNS;
		XalanDOMString		elemLocalName;

		const unsigned int	indexOfNSSep = indexOf(elementName, ':');

		if(indexOfNSSep == length(elementName))
		{
			elemLocalName = elementName;
		}
		else
		{
			const XalanDOMString	prefix = substring(elementName, 0, indexOfNSSep);

			if(equals(prefix, DOMServices::s_XMLString))
			{
				elemNS = DOMServices::s_XMLNamespaceURI;
			}
			else
			{
				elemNS = getResultNamespaceForPrefix(prefix);
			}

			if(0 == length(elemNS))
			{
				error(XalanDOMString("Prefix must resolve to a namespace: ") + prefix);
			 // throw new RuntimeException(+prefix);
			}

			elemLocalName = substring(elementName, indexOfNSSep + 1);
		}

		for(Stylesheet::QNameVectorType::size_type i = 0; i < theSize && is == false; i++)
		{
			const QName& qname = cdataElems[i];

			is = qname.equals(QName(elemNS, elemLocalName));
		}
	}

	return is;
}
	


bool
XSLTEngineImpl::qnameEqualsResultElemName(
			const QName&			qname,
			const XalanDOMString&	elementName)
{
	XalanDOMString		elemNS;
	XalanDOMString		elemLocalName;

	const unsigned int	indexOfNSSep = indexOf(elementName, ':');

	if(indexOfNSSep < length(elementName))
	{
		const XalanDOMString	prefix = substring(elementName, 0, indexOfNSSep);

		if(equals(prefix, DOMServices::s_XMLString))
		{
			elemNS = DOMServices::s_XMLNamespaceURI;
		}
		else
		{
			elemNS = getResultNamespaceForPrefix(prefix);
		}

		if(0 == elemNS.length())
		{
			error(XalanDOMString("Prefix must resolve to a namespace: ") + prefix);
		}

		elemLocalName =  substring(elementName, indexOfNSSep+1);
	}
	else
	{
		elemLocalName = elementName;
	}

	return qname.equals(QName(elemNS, elemLocalName));
}



XalanDOMString
XSLTEngineImpl::getResultNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	// Search vector from first element back
	return QName::getNamespaceForPrefix(m_resultNameSpaces, prefix, false);
}
  


XalanDOMString
XSLTEngineImpl::getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const
{
	// Search vector from first element back
	return QName::getPrefixForNamespace(m_resultNameSpaces, theNamespace, false);
}



XalanDOMString
XSLTEngineImpl::getPrefixForNamespace(
			const XalanDOMString&	theNamespace,
			const XalanElement&	namespaceContext) const
{
	int 				type;
	const XalanNode*	parent = &namespaceContext;
	XalanDOMString		prefix;

	while (0 != parent && 0 == length(prefix)
		   && ((type = parent->getNodeType()) == XalanNode::ELEMENT_NODE
				|| type == XalanNode::ENTITY_REFERENCE_NODE))
	{
		if (type == XalanNode::ELEMENT_NODE) 
		{
			const XalanNamedNodeMap* const	nnm =
				parent->getAttributes();
			assert(nnm != 0);

			const unsigned int	theLength = nnm->getLength();

			for (unsigned int i = 0; i < theLength;  i ++) 
			{
				const XalanNode* const	attr = nnm->item(i);
				assert(attr != 0);

				const XalanDOMString 	aname = attr->getNodeName();

				const bool				isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

				if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
				{
					const unsigned int		index = indexOf(aname, ':');
					assert(index < length(aname));

					const XalanDOMString 	namespaceOfPrefix = attr->getNodeValue();

					if((0 != length(namespaceOfPrefix)) &&
						equals(namespaceOfPrefix, theNamespace))
					{
						prefix = isPrefix ? substring(aname, index + 1) : XalanDOMString();
					}
				}
			}
		}

		parent = m_xpathSupport.getParentOfNode(*parent);
	}

	return prefix;
}



void
XSLTEngineImpl::copyNamespaceAttributes(
			const XalanNode&	src,
			bool				/* srcIsStylesheetTree */) 
{
	int type;

	const XalanNode*	parent = &src;

	while (parent != 0
		   && ((type = parent->getNodeType()) == XalanNode::ELEMENT_NODE
			   || type == XalanNode::ENTITY_REFERENCE_NODE)) 
	{
		if (type == XalanNode::ELEMENT_NODE) 
		{
			const XalanNamedNodeMap* const	nnm =
				parent->getAttributes();
			assert(nnm != 0);

			const unsigned int	nAttrs = nnm->getLength();

			for (unsigned int i = 0;  i < nAttrs; i++) 
			{
				const XalanNode* const	attr = nnm->item(i);

				const XalanDOMString 	aname = attr->getNodeName();

				const bool				isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

				if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
				{
					const XalanDOMString 	prefix = isPrefix ? substring(aname, 6) : XalanDOMString();
					const XalanDOMString 	desturi = getResultNamespaceForPrefix(prefix);
					XalanDOMString			srcURI = attr->getNodeValue();
					/*
					@@ JMD: Not used anymore in java ...
					const bool			isXSLNS =
						srcIsStylesheetTree && equalsIgnoreCase(srcURI, s_XSLNameSpaceURL)
						|| 0 != m_stylesheetRoot->lookupExtensionNSHandler(srcURI)
						|| srcIsStylesheetTree && equalsIgnoreCase(srcURI, s_XSLT4JNameSpaceURL);

					if(startsWith(srcURI, XALAN_STATIC_UCODE_STRING("quote:")))
					{
						srcURI = substring(srcURI, 6);
					}

					if(!equalsIgnoreCase(srcURI, desturi) && !isXSLNS)
					*/
					if(!equalsIgnoreCase(srcURI, desturi))
					{
						addResultAttribute(m_pendingAttributes, aname, srcURI);
					}
				}
			}
		}

		parent = parent->getParentNode();
	}
}



XObject*
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XPathExecutionContext&	executionContext)
{
	FactoryObjectAutoPointer<XPath>		theXPath(&m_xpathFactory,
												 m_xpathFactory.create());

    m_xpathProcessor->initXPath(*theXPath.get(),
								str,
								*executionContext.getPrefixResolver(),
								m_xobjectFactory,
								m_xpathEnvSupport);

    return theXPath->execute(executionContext.getCurrentNode(),
							 *executionContext.getPrefixResolver(),
							 executionContext);
}



XObject*
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext)
{
	FactoryObjectAutoPointer<XPath>		theXPath(&m_xpathFactory,
												 m_xpathFactory.create());

    m_xpathProcessor->initXPath(*theXPath.get(),
								str,
								prefixResolver,
								m_xobjectFactory,
								m_xpathEnvSupport);

    return theXPath->execute(contextNode, prefixResolver, executionContext);
}



XObject*
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		prefixResolver,
			XPathExecutionContext&	executionContext)
{
	ElementPrefixResolverProxy	theProxy(&prefixResolver,
										 m_xpathEnvSupport,
										 m_xpathSupport);

	return evalXPathStr(str, contextNode, theProxy, executionContext);
}



// $$$ ToDo:  This really should not be here...
XPath*
XSLTEngineImpl::createProcessingXPath(
		const XalanDOMString&		str,
		XPathExecutionContext&	executionContext,
		const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initXPath(*xpath,
								str,
								resolver,
								executionContext.getXObjectFactory(),
								m_xpathEnvSupport);

	return xpath;
}



/**
 * Evaluate an xpath string and return the result.
 */
XPath*
XSLTEngineImpl::createXPath(
		const XalanDOMString&	str, 
		const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initXPath(*xpath,
								str,
								resolver,
								m_xobjectFactory,
								m_xpathEnvSupport);

	return xpath;
}

/**
 * Evaluate an xpath string and return the result.
 */
double
XSLTEngineImpl::evalMatchPatternStr(
			const XalanDOMString&	str,
			XalanNode*				context,
			XPathExecutionContext&	executionContext)
{
	FactoryObjectAutoPointer<XPath>		theXPath(&m_xpathFactory,
												 m_xpathFactory.create());

	const PrefixResolver* const		theResolver =
					executionContext.getPrefixResolver();
	assert(theResolver != 0);

	// This needs to use a factory method of some sort.
	m_xpathProcessor->initMatchPattern(*theXPath.get(),
									   str,
									   *theResolver,
									   executionContext.getXObjectFactory(),
									   m_xpathEnvSupport);

	return theXPath->getMatchScore(context, *theResolver, executionContext);
}



/**
 * Create and initialize an xpath and return it.
 */
XPath*
XSLTEngineImpl::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initMatchPattern(*xpath, str, resolver, m_xobjectFactory, m_xpathEnvSupport);

	return xpath;
}



XPath* XSLTEngineImpl::getExpression(
					const AttributeList&	attrs,
					const XalanDOMString&	key,
					const PrefixResolver&	resolver)
{
    const XMLCh* const	val = attrs.getValue(c_wstr(key));

    return 0 != val ? createXPath(XalanDOMString(val), resolver) : 0;
}



XalanDOMString
XSLTEngineImpl::getAttrVal(
			const XalanElement& 	el,
			const XalanDOMString&	key,
			const XalanNode&		/* contextNode */		)
{
	return getAttrVal(el, key);
}



XalanDOMString
XSLTEngineImpl::getAttrVal(
			const XalanElement&		el,
			const XalanDOMString&	key)
{
	const XalanAttr* const	a = el.getAttributeNode(key);

	return 0 == a ? XalanDOMString() : a->getValue();
}




XalanDOMString
XSLTEngineImpl::evaluateAttrVal(
			XalanNode*				contextNode,
			const XalanElement&		namespaceContext,
			const XalanDOMString&	stringedValue,
			XPathExecutionContext&	executionContext)
{
	XalanDOMString		expressedValue;

	StringTokenizer 	tokenizer(stringedValue, XALAN_STATIC_UCODE_STRING("{}\"\'"), true);

	const unsigned int	nTokens = tokenizer.countTokens();

	if(nTokens < 2)
	{
		expressedValue = stringedValue; // then do the simple thing
	}
	else
	{
		XalanDOMString	buffer;
		XalanDOMString	t; // base token
		XalanDOMString	lookahead; // next token
		XalanDOMString	error; // if not empty, break from loop

		ElementPrefixResolverProxy	theProxy(&namespaceContext, m_xpathEnvSupport, m_xpathSupport);

		while(tokenizer.hasMoreTokens())
		{
			if(length(lookahead) != 0)
			{
				t = lookahead;
				clear(lookahead);
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
						if(equals(lookahead, XALAN_STATIC_UCODE_STRING("{")))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);
							clear(lookahead);
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
							XalanDOMString expression = lookahead; // Probably should make into StringBuffer

							while(0 != length(lookahead) && !equals(lookahead, XALAN_STATIC_UCODE_STRING("}")))
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
											XalanDOMString	quote = lookahead;
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
											const XObject* const	xobj =
												evalXPathStr(expression, contextNode, theProxy, executionContext);

											const XalanDOMString			exprResult(xobj->str());

											append(buffer, exprResult);

											clear(lookahead); // breaks out of inner while loop
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
							} // (0 != length(lookahead) && !equals(lookahead, "}"))

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
						if(equals(lookahead, XALAN_STATIC_UCODE_STRING("}")))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);
							clear(lookahead); // swallow
						}
						else
						{
							// Illegal, I think...
							warn("Found \"}\" but no attribute template open!");
							append(buffer, XALAN_STATIC_UCODE_STRING("}"));
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
				// $$$ ToDo: Fix this when XalanDOMString::operator+() is const.
				XalanDOMString	message("Attr Template, ");

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
			const XalanAttr&		attr,
			XalanNode*				/* contextNode */,
			const Stylesheet* 		/* stylesheetTree */,
			AttributeListImpl&		attrList, 
			const XalanElement& 	/* namespaceContext */)
{
	const XalanDOMString 	attrName = trim(attr.getName());

	XalanDOMString			stringedValue = attr.getValue();
//	stringedValue = evaluateAttrVal(contextNode,
//									namespaceContext,
//									stringedValue);

	// evaluateAttrVal might return a null value if the template expression 
	// did not turn up a result, in which case I'm going to not add the 
	// attribute.
	// TODO: Find out about empty attribute template expression handling.
	if(0 != length(stringedValue))
	{
		if((equals(attrName, DOMServices::s_XMLNamespace) || startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator))
		   && startsWith(stringedValue, XALAN_STATIC_UCODE_STRING("quote:")))
		{
			stringedValue = substring(stringedValue, 6);
		}

		addResultAttribute(attrList, attrName, stringedValue);
	}
}



void
XSLTEngineImpl::copyAttributesToAttList(
			XalanNode*				contextNode,
			const Stylesheet* 		stylesheetTree,
			const XalanElement& 	templateChild,
			AttributeListImpl&		attList)
{
	assert(m_stylesheetRoot != 0);
	assert(stylesheetTree != 0);
	const XalanNamedNodeMap* const	attributes =
		templateChild.getAttributes();

	const unsigned int	nAttributes = (0 != attributes) ? attributes->getLength() : 0;

	XalanDOMString	attrSetUseVal;

	for(unsigned int	i = 0; i < nAttributes; i++)  
	{	
		const XalanAttr* const 	attr =
			static_cast<const XalanAttr*>(attributes->item(i));
		assert(attr != 0);

		const XalanDOMString	theTemp(s_XSLNameSpaceURL + ":use");

		if(equalsIgnoreCase(m_parserLiaison.getExpandedAttributeName(*attr), theTemp))
		{
			attrSetUseVal = attr->getValue();
		}
		else
		{
			copyAttributeToTarget(*attr,
								  contextNode,
								  stylesheetTree, 
								  attList,
								  templateChild);
		}
	}
}


 
XalanElement*
XSLTEngineImpl::getElementByID(
			const XalanDOMString&	id,
			const XalanDocument&	doc) const
{
	return m_xpathSupport.getElementByID(id, doc);
}



bool
XSLTEngineImpl::shouldStripSourceNode(const XalanNode&	textNode) const
{
	bool		strip = false; // return value
	int type = textNode.getNodeType();
	if((XalanNode::TEXT_NODE == type) || (XalanNode::CDATA_SECTION_NODE == type))
	{
		const XalanText& 	theTextNode =
				static_cast<const XalanText&>(textNode);

		if(!m_xpathSupport.isIgnorableWhitespace(theTextNode))
		{
			XalanDOMString data = theTextNode.getData();
			if(0 == length(data))
			{
				return true;
			}
			else if(!isWhiteSpace(data))
			{
				return false;
			}
		}

		XalanNode*	parent = m_xpathSupport.getParentOfNode(textNode);

		while(0 != parent)
		{
			if(parent->getNodeType() == XalanNode::ELEMENT_NODE)
			{
				const XalanElement*	const	parentElem =
					static_cast<const XalanElement*>(parent);

				const XalanAttr* const		attr =
					parentElem->getAttributeNode(XALAN_STATIC_UCODE_STRING("xml:space"));

				if(0 != attr)
				{
					const XalanDOMString 	xmlSpaceVal = attr->getValue();

					if(equals(xmlSpaceVal, XALAN_STATIC_UCODE_STRING("preserve")))
					{
						strip = false;
					}
					else if(equals(xmlSpaceVal, XALAN_STATIC_UCODE_STRING("default")))
					{
						strip = true;
					}
					else
					{
						error("xml:space in the source XML has an illegal value: " + xmlSpaceVal);
					}
					break;
				}

				double highPreserveScore = XPath::s_MatchScoreNone;
				double highStripScore = XPath::s_MatchScoreNone;

				ElementPrefixResolverProxy		theProxy(parentElem, m_xpathEnvSupport, m_xpathSupport);

				{
					// $$$ ToDo:  All of this should be moved into a member of
					// Stylesheet, so as not to expose these two data members...
					typedef Stylesheet::XPathVectorType		XPathVectorType;

					const XPathVectorType&	theElements =
						m_stylesheetRoot->getWhitespacePreservingElements();

					const XPathVectorType::size_type	nTests =
						theElements.size();

					XPathExecutionContextDefault	theExecutionContext(m_xpathEnvSupport,
																		m_xpathSupport,
																		m_xobjectFactory,
																		parent,
																		NodeRefList(),
																		&theProxy);

					for(XPathVectorType::size_type i = 0; i < nTests; i++)
					{
						const XPath* const	matchPat = theElements[i];
						assert(matchPat != 0);

						const double	score = matchPat->getMatchScore(parent, theProxy, theExecutionContext);

						if(score > highPreserveScore)
							highPreserveScore = score;
					}
				}

				{
					typedef Stylesheet::XPathVectorType		XPathVectorType;

					const XPathVectorType&	theElements =
						m_stylesheetRoot->getWhitespaceStrippingElements();

					const XPathVectorType::size_type	nTests =
						theElements.size();

					XPathExecutionContextDefault	theExecutionContext(m_xpathEnvSupport,
																		m_xpathSupport,
																		m_xobjectFactory,
																		parent,
																		NodeRefList(),
																		&theProxy);

					for(XPathVectorType::size_type i = 0; i < nTests; i++)
					{
						const XPath* const	matchPat =
							theElements[i];
						assert(matchPat != 0);

						const double	score = matchPat->getMatchScore(parent, theProxy, theExecutionContext);

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

			parent = parent->getParentNode();
		}
	}

	return strip;
}



XalanDOMString
XSLTEngineImpl::fixWhiteSpace(
			const XalanDOMString&	string, 
			bool					trimHead, 
			bool					trimTail, 
			bool					doublePunctuationSpaces) 
{
	const XMLCh* const	theStringData = c_wstr(string);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XMLCh>		XMLChVectorType;
#else
	typedef std::vector<XMLCh>	XMLChVectorType;
#endif

	XMLChVectorType		buf(theStringData,
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

	XMLChVectorType::const_iterator	start = buf.begin();
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
		return XalanDOMString(start, theLength);
	}
}



const XalanDOMString
XSLTEngineImpl::getNormalizedText(const XalanText&	tx) const
{
	if(m_outputCarriageReturns == false && m_outputLinefeeds == false)
	{
		return tx.getData();
	}

	const XalanDOMString 	src = tx.getData();

	const int				nSrcChars = src.length();

#if !defined(XALAN_NO_NAMESPACES)
		using std::vector;
#endif

	vector<XMLCh>		sb;

	XMLCh					prevChar = 0x00;

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

	return XalanDOMString(sb.begin(), sb.size());
}



XMLParserLiaison&
XSLTEngineImpl::getXMLParserLiaison() const
{
	return m_parserLiaison;
}



const XalanDOMString
XSLTEngineImpl::getUniqueNSValue() const
{
	return XALAN_STATIC_UCODE_STRING("ns") + LongToDOMString(m_uniqueNSValue++);
}



XalanDocument*
XSLTEngineImpl::getDOMFactory() const
{
	if(m_resultTreeFactory == 0)
	{
		m_resultTreeFactory = m_parserLiaison.createDocument();
	}

	return m_resultTreeFactory;
}



/**
 * Given a name, locate a variable in the current context, and return 
 * the Object.
 */
XObject*
XSLTEngineImpl::getVariable(const QName& qname) const
{
	return m_variableStacks.getXObjectVariable(qname);
}



XObject*
XSLTEngineImpl::getParamVariable(const QName&	theName) const
{
	return m_variableStacks.getXObjectParamVariable(theName);
}



void
XSLTEngineImpl::pushVariable(
				const QName&		name,
				XObject*			val,
				const XalanNode*	e)
{
	m_variableStacks.pushVariable(name, val, e);
}



/**
 * Create a document fragment.  This function may return null.
 */
ResultTreeFragBase* XSLTEngineImpl::createDocFrag() const
{
	return new ResultTreeFrag(*getDOMFactory(), m_xpathSupport);
}
  


XObject*
XSLTEngineImpl::getXObjectVariable(const XalanDOMString&	name) const
{
	assert(m_stylesheetRoot != 0);
	assert(m_stylesheetExecutionContext != 0);

	XObject*	theResult = m_variableStacks.getXObjectVariable(name);

    if(0 == theResult)
    {
		theResult = m_stylesheetRoot->getTopLevelVariable(name, *m_stylesheetExecutionContext);
    }

    return theResult;
}



XLocator*
XSLTEngineImpl::getXLocatorFromNode(const XalanNode*	node) const
{
	return m_xpathEnvSupport.getXLocatorFromNode(node);
}
	


void
XSLTEngineImpl::associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator)
{
	m_xpathEnvSupport.associateXLocatorToNode(node, xlocator);
}



XObject*
XSLTEngineImpl::getTopLevelVariable(const XalanDOMString&	theName) const
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
	return new ResultTreeFrag(*getDOMFactory(), m_xpathSupport);
}



const XalanDOMString
XSLTEngineImpl::getStyleSheetURIFromDoc(const XalanNode&	sourceTree)
{
	XalanDOMString		stylesheetURI;

	const XalanNode*	child = sourceTree.getFirstChild();

	// $$$ ToDo: is this first one still valid?
	const XalanDOMString	stylesheetNodeName1(XALAN_STATIC_UCODE_STRING("xml-stylesheet"));
	const XalanDOMString	stylesheetNodeName2(XALAN_STATIC_UCODE_STRING("xml:stylesheet"));

	// $$$ ToDo: This code is much like that in process().
	// Why is it repeated???
	// $$$ ToDo: Move these embedded strings from inside these loops
	// out here...
	// $$$ ToDo: These loops are a mess of repeated use of the
	// same data values.
	while(child != 0)
	{
		if(XalanNode::PROCESSING_INSTRUCTION_NODE == child->getNodeType())
		{
			const XalanDOMString	nodeName(child->getNodeName());

			if(equals(nodeName, stylesheetNodeName1) ||
				equals(nodeName, stylesheetNodeName2))
			{
				bool	isOK = true;

				const XalanDOMString	nodeValue(child->getNodeValue());

				StringTokenizer 		tokenizer(nodeValue, XALAN_STATIC_UCODE_STRING(" \t="));

				while(tokenizer.hasMoreTokens() == true)
				{
					const XalanDOMString	nextToken(tokenizer.nextToken());

					if(equals(nextToken, XALAN_STATIC_UCODE_STRING("type")) == true)
					{
						const XalanDOMString	typeVal =
							substring(nextToken, 1, length(nextToken) - 1);

						if(equals(typeVal, XALAN_STATIC_UCODE_STRING("text/xsl")) == false)
						{
							isOK = false;
						}
					}
				}  

				if(isOK == true)
				{
					StringTokenizer 	tokenizer(nodeValue, XALAN_STATIC_UCODE_STRING(" \t="));

					while(tokenizer.hasMoreTokens() == true)
					{
						const XalanDOMString	nextToken(tokenizer.nextToken());

						if(equals(nextToken, XALAN_STATIC_UCODE_STRING("href")))
						{
							stylesheetURI =
								substring(nextToken, 1, nextToken.length() - 1);
						}
					}
					break;
				}
			}
		}

		child = child->getNextSibling();
	}

	return stylesheetURI;
}



void
XSLTEngineImpl::setStylesheetParam(
			const XalanDOMString&	theName,
			const XalanDOMString&	expression)
{
	const QName		qname(theName, 0, m_xpathEnvSupport, m_xpathSupport);

	m_topLevelParams.push_back(Arg(qname, expression, true));
}



void
XSLTEngineImpl::setStylesheetParam(
			const XalanDOMString&	theName,
			XObject*				theValue)
{
	const QName		qname(theName, 0, m_xpathEnvSupport, m_xpathSupport);

	m_topLevelParams.push_back(Arg(qname, theValue, true));
}



void
XSLTEngineImpl::resolveTopLevelParams(StylesheetExecutionContext&	executionContext)
{
	m_stylesheetRoot->pushTopLevelVariables(executionContext, m_topLevelParams);
	getVariableStacks().markGlobalStackFrame();
	getVariableStacks().pushContextMarker(0, 0);
}



void
XSLTEngineImpl::resetCurrentState(
			XalanNode*	/* sourceTree */,
			XalanNode*	xmlNode)
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
			const XalanDOMString&	/* classid */,
			const XalanDOMString&	/* codetype */)
{
#if 1
	error("Xalan does not support extensions at this time!");
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
			const XalanDOMString&	/* mimeType */,
			DispatcherFactory*	/* factory */)
{
#if 1
	error("Xalan does not support extensions at this time!");
#else
	if(0 != m_diagnosticsPrintWriter)
	{
		diag(XalanDOMString("Registering dispatcher factory for: ") + mimeType);
	}

	m_dispatcherFactories.put(mimeType, factory);
#endif
}



  

// $$$ ToDo: This really belongs in DOMServices or DOMSupport()
XalanElement*
XSLTEngineImpl::findElementByAttribute(
			XalanElement& 			elem,
			const XalanDOMString&	targetElementName, 
			const XalanDOMString&	targetAttributeName,
			const XalanDOMString&	targetAttributeValue)
{
	XalanElement* 			theFoundElement = 0;

	const XalanDOMString 	tagName = elem.getTagName();

	if(0 == length(targetElementName) || equals(tagName, targetElementName))
	{
		const XalanNamedNodeMap* const	attributes = elem.getAttributes();

		try
		{
			const unsigned int	nAttributes = 0 != attributes ? attributes->getLength() : 0;

			for(unsigned int i = 0; i < nAttributes; i++)  
			{
				const XalanAttr* 		attr = 
						  static_cast<const XalanAttr*>(attributes->item(i));

				const XalanDOMString 	attrName = attr->getName();

				if(equals(attrName, targetAttributeName))
				{
					const XalanDOMString	attrVal = attr->getValue();

					if(equals(attrVal, targetAttributeValue))
					{
						theFoundElement = &elem;
						break;
					}
				}
			}
		}
		catch(const XalanDOMException&)
		{
		}
	}

	if(0 == theFoundElement)
	{
		XalanNode*	childNode = elem.getFirstChild();

		while(childNode != 0) 
		{
			if (childNode->getNodeType() == XalanNode::ELEMENT_NODE) 
			{
				XalanElement*	child = 
						  static_cast<XalanElement*>(childNode);

				const XalanDOMString 	childName = child->getTagName();

				if(0 != length(childName))
				{
					theFoundElement = findElementByAttribute(
													 *child,
													 targetElementName, 
													 targetAttributeName,
													 targetAttributeValue);

					if(0 != theFoundElement)
					{
						break;
					}
				}
			}

			childNode = childNode->getNextSibling();
		}
	}

	return theFoundElement;
}



XMLURL*
XSLTEngineImpl::getURLFromString (const XalanDOMString&	urlString) const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	auto_ptr<XMLURL>	url(new XMLURL);

	try 
	{
		url->setURL(c_wstr(urlString));
	}
	// 'urlString' not a valid url, try to construct a file url
	catch (const MalformedURLException&)
	{
		XalanDOMString fullpath("file:///");

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

	return url.release();
}



XMLURL* XSLTEngineImpl::getURLFromString(const XalanDOMString&	urlString, const XalanDOMString& base) const
{
	if (isEmpty(base))
		return getURLFromString(urlString);

	// We'll only do the really simple case for now:
	// base is a complete file URL and urlString is a forward relative path, i.e. 
	// in the same directory as the urlString or a subdirectory

	XalanDOMString context;

	// just to be robust, we'll accept a forward or back slash
	const unsigned int	theLength = length(base);

	const unsigned int	i1 = lastIndexOf(base,'/');
	const unsigned int	i2 = lastIndexOf(base,'\\');

	const unsigned int	i = i1 > i2 && i1 < theLength ? i1 : i2 < theLength ? i2 : i1;

	if (i < theLength)
		context = substring(base, 0, i + 1);

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



FormatterListener*
XSLTEngineImpl::getFormatterListener() const
{
	return m_flistener;
}



void
XSLTEngineImpl::setFormatterListener(FormatterListener*		flistener)
{
	m_flistener = flistener;
}



XObject*
XSLTEngineImpl::createXResultTreeFrag(const ResultTreeFragBase&  r) const
{
	return m_xobjectFactory.createResultTreeFrag(r);
}



//@@ JMD: NOTE: java implementation of these classes does not pass reference
//to processor

//////////////////////////////////////////////////////////////////////////////
// CLASS XSLTEngineImpl::StackGuard
//////////////////////////////////////////////////////////////////////////////

XSLTEngineImpl::StackGuard::StackGuard(
			XSLTEngineImpl&			processor,
			const XalanElement*		xslTemplate,
			const XalanNode*		sourceXML) :
	m_processor(&processor),
	m_xslRule(xslTemplate),
	m_sourceXML(sourceXML),
	m_stack()
{
}



XSLTEngineImpl::StackGuard::~StackGuard()
{
}



void
XSLTEngineImpl::StackGuard::print(PrintWriter&	pw) const
{
	// for the moment, these diagnostics are really bad...
	const int	theType = m_sourceXML->getNodeType();

	if(theType == XalanNode::TEXT_NODE)
	{
		const XalanText* const	tx =
			static_cast<const XalanText*>(m_sourceXML);

		pw.println(tx->getData());
	}
	else if(theType == XalanNode::ELEMENT_NODE)
	{
		pw.println(m_sourceXML->getNodeName());
	}
}



void
XSLTEngineImpl::StackGuard::checkForInfiniteLoop(const StackGuard&	guard) const
{
	const int	nRules = m_stack.size();

	int			loopCount = 0;

	for(int i = (nRules - 1); i >= 0; --i)
	{
		if(m_stack[i] == guard)
		{
			loopCount++;
		}

		if(loopCount >= 4)
		{
			DOMStringPrintWriter	pw;

			pw.println(XalanDOMString("Infinite loop diagnosed!  Stack trace:"));

			int		k = 0;

			for(; k < nRules; k++)
			{
				pw.println(XalanDOMString("Source Elem #") +
								LongToDOMString(k) +
								XalanDOMString(" "));

				m_stack[i].print(pw);
			}

			pw.println(XalanDOMString("Source Elem #") +
							LongToDOMString(k) +
							XalanDOMString(" "));

			guard.print(pw);

			pw.println(XalanDOMString("End of infinite loop diagnosis."));

			m_processor->diag(pw.getString());

			throw XSLTEngineImpl::XSLInfiniteLoopException();
		}
	}
}



void
XSLTEngineImpl::StackGuard::push(
				const XalanElement*		xslTemplate,
				const XalanNode*		sourceXML)
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
	m_stackEntries(),
	m_processor(theProcessor),
	m_globalStackFrameIndex(-1),
	m_currentStackFrameIndex(0)
{
	pushContextMarker(0, 0);	
}



XSLTEngineImpl::VariableStack::~VariableStack()
{
	reset();
}



void
XSLTEngineImpl::VariableStack::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	m_stack.clear();

	// Delete all entries that we created...
	for_each(m_stackEntries.begin(),
			 m_stackEntries.end(),
			 DeleteFunctor<StackEntry>());

	m_stackEntries.clear();

	pushContextMarker(0, 0);	
}



void
XSLTEngineImpl::VariableStack::pushElementMarker(const XalanNode*	elem)
{
	StackEntry* const	theEntry = new ElementMarker(elem);

	m_stackEntries.insert(theEntry);

	push(theEntry);
}



void
XSLTEngineImpl::VariableStack::popElementMarker(const XalanNode*	elem)
{
	if(elementMarkerAlreadyPushed(elem) == true)
	{
		const int	nElems = m_stack.size();

		bool		fFound = false;

		// Sub 1 extra for the context marker.
		for(int i = (nElems - 1); i >= 0 && fFound == false; i--)
		{
			const StackEntry* const		theEntry = m_stack[i];
			assert(theEntry != 0);

			if(theEntry->getType() == StackEntry::eElementMarker)
			{
				pop();
				fFound = true;
			}
			else
			{
				pop();
			}

		}
	}
}



bool
XSLTEngineImpl::VariableStack::elementMarkerAlreadyPushed(const XalanNode*	elem) const
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

			if(theElementMarkerEntry->getElement() == elem)
			{
				return true;
			}
		}
	}

	return false;
}



void
XSLTEngineImpl::VariableStack::pushContextMarker(
			const XalanNode*	caller,
			const XalanNode*	sourceNode)
{
	StackEntry* const	theEntry = new ContextMarker(caller, sourceNode);

	m_stackEntries.insert(theEntry);

	push(theEntry);
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

		const StackEntry::eStackEntryType	type = theEntry->getType();
		assert(type < 4 && type >= 0);

		fFound = type == StackEntry::eContextMarker ? true : false;

		pop();
	}
}



class PopPushStackEntry
{
public:

	PopPushStackEntry(
			XSLTEngineImpl::VariableStack&	theVariableStack) :
		m_variableStack(theVariableStack),
		m_stackEntry(theVariableStack.back())
	{
		theVariableStack.pop();
	}

	~PopPushStackEntry()
	{
		m_variableStack.push(m_stackEntry);
	}

private:

	XSLTEngineImpl::VariableStack&	m_variableStack;

	StackEntry* const				m_stackEntry;
};



class CommitPushElementMarker
{
public:

	CommitPushElementMarker(
			XSLTEngineImpl::VariableStack&	theVariableStack,
			const XalanNode*				targetTemplate) :
		m_variableStack(&theVariableStack),
		m_targetTemplate(targetTemplate)
	{
		theVariableStack.pushElementMarker(targetTemplate);
	}

	~CommitPushElementMarker()
	{
		if (m_variableStack != 0)
		{
			m_variableStack->popElementMarker(m_targetTemplate);
		}
	}

	void
	commit()
	{
		m_variableStack = 0;
	}

private:

	XSLTEngineImpl::VariableStack*	m_variableStack;

	const XalanNode* const			m_targetTemplate;
};



void
XSLTEngineImpl::VariableStack::pushParams(
				StylesheetExecutionContext&		executionContext,
				const ElemTemplateElement&		xslCallTemplateElement,
				XalanNode*						sourceTree, 
				XalanNode*						sourceNode,
				const QName&					mode,
				const XalanNode*				targetTemplate)
{
	StackEntry* const		theStackEntry = m_stack.back();

	if (theStackEntry->getType() != StackEntry::eContextMarker)
	{
		// @@ $$$ ToDo: Fix this!!!
		// throw InvalidStackContext();
		return;
	}

	VariableStackStackType		tempStack;

	const ElemTemplateElement*	child =
			xslCallTemplateElement.getFirstChildElem();

	if (0 != child)
	{
		// This object will take care of popping, then
		// pushing the context marker at the top of the
		// stack, even if an exception is thrown...
		PopPushStackEntry	thePopPush(*this);

		while(0 != child)
		{
			if(Constants::ELEMNAME_WITHPARAM == child->getXSLToken())
			{
				const ElemWithParam* const	xslParamElement =
#if defined(XALAN_OLD_STYLE_CASTS)
							(ElemWithParam*)child;
#else
				static_cast<const ElemWithParam*>(child);
#endif

				Arg*	theArg = 0;

				const XPath* const	pxpath = xslParamElement->getSelectPattern();

				if(0 != pxpath)
				{
					XObject* const	theXObject =
								pxpath->execute(sourceNode,
										*xslParamElement,
										executionContext);

					theArg = new Arg(xslParamElement->getQName(), theXObject, true);
				}
				else
				{
					ResultTreeFragBase* const	theDocFragment =
								m_processor.createResultTreeFrag(executionContext,
										*xslParamElement,
										sourceTree,
										sourceNode,
										mode);
					assert(theDocFragment != 0);

#if !defined(XALAN_NO_NAMESPACES)
					using std::auto_ptr;
#endif

					// Make sure this sucker gets cleaned up...
					auto_ptr<ResultTreeFragBase>	theGuard(theDocFragment);

					XObject* const	var = m_processor.createXResultTreeFrag(*theDocFragment);

					theArg = new Arg(xslParamElement->getQName(), var, true);
				}
				assert(theArg != 0);

				m_stackEntries.insert(theArg);

				tempStack.push_back(theArg);
			}

			child = child->getNextSiblingElem();
		}
	}

	// This object will push an element marker, and pop it
	// if we don't call it's commit() member function.  So
	// if an exception is thrown will transferring the
	// parameters, the element marker will be popped.
	// This keeps the stack in a consistent state.
	CommitPushElementMarker		thePusher(*this,
										  targetTemplate);

	const VariableStackStackType::size_type		nParams = tempStack.size();

	for(VariableStackStackType::size_type i = 0; i < nParams; ++i)
	{
		push(tempStack[i]);
	}

	thePusher.commit();
}



void
XSLTEngineImpl::VariableStack::pushVariable(
			const QName&		name,
			XObject*			val,
			const XalanNode*	e)
{
	if(elementMarkerAlreadyPushed(e) == false)
	{
		pushElementMarker(e);
	}

	StackEntry* const	theEntry = new Arg(name, val, false);

	m_stackEntries.insert(theEntry);

	push(theEntry);
}




XObject*
XSLTEngineImpl::VariableStack::findXObject(
			const QName&	name,
			bool			fSearchGlobalSpace) const
{
	XObject*		theXObject = 0;

	const Arg* const	theArg = findArg(name, fSearchGlobalSpace);

	if (theArg != 0)
	{
		if (theArg->getArgType() == Arg::eXObject)
		{
			theXObject = theArg->getXObjectPtr();
		}
	}

	return theXObject;
}





const Arg*
XSLTEngineImpl::VariableStack::findArg(
			const QName&	qname,
			bool			fSearchGlobalSpace) const
{
	const Arg*	theResult = 0;

	const int	nElems = getCurrentStackFrameIndex();

	// Sub 1 extra for the context marker.
	for(int i = (nElems - 1); i >= 0; i--)
	{
		const StackEntry* const		theEntry =
			m_stack[i];
		assert(theEntry != 0);

		if(theEntry->getType() == StackEntry::eArgument)
		{
			const Arg* const	theArg =
				static_cast<const Arg*>(theEntry);

			if(theArg->getName().equals(qname))
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
		for(int i = (m_globalStackFrameIndex-1); i >= 2; i--)
		{
			const StackEntry* const		theEntry = m_stack[i];
			assert(theEntry != 0);
			if(theEntry->getType() == StackEntry::eArgument)
			{
				const Arg* const	theArg =
					static_cast<const Arg*>(theEntry);

				if(theArg->getName().equals(qname))
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
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("current"), FunctionCurrent());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("document"), FunctionDocument());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("element-available"), FunctionElementAvailable());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("function-available"), FunctionFunctionAvailable());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("format-number"), FunctionFormatNumber());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("generate-id"), FunctionGenerateID());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("key"), FunctionKey());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("system-property"), FunctionSystemProperty());
	XPath::installFunction(XALAN_STATIC_UCODE_STRING("unparsed-entity-uri"), FunctionUnparsedEntityURI());
}



void
XSLTEngineImpl::InitializeAttributeKeysTable()
{
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_METHOD] = Constants::TATTRNAME_OUTPUT_METHOD;
	s_attributeKeys[Constants::ATTRNAME_AMOUNT] = Constants::TATTRNAME_AMOUNT;
	s_attributeKeys[Constants::ATTRNAME_ANCESTOR] = Constants::TATTRNAME_ANCESTOR;
	s_attributeKeys[Constants::ATTRNAME_ARCHIVE] = Constants::TATTRNAME_ARCHIVE;
	s_attributeKeys[Constants::ATTRNAME_ATTRIBUTE] = Constants::TATTRNAME_ATTRIBUTE;
	s_attributeKeys[Constants::ATTRNAME_ATTRIBUTE_SET] = Constants::TATTRNAME_ATTRIBUTE_SET;
	s_attributeKeys[Constants::ATTRNAME_CASEORDER] = Constants::TATTRNAME_CASEORDER;
	s_attributeKeys[Constants::ATTRNAME_CLASS] = Constants::TATTRNAME_CLASS;
	s_attributeKeys[Constants::ATTRNAME_CLASSID] = Constants::TATTRNAME_CLASSID;
	s_attributeKeys[Constants::ATTRNAME_CODEBASE] = Constants::TATTRNAME_CODEBASE;
	s_attributeKeys[Constants::ATTRNAME_CODETYPE] = Constants::TATTRNAME_CODETYPE;
	s_attributeKeys[Constants::ATTRNAME_CONDITION] = Constants::TATTRNAME_CONDITION;
	s_attributeKeys[Constants::ATTRNAME_COPYTYPE] = Constants::TATTRNAME_COPYTYPE;
	s_attributeKeys[Constants::ATTRNAME_COUNT] = Constants::TATTRNAME_COUNT;
	s_attributeKeys[Constants::ATTRNAME_DATATYPE] = Constants::TATTRNAME_DATATYPE;
	s_attributeKeys[Constants::ATTRNAME_DEFAULT] = Constants::TATTRNAME_DEFAULT;
	s_attributeKeys[Constants::ATTRNAME_DEFAULTSPACE] = Constants::TATTRNAME_DEFAULTSPACE;
	s_attributeKeys[Constants::ATTRNAME_DEPTH] = Constants::TATTRNAME_DEPTH;
	s_attributeKeys[Constants::ATTRNAME_DIGITGROUPSEP] = Constants::TATTRNAME_DIGITGROUPSEP;
	s_attributeKeys[Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING] = Constants::TATTRNAME_DISABLE_OUTPUT_ESCAPING;
	s_attributeKeys[Constants::ATTRNAME_ELEMENT] = Constants::TATTRNAME_ELEMENT;
	s_attributeKeys[Constants::ATTRNAME_ELEMENTS] = Constants::TATTRNAME_ELEMENTS;
	s_attributeKeys[Constants::ATTRNAME_EXPR] = Constants::TATTRNAME_EXPR;
	s_attributeKeys[Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES] = Constants::TATTRNAME_EXTENSIONELEMENTPREFIXES;
	s_attributeKeys[Constants::ATTRNAME_FORMAT] = Constants::TATTRNAME_FORMAT;
	s_attributeKeys[Constants::ATTRNAME_FROM] = Constants::TATTRNAME_FROM;
	s_attributeKeys[Constants::ATTRNAME_GROUPINGSEPARATOR] = Constants::TATTRNAME_GROUPINGSEPARATOR;
	s_attributeKeys[Constants::ATTRNAME_GROUPINGSIZE] = Constants::TATTRNAME_GROUPINGSIZE;
	s_attributeKeys[Constants::ATTRNAME_HREF] = Constants::TATTRNAME_HREF;
	s_attributeKeys[Constants::ATTRNAME_ID] = Constants::TATTRNAME_ID;
	s_attributeKeys[Constants::ATTRNAME_IMPORTANCE] = Constants::TATTRNAME_IMPORTANCE;
	s_attributeKeys[Constants::ATTRNAME_INDENTRESULT] = Constants::TATTRNAME_INDENTRESULT;
	s_attributeKeys[Constants::ATTRNAME_LANG] = Constants::TATTRNAME_LANG;
	s_attributeKeys[Constants::ATTRNAME_LETTERVALUE] = Constants::TATTRNAME_LETTERVALUE;
	s_attributeKeys[Constants::ATTRNAME_LEVEL] = Constants::TATTRNAME_LEVEL;
	s_attributeKeys[Constants::ATTRNAME_MATCH] = Constants::TATTRNAME_MATCH;
	s_attributeKeys[Constants::ATTRNAME_METHOD] = Constants::TATTRNAME_METHOD;
	s_attributeKeys[Constants::ATTRNAME_MODE] = Constants::TATTRNAME_MODE;
	s_attributeKeys[Constants::ATTRNAME_NAME] = Constants::TATTRNAME_NAME;
	s_attributeKeys[Constants::ATTRNAME_NAMESPACE] = Constants::TATTRNAME_NAMESPACE;
	s_attributeKeys[Constants::ATTRNAME_NDIGITSPERGROUP] = Constants::TATTRNAME_NDIGITSPERGROUP;
	s_attributeKeys[Constants::ATTRNAME_NS] = Constants::TATTRNAME_NS;
	s_attributeKeys[Constants::ATTRNAME_ONLY] = Constants::TATTRNAME_ONLY;
	s_attributeKeys[Constants::ATTRNAME_ORDER] = Constants::TATTRNAME_ORDER;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS] = Constants::TATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC] = Constants::TATTRNAME_OUTPUT_DOCTYPE_PUBLIC;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM] = Constants::TATTRNAME_OUTPUT_DOCTYPE_SYSTEM;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_ENCODING] = Constants::TATTRNAME_OUTPUT_ENCODING;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_INDENT] = Constants::TATTRNAME_OUTPUT_INDENT;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_MEDIATYPE] = Constants::TATTRNAME_OUTPUT_MEDIATYPE;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_STANDALONE] = Constants::TATTRNAME_OUTPUT_STANDALONE;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_VERSION] = Constants::TATTRNAME_OUTPUT_VERSION;
	s_attributeKeys[Constants::ATTRNAME_OUTPUT_OMITXMLDECL] = Constants::TATTRNAME_OUTPUT_OMITXMLDECL;
	s_attributeKeys[Constants::ATTRNAME_PRIORITY] = Constants::TATTRNAME_PRIORITY;
	s_attributeKeys[Constants::ATTRNAME_REFID] = Constants::TATTRNAME_REFID;
	s_attributeKeys[Constants::ATTRNAME_RESULTNS] = Constants::TATTRNAME_RESULTNS;
	s_attributeKeys[Constants::ATTRNAME_SELECT] = Constants::TATTRNAME_SELECT;
	s_attributeKeys[Constants::ATTRNAME_SEQUENCESRC] = Constants::TATTRNAME_SEQUENCESRC;
	s_attributeKeys[Constants::ATTRNAME_STYLE] = Constants::TATTRNAME_STYLE;
	s_attributeKeys[Constants::ATTRNAME_TEST] = Constants::TATTRNAME_TEST;
	s_attributeKeys[Constants::ATTRNAME_TOSTRING] = Constants::TATTRNAME_TOSTRING;
	s_attributeKeys[Constants::ATTRNAME_TYPE] = Constants::TATTRNAME_TYPE;
	s_attributeKeys[Constants::ATTRNAME_USE] = Constants::TATTRNAME_USE;
	s_attributeKeys[Constants::ATTRNAME_USEATTRIBUTESETS] = Constants::TATTRNAME_USEATTRIBUTESETS;
	s_attributeKeys[Constants::ATTRNAME_VALUE] = Constants::TATTRNAME_VALUE;

	s_attributeKeys[Constants::ATTRNAME_XMLNSDEF] = Constants::TATTRNAME_XMLNSDEF;
	s_attributeKeys[Constants::ATTRNAME_XMLNS] = Constants::TATTRNAME_XMLNS;
	s_attributeKeys[Constants::ATTRNAME_XMLSPACE] = Constants::TATTRNAME_XMLSPACE;
}



void
XSLTEngineImpl::InitializeElementKeysTable()
{
	s_elementKeys[Constants::ELEMNAME_APPLY_TEMPLATES_STRING] = Constants::ELEMNAME_APPLY_TEMPLATES;
	s_elementKeys[Constants::ELEMNAME_WITHPARAM_STRING] = Constants::ELEMNAME_WITHPARAM;
	s_elementKeys[Constants::ELEMNAME_CONSTRUCT_STRING] = Constants::ELEMNAME_CONSTRUCT;
	s_elementKeys[Constants::ELEMNAME_CONTENTS_STRING] = Constants::ELEMNAME_CONTENTS;
	s_elementKeys[Constants::ELEMNAME_COPY_STRING] = Constants::ELEMNAME_COPY;
	s_elementKeys[Constants::ELEMNAME_COPY_OF_STRING] = Constants::ELEMNAME_COPY_OF;

	s_elementKeys[Constants::ELEMNAME_ATTRIBUTESET_STRING] = Constants::ELEMNAME_DEFINEATTRIBUTESET;

	s_elementKeys[Constants::ELEMNAME_USE_STRING] = Constants::ELEMNAME_USE;

	s_elementKeys[Constants::ELEMNAME_VARIABLE_STRING] = Constants::ELEMNAME_VARIABLE;
	s_elementKeys[Constants::ELEMNAME_PARAMVARIABLE_STRING] = Constants::ELEMNAME_PARAMVARIABLE;

	s_elementKeys[Constants::ELEMNAME_DISPLAYIF_STRING] = Constants::ELEMNAME_DISPLAYIF;
	s_elementKeys[Constants::ELEMNAME_EMPTY_STRING] = Constants::ELEMNAME_EMPTY;
	s_elementKeys[Constants::ELEMNAME_EVAL_STRING] = Constants::ELEMNAME_EVAL;
	s_elementKeys[Constants::ELEMNAME_CALLTEMPLATE_STRING] = Constants::ELEMNAME_CALLTEMPLATE;
	s_elementKeys[Constants::ELEMNAME_TEMPLATE_STRING] = Constants::ELEMNAME_TEMPLATE;
	s_elementKeys[Constants::ELEMNAME_STYLESHEET_STRING] = Constants::ELEMNAME_STYLESHEET;
	s_elementKeys[Constants::ELEMNAME_TRANSFORM_STRING] = Constants::ELEMNAME_STYLESHEET;
	s_elementKeys[Constants::ELEMNAME_IMPORT_STRING] = Constants::ELEMNAME_IMPORT;
	s_elementKeys[Constants::ELEMNAME_INCLUDE_STRING] = Constants::ELEMNAME_INCLUDE;
	s_elementKeys[Constants::ELEMNAME_FOREACH_STRING] = Constants::ELEMNAME_FOREACH;
	s_elementKeys[Constants::ELEMNAME_VALUEOF_STRING] = Constants::ELEMNAME_VALUEOF;
	s_elementKeys[Constants::ELEMNAME_KEY_STRING] = Constants::ELEMNAME_KEY;
	s_elementKeys[Constants::ELEMNAME_STRIPSPACE_STRING] = Constants::ELEMNAME_STRIPSPACE;
	s_elementKeys[Constants::ELEMNAME_PRESERVESPACE_STRING] = Constants::ELEMNAME_PRESERVESPACE;
	s_elementKeys[Constants::ELEMNAME_NUMBER_STRING] = Constants::ELEMNAME_NUMBER;
	s_elementKeys[Constants::ELEMNAME_IF_STRING] = Constants::ELEMNAME_IF;
	s_elementKeys[Constants::ELEMNAME_CHOOSE_STRING] = Constants::ELEMNAME_CHOOSE;
	s_elementKeys[Constants::ELEMNAME_WHEN_STRING] = Constants::ELEMNAME_WHEN;
	s_elementKeys[Constants::ELEMNAME_OTHERWISE_STRING] = Constants::ELEMNAME_OTHERWISE;
	s_elementKeys[Constants::ELEMNAME_TEXT_STRING] = Constants::ELEMNAME_TEXT;
	s_elementKeys[Constants::ELEMNAME_ELEMENT_STRING] = Constants::ELEMNAME_ELEMENT;
	s_elementKeys[Constants::ELEMNAME_ATTRIBUTE_STRING] = Constants::ELEMNAME_ATTRIBUTE;
	s_elementKeys[Constants::ELEMNAME_SORT_STRING] = Constants::ELEMNAME_SORT;
	s_elementKeys[Constants::ELEMNAME_PI_STRING] = Constants::ELEMNAME_PI;
	s_elementKeys[Constants::ELEMNAME_COMMENT_STRING] = Constants::ELEMNAME_COMMENT;
   
	s_elementKeys[Constants::ELEMNAME_COUNTER_STRING] = Constants::ELEMNAME_COUNTER;
	s_elementKeys[Constants::ELEMNAME_COUNTERS_STRING] = Constants::ELEMNAME_COUNTERS;
	s_elementKeys[Constants::ELEMNAME_COUNTERINCREMENT_STRING] = Constants::ELEMNAME_COUNTERINCREMENT;
	s_elementKeys[Constants::ELEMNAME_COUNTERRESET_STRING] = Constants::ELEMNAME_COUNTERRESET;
	s_elementKeys[Constants::ELEMNAME_COUNTERSCOPE_STRING] = Constants::ELEMNAME_COUNTERSCOPE;
	
	s_elementKeys[Constants::ELEMNAME_APPLY_IMPORTS_STRING] = Constants::ELEMNAME_APPLY_IMPORTS;
	
	s_elementKeys[Constants::ELEMNAME_EXTENSION_STRING] = Constants::ELEMNAME_EXTENSION;
	s_elementKeys[Constants::ELEMNAME_MESSAGE_STRING] = Constants::ELEMNAME_MESSAGE;
	s_elementKeys[Constants::ELEMNAME_LOCALE_STRING] = Constants::ELEMNAME_LOCALE;
	s_elementKeys[Constants::ELEMNAME_FALLBACK_STRING] = Constants::ELEMNAME_FALLBACK;
	s_elementKeys[Constants::ELEMNAME_OUTPUT_STRING] = Constants::ELEMNAME_OUTPUT;

	s_elementKeys[Constants::ELEMNAME_DECIMALFORMAT_STRING] = Constants::ELEMNAME_DECIMALFORMAT;
	s_elementKeys[Constants::ELEMNAME_NSALIAS_STRING] = Constants::ELEMNAME_NSALIAS;
}



void
XSLTEngineImpl::InitializeXSLT4JElementKeys()
{
	s_XSLT4JElementKeys[Constants::ELEMNAME_COMPONENT_STRING] = Constants::ELEMNAME_COMPONENT;
	s_XSLT4JElementKeys[Constants::ELEMNAME_SCRIPT_STRING] = Constants::ELEMNAME_SCRIPT;
}



//////////////////////////////////////////////////////////////////////////////
