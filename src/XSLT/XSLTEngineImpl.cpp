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



#include <sax/DocumentHandler.hpp>
#include <sax/EntityResolver.hpp>
#include <sax/Locator.hpp>
#include <sax/SAXException.hpp>
#include <util/PlatformUtils.hpp>
#include <framework/URLInputSource.hpp>



#include <Include/STLHelper.hpp>



#include <XalanDOM/XalanDOMException.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanComment.hpp>
#include <XalanDOM/XalanCDATASection.hpp>
#include <XalanDOM/XalanNodeList.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanProcessingInstruction.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/PrintWriter.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>
#include <DOMSupport/DOMSupport.hpp>



#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/QNameByReference.hpp>
#include <XPath/ResultTreeFrag.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XResultTreeFrag.hpp>



#include "Constants.hpp"
#include "ElemWithParam.hpp"
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
#include "XSLTResultTarget.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



const XalanDOMString	XSLTEngineImpl::s_emptyString;



//==========================================================
// SECTION: Constructors
//==========================================================

XSLTEngineImpl::XSLTEngineImpl(
			XMLParserLiaison&	parserLiaison,
			XPathEnvSupport&	xpathEnvSupport,
			DOMSupport&			domSupport,
			XObjectFactory&		xobjectFactory,
			XPathFactory&		xpathFactory) :
	XSLTProcessor(),
	DocumentHandler(),
	PrefixResolver(),
	m_outputCarriageReturns(false),
	m_outputLinefeeds(false),
	m_useDOMResultTreeFactory(false),
	m_domResultTreeFactory(0),
	m_resultNameSpacePrefix(),
	m_resultNameSpaceURL(),
	m_xpathFactory(xpathFactory),
	m_xobjectFactory(xobjectFactory),
	m_xpathProcessor(new XPathProcessorImpl),
	m_cdataStack(),
	m_stylesheetLocatorStack(),
	m_defaultProblemListener(),
	m_problemListener(&m_defaultProblemListener),
	m_stylesheetRoot(0),
	m_traceSelects(false),
	m_quietConflictWarnings(false),
	m_diagnosticsPrintWriter(0),
	m_durationsTable(),
	m_traceListeners(),
	m_uniqueNSValue(0),
	m_topLevelParams(),
	m_parserLiaison(parserLiaison),
	m_xpathEnvSupport(xpathEnvSupport),
	m_domSupport(domSupport),
	m_executionContext(0),
	m_outputContextStack(),
	m_resultNamespacesStack(),
	m_dummyAttributesList()
{
	m_outputContextStack.pushContext();
}



void
XSLTEngineImpl::reset()
{
	m_topLevelParams.clear();
	m_durationsTable.clear();
	m_stylesheetLocatorStack.clear();
	m_cdataStack.clear();

	if (m_domResultTreeFactory != 0)
	{
		m_parserLiaison.destroyDocument(m_domResultTreeFactory);
		m_domResultTreeFactory = 0;
	}

	m_stylesheetRoot = 0;

	m_outputContextStack.reset();

	m_outputContextStack.pushContext();

	m_xpathEnvSupport.reset();
	m_xpathFactory.reset();
	m_xobjectFactory.reset();
	m_domSupport.reset();

	m_resultNamespacesStack.clear();
}



XSLTEngineImpl::~XSLTEngineImpl()
{
	reset();
}



//==========================================================
// SECTION: Main API Functions
//==========================================================



static const XalanDOMChar	s_dummyString = 0;


void
XSLTEngineImpl::process(
			const XSLTInputSource&			inputSource, 
	        const XSLTInputSource&			stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext)
{
	try
	{
		XalanDOMString	xslIdentifier;

		if (0 == stylesheetSource.getSystemId())
		{
			xslIdentifier = XalanDOMString(XALAN_STATIC_UCODE_STRING("Input XSL"));
		}
		else
		{
			xslIdentifier = stylesheetSource.getSystemId();
		}

		bool totalTimeID = true;

		pushTime(&totalTimeID);

		XalanNode*	sourceTree = getSourceTreeFromInput(inputSource);

		try
		{
			m_stylesheetRoot = processStylesheet(stylesheetSource, constructionContext);
		}
		catch(const XSLException&)
		{
		}
		catch(const SAXException&)
		{
		}
		catch(const XMLException&)
		{
		}

		if(0 != sourceTree && m_stylesheetRoot == 0)
		{
			// Didn't get a stylesheet from the input source, so look for a
			// stylesheet processing instruction...
			XalanDOMString			stylesheetURI;

			// The PI must be a child of the document...
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

						StringTokenizer 	tokenizer(child->getNodeValue(), XALAN_STATIC_UCODE_STRING(" \t="));

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
							StringTokenizer 	tokenizer(child->getNodeValue(), XALAN_STATIC_UCODE_STRING(" \t="));

							while(tokenizer.hasMoreTokens())
							{
								const XalanDOMString	theCurrentToken = tokenizer.nextToken();

								if(equals(theCurrentToken, XALAN_STATIC_UCODE_STRING("href")))
								{
									stylesheetURI = tokenizer.nextToken();
									stylesheetURI = substring(stylesheetURI, 1, length(stylesheetURI) - 1);
									hrefs.push_back(stylesheetURI);
								}
							}
						}
					}
				}

				child = child->getNextSibling();
			}

			bool isRoot = true;
			Stylesheet* prevStylesheet = 0;
			
			if (hrefs.empty() == false)
			{
				const XalanDOMChar* const	pxch = inputSource.getSystemId();

				const XalanDOMString		sysid(pxch == 0 ? &s_dummyString : pxch); 

				do
				{
					const XalanDOMString&	ref =  hrefs.back();

					Stylesheet* stylesheet =
							getStylesheetFromPIURL(
								ref,
								*sourceTree,
								sysid,
								isRoot,
								constructionContext);

					if(false == isRoot)
					{
						prevStylesheet->addImport(stylesheet, false);
					}

					prevStylesheet = stylesheet;
					isRoot = false;
					hrefs.pop_back();
				} while(!hrefs.empty());
			}
		}

		if(0 == m_stylesheetRoot)
		{
			error("Failed to process stylesheet!");
		}
		else if(0 != sourceTree)
		{
			executionContext.setStylesheetRoot(m_stylesheetRoot);

			FormatterListener* const	theFormatter =
				outputTarget.getDocumentHandler();

			if (theFormatter != 0)
			{
				theFormatter->setPrefixResolver(this);
			}

			m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);

			if(0 != m_diagnosticsPrintWriter)
			{
				displayDuration(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Total time")), &totalTimeID);
			}
		}
	}
	catch(SAXException& se)
	{
		message("SAX Exception");

		throw se;
	}
	catch (...)
	{
		message("Unknown Exception");

		throw;
	}
}



void
XSLTEngineImpl::process(
			const XSLTInputSource&			inputSource, 
	        XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext)
{
	bool	totalTimeID = true;

	if(0 != m_diagnosticsPrintWriter)
	{
		pushTime(&totalTimeID);
	}

	XalanNode* const	sourceTree = getSourceTreeFromInput(inputSource);

	if(0 != sourceTree)
	{
		if (m_stylesheetRoot == 0)
		{
			error("No stylesheet is available to process!");
		}

		m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);
	}

	if(0 != m_diagnosticsPrintWriter)
	{
		displayDuration(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Total time")), &totalTimeID);
	}
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
			const XalanDOMString&			xsldocURLString,
			StylesheetConstructionContext&	constructionContext)
{
	try
	{
		XSLTInputSource		input(c_wstr(xsldocURLString));

		return processStylesheet(input, constructionContext);
	}
	catch(SAXException& se)
	{
		message("processStylesheet not successfull!");

		throw se;
	}

	return 0;
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
  			const XSLTInputSource&			stylesheetSource,
			StylesheetConstructionContext&	constructionContext)
{
	StylesheetRoot*		theStylesheet = 0;

	const XalanDOMChar* const	systemID = stylesheetSource.getSystemId();
	XalanNode* const			stylesheetNode = stylesheetSource.getNode();

	if (systemID != 0 || stylesheetNode != 0 || stylesheetSource.getStream() != 0)
	{
		XalanDOMString	xslIdentifier;

		try
		{
			theStylesheet = constructionContext.create(stylesheetSource);

			StylesheetHandler	stylesheetProcessor(*theStylesheet, constructionContext);

			if(stylesheetNode != 0)
			{
				xslIdentifier = XALAN_STATIC_UCODE_STRING("Input XSL");

				FormatterTreeWalker tw(stylesheetProcessor);

				tw.traverse(stylesheetSource.getNode());
			}
			else
			{
				if (systemID != 0)
				{
					xslIdentifier = systemID;
				}

				diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") + xslIdentifier + XALAN_STATIC_UCODE_STRING(" =========="));
				pushTime(&xslIdentifier);
				m_parserLiaison.parseXMLStream(stylesheetSource,
											   stylesheetProcessor);
				if(0 != m_diagnosticsPrintWriter)
					displayDuration(XALAN_STATIC_UCODE_STRING("Parse of ") + xslIdentifier, &xslIdentifier);
			}

			theStylesheet->postConstruction(constructionContext);
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
	}

	return theStylesheet;
}



//==========================================================
// SECTION: XML Parsing Functions
//==========================================================

XalanNode*
XSLTEngineImpl::getSourceTreeFromInput(const XSLTInputSource&	inputSource)
{
	XalanNode*		sourceTree = inputSource.getNode();

	if(0 == sourceTree)
	{
		const XalanDOMString	xmlIdentifier = 0 != inputSource.getSystemId() ?
												XalanDOMString(inputSource.getSystemId()) :
												StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Input XML"));

		// In case we have a fragment identifier, go ahead and 
		// try to parse the XML here.
		try
		{
			diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") +
					xmlIdentifier +
					XALAN_STATIC_UCODE_STRING(" =========="));

			pushTime(&xmlIdentifier);

#if defined(XALAN_VQ_SPECIAL_TRACE)
			QuantifyStartRecordingData();
#endif
			XalanDocument* const	theDocument =
						m_parserLiaison.parseXMLStream(inputSource,
													   xmlIdentifier);
			assert(theDocument != 0);

#if defined(XALAN_VQ_SPECIAL_TRACE)
			QuantifyStopRecordingData();
#endif
			if(0 != m_diagnosticsPrintWriter)
				displayDuration(XALAN_STATIC_UCODE_STRING("Parse of ") +
									xmlIdentifier,
								&xmlIdentifier);

			m_xpathEnvSupport.setSourceDocument(xmlIdentifier, theDocument);

			sourceTree = theDocument;
		}
		// catch(Exception e)
		// $$$ ToDo: Fix this!!!
		catch(...)
		{
			error("Could not parse " + xmlIdentifier + " document!");
		}
	}

	return sourceTree;
}



const XalanDOMString*
XSLTEngineImpl::getNamespaceForPrefix(const XalanDOMString&		prefix) const
{
	return m_resultNamespacesStack.getNamespaceForPrefix(prefix);
}



const XalanDOMString&
XSLTEngineImpl::getURI() const
{
	return s_emptyString;
}



XalanDocument*
XSLTEngineImpl::parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler,
			XalanDocument*			docToRegister)
{
	
	XalanDocument*	doc =
			m_xpathEnvSupport.getSourceDocument(urlString);

	if(doc == 0)
	{
		EntityResolver* const	theResolver = 
			m_parserLiaison.getEntityResolver();

		if (theResolver == 0)
		{
			const XSLTInputSource	inputSource(c_wstr(urlString));

			doc = parseXML(inputSource, docHandler, docToRegister);
		}
		else
		{
			const XalanAutoPtr<InputSource>		resolverInputSource =
				theResolver->resolveEntity(0, c_wstr(urlString));

			if (resolverInputSource.get() != 0)
			{
				doc = parseXML(*resolverInputSource.get(), docHandler, docToRegister);
			}
			else
			{
				XSLTInputSource		inputSource(c_wstr(urlString));

				doc = parseXML(inputSource, docHandler, docToRegister);
			}
		}

		if (doc != 0)
		{
			m_xpathEnvSupport.setSourceDocument(urlString, doc);
		}
	}

	return doc;
}



XalanDocument*
XSLTEngineImpl::parseXML(
			const InputSource&	inputSource,
			DocumentHandler*	docHandler,
			XalanDocument*		docToRegister)
{
	if(0 != docHandler)
	{
		m_parserLiaison.parseXMLStream(inputSource, *docHandler);

		return docToRegister;
	}
	else
	{
		return m_parserLiaison.parseXMLStream(inputSource);
	}
}



Stylesheet*
XSLTEngineImpl::getStylesheetFromPIURL(
			const XalanDOMString&			xslURLString,
			XalanNode&						fragBase,
			const XalanDOMString&			xmlBaseIdent,
			bool							isRoot,
			StylesheetConstructionContext&	constructionContext)
{
	Stylesheet*				stylesheet = 0;

	XalanDOMString			stringHolder;

	const XalanDOMString	localXSLURLString = trim(xslURLString);

	const unsigned int		fragIndex = indexOf(localXSLURLString, XalanUnicode::charNumberSign);

	const XalanDocument*	stylesheetDoc = 0;

	if(fragIndex == 0)
	{
		diag("Locating stylesheet from fragment identifier...");

		const XalanDOMString	fragID = substring(localXSLURLString, 1);

		const XalanElement*		nsNode = 0;

		const XalanNode::NodeType	theType = fragBase.getNodeType();

		if (theType == XalanNode::DOCUMENT_NODE)
		{
			const XalanDocument&	doc =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanDocument&)fragBase;
#else
				static_cast<const XalanDocument&>(fragBase);
#endif

			nsNode = doc.getDocumentElement(); 
		}
		else if	(theType == XalanNode::ELEMENT_NODE)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			nsNode = (const XalanElement*)&fragBase;
#else
			nsNode = static_cast<const XalanElement*>(&fragBase);
#endif
		}
		else
		{
			XalanNode* const	node = fragBase.getParentNode();

			if	(node->getNodeType() == XalanNode::ELEMENT_NODE) 
			{
#if defined(XALAN_OLD_STYLE_CASTS)
				nsNode = (const XalanElement*)&fragBase;
#else
				nsNode = static_cast<XalanElement*>(node);
#endif
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

		ElementPrefixResolverProxy		theProxy(nsNode, m_xpathEnvSupport, m_domSupport);

		XPathExecutionContextDefault	theExecutionContext(m_xpathEnvSupport,
															m_domSupport,
															m_xobjectFactory,
															&fragBase,
															0,
															&theProxy);

		const XObjectPtr	xobj(evalXPathStr(ds, theExecutionContext));
		assert(xobj.null() == false);

		MutableNodeRefList	nl(xobj->nodeset());

		if(nl.getLength() == 0)
		{
			NodeRefList		theEmptyList;

			ds = XALAN_STATIC_UCODE_STRING("//*[@id='");
			ds += fragID;
			ds += XALAN_STATIC_UCODE_STRING("']");

			theExecutionContext.setContextNodeList(theEmptyList);

			const XObjectPtr	xobj(evalXPathStr(ds, theExecutionContext));
			assert(xobj.null() == false);

			nl = xobj->nodeset();

			if(nl.getLength() == 0)
			{
				ds = XALAN_STATIC_UCODE_STRING("//*[@name='");
				ds += fragID;
				ds += XALAN_STATIC_UCODE_STRING("']");

				theExecutionContext.setContextNodeList(theEmptyList);

				const XObjectPtr	xobj(evalXPathStr(ds, theExecutionContext));
				assert(xobj.null() == false);

				nl = xobj->nodeset();

				if(nl.getLength() == 0)
				{
					// Well, hell, maybe it's an XPath...
					theExecutionContext.setContextNodeList(theEmptyList);

					const XObjectPtr	xobj(evalXPathStr(fragID, theExecutionContext));
					assert(xobj.null() == false);

					nl = xobj->nodeset();
				}
			}
		}

		if(nl.getLength() == 0)
		{
			error("Could not find fragment: " + fragID);
		}

		XalanNode* const	frag = nl.item(0);

		if(XalanNode::ELEMENT_NODE == frag->getNodeType())
		{
			pushTime(frag);

			XalanAutoPtr<Stylesheet>	theGuard;

			if(isRoot)
			{
				StylesheetRoot* const	theLocalRoot =
					constructionContext.create(stringHolder);

				stylesheet = theLocalRoot;

				m_stylesheetRoot = theLocalRoot;
			}
			else
			{
#if defined(XALAN_OLD_STYLE_CASTS)
				stylesheet = constructionContext.create(*((StylesheetRoot*)m_stylesheetRoot), stringHolder);
#else
				stylesheet = constructionContext.create(*const_cast<StylesheetRoot*>(m_stylesheetRoot), stringHolder);
#endif

				theGuard.reset(stylesheet);
			}

			StylesheetHandler stylesheetProcessor(*stylesheet, constructionContext);

			FormatterTreeWalker tw(stylesheetProcessor);

			tw.traverse(frag, frag->getParentNode());

			displayDuration(
					XalanDOMString(XALAN_STATIC_UCODE_STRING("Setup of ")) +
					localXSLURLString,
					frag);

			stylesheet->postConstruction(constructionContext);

			theGuard.release();
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

		XalanAutoPtr<Stylesheet>	theGuard;

		if(isRoot)
		{
			StylesheetRoot* const	theLocalRoot =
					constructionContext.create(localXSLURLString);

			stylesheet = theLocalRoot;

			m_stylesheetRoot = theLocalRoot;
		}
		else
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			stylesheet = new Stylesheet(*(StylesheetRoot*)m_stylesheetRoot, localXSLURLString, constructionContext);
#else
			stylesheet = new Stylesheet(*const_cast<StylesheetRoot*>(m_stylesheetRoot), localXSLURLString, constructionContext);
#endif

			theGuard.reset(stylesheet);
		}

		StylesheetHandler stylesheetProcessor(*stylesheet, constructionContext);

		typedef StylesheetConstructionContext::URLAutoPtrType	URLAutoPtrType;

		URLAutoPtrType	xslURL(constructionContext.getURLFromString(localXSLURLString, xmlBaseIdent));

		XSLTInputSource		inputSource(xslURL->getURLText());

		m_parserLiaison.parseXMLStream(inputSource, stylesheetProcessor);

		stylesheet->postConstruction(constructionContext);

		theGuard.release();

		displayDuration("Parsing and init of " + localXSLURLString, &localXSLURLString);
	}

	return stylesheet;
}


//==========================================================
// SECTION: Stylesheet Tables
//==========================================================


double
XSLTEngineImpl::getXSLTVerSupported()
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

	const XalanDOMString& 	ns = node.getNamespaceURI();

	if(equals(ns, s_XSLNameSpaceURL))
	{
		const XalanDOMString& 	localName =
			DOMServices::getLocalNameOfNode(node);

		const ElementKeysMapType::const_iterator		j =
						s_elementKeys.find(localName);

		if(j != s_elementKeys.end())
		{
			tok = (*j).second;
		}
	}
	else if(equals(ns, s_XSLT4JNameSpaceURL))
	{
		const XalanDOMString&	localName =
			DOMServices::getLocalNameOfNode(node);

		const ElementKeysMapType::const_iterator		j =
						s_XSLT4JElementKeys.find(localName);

		if(j != s_XSLT4JElementKeys.end())
		{
			tok = (*j).second;
		}
	}

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
			StylesheetExecutionContext&		executionContext,
			const XObject&					value)
{
	const XObject::eObjectType	type = value.getType();

	switch(type)
	{
	case XObject::eTypeBoolean:
	case XObject::eTypeNumber:
	case XObject::eTypeString:
		{
			const XalanDOMString&	s = value.str();

			characters(toCharArray(s), 0, length(s));
		}
		break;				

	case XObject::eTypeNodeSet:
		{
			XalanDOMString			s;

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
		outputResultTreeFragment(executionContext, value);
		break;

	case XObject::eTypeNull:
	case XObject::eTypeUnknown:
	case XObject::eUnknown:
	default:
		assert(0);
	}
}



const StylesheetRoot*
XSLTEngineImpl::getStylesheetRoot() const
{
	return m_stylesheetRoot;
}



void
XSLTEngineImpl::setStylesheetRoot(const StylesheetRoot*		theStylesheet)
{
	m_stylesheetRoot = theStylesheet;
}



void
XSLTEngineImpl::setExecutionContext(StylesheetExecutionContext*		theExecutionContext)
{
	m_executionContext = theExecutionContext;
}



//==========================================================
// SECTION: Diagnostic functions
//==========================================================

unsigned long
XSLTEngineImpl::getTraceListeners() const
{
	return m_traceListeners.size();
}



void
XSLTEngineImpl::addTraceListener(TraceListener* tl)
{
	if (tl != 0)
	{
		m_traceListeners.push_back(tl);
	}
}



void
XSLTEngineImpl::removeTraceListener(TraceListener*	tl)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::remove;
#endif

	const TraceListenerVectorType::iterator		i =
		remove(
			m_traceListeners.begin(),
			m_traceListeners.end(),
			tl);

	m_traceListeners.erase(i);
}



void
XSLTEngineImpl::fireGenerateEvent(const GenerateEvent&	ge)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(
		m_traceListeners.begin(),
		m_traceListeners.end(),
		TraceListener::TraceListenerGenerateFunctor(ge));
}



void
XSLTEngineImpl::fireSelectEvent(const SelectionEvent&	se)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(
		m_traceListeners.begin(),
		m_traceListeners.end(),
		TraceListener::TraceListenerSelectFunctor(se));
}



void
XSLTEngineImpl::fireTraceEvent(const TracerEvent& te)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(
		m_traceListeners.begin(),
		m_traceListeners.end(),
		TraceListener::TraceListenerTraceFunctor(te));
}



bool
XSLTEngineImpl::getTraceSelects() const
{
	return m_traceSelects;
}



void
XSLTEngineImpl::setTraceSelects(bool	b)
{
	m_traceSelects = b;
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
									   msg, 0, 0, 0);

		if(shouldThrow == true)
		{
			throw XSLTProcessorException(msg);
		}
	}
}



void
XSLTEngineImpl::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
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

	const XalanDOMChar* id = (0 == locator) ?
						0 : (0 == locator->getPublicId()) ?
					 locator->getPublicId() : locator->getSystemId();

	const bool	shouldThrow =
		m_problemListener->problem(
				ProblemListener::eXSLPROCESSOR, 
				classification,
				styleNode,
				sourceNode,
				msg, 
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
XSLTEngineImpl::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
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
XSLTEngineImpl::error(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
XSLTEngineImpl::pushTime(const void*	key) const
{
	if(0 != key)
	{
#if defined(XALAN_NO_MUTABLE)
		((XSLTEngineImpl*)this)->m_durationsTable.insert(DurationsTableMapType::value_type(key, clock()));
#else
		m_durationsTable.insert(DurationsTableMapType::value_type(key, clock()));
#endif
	}
}



clock_t
XSLTEngineImpl::popDuration(const void*		key) const
{
	clock_t 	clockTicksDuration = 0;

	if(0 != key)
	{
		const DurationsTableMapType::iterator	i =
#if defined(XALAN_NO_MUTABLE)
				((XSLTEngineImpl*)this)->m_durationsTable.find(key);
#else
				m_durationsTable.find(key);
#endif

		if (i != m_durationsTable.end())
		{
			clockTicksDuration = clock() - (*i).second;

#if defined(XALAN_NO_MUTABLE)
			((XSLTEngineImpl*)this)->m_durationsTable.erase(i);
#else
			m_durationsTable.erase(i);
#endif
		}
	}

	return clockTicksDuration;
}



void
XSLTEngineImpl::displayDuration(
			const XalanDOMString&	info,
			const void*				key) const
{
	if(0 != key)
	{
		const clock_t	theDuration = popDuration(key);

		if(0 != m_diagnosticsPrintWriter)
		{
			const double	millis = (double(theDuration) / CLOCKS_PER_SEC) * 1000.0L;

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



void
XSLTEngineImpl::diag(const char*	s) const
{
	diag(TranscodeFromLocalCodePage(s));
}



void
XSLTEngineImpl::setQuietConflictWarnings(bool	b)
{
	m_quietConflictWarnings = b;
}



void
XSLTEngineImpl::setDocumentLocator(const Locator* const		/* locator */)
{
	// Do nothing for now
}



void
XSLTEngineImpl::traceSelect(
			const XalanElement& 	theTemplate,
			const NodeRefListBase&	nl) const
{
	if (0 != m_diagnosticsPrintWriter)
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

		m_diagnosticsPrintWriter->println(msg);
	}
}



void
XSLTEngineImpl::startDocument()
{
	assert(getFormatterListener() != 0);
	assert(m_executionContext != 0);

	if (getHasPendingStartDocument() == false)
	{
		m_resultNamespacesStack.pushContext();

		setHasPendingStartDocument(true);

		setMustFlushPendingStartDocument(false);
	}
	else if (getMustFlushPendingStartDocument() == true)
	{
		getFormatterListener()->startDocument();

		if(getTraceListeners() > 0)
		{
			const GenerateEvent		ge(GenerateEvent::EVENTTYPE_STARTDOCUMENT);

			fireGenerateEvent(ge);
		}

		// Reset this, but leave getMustFlushPendingStartDocument() alone,
		// since it will still be needed.
		setHasPendingStartDocument(false);
	}
}



void
XSLTEngineImpl::endDocument()
{
	assert(getFormatterListener() != 0);
	assert(m_executionContext != 0);

	setMustFlushPendingStartDocument(true);

	flushPending();

	getFormatterListener()->endDocument();

	if(getTraceListeners() > 0)
	{
		const GenerateEvent		ge(GenerateEvent::EVENTTYPE_ENDDOCUMENT);

		fireGenerateEvent(ge);
	}

	m_resultNamespacesStack.popContext();

	assert(m_resultNamespacesStack.size() == 0);
}



void
XSLTEngineImpl::addResultNamespaceDecl(
			const XalanDOMString&	prefix, 
	        const XalanDOMString&	namespaceVal)
{
	m_resultNamespacesStack.addDeclaration(prefix, namespaceVal);
}



void
XSLTEngineImpl::addResultAttribute(
			AttributeListImpl&	attList,
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
{
	bool	fExcludeAttribute = false;

	if (equals(aname, DOMServices::s_XMLNamespace)) 
	{
		// OK, we're adding a default namespace declaration.  So see if the length
		// of the namespace is 0.  If it's not, go ahead and add the declaration.
		// If it's not, it means we're "turning off" the previous default
		// declaration.

		const XalanDOMString* const		currentDefaultNamespace =
					getNamespaceForPrefix(s_emptyString);

		// Note that we use an empty string for the prefix, instead of "xmlns", since the
		// prefix really is "".
		if (length(value) != 0)
		{
			if (currentDefaultNamespace != 0 &&
				equals(*currentDefaultNamespace, value) == true)
			{
				fExcludeAttribute = true;
			}
			else
			{
				addResultNamespaceDecl(s_emptyString, value);
			}
		}
		else
		{
			// OK, we're turning of the previous default namespace declaration.
			// Check to see if there is one, and if there isn't, don't add
			// the namespace declaration _and_ don't add the attribute.
			if (currentDefaultNamespace != 0 && length(*currentDefaultNamespace) != 0)
			{
				addResultNamespaceDecl(s_emptyString, value);
			}
			else
			{
				fExcludeAttribute = true;
			}
		}
	}
	else if (startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator) == true)
	{
		assert(m_executionContext != 0);

		StylesheetExecutionContext::GetAndReleaseCachedString	prefixGuard(*m_executionContext);

		XalanDOMString&		prefix = prefixGuard.get();

		prefix = substring(aname, DOMServices::s_XMLNamespaceWithSeparatorLength);

		addResultNamespaceDecl(prefix, value);
	}

	if (fExcludeAttribute == false)
	{
		attList.addAttribute(
			c_wstr(aname),
			c_wstr(Constants::ATTRTYPE_CDATA),
			c_wstr(value));
	}
}



bool
XSLTEngineImpl::pendingAttributesHasDefaultNS() const
{
	const AttributeListImpl&	thePendingAttributes =
		getPendingAttributes();

	const unsigned int	n = thePendingAttributes.getLength();

	for(unsigned int i = 0; i < n; i++)
	{
		if(equals(thePendingAttributes.getName(i),
				  DOMServices::s_XMLNamespace) == true)
		{
			return true;
		}
	}

	return false;
}



void
XSLTEngineImpl::flushPending()
{
	if(getHasPendingStartDocument() == true && 0 != length(getPendingElementName()))
	{
		assert(getFormatterListener() != 0);
		assert(m_executionContext != 0);

		if (m_stylesheetRoot->isOutputMethodSet() == false)
		{
			if (equalsIgnoreCase(getPendingElementName(),
								 Constants::ELEMNAME_HTML_STRING) == true &&
				pendingAttributesHasDefaultNS() == false)
			{
				if (getFormatterListener()->getOutputFormat() == FormatterListener::OUTPUT_METHOD_XML)
				{
					// Yuck!!! Ugly hack to switch to HTML on-the-fly.
					FormatterToXML* const	theFormatter =
#if defined(XALAN_OLD_STYLE_CASTS)
						(FormatterToXML*)getFormatterListener();
#else
						static_cast<FormatterToXML*>(getFormatterListener());
#endif

					setFormatterListenerImpl(
						m_executionContext->createFormatterToHTML(
							*theFormatter->getWriter(),
							theFormatter->getEncoding(),
							theFormatter->getMediaType(),
							theFormatter->getDoctypeSystem(),
							theFormatter->getDoctypePublic(),
							true,	// indent
							theFormatter->getIndent() > 0 ? theFormatter->getIndent() :
											StylesheetExecutionContext::eDefaultHTMLIndentAmount));
				}
			}
		}
	}

	XalanDOMString&		thePendingElementName = getPendingElementNameImpl();

	if(getHasPendingStartDocument() == true && getMustFlushPendingStartDocument() == true)
	{
		startDocument();
	}

	if(0 != length(thePendingElementName) && getMustFlushPendingStartDocument() == true)
	{
		assert(getFormatterListener() != 0);
		assert(m_executionContext != 0);

		m_cdataStack.push_back(isCDataResultElem(thePendingElementName) ? true : false);

		AttributeListImpl&	thePendingAttributes =
				getPendingAttributesImpl();

		getFormatterListener()->startElement(c_wstr(thePendingElementName), thePendingAttributes);

		if(getTraceListeners() > 0)
		{
			const GenerateEvent		ge(
				GenerateEvent::EVENTTYPE_STARTELEMENT,
				thePendingElementName,
				&thePendingAttributes);

			fireGenerateEvent(ge);
		}

		thePendingAttributes.clear();

		clear(thePendingElementName);
	}
}



void
XSLTEngineImpl::startElement(const XMLCh* const	name)
{
	assert(getFormatterListener() != 0);
	assert(name != 0);

	flushPending();

	m_resultNamespacesStack.pushContext();

	setPendingElementName(name);

	setMustFlushPendingStartDocument(true);
}



void
XSLTEngineImpl::startElement(
			const XMLCh* const	name,
			AttributeList&		atts)
{
	assert(getFormatterListener() != 0);
	assert(name != 0);

	flushPending();

	const unsigned int	nAtts = atts.getLength();

	assert(m_outputContextStack.size() > 0);

	AttributeListImpl&	thePendingAttributes =
		getPendingAttributesImpl();

	thePendingAttributes.clear();

	for(unsigned int i = 0; i < nAtts; i++)
	{
		thePendingAttributes.addAttribute(
			atts.getName(i),
			atts.getType(i),
			atts.getValue(i));
	}

	m_resultNamespacesStack.pushContext();

	setPendingElementName(name);
}



void
XSLTEngineImpl::endElement(const XMLCh* const 	name)
{
	assert(getFormatterListener() != 0);
	assert(name != 0);

	flushPending();

	getFormatterListener()->endElement(name);

	if(getTraceListeners() > 0)
	{
		const GenerateEvent		ge(GenerateEvent::EVENTTYPE_ENDELEMENT, name);

		fireGenerateEvent(ge);
	}

	m_resultNamespacesStack.popContext();

	if(m_stylesheetRoot->hasCDATASectionElements() == true)
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
	assert(getFormatterListener() != 0);
	assert(ch != 0);

	doFlushPending();

	if(generateCDATASection() == true)
	{
		getFormatterListener()->cdata(ch + start, length);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(ch, start, length, true);
		}
	}
	else
	{
		getFormatterListener()->characters(ch + start, length);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(ch, start, length, false);
		}
	}
}



void
XSLTEngineImpl::characters(const XalanNode&		node)
{
	assert(getFormatterListener() != 0);

	doFlushPending();

	if(generateCDATASection() == true)
	{
		DOMServices::getNodeData(node, *getFormatterListener(), &FormatterListener::cdata);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(node, true);
		}
	}
	else
	{
		DOMServices::getNodeData(node, *getFormatterListener(), &FormatterListener::characters);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(node, false);
		}
	}
}



void
XSLTEngineImpl::characters(const XObjectPtr&	xobject)
{
	assert(getFormatterListener() != 0);
	assert(xobject.null() == false);

	doFlushPending();

	if(generateCDATASection() == true)
	{
		xobject->str(*getFormatterListener(), &FormatterListener::cdata);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(xobject, true);
		}
	}
	else
	{
		xobject->str(*getFormatterListener(), &FormatterListener::characters);

		if(getTraceListeners() > 0)
		{
			fireCharacterGenerateEvent(xobject, false);
		}
	}
}



void 
XSLTEngineImpl::charactersRaw(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length)
{
	assert(ch != 0);

	doFlushPending();

	getFormatterListener()->charactersRaw(ch, length);

	if(getTraceListeners() > 0)
	{
		fireCharacterGenerateEvent(ch, start, length, false);
	}
}



void
XSLTEngineImpl::charactersRaw(const XalanNode&	node)
{
	doFlushPending();

	DOMServices::getNodeData(node, *getFormatterListener(), &FormatterListener::charactersRaw);

	if(getTraceListeners() > 0)
	{
		fireCharacterGenerateEvent(node, false);
	}
}



void
XSLTEngineImpl::charactersRaw(const XObjectPtr&		xobject)
{
	doFlushPending();

	xobject->str(*getFormatterListener(), &FormatterListener::charactersRaw);

	if(getTraceListeners() > 0)
	{
		fireCharacterGenerateEvent(xobject, false);
	}
}



void
XSLTEngineImpl::resetDocument()
{
	assert(getFormatterListener() != 0);

	flushPending();
	
	getFormatterListener()->resetDocument();
}



void
XSLTEngineImpl::ignorableWhitespace(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	assert(getFormatterListener() != 0);
	assert(ch != 0);

	doFlushPending();

	getFormatterListener()->ignorableWhitespace(ch, length);

	if(getTraceListeners() > 0)
	{
		GenerateEvent ge(GenerateEvent::EVENTTYPE_IGNORABLEWHITESPACE,
					ch, 0, length);

		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	assert(getFormatterListener() != 0);
	assert(target != 0);
	assert(data != 0);

	doFlushPending();

	getFormatterListener()->processingInstruction(target, data);

	if(getTraceListeners() > 0)
	{
		GenerateEvent ge(
				GenerateEvent::EVENTTYPE_PI,
                target,
				data);

		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::comment(const XMLCh* const	data)
{
	assert(getFormatterListener() != 0);
	assert(data != 0);

	doFlushPending();

	getFormatterListener()->comment(data);

	if(getTraceListeners() > 0)
	{
		GenerateEvent ge(GenerateEvent::EVENTTYPE_COMMENT,
                                          data);
		fireGenerateEvent(ge);
	}
}


void
XSLTEngineImpl::entityReference(const XMLCh* const	name)
{
	assert(getFormatterListener() != 0);
	assert(name != 0);

	doFlushPending();

	getFormatterListener()->entityReference(name);

	if(getTraceListeners() > 0)
	{
		GenerateEvent ge(GenerateEvent::EVENTTYPE_ENTITYREF,
                                          name);

		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::cdata(
			const XMLCh* const	ch,
			const unsigned int 	start,
			const unsigned int 	length)
{
	assert(getFormatterListener() != 0);
	assert(ch != 0);

	setMustFlushPendingStartDocument(true);

	flushPending();

	if(m_stylesheetRoot->hasCDATASectionElements() == true &&
	   0 != m_cdataStack.size())
	{
		getFormatterListener()->cdata(ch, length);

		if(getTraceListeners() > 0)
		{
			GenerateEvent ge(GenerateEvent::EVENTTYPE_CDATA, ch, start,
					length);

			fireGenerateEvent(ge);
		}
	}
	else
	{
		getFormatterListener()->characters(ch, length);

		if(getTraceListeners() > 0)
		{
			GenerateEvent ge(GenerateEvent::EVENTTYPE_CHARACTERS, ch,
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
	const XalanNode::NodeType	theType = node.getNodeType();

	switch(theType)
	{
	case XalanNode::TEXT_NODE:
		{
			bool	stripWhiteSpace = false;

			// If stripWhiteSpace is false, then take this as an override and 
			// just preserve the space, otherwise use the XSL whitespace rules.
			if(!overrideStrip)
			{
				stripWhiteSpace = isLiteral ? true : false;
			  // was: stripWhiteSpace = isLiteral ? true : shouldStripSourceNode(node);
			}

			const XalanText& 	tx =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanText&)node;
#else
				static_cast<const XalanText&>(node);
#endif

			if(stripWhiteSpace == false || tx.isIgnorableWhitespace() == false)
			{
				assert(tx.getParentNode() == 0 ||
					   tx.getParentNode()->getNodeType() != XalanNode::DOCUMENT_NODE);

				const XalanDOMString&	data = tx.getData();

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
					if(tx.isIgnorableWhitespace())
					{
						ignorableWhitespace(toCharArray(data), length(data));
					}
					else
					{
						characters(toCharArray(data), 0, length(data));
					}
				}
			}			
		}
		break;

	case XalanNode::ELEMENT_NODE:
		{
			startElement(c_wstr(DOMServices::getNameOfNode(node)));

			if(shouldCloneAttributes == true)
			{
				copyAttributesToAttList(&node,
										m_stylesheetRoot,
#if defined(XALAN_OLD_STYLE_CASTS)
										(const XalanElement&)node,
#else
										static_cast<const XalanElement&>(node),
#endif
										getPendingAttributesImpl());

				copyNamespaceAttributes(node);
			}
		}
		break;

	case XalanNode::CDATA_SECTION_NODE:
		{
			const XalanCDATASection& 	theCDATA =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanCDATASection&)node;
#else
				static_cast<const XalanCDATASection&>(node);
#endif

			const XalanDOMString& 	data = theCDATA.getData();

			cdata(toCharArray(data), 0, length(data));
		}
		break;

	case XalanNode::ATTRIBUTE_NODE:
		{
			const XalanAttr& 	attr =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanAttr&)node;
#else
				static_cast<const XalanAttr&>(node);
#endif
			addResultAttribute(getPendingAttributesImpl(),
							   DOMServices::getNameOfNode(attr),
							   attr.getValue());
		}
		break;

	case XalanNode::COMMENT_NODE:
		{
			const XalanComment&		theComment =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanComment&)node;
#else
				static_cast<const XalanComment&>(node);
#endif

			const XalanDOMString& 	theData = theComment.getData();

			comment(c_wstr(theData));
		}
		break;

	case XalanNode::DOCUMENT_FRAGMENT_NODE:
		{
			error("No clone of a document fragment!");
		}
		break;
	
	case XalanNode::ENTITY_REFERENCE_NODE:
		{
			const XalanDOMString &	theName = node.getNodeName();

			entityReference(c_wstr(theName));
		}
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		{
			const XalanProcessingInstruction&	pi =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanProcessingInstruction&)node;
#else
				static_cast<const XalanProcessingInstruction&>(node);
#endif

			const XalanDOMString& 	theTarget = pi.getTarget();
			const XalanDOMString& 	theData = pi.getData();

			processingInstruction(c_wstr(theTarget),
								  c_wstr(theData));
		}
		break;

	// Can't really do this, but we won't throw an error so that copy-of will
	// work
	case XalanNode::DOCUMENT_NODE:
	case XalanNode::DOCUMENT_TYPE_NODE:
	break;

	default:
		error("Cannot create item in result tree: " + node.getNodeName());
	break;

	}
}



void
XSLTEngineImpl::outputResultTreeFragment(
			StylesheetExecutionContext&		executionContext,
			const XObject&					theTree)
{
	const ResultTreeFragBase&	docFrag = theTree.rtree(executionContext);

	const XalanNodeList* const	nl = docFrag.getChildNodes();
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
XSLTEngineImpl::isCDataResultElem(const XalanDOMString&		elementName) const
{
	assert(m_executionContext != 0);

	if(m_stylesheetRoot->hasCDATASectionElements() == false)
	{
		return false;
	}
	else
	{
		bool	fResult = false;

		const unsigned int	indexOfNSSep = indexOf(elementName, XalanUnicode::charColon);

		if(indexOfNSSep == length(elementName))
		{
			fResult =
				m_stylesheetRoot->isCDATASectionElementName(QNameByReference(s_emptyString, elementName));
		}
		else
		{
			typedef StylesheetExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

			GetAndReleaseCachedString	elemLocalNameGuard(*m_executionContext);
			GetAndReleaseCachedString	prefixGuard(*m_executionContext);

			XalanDOMString&		elemLocalName = elemLocalNameGuard.get();
			XalanDOMString&		prefix = prefixGuard.get();

			substring(elementName, prefix, 0, indexOfNSSep);
			substring(elementName, elemLocalName, indexOfNSSep + 1);

			if(equals(prefix, DOMServices::s_XMLString))
			{
				fResult =
					m_stylesheetRoot->isCDATASectionElementName(QNameByReference(DOMServices::s_XMLNamespaceURI, elementName));
			}
			else
			{
				const XalanDOMString* const		elemNS =
					getResultNamespaceForPrefix(prefix);

				if(elemNS == 0)
				{
					error("Prefix must resolve to a namespace: " + prefix);
				}
				else
				{
					fResult =
						m_stylesheetRoot->isCDATASectionElementName(QNameByReference(*elemNS, elementName));
				}
			}
		}

		return fResult;
	}
}
	


// $$$ ToDo: This should not be here!!!!
bool
XSLTEngineImpl::qnameEqualsResultElemName(
			const QName&			qname,
			const XalanDOMString&	elementName) const
{
	const XalanDOMString*	elemNS = 0;
	XalanDOMString			elemLocalName;

	const unsigned int	indexOfNSSep = indexOf(elementName, XalanUnicode::charColon);

	if(indexOfNSSep < length(elementName))
	{
		const XalanDOMString	prefix = substring(elementName, 0, indexOfNSSep);

		if(equals(prefix, DOMServices::s_XMLString))
		{
			elemNS = &DOMServices::s_XMLNamespaceURI;
		}
		else
		{
			elemNS = getResultNamespaceForPrefix(prefix);
		}

		if(elemNS == 0)
		{
			error("Prefix must resolve to a namespace: " + prefix);
		}

		elemLocalName =  substring(elementName, indexOfNSSep+1);
	}
	else
	{
		elemLocalName = elementName;
	}

	assert(elemNS != 0);

	return qname.equals(QNameByReference(*elemNS, elemLocalName));
}



const XalanDOMString*
XSLTEngineImpl::getResultNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	return m_resultNamespacesStack.getNamespaceForPrefix(prefix);
}
  


const XalanDOMString*
XSLTEngineImpl::getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const
{
	return m_resultNamespacesStack.getPrefixForNamespace(theNamespace);
}



inline bool
isPrefixUsed(
			const XalanDOMString&	thePrefix,
			unsigned int			thePrefixLength,
			const XalanDOMChar*		theName,
			unsigned int			theNameLength)
{
	assert(thePrefixLength != 0);

	// The name must be greater than the length of the prefix + 1, since
	// there must be a ':' to separate the prefix from the local part...
	if (theNameLength <= thePrefixLength + 1)
	{
		return false;
	}
	else
	{
		assert(theName != 0);

		const unsigned int	theIndex = indexOf(
			theName,
			XalanUnicode::charColon);

		// OK, if the index of the ':' is the same as the length of the prefix,
		// and theElementName starts with thePrefix, then the prefix is in use.
		if (theIndex == thePrefixLength &&
			startsWith(theName, thePrefix) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



inline bool
isPrefixUsed(
			const XalanDOMString&	thePrefix,
			unsigned int			thePrefixLength,
			const XalanDOMString&	theName)
{
	return isPrefixUsed(thePrefix, thePrefixLength, c_wstr(theName), length(theName));
}



inline bool
isPrefixUsedOrDeclared(
			const XalanDOMString&	thePrefix,
			unsigned int			thePrefixLength,
			const XalanDOMChar*		theName,
			unsigned int			theNameLength)
{
	if (isPrefixUsed(thePrefix, thePrefixLength, theName, theNameLength) == true)
	{
		return true;
	}
	else
	{
		const unsigned int	theDeclarationLength =
			thePrefixLength + DOMServices::s_XMLNamespaceWithSeparatorLength;

		// If this is a namespace declaration for this prefix, then all of
		// these conditions must be true...
		if (theDeclarationLength == theNameLength &&
			startsWith(theName, DOMServices::s_XMLNamespaceWithSeparator) == true &&
			endsWith(theName, c_wstr(thePrefix)) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



inline bool
isPendingAttributePrefix(
			const AttributeList&		thePendingAttributes,
			const XalanDOMString&		thePrefix,
			unsigned int				thePrefixLength)
{
	const unsigned int	thePendingAttributesCount =
				thePendingAttributes.getLength();

	if (thePendingAttributesCount == 0)
	{
		// No attributes, no problem...
		return false;
	}
	else
	{
		bool	fResult = false;

		// Check each attribute...
		for (unsigned int i = 0; i < thePendingAttributesCount; ++i)
		{
			const XalanDOMChar* const	thePendingAttributeName =
							thePendingAttributes.getName(i);
			assert(thePendingAttributeName != 0);

			if (isPrefixUsedOrDeclared(
					thePrefix,
					thePrefixLength,
					thePendingAttributeName,
					length(thePendingAttributeName)) == true)
			{
				fResult = true;

				break;
			}
		}

		return fResult;
	}
}



bool
XSLTEngineImpl::isPendingResultPrefix(const XalanDOMString&		thePrefix) const
{
	const unsigned int	thePrefixLength = length(thePrefix);
	assert(thePrefixLength > 0);

	// The element name must be greater than the length of the prefix + 1, since
	// there must be a ':' to separate the prefix from the local part...
	if (isPrefixUsed(thePrefix, thePrefixLength, getPendingElementName()) == true)
	{
		return true;
	}
	else
	{
		// The element is not using the prefix, so check the
		// pending attributes...
		return isPendingAttributePrefix(
						getPendingAttributes(),
						thePrefix,
						thePrefixLength);
	}
}



void
XSLTEngineImpl::addResultNamespace(
			const XalanNode&	theNode,
			AttributeListImpl&	thePendingAttributes,
			bool				fOnlyIfPrefixNotPresent)
{
	assert(theNode.getNodeType() == XalanNode::ATTRIBUTE_NODE);

	const XalanDOMString& 	aname = theNode.getNodeName();

	const bool	isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

	const XalanDOMString 	prefix = isPrefix == true ?
			substring(aname, DOMServices::s_XMLNamespaceWithSeparatorLength) : XalanDOMString();

	if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
	{
		if (fOnlyIfPrefixNotPresent == true)
		{
			if (m_resultNamespacesStack.prefixIsPresentLocal(prefix) == false)
			{
				const XalanDOMString* const 	desturi = getResultNamespaceForPrefix(prefix);
				const XalanDOMString&			srcURI = theNode.getNodeValue();

				if(desturi == 0 || equals(srcURI, *desturi) == false)
				{
					addResultAttribute(thePendingAttributes, aname, srcURI);
				}
			}
		}
		else
		{
			const XalanDOMString* const 	desturi = getResultNamespaceForPrefix(prefix);
			const XalanDOMString&			srcURI = theNode.getNodeValue();

			if(desturi == 0 || equals(srcURI, *desturi) == false)
			{
				addResultAttribute(thePendingAttributes, aname, srcURI);
			}
		}
	}
}



void
XSLTEngineImpl::copyNamespaceAttributes(const XalanNode&	src) 
{
	int type;

	const XalanNode*	parent = &src;

	while (parent != 0 &&
		   (type = parent->getNodeType()) == XalanNode::ELEMENT_NODE) 
	{
		const XalanNamedNodeMap* const	nnm =
				parent->getAttributes();
		assert(nnm != 0);

		const unsigned int	nAttrs = nnm->getLength();

		assert(m_outputContextStack.size() > 0);

		AttributeListImpl&	thePendingAttributes =
				getPendingAttributesImpl();

		for (unsigned int i = 0;  i < nAttrs; i++) 
		{
			const XalanNode* const	attr = nnm->item(i);
			assert(attr != 0);

			addResultNamespace(*attr, thePendingAttributes, true);
		}

		parent = parent->getParentNode();
	}
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XPathExecutionContext&	executionContext)
{
	assert(executionContext.getPrefixResolver() != 0);

	XPath* const	theXPath = m_xpathFactory.create();

	XPathGuard	theGuard(m_xpathFactory,
						 theXPath);

    m_xpathProcessor->initXPath(*theXPath,
								str,
								*executionContext.getPrefixResolver());

    return theXPath->execute(executionContext.getCurrentNode(),
							 *executionContext.getPrefixResolver(),
							 executionContext);
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext)
{
	XPath* const	theXPath = m_xpathFactory.create();

	XPathGuard	theGuard(m_xpathFactory,
						 theXPath);

    m_xpathProcessor->initXPath(*theXPath,
								str,
								prefixResolver);

    return theXPath->execute(contextNode, prefixResolver, executionContext);
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		prefixResolver,
			XPathExecutionContext&	executionContext)
{
	ElementPrefixResolverProxy	theProxy(&prefixResolver,
										 m_xpathEnvSupport,
										 m_domSupport);

	return evalXPathStr(str, contextNode, theProxy, executionContext);
}




/**
 * Create and initialize an xpath and return it.
 */
const XPath*
XSLTEngineImpl::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initMatchPattern(*xpath, str, resolver);

	return xpath;
}



void
XSLTEngineImpl::returnXPath(const XPath*	xpath)
{
	m_xpathFactory.returnObject(xpath);
}



void
XSLTEngineImpl::copyAttributeToTarget(
			const XalanAttr&		attr,
			XalanNode*				/* contextNode */,
			const Stylesheet* 		/* stylesheetTree */,
			AttributeListImpl&		attrList,
			const XalanElement& 	/* namespaceContext */)
{
	const XalanDOMString& 	attrName = attr.getName();

	const XalanDOMString&	attrValue = attr.getValue();

	// TODO: Find out about empty attribute template expression handling.
	if(0 != length(attrValue))
	{
		addResultAttribute(attrList, attrName, attrValue);
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

	if (attributes != 0)
	{
		const unsigned int	nAttributes = attributes->getLength();

		for(unsigned int i = 0; i < nAttributes; ++i)  
		{	
			const XalanAttr* const 	attr =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanAttr*)attributes->item(i);
#else
				static_cast<const XalanAttr*>(attributes->item(i));
#endif
			assert(attr != 0);

			copyAttributeToTarget(
					*attr,
					contextNode,
					stylesheetTree, 
					attList,
					templateChild);
		}
	}
}



bool
XSLTEngineImpl::shouldStripSourceNode(
			XPathExecutionContext&	executionContext,
			const XalanNode&		textNode) const
{
	if (m_stylesheetRoot == 0)
	{
		return false;
	}
	else
	{
		bool	strip = false; // return value

		if((m_stylesheetRoot->getWhitespacePreservingElements().size() > 0 ||
			m_stylesheetRoot->getWhitespaceStrippingElements().size() > 0))
		{
			const XalanNode::NodeType	type = textNode.getNodeType();

			if(XalanNode::TEXT_NODE == type || XalanNode::CDATA_SECTION_NODE == type)
			{
				const XalanText& 	theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const XalanText&)textNode;
#else
						static_cast<const XalanText&>(textNode);
#endif

				if(!theTextNode.isIgnorableWhitespace())
				{
					const XalanDOMString&	data = theTextNode.getData();

					if(0 == length(data))
					{
						return true;
					}
					else if(!isXMLWhitespace(data))
					{
						return false;
					}
				}

				XalanNode*	parent = DOMServices::getParentOfNode(textNode);

				while(0 != parent)
				{
					if(parent->getNodeType() == XalanNode::ELEMENT_NODE)
					{
						const XalanElement*	const	parentElem =
#if defined(XALAN_OLD_STYLE_CASTS)
							(const XalanElement*)parent;
#else
							static_cast<const XalanElement*>(parent);
#endif

						double	highPreserveScore = XPath::s_MatchScoreNone;
						double	highStripScore = XPath::s_MatchScoreNone;

						ElementPrefixResolverProxy	theProxy(parentElem, m_xpathEnvSupport, m_domSupport);

						{
							// $$$ ToDo:  All of this should be moved into a member of
							// Stylesheet, so as not to expose these two data members...
							typedef Stylesheet::XPathVectorType		XPathVectorType;

							const XPathVectorType&	theElements =
								m_stylesheetRoot->getWhitespacePreservingElements();

							const XPathVectorType::size_type	nTests =
								theElements.size();

							for(XPathVectorType::size_type i = 0; i < nTests; i++)
							{
								const XPath* const	matchPat = theElements[i];
								assert(matchPat != 0);

								const double	score = matchPat->getMatchScore(parent, theProxy, executionContext);

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

							for(XPathVectorType::size_type i = 0; i < nTests; i++)
							{
								const XPath* const	matchPat =
									theElements[i];
								assert(matchPat != 0);

								const double	score = matchPat->getMatchScore(parent, theProxy, executionContext);

								if(score > highStripScore)
									highStripScore = score;
							}
						}

						if(highPreserveScore > XPath::s_MatchScoreNone ||
						   highStripScore > XPath::s_MatchScoreNone)
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
		}

		return strip;
	}
}



XMLParserLiaison&
XSLTEngineImpl::getXMLParserLiaison() const
{
	return m_parserLiaison;
}



const XalanDOMString
XSLTEngineImpl::getUniqueNamespaceValue()
{
	XalanDOMString	theResult;

	getUniqueNamespaceValue(theResult);

	return theResult;
}



void
XSLTEngineImpl::getUniqueNamespaceValue(XalanDOMString&		theValue)
{
	append(theValue, s_uniqueNamespacePrefix);

	UnsignedLongToDOMString(m_uniqueNSValue++, theValue);
}



XalanDocument*
XSLTEngineImpl::getDOMFactory() const
{
	if(m_domResultTreeFactory == 0)
	{
#if defined(XALAN_NO_MUTABLE)
		((XSLTEngineImpl*)this)->m_domResultTreeFactory = m_parserLiaison.createDOMFactory();
#else
		m_domResultTreeFactory = m_parserLiaison.createDOMFactory();
#endif
	}

	return m_domResultTreeFactory;
}



void
XSLTEngineImpl::setStylesheetParam(
			const XalanDOMString&	theName,
			const XalanDOMString&	expression)
{
	const QNameByValue	qname(theName, 0, m_xpathEnvSupport, m_domSupport);

	m_topLevelParams.push_back(ParamVectorType::value_type(qname, expression));
}



void
XSLTEngineImpl::setStylesheetParam(
			const XalanDOMString&	theName,
			XObjectPtr				theValue)
{
	const QNameByValue	qname(theName, 0, m_xpathEnvSupport, m_domSupport);

	m_topLevelParams.push_back(ParamVectorType::value_type(qname, theValue));
}



void
XSLTEngineImpl::resolveTopLevelParams(StylesheetExecutionContext&	executionContext)
{
	executionContext.pushTopLevelVariables(m_topLevelParams);
}



FormatterListener*
XSLTEngineImpl::getFormatterListener() const
{
	return getFormatterListenerImpl();
}



void
XSLTEngineImpl::setFormatterListener(FormatterListener*		flistener)
{
	if (getHasPendingStartDocument() == true && getFormatterListener() != 0)
	{
		setMustFlushPendingStartDocument(true);

		flushPending();
	}

	setFormatterListenerImpl(flistener);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
			const XalanNode&	theNode,
			bool				isCDATA)
{
	fireCharacterGenerateEvent(DOMServices::getNodeData(theNode), isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
			const XObjectPtr&	theXObject,
			bool				isCDATA)
{
	fireCharacterGenerateEvent(theXObject->str(), isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
			const XalanDOMString&	theString,
			bool					isCDATA)
{
	fireCharacterGenerateEvent(c_wstr(theString), 0, length(theString), isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length,
			bool			isCDATA)
{
	const GenerateEvent		ge(
		isCDATA == true ? GenerateEvent::EVENTTYPE_CDATA : GenerateEvent::EVENTTYPE_CHARACTERS,
		ch,
		start,
		length);

	fireGenerateEvent(ge);
}



void
XSLTEngineImpl::installFunctions()
{
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("current")), FunctionCurrent());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("document")), FunctionDocument());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("element-available")), FunctionElementAvailable());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("function-available")), FunctionFunctionAvailable());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("format-number")), FunctionFormatNumber());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("generate-id")), FunctionGenerateID());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("key")), FunctionKey());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("system-property")), FunctionSystemProperty());
	XPath::installFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("unparsed-entity-uri")), FunctionUnparsedEntityURI());
}



void
XSLTEngineImpl::uninstallFunctions()
{
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("current")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("document")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("element-available")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("function-available")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("format-number")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("generate-id")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("key")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("system-property")));
	XPath::uninstallFunction(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("unparsed-entity-uri")));
}



void
XSLTEngineImpl::initializeAttributeKeysTable(AttributeKeysMapType&	theAttributeKeys)
{
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
	theAttributeKeys[Constants::ATTRNAME_OUTPUT_OMITXMLDECL] = Constants::TATTRNAME_OUTPUT_OMITXMLDECL;
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
}



void
XSLTEngineImpl::initializeElementKeysTable(ElementKeysMapType&	theElementKeys)
{
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

	theElementKeys[Constants::ELEMNAME_DECIMALFORMAT_STRING] = Constants::ELEMNAME_DECIMALFORMAT;
	theElementKeys[Constants::ELEMNAME_NSALIAS_STRING] = Constants::ELEMNAME_NSALIAS;
}



void
XSLTEngineImpl::initializeXSLT4JElementKeys(ElementKeysMapType&		theElementKeys)
{
	theElementKeys[Constants::ELEMNAME_COMPONENT_STRING] = Constants::ELEMNAME_COMPONENT;
	theElementKeys[Constants::ELEMNAME_SCRIPT_STRING] = Constants::ELEMNAME_SCRIPT;
}



static XalanDOMString							s_XSLNameSpaceURL;

static XalanDOMString							s_XSLT4JNameSpaceURL;

static XalanDOMString							s_uniqueNamespacePrefix;

static XSLTEngineImpl::AttributeKeysMapType		s_attributeKeys;

static XSLTEngineImpl::ElementKeysMapType		s_elementKeys;

static XSLTEngineImpl::ElementKeysMapType		s_XSLT4JElementKeys;



const double			XSLTEngineImpl::s_XSLTVerSupported(1.0);

const XalanDOMString&	XSLTEngineImpl::s_XSLNameSpaceURL = ::s_XSLNameSpaceURL;

const XalanDOMString&	XSLTEngineImpl::s_XSLT4JNameSpaceURL = ::s_XSLT4JNameSpaceURL;

const XalanDOMString&	XSLTEngineImpl::s_uniqueNamespacePrefix = ::s_uniqueNamespacePrefix;


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
const bool										XSLTEngineImpl::s_resolveContentsEarly = true;

const XSLTEngineImpl::AttributeKeysMapType&		XSLTEngineImpl::s_attributeKeys = ::s_attributeKeys;

const XSLTEngineImpl::ElementKeysMapType&		XSLTEngineImpl::s_elementKeys = ::s_elementKeys;

const XSLTEngineImpl::ElementKeysMapType&		XSLTEngineImpl::s_XSLT4JElementKeys = ::s_XSLT4JElementKeys;



void
XSLTEngineImpl::initialize()
{
	::s_XSLNameSpaceURL = XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform");

	::s_XSLT4JNameSpaceURL = XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xslt");

	::s_uniqueNamespacePrefix = XALAN_STATIC_UCODE_STRING("ns");

	installFunctions();

	initializeAttributeKeysTable(::s_attributeKeys);

	initializeElementKeysTable(::s_elementKeys);

	initializeXSLT4JElementKeys(::s_XSLT4JElementKeys);
}



void
XSLTEngineImpl::terminate()
{
	ElementKeysMapType().swap(::s_XSLT4JElementKeys);

	ElementKeysMapType().swap(::s_elementKeys);

	AttributeKeysMapType().swap(::s_attributeKeys);

	uninstallFunctions();

	releaseMemory(::s_uniqueNamespacePrefix);

	releaseMemory(::s_XSLT4JNameSpaceURL);

	releaseMemory(::s_XSLNameSpaceURL);
}
