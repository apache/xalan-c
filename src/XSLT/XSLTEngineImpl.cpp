/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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



#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/Locator.hpp>



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
#include <PlatformSupport/XalanLocator.hpp>
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
#include <XPath/XalanQNameByReference.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>



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
	PrefixResolver(),
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
	m_quietConflictWarnings(true),
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
	m_dummyAttributesList(),
	m_scratchString(),
	m_attributeNamesVisited(),
	m_attributeNamesVisitedEnd(m_attributeNamesVisited.end()),
	m_hasStripOrPreserveSpace(false),
	m_hasCDATASectionElements(false)
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

	m_attributeNamesVisited.clear();

	m_hasStripOrPreserveSpace = false;
	m_hasCDATASectionElements = false;
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

	if(m_diagnosticsPrintWriter != 0)
	{
		pushTime(&totalTimeID);
	}

	XalanNode*	sourceTree = getSourceTreeFromInput(inputSource);

	m_stylesheetRoot = processStylesheet(stylesheetSource, constructionContext);

	if(0 != sourceTree && m_stylesheetRoot == 0)
	{
		// Didn't get a stylesheet from the input source, so look for a
		// stylesheet processing instruction...

		// The PI must be a child of the document...
		const XalanNode*	child = sourceTree->getFirstChild();

		XalanDOMString		theCurrentToken;
		XalanDOMString		theStylesheetURI;

		bool			isOK = false;

		while(child != 0 && isOK == false && theStylesheetURI.empty() == true)
		{
			if(XalanNode::PROCESSING_INSTRUCTION_NODE == child->getNodeType())
			{
				const XalanDOMString&	nodeName = child->getNodeName();

				if(equals(nodeName, s_stylesheetNodeName))
				{
					StringTokenizer 	tokenizer(child->getNodeValue(), s_piTokenizerString);

					while(tokenizer.hasMoreTokens() == true && (isOK == false || theStylesheetURI.empty() == true))
					{
						tokenizer.nextToken(theCurrentToken);

						if(equals(theCurrentToken, s_typeString))
						{
							tokenizer.nextToken(theCurrentToken);


							const XalanDOMString::size_type		theLength =
									theCurrentToken.length();

							if (theLength > 2)
							{
								theCurrentToken.erase(theLength - 1, 1);
								theCurrentToken.erase(0, 1);

								if(equals(theCurrentToken, s_typeValueString1) ||
									equals(theCurrentToken, s_typeValueString2) ||
									equals(theCurrentToken, s_typeValueString3) ||
									equals(theCurrentToken, s_typeValueString4))
								{
									isOK = true;
								}
							}
						}
						else if(equals(theCurrentToken, s_hrefString))
						{
							tokenizer.nextToken(theCurrentToken);

							const XalanDOMString::size_type		theLength =
									theCurrentToken.length();

							if (theLength > 2)
							{
								// Trim of the starting and trailing delimiters...
								theStylesheetURI.assign(theCurrentToken, 1, theLength - 2);
							}
						}
					}
				}
			}

			child = child->getNextSibling();
		}

		if (isOK == true && theStylesheetURI.empty() == false)
		{
			const XalanDOMChar* const	pxch = inputSource.getSystemId();

			const XalanDOMString		sysid(pxch == 0 ? &s_dummyString : pxch); 

			getStylesheetFromPIURL(
					theStylesheetURI,
					*sourceTree,
					sysid,
					true,
					constructionContext);
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

		if (theFormatter != 0 && theFormatter->getPrefixResolver() == 0)
		{
			theFormatter->setPrefixResolver(this);
		}

		m_hasStripOrPreserveSpace = m_stylesheetRoot->getWhitespacePreservingElements().empty() == false ||
			m_stylesheetRoot->getWhitespaceStrippingElements().empty() == false;

		m_hasCDATASectionElements = m_stylesheetRoot->hasCDATASectionElements();

		m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);
	}

	if(m_diagnosticsPrintWriter != 0)
	{
		displayDuration(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Total time")), &totalTimeID);
	}
}



void
XSLTEngineImpl::process(
			const XSLTInputSource&			inputSource, 
	        XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext)
{
	bool	totalTimeID = true;

	if(m_diagnosticsPrintWriter != 0)
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

		FormatterListener* const	theFormatter =
				outputTarget.getDocumentHandler();

		if (theFormatter != 0 && theFormatter->getPrefixResolver() == 0)
		{
			theFormatter->setPrefixResolver(this);
		}

		m_hasStripOrPreserveSpace = m_stylesheetRoot->getWhitespacePreservingElements().empty() == false ||
			m_stylesheetRoot->getWhitespaceStrippingElements().empty() == false;

		m_hasCDATASectionElements = m_stylesheetRoot->hasCDATASectionElements();

		m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);
	}

	if(m_diagnosticsPrintWriter != 0)
	{
		displayDuration(StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Total time")), &totalTimeID);
	}
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
			const XalanDOMString&			xsldocURLString,
			StylesheetConstructionContext&	constructionContext)
{
	const XSLTInputSource	input(c_wstr(xsldocURLString));

	return processStylesheet(input, constructionContext);
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

			if(m_diagnosticsPrintWriter != 0)
			{
				diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") + xslIdentifier + XALAN_STATIC_UCODE_STRING(" =========="));

				pushTime(&xslIdentifier);
			}

			m_parserLiaison.parseXMLStream(stylesheetSource,
										   stylesheetProcessor);

			if(m_diagnosticsPrintWriter != 0)
			{
				displayDuration(XALAN_STATIC_UCODE_STRING("Parse of ") + xslIdentifier, &xslIdentifier);
			}
		}

		theStylesheet->postConstruction(constructionContext);
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

		if(m_diagnosticsPrintWriter != 0)
		{
			// In case we have a fragment identifier, go ahead and 
			// try to parse the XML here.
			diag(XALAN_STATIC_UCODE_STRING("========= Parsing ") +
						xmlIdentifier +
						XALAN_STATIC_UCODE_STRING(" =========="));

			pushTime(&xmlIdentifier);
		}

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
		{
			displayDuration(
				XALAN_STATIC_UCODE_STRING("Parse of ") + xmlIdentifier,
				&xmlIdentifier);
		}

		m_xpathEnvSupport.setSourceDocument(xmlIdentifier, theDocument);

		sourceTree = theDocument;
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
				const XSLTInputSource	inputSource(c_wstr(urlString));

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

	XalanDOMString			localXSLURLString = trim(xslURLString);

	const XalanDOMString::size_type		fragIndex = indexOf(localXSLURLString, XalanUnicode::charNumberSign);

	const XalanDocument*	stylesheetDoc = 0;

	if(fragIndex == 0)
	{
		diag("Locating stylesheet from fragment identifier...");

		const XalanDOMString	fragID(localXSLURLString, 1);

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

		NodeRefList		nl(xobj->nodeset());

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
			if(m_diagnosticsPrintWriter != 0)
			{
				pushTime(frag);
			}

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

			if(m_diagnosticsPrintWriter != 0)
			{
				displayDuration(
						XalanDOMString(XALAN_STATIC_UCODE_STRING("Setup of ")) + localXSLURLString,
						frag);
			}

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
		if(m_diagnosticsPrintWriter != 0)
		{
			diag(XalanDOMString(XALAN_STATIC_UCODE_STRING("========= Parsing and preparing ")) +
					localXSLURLString +
					XALAN_STATIC_UCODE_STRING(" =========="));
			pushTime(&localXSLURLString);
		}

		XalanAutoPtr<Stylesheet>	theGuard;

		const XalanDocument* const	theOwnerDocument =
				fragBase.getNodeType() == XalanNode::DOCUMENT_NODE ?
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanDocument*)&fragBase :
#else
				static_cast<const XalanDocument*>(&fragBase) :
#endif
				fragBase.getOwnerDocument();
		assert(theOwnerDocument != 0);

		// Catch any XMLExceptions thrown, since we may not
		// be able to resolve the URL.  In that case, the
		// parser will throw an error.  We do this because
		// we don't know what sort of EntityResolvers might
		// be active, so we never want to error out here.
		try
		{
			if (length(xmlBaseIdent) == 0)
			{
				URISupport::getURLStringFromString(
							localXSLURLString,
							m_xpathEnvSupport.findURIFromDoc(theOwnerDocument),
							localXSLURLString);
			}
			else
			{
				URISupport::getURLStringFromString(
							localXSLURLString,
							xmlBaseIdent,
							localXSLURLString);
			}
		}
		catch(const XMLException&)
		{
		}

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

		URLAutoPtrType	xslURL(constructionContext.getURLFromString(localXSLURLString));

		XSLTInputSource		inputSource(xslURL->getURLText());

		m_parserLiaison.parseXMLStream(inputSource, stylesheetProcessor);

		stylesheet->postConstruction(constructionContext);

		theGuard.release();

		if(m_diagnosticsPrintWriter != 0)
		{
			displayDuration("Parsing and init of " + localXSLURLString, &localXSLURLString);
		}
	}

	return stylesheet;
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

XSLTEngineImpl::size_type
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
			const XalanNode*		sourceNode,
			const XalanNode*		styleNode) const
{
	problem(msg, ProblemListener::eMESSAGE, sourceNode, styleNode);
}



void
XSLTEngineImpl::message(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	problem(msg, ProblemListener::eMESSAGE, sourceNode, styleNode);
}



void
XSLTEngineImpl::message(
			const XalanDOMString&	msg,
			const Locator&			locator,
			const XalanNode*		sourceNode) const
{
	problem(msg, ProblemListener::eMESSAGE, locator, sourceNode);
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
			const XalanNode*					sourceNode,
			const XalanNode*					styleNode) const
{
	const Locator* const	locator = getLocatorFromStack();

	const XalanDOMChar*		id = 0;

	XalanDOMString			uri;

	XalanLocator::size_type		lineNumber = -1;
	XalanLocator::size_type		columnNumber = -1;

	if (locator != 0)
	{
		id = locator->getPublicId();

		if (id == 0)
		{
			id = locator->getSystemId();
		}

		if (id != 0)
		{
			uri = id;
		}

		lineNumber = locator->getLineNumber();
		columnNumber = locator->getColumnNumber();
	}

	if (m_problemListener != 0)
	{
		m_problemListener->problem(
					ProblemListener::eXSLPROCESSOR,
					classification,
					sourceNode,
					styleNode,
					msg,
					id,
					lineNumber,
					columnNumber);
	}

	if (classification == ProblemListener::eERROR)
	{
		throw XSLTProcessorException(msg, uri, lineNumber, columnNumber);
	}
}



void
XSLTEngineImpl::problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const XalanNode*					sourceNode,
			const ElemTemplateElement*			styleNode) const
{
	const XalanDOMChar*		id = 0;

	XalanDOMString			uri;

	int						lineNumber = -1;
	int 					columnNumber = -1;

	const Locator*			locator = getLocatorFromStack();

	if (locator == 0 && styleNode != 0)
	{
		locator = styleNode->getLocator();
	}

	if (locator != 0)
	{
		id = locator->getPublicId();

		if (id == 0)
		{
			id = locator->getSystemId();
		}

		if (id != 0)
		{
			uri = id;
		}

		lineNumber = locator->getLineNumber();
		columnNumber = locator->getColumnNumber();
	}
	else if (styleNode != 0)
	{
		lineNumber = styleNode->getLineNumber();
		columnNumber = styleNode->getColumnNumber();

		uri = styleNode->getURI();
	}

	if (m_problemListener != 0)
	{
		m_problemListener->problem(
					ProblemListener::eXSLPROCESSOR,
					classification,
					sourceNode,
					styleNode,
					msg,
					id,
					lineNumber,
					columnNumber);
	}

	if (classification == ProblemListener::eERROR)
	{
		throw XSLTProcessorException(msg, uri, lineNumber, columnNumber);
	}
}



static const XalanDOMChar	theDummy = 0;

void
XSLTEngineImpl::problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const Locator&						locator,
			const XalanNode*					sourceNode) const
{
	const XalanDOMChar*		id = locator.getSystemId();

	if (id == 0)
	{
		id = &theDummy;
	}

	const XalanLocator::size_type	lineNumber = locator.getLineNumber();
	const XalanLocator::size_type 	columnNumber = locator.getColumnNumber();

	if (m_problemListener != 0)
	{
		m_problemListener->problem(
					ProblemListener::eXSLPROCESSOR,
					classification,
					sourceNode,
					0,
					msg,
					id,
					lineNumber,
					columnNumber);
	}

	if (classification == ProblemListener::eERROR)
	{
		throw XSLTProcessorException(msg, XalanDOMString(id), lineNumber, columnNumber);
	}
}



void
XSLTEngineImpl::warn(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		styleNode) const
{
	problem(msg, ProblemListener::eWARNING, sourceNode, styleNode);
}



void
XSLTEngineImpl::warn(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	const Locator* const	locator = styleNode == 0 ? 0 : styleNode->getLocator();

	if (locator != 0)
	{
		problem(msg, ProblemListener::eWARNING, *locator, sourceNode);
	}
	else
	{
		problem(msg, ProblemListener::eWARNING, sourceNode, styleNode);
	}
}



void
XSLTEngineImpl::warn(
			const XalanDOMString&		msg,
			const Locator&				locator,
			const XalanNode*			sourceNode) const
{
	problem(msg, ProblemListener::eWARNING, locator, sourceNode);
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
XSLTEngineImpl::warn(
			const char*					msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
XSLTEngineImpl::error(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		styleNode) const
{
	problem(msg, ProblemListener::eERROR, sourceNode, styleNode);
}



void
XSLTEngineImpl::error(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	problem(msg, ProblemListener::eERROR, sourceNode, styleNode);
}



void
XSLTEngineImpl::error(
			const XalanDOMString&		msg,
			const Locator&				locator,
			const XalanNode*			sourceNode) const
{
	problem(msg, ProblemListener::eERROR, locator, sourceNode);
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
XSLTEngineImpl::pushTime(const void*	key)
{
	if(0 != key)
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		m_durationsTable[key] = std::clock();
#else
		m_durationsTable[key] = clock();
#endif
	}
}



XSLTEngineImpl::ClockType
XSLTEngineImpl::popDuration(const void*		key)
{
	ClockType 	clockTicksDuration = 0;

	if(0 != key)
	{
		const DurationsTableMapType::iterator	i =
				m_durationsTable.find(key);

		if (i != m_durationsTable.end())
		{
#if defined(XALAN_STRICT_ANSI_HEADERS)
			clockTicksDuration = std::clock() - (*i).second;
#else
			clockTicksDuration = clock() - (*i).second;
#endif

			m_durationsTable.erase(i);
		}
	}

	return clockTicksDuration;
}



void
XSLTEngineImpl::displayDuration(
			const XalanDOMString&	info,
			const void*				key)
{
	if(0 != key)
	{
		const ClockType	theDuration = popDuration(key);

		if(m_diagnosticsPrintWriter != 0)
		{
			const double	millis = (double(theDuration) / CLOCKS_PER_SEC) * 1000.0;

			m_diagnosticsPrintWriter->print(info);

			m_diagnosticsPrintWriter->print(" took ");
			m_diagnosticsPrintWriter->print(millis);
			m_diagnosticsPrintWriter->println(" milliseconds.");
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
XSLTEngineImpl::setDocumentLocator(const Locator*	/* locator */)
{
	// Do nothing for now
}



void
XSLTEngineImpl::traceSelect(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		theTemplate,
			const NodeRefListBase&			nl,
			const XPath*					xpath) const
{
	if (0 != m_diagnosticsPrintWriter)
	{
		XalanDOMString	msg = theTemplate.getNodeName() + XalanDOMString(XALAN_STATIC_UCODE_STRING(": "));

		if(xpath != 0)
		{
			msg += xpath->getExpression().getCurrentPattern();
			msg += XALAN_STATIC_UCODE_STRING(", ");
			msg += UnsignedLongToDOMString(nl.getLength());
			msg += XALAN_STATIC_UCODE_STRING(" selected");
		}
		else
		{
			msg += XALAN_STATIC_UCODE_STRING("*|text(), (default select), ");
			msg += UnsignedLongToDOMString(nl.getLength());
			msg += XALAN_STATIC_UCODE_STRING(" selected");
		}

		const XalanQName* const		mode = executionContext.getCurrentMode();

		if(mode != 0 && mode->isEmpty() == false)
		{
			msg += XALAN_STATIC_UCODE_STRING(", mode = {");
			msg += mode->getNamespace();
			msg += XALAN_STATIC_UCODE_STRING("}");
			msg += mode->getLocalPart();
		}

		m_diagnosticsPrintWriter->println(msg);
	}
}



void
XSLTEngineImpl::startDocument()
{
	assert(getFormatterListener() != 0);
	assert(m_executionContext != 0);
	assert(m_cdataStack.empty() == true);

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
	assert(m_cdataStack.empty() == true);

	setMustFlushPendingStartDocument(true);

	flushPending();

	getFormatterListener()->endDocument();

	if(getTraceListeners() > 0)
	{
		const GenerateEvent		ge(GenerateEvent::EVENTTYPE_ENDDOCUMENT);

		fireGenerateEvent(ge);
	}

	m_resultNamespacesStack.popContext();

	assert(m_resultNamespacesStack.empty() == true);
}



void
XSLTEngineImpl::addResultAttribute(
			AttributeListImpl&			attList,
			const XalanDOMString&		aname,
			const XalanDOMChar*			value)
{
	assert(value != 0);

	// Always exclude the implicit XML declaration...
	if (equals(aname, DOMServices::s_XMLNamespacePrefix) == false) 
	{
		bool	fExcludeAttribute = false;

		if (equals(aname, DOMServices::s_XMLNamespace) == true)
		{
			// OK, we're adding a default namespace declaration.  So see if the length
			// of the namespace is 0.  If it's not, go ahead and add the declaration.
			// If it's not, it means we're "turning off" the previous default
			// declaration.

			const XalanDOMString* const		currentDefaultNamespace =
						getNamespaceForPrefix(s_emptyString);

			const XalanDOMString::size_type		theLength = length(value);

			// Note that we use an empty string for the prefix, instead of "xmlns", since the
			// prefix really is "".
			if (theLength != 0)
			{
				if (currentDefaultNamespace != 0 &&
					equals(*currentDefaultNamespace, value, theLength) == true)
				{
					fExcludeAttribute = true;
				}
				else
				{
					addResultNamespaceDecl(s_emptyString, value, theLength);
				}
			}
			else
			{
				// OK, we're turning of the previous default namespace declaration.
				// Check to see if there is one, and if there isn't, don't add
				// the namespace declaration _and_ don't add the attribute.
				if (currentDefaultNamespace != 0 && length(*currentDefaultNamespace) != 0)
				{
					addResultNamespaceDecl(s_emptyString, value, theLength);
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

			substring(aname, prefix, DOMServices::s_XMLNamespaceWithSeparatorLength);

			const XalanDOMString* const	theNamespace = getResultNamespaceForPrefix(prefix);

			const XalanDOMString::size_type		theLength = length(value);

			if (theNamespace == 0 || equals(*theNamespace, value, theLength) == false)
			{
				addResultNamespaceDecl(prefix, value, theLength);
			}
			else
			{
				fExcludeAttribute = true;
			}
		}

		if (fExcludeAttribute == false)
		{
			attList.addAttribute(
				c_wstr(aname),
				c_wstr(Constants::ATTRTYPE_CDATA),
				c_wstr(value));
		}
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
			if (equalsIgnoreCaseASCII(getPendingElementName(),
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

		if(m_hasCDATASectionElements == true)
		{
			m_cdataStack.push_back(isCDataResultElem(thePendingElementName));
		}

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
XSLTEngineImpl::startElement(const XalanDOMChar*	name)
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
			const XalanDOMChar*		name,
			AttributeList&			atts)
{
	assert(getFormatterListener() != 0);
	assert(name != 0);

	flushPending();

	const unsigned int	nAtts = atts.getLength();

	assert(m_outputContextStack.empty() == false);

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
XSLTEngineImpl::endElement(const XalanDOMChar*	name)
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

	if(m_hasCDATASectionElements == true)
	{
		assert(m_cdataStack.empty() == false);

		m_cdataStack.pop_back();
	}
}



void
XSLTEngineImpl::characters(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type 	length)
{
	characters(ch,
			   0,
			   length);
}



void
XSLTEngineImpl::characters(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	assert(getFormatterListener() != 0);
	assert(ch != 0);
	assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());

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
	assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());

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
	assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());

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
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
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
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	length)
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
			const XalanDOMChar*		target,
			const XalanDOMChar*		data)
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
XSLTEngineImpl::comment(const XalanDOMChar*		data)
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
XSLTEngineImpl::entityReference(const XalanDOMChar*		name)
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
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
{
	assert(getFormatterListener() != 0);
	assert(ch != 0);

	setMustFlushPendingStartDocument(true);

	flushPending();

	getFormatterListener()->cdata(ch, length);

	if(getTraceListeners() > 0)
	{
		GenerateEvent ge(GenerateEvent::EVENTTYPE_CDATA, ch, start,
					length);

		fireGenerateEvent(ge);
	}
}



void
XSLTEngineImpl::checkDefaultNamespace(
			const XalanDOMString&	theElementName,
			const XalanDOMString&	theElementNamespaceURI)
{
	// Check for elements with no prefix...
	if (indexOf(theElementName, XalanUnicode::charColon) == theElementName.length())
	{
		// Get the current default namespace URI..
		const XalanDOMString* const		theResultNamespace =
			getResultNamespaceForPrefix(s_emptyString);

		// If there is one, and the URIs are different, add a new declaration.  This
		// will also "turn-off" the default namespace, if necessary.
		if (theResultNamespace != 0 && theElementNamespaceURI != *theResultNamespace)
		{
			addResultAttribute(DOMServices::s_XMLNamespace, theElementNamespaceURI);
		}
	}
}



void
XSLTEngineImpl::warnCopyTextNodesOnly(
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	warn(
			"Only text nodes can be copied in this context.  The node is ignored",
			sourceNode,
			styleNode);
}



void
XSLTEngineImpl::cloneToResultTree(
			const XalanText&	node,
			bool				isLiteral,
			bool				overrideStrip)
{
	bool	stripWhiteSpace = false;

	// If stripWhiteSpace is false, then take this as an override and 
	// just preserve the space, otherwise use the XSL whitespace rules.
	if(!overrideStrip)
	{
		stripWhiteSpace = isLiteral ? true : false;
	}

	const bool	isIgnorableWhitespace = node.isIgnorableWhitespace();

	if(stripWhiteSpace == false || isIgnorableWhitespace == false)
	{
		assert(node.getParentNode() == 0 ||
			   node.getParentNode()->getNodeType() != XalanNode::DOCUMENT_NODE);

		const XalanDOMString&	data = node.getData();

		if(0 != length(data))
		{
			if(isIgnorableWhitespace == true)
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



void
XSLTEngineImpl::cloneToResultTree(
			const XalanNode&			node,
			bool						cloneTextNodesOnly,
			const ElemTemplateElement*	styleNode)
{
	XalanNode::NodeType		posNodeType = node.getNodeType();

	if (cloneTextNodesOnly == true &&
		posNodeType != XalanNode::TEXT_NODE)
	{
		warnCopyTextNodesOnly(
			&node,
			styleNode);
	}
	else
	{
		const XalanNode*	pos = &node;

		while(pos != 0)
		{
			if(posNodeType != XalanNode::ATTRIBUTE_NODE)
			{
				flushPending();
			}

			cloneToResultTree(
							*pos,
							posNodeType,
							false,
							false,
							true,
							false,
							styleNode);

			const XalanNode*	nextNode = pos->getFirstChild();

			while(nextNode == 0)
			{
				if(XalanNode::ELEMENT_NODE == posNodeType)
				{
					endElement(c_wstr(pos->getNodeName()));
				}

				if(&node == pos)
					break;

				nextNode = pos->getNextSibling();

				if(nextNode == 0)
				{
					pos = pos->getParentNode();
					assert(pos != 0);

					posNodeType = pos->getNodeType();

					if(&node == pos)
					{
						if(XalanNode::ELEMENT_NODE == posNodeType)
						{
							endElement(c_wstr(pos->getNodeName()));
						}

						nextNode = 0;
						break;
					}
				}
			}

			pos = nextNode;

			if (pos != 0)
			{
				posNodeType = pos->getNodeType();
			}
		}
	}
}



void
XSLTEngineImpl::cloneToResultTree(
			const XalanNode&			node,
			XalanNode::NodeType			nodeType,
			bool						isLiteral,
			bool						overrideStrip,
			bool						shouldCloneAttributes,
			bool						cloneTextNodesOnly,
			const ElemTemplateElement*	styleNode)
{
	assert(nodeType == node.getNodeType());
	assert(m_executionContext != 0);

	if(cloneTextNodesOnly == true)
	{
		if (nodeType != XalanNode::TEXT_NODE)
		{
			warnCopyTextNodesOnly(
					&node,
					styleNode);
		}
		else
		{
			const XalanText& 	tx =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanText&)node;
#else
				static_cast<const XalanText&>(node);
#endif

			cloneToResultTree(tx, isLiteral, overrideStrip);
		}
	}
	else
	{
		switch(nodeType)
		{
		case XalanNode::TEXT_NODE:
			{
				const XalanText& 	tx =
	#if defined(XALAN_OLD_STYLE_CASTS)
					(const XalanText&)node;
	#else
					static_cast<const XalanText&>(node);
	#endif

				cloneToResultTree(tx, isLiteral, overrideStrip);
			}
			break;

		case XalanNode::ELEMENT_NODE:
			{
				const XalanDOMString&	theElementName =
					node.getNodeName();

				startElement(c_wstr(theElementName));

				if(shouldCloneAttributes == true)
				{
					copyAttributesToAttList(
						node,
						getPendingAttributesImpl());

					copyNamespaceAttributes(node);
				}

				checkDefaultNamespace(theElementName, node.getNamespaceURI());
			}
			break;

		case XalanNode::CDATA_SECTION_NODE:
			{
				const XalanDOMString& 	data = node.getNodeValue();

				cdata(toCharArray(data), 0, length(data));
			}
			break;

		case XalanNode::ATTRIBUTE_NODE:
			if (length(getPendingElementName()) != 0)
			{
				addResultAttribute(
						getPendingAttributesImpl(),
						node.getNodeName(),
						node.getNodeValue());
			}
			else
			{
				warn(
					"Attempting to add an attribute when there is no open element.  The attribute will be ignored",
					&node,
					styleNode);
			}
			break;

		case XalanNode::COMMENT_NODE:
			comment(c_wstr(node.getNodeValue()));
			break;

		case XalanNode::DOCUMENT_FRAGMENT_NODE:
			error("No clone of a document fragment!");
			break;
		
		case XalanNode::ENTITY_REFERENCE_NODE:
			entityReference(c_wstr(node.getNodeName()));
			break;

		case XalanNode::PROCESSING_INSTRUCTION_NODE:
			processingInstruction(
					c_wstr(node.getNodeName()),
					c_wstr(node.getNodeValue()));
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
}



void
XSLTEngineImpl::outputToResultTree(
			const XObject& 				value,
			bool						outputTextNodesOnly,
			const ElemTemplateElement*	styleNode)
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
			const NodeRefListBase&	nl = value.nodeset();

			const NodeRefListBase::size_type	nChildren = nl.getLength();

			for(NodeRefListBase::size_type i = 0; i < nChildren; i++)
			{
				XalanNode*			pos = nl.item(i);
				assert(pos != 0);

				XalanNode::NodeType		posNodeType = pos->getNodeType();

				if (outputTextNodesOnly == true &&
					posNodeType != XalanNode::TEXT_NODE)
				{
					warnCopyTextNodesOnly(
							pos,
							styleNode);
				}
				else
				{
					XalanNode* const	top = pos;

					while(0 != pos)
					{
						flushPending();

						XalanNode::NodeType		posNodeType = pos->getNodeType();

						cloneToResultTree(*pos, posNodeType, false, false, false, false, styleNode);

						XalanNode*	nextNode = pos->getFirstChild();

						while(0 == nextNode)
						{
							if(XalanNode::ELEMENT_NODE == posNodeType)
							{
								endElement(c_wstr(pos->getNodeName()));
							}

							if(top == pos)
								break;

							nextNode = pos->getNextSibling();

							if(0 == nextNode)
							{
								pos = pos->getParentNode();
								assert(pos != 0);

								posNodeType = pos->getNodeType();

								if(top == pos)
								{
									if(XalanNode::ELEMENT_NODE == posNodeType)
									{
										endElement(c_wstr(pos->getNodeName()));
									}

									nextNode = 0;
									break;
								}
							}
						}

						pos = nextNode;

						if (pos != 0)
						{
							posNodeType = pos->getNodeType();
						}
					}
				}
			}
		}
		break;
		
	case XObject::eTypeResultTreeFrag:
		outputResultTreeFragment(value, outputTextNodesOnly, styleNode);
		break;

	case XObject::eTypeNull:
	case XObject::eTypeUnknown:
	case XObject::eUnknown:
	default:
		assert(0);
	}
}



void
XSLTEngineImpl::outputResultTreeFragment(
			const XObject& 				theTree,
			bool						outputTextNodesOnly,
			const ElemTemplateElement*	styleNode)
{
	const ResultTreeFragBase&	docFrag = theTree.rtree();

	const XalanNodeList* const	nl = docFrag.getChildNodes();
	assert(nl != 0);

	const unsigned int			nChildren = nl->getLength();

	for(unsigned int i = 0; i < nChildren; i++)
	{
		XalanNode*				pos = nl->item(i);
		assert(pos != 0);

		XalanNode::NodeType		posNodeType = pos->getNodeType();

		if (outputTextNodesOnly == true &&
			posNodeType != XalanNode::TEXT_NODE)
		{
			warnCopyTextNodesOnly(
					pos,
					styleNode);
		}
		else
		{
			XalanNode* const		top = pos;

			while(0 != pos)
			{
				flushPending();

				cloneToResultTree(*pos, posNodeType, false, false, true, false, styleNode);

				XalanNode*	nextNode = pos->getFirstChild();

				while(0 == nextNode)
				{
					if(XalanNode::ELEMENT_NODE == posNodeType)
					{
						endElement(c_wstr(pos->getNodeName()));
					}

					if(top == pos)
						break;

					nextNode = pos->getNextSibling();

					if(0 == nextNode)
					{
						pos = pos->getParentNode();

						if(0 == pos)
						{
							nextNode = 0;

							break;
						}
						else
						{
							assert(0 != pos);

							posNodeType = pos->getNodeType();

							if(top == pos)
							{
								if(XalanNode::ELEMENT_NODE == posNodeType)
								{
									endElement(c_wstr(pos->getNodeName()));
								}

								nextNode = 0;

								break;
							}
						}
					}
				}

				pos = nextNode;

				if (pos != 0)
				{
					posNodeType = pos->getNodeType();
				}
			}
		}
	}
}



bool
XSLTEngineImpl::isCDataResultElem(const XalanDOMString&		elementName) const
{
	assert(m_executionContext != 0);
	assert(m_hasCDATASectionElements == true);

	bool	fResult = false;

	const XalanDOMString::size_type		indexOfNSSep = indexOf(elementName, XalanUnicode::charColon);

	if(indexOfNSSep == length(elementName))
	{
		const XalanDOMString* const		elemNS =
					getResultNamespaceForPrefix(s_emptyString);

		if (elemNS != 0)
		{
			fResult = m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(*elemNS, elementName));
		}
		else
		{
			fResult = m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(s_emptyString, elementName));
		}
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
					m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(DOMServices::s_XMLNamespaceURI, elemLocalName));
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
						m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(*elemNS, elemLocalName));
			}
		}
	}

	return fResult;
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
			const XalanDOMString&		thePrefix,
			XalanDOMString::size_type	thePrefixLength,
			const XalanDOMChar*			theName,
			XalanDOMString::size_type	theNameLength)
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

		const XalanDOMString::size_type		theIndex = indexOf(
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
			const XalanDOMString&		thePrefix,
			XalanDOMString::size_type	thePrefixLength,
			const XalanDOMString&		theName)
{
	return isPrefixUsed(thePrefix, thePrefixLength, c_wstr(theName), length(theName));
}



inline bool
isPrefixUsedOrDeclared(
			const XalanDOMString&		thePrefix,
			XalanDOMString::size_type	thePrefixLength,
			const XalanDOMChar*			theName,
			XalanDOMString::size_type	theNameLength)
{
	if (isPrefixUsed(thePrefix, thePrefixLength, theName, theNameLength) == true)
	{
		return true;
	}
	else
	{
		const XalanDOMString::size_type		theDeclarationLength =
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
			XalanDOMString::size_type	thePrefixLength)
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
	const XalanDOMString::size_type		thePrefixLength = length(thePrefix);
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
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theName,
			const XalanNode&		theNode,
			AttributeListImpl&		thePendingAttributes,
			bool					fOnlyIfPrefixNotPresent)
{
	if (fOnlyIfPrefixNotPresent == false ||
		m_resultNamespacesStack.prefixIsPresentLocal(thePrefix) == false)
	{
		const XalanDOMString* const 	desturi = getResultNamespaceForPrefix(thePrefix);
		const XalanDOMString&			srcURI = theNode.getNodeValue();

		if(desturi == 0 || equals(srcURI, *desturi) == false)
		{
			addResultAttribute(thePendingAttributes, theName, srcURI);
			addResultNamespaceDecl(thePrefix, srcURI);
		}
	}
}



void
XSLTEngineImpl::addResultNamespace(
			const XalanNode&	theNode,
			AttributeListImpl&	thePendingAttributes,
			bool				fOnlyIfPrefixNotPresent)
{
	assert(m_executionContext != 0);
	assert(theNode.getNodeType() == XalanNode::ATTRIBUTE_NODE);

	const XalanDOMString& 	aname = theNode.getNodeName();

	if (equals(aname, DOMServices::s_XMLNamespace) == true)
	{
		// Default namespace declaration...
		addResultNamespace(s_emptyString, aname, theNode, thePendingAttributes, fOnlyIfPrefixNotPresent);
	}
	else if (startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator))
	{
		StylesheetExecutionContext::GetAndReleaseCachedString	prefixGuard(*m_executionContext);

		XalanDOMString& 	thePrefix = prefixGuard.get();

		substring(aname, thePrefix, DOMServices::s_XMLNamespaceWithSeparatorLength);

		addResultNamespace(thePrefix, aname, theNode, thePendingAttributes, fOnlyIfPrefixNotPresent);
	}
}



void
XSLTEngineImpl::copyNamespaceAttributes(const XalanNode&	src) 
{
	assert(m_attributeNamesVisited.empty() == true);

	const XalanNode*	parent = &src;

	while (parent != 0 &&
		   parent->getNodeType() == XalanNode::ELEMENT_NODE) 
	{
		const XalanNamedNodeMap* const	nnm =
				parent->getAttributes();
		assert(nnm != 0);

		const unsigned int	nAttrs = nnm->getLength();

		assert(m_outputContextStack.empty() == false);

		AttributeListImpl&	thePendingAttributes =
				getPendingAttributesImpl();

		for (unsigned int i = 0;  i < nAttrs; i++) 
		{
			const XalanNode* const	attr = nnm->item(i);
			assert(attr != 0);

			const XalanDOMString&	nodeName = attr->getNodeName();

			if (m_attributeNamesVisited.find(&nodeName) == m_attributeNamesVisitedEnd)
			{
				addResultNamespace(*attr, thePendingAttributes, true);

				m_attributeNamesVisited.insert(&nodeName);
			}
		}

		parent = parent->getParentNode();
	}

	m_attributeNamesVisited.clear();
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
								*executionContext.getPrefixResolver(),
								getLocatorFromStack());

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
								prefixResolver,
								getLocatorFromStack());

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

	m_xpathProcessor->initMatchPattern(*xpath, str, resolver, getLocatorFromStack());

	return xpath;
}



void
XSLTEngineImpl::returnXPath(const XPath*	xpath)
{
	m_xpathFactory.returnObject(xpath);
}



inline void
XSLTEngineImpl::copyAttributeToTarget(
			const XalanDOMString&	attrName,
			const XalanDOMString&	attrValue,
			AttributeListImpl&		attrList)
{
	addResultAttribute(attrList, attrName, attrValue);
}



void
XSLTEngineImpl::copyAttributesToAttList(
			const XalanNode& 	node,
			AttributeListImpl&	attList)
{
	assert(m_stylesheetRoot != 0);

	const XalanNamedNodeMap* const	attributes =
		node.getAttributes();

	if (attributes != 0)
	{
		const unsigned int	nAttributes = attributes->getLength();

		for(unsigned int i = 0; i < nAttributes; ++i)  
		{	
			const XalanNode* const 	attr = attributes->item(i);
			assert(attr != 0);

			copyAttributeToTarget(
				attr->getNodeName(),
				attr->getNodeValue(),
				attList);
		}
	}
}



bool
XSLTEngineImpl::shouldStripSourceNode(
			XPathExecutionContext&	executionContext,
			const XalanNode&		textNode) const
{
	if (m_hasStripOrPreserveSpace == false || m_stylesheetRoot == 0)
	{
		return false;
	}
	else
	{
		bool	strip = false;

		assert(m_stylesheetRoot->getWhitespacePreservingElements().empty() == false ||
			   m_stylesheetRoot->getWhitespaceStrippingElements().empty() == false);

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
					XPath::eMatchScore	highPreserveScore = XPath::eMatchScoreNone;
					XPath::eMatchScore	highStripScore = XPath::eMatchScoreNone;

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

							const XPath::eMatchScore	score = matchPat->getMatchScore(parent, executionContext);

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

							const XPath::eMatchScore	score = matchPat->getMatchScore(parent, executionContext);

							if(score > highStripScore)
								highStripScore = score;
						}
					}

					if(highPreserveScore > XPath::eMatchScoreNone ||
					   highStripScore > XPath::eMatchScoreNone)
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
	do
	{
		assign(m_scratchString, s_uniqueNamespacePrefix);

		UnsignedLongToDOMString(m_uniqueNSValue++, m_scratchString);
	} while(getResultNamespaceForPrefix(m_scratchString) != 0);

	append(theValue, m_scratchString);
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
	const XalanQNameByValue		qname(theName, 0, m_xpathEnvSupport, m_domSupport);

	m_topLevelParams.push_back(ParamVectorType::value_type(qname, expression));
}



void
XSLTEngineImpl::setStylesheetParam(
			const XalanDOMString&	theName,
			XObjectPtr				theValue)
{
	const XalanQNameByValue		qname(theName, 0, m_xpathEnvSupport, m_domSupport);

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
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length,
			bool						isCDATA)
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



static XalanDOMString	s_XSLNameSpaceURL;

static XalanDOMString	s_XalanNamespaceURL;

static XalanDOMString	s_uniqueNamespacePrefix;

static XalanDOMString	s_stylesheetNodeName;

static XalanDOMString	s_typeString;

static XalanDOMString	s_hrefString;

static XalanDOMString	s_piTokenizerString;

static XalanDOMString	s_typeValueString1;

static XalanDOMString	s_typeValueString2;

static XalanDOMString	s_typeValueString3;

static XalanDOMString	s_typeValueString4;



const XalanDOMString&	XSLTEngineImpl::s_XSLNameSpaceURL = ::s_XSLNameSpaceURL;

const XalanDOMString&	XSLTEngineImpl::s_XalanNamespaceURL = ::s_XalanNamespaceURL;

const XalanDOMString&	XSLTEngineImpl::s_uniqueNamespacePrefix = ::s_uniqueNamespacePrefix;

const XalanDOMString&	XSLTEngineImpl::s_stylesheetNodeName = ::s_stylesheetNodeName;

const XalanDOMString&	XSLTEngineImpl::s_typeString = ::s_typeString;

const XalanDOMString&	XSLTEngineImpl::s_hrefString = ::s_hrefString;

const XalanDOMString&	XSLTEngineImpl::s_piTokenizerString = ::s_piTokenizerString;

const XalanDOMString&	XSLTEngineImpl::s_typeValueString1 = ::s_typeValueString1;

const XalanDOMString&	XSLTEngineImpl::s_typeValueString2 = ::s_typeValueString2;

const XalanDOMString&	XSLTEngineImpl::s_typeValueString3 = ::s_typeValueString3;

const XalanDOMString&	XSLTEngineImpl::s_typeValueString4 = ::s_typeValueString4;


#if 0
#include <fstream>

void
dumpTable(
			const XSLTEngineImpl::ElementKeysMapType&	theTable,
			std::ostream&								theSourceStream,
			std::ostream&								theHeaderStream)
{
	XSLTEngineImpl::ElementKeysMapType::const_iterator	i = theTable.begin();

	while(i != theTable.end())
	{
		theSourceStream << "const XalanDOMChar\tXSLTEngineImpl::s_";

		const XalanDOMString&	theString = (*i).first;

		theHeaderStream << "\t// The string \"" << theString << "\"\n\tstatic const XalanDOMChar\ts_";

		bool	nextCap = false;

		XalanDOMString::const_iterator	j = theString.begin();

		while(*j)
		{
			if (*j == '-')
			{
				nextCap = true;
			}
			else
			{
				assert(*j >= 'a' && *j <= 'z');

				if (nextCap)
				{
					theSourceStream << char(*j -'a' + 'A');
					theHeaderStream << char(*j -'a' + 'A');

					nextCap = false;
				}
				else
				{
					theSourceStream << char(*j);
					theHeaderStream << char(*j);
				}
			}

			++j;
		}

		j = theString.begin();

		theSourceStream << "[] =\n{\n";
		theHeaderStream << "[];\n\n";

		while(*j)
		{
			if (*j == '-')
			{
				theSourceStream << "\tXalanUnicode::charHyphenMinus,\n";
			}
			else
			{
				assert(*j >= 'a' && *j <= 'z');

				theSourceStream << "\tXalanUnicode::charLetter_";

				theSourceStream << char(*j) << ",\n";
			}

			++j;
		}

		theSourceStream << "\t0\n};\n\n";

		++i;
	}
}
#endif


void
XSLTEngineImpl::initialize()
{
	::s_XSLNameSpaceURL = XALAN_STATIC_UCODE_STRING("http://www.w3.org/1999/XSL/Transform");

	::s_XalanNamespaceURL = XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xalan");

	::s_uniqueNamespacePrefix = XALAN_STATIC_UCODE_STRING("ns");

	::s_stylesheetNodeName = XALAN_STATIC_UCODE_STRING("xml-stylesheet");

	::s_typeString = XALAN_STATIC_UCODE_STRING("type");

	::s_hrefString = XALAN_STATIC_UCODE_STRING("href");

	::s_piTokenizerString = XALAN_STATIC_UCODE_STRING(" \t=");

	::s_typeValueString1 = XALAN_STATIC_UCODE_STRING("text/xml");

	::s_typeValueString2 = XALAN_STATIC_UCODE_STRING("text/xsl");

	::s_typeValueString3 = XALAN_STATIC_UCODE_STRING("application/xml");

	::s_typeValueString4 = XALAN_STATIC_UCODE_STRING("application/xml+xslt");

	installFunctions();
}



void
XSLTEngineImpl::terminate()
{
	uninstallFunctions();

	releaseMemory(::s_uniqueNamespacePrefix);

	releaseMemory(::s_XalanNamespaceURL);

	releaseMemory(::s_XSLNameSpaceURL);

	releaseMemory(::s_stylesheetNodeName);

	releaseMemory(::s_typeString);

	releaseMemory(::s_hrefString);

	releaseMemory(::s_piTokenizerString);

	releaseMemory(::s_typeValueString1);

	releaseMemory(::s_typeValueString2);

	releaseMemory(::s_typeValueString3);

	releaseMemory(::s_typeValueString4);
}
