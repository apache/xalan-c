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
#include "StylesheetHandler.hpp"

#include <algorithm>
#include <PlatformSupport/STLHelper.hpp>


#include <sax/Locator.hpp>
#include <sax/SAXException.hpp>

#include <util/XMLURL.hpp>


#include <XMLSupport/Formatter.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>



#include "Constants.hpp"
#include "ElemApplyImport.hpp"
#include "ElemApplyTemplates.hpp"
#include "ElemAttribute.hpp"
#include "ElemAttributeSet.hpp"
#include "ElemCallTemplate.hpp"
#include "ElemChoose.hpp"
#include "ElemComment.hpp"
#include "ElemCopy.hpp"
#include "ElemCopyOf.hpp"
#include "ElemDecimalFormat.hpp"
#include "ElemElement.hpp"
#include "ElemEmpty.hpp"
#include "ElemExtensionCall.hpp"
#include "ElemForEach.hpp"
#include "ElemIf.hpp"
#include "ElemLiteralResult.hpp"
#include "ElemMessage.hpp"
#include "ElemNumber.hpp"
#include "ElemOtherwise.hpp"
#include "ElemParam.hpp"
#include "ElemPI.hpp"
#include "ElemSort.hpp"
#include "ElemTemplate.hpp"
#include "ElemText.hpp"
#include "ElemTextLiteral.hpp"
#include "ElemUse.hpp"
#include "ElemValueOf.hpp"
#include "ElemWhen.hpp"
#include "ElemWithParam.hpp"
#include "ExtensionNSHandler.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"



StylesheetHandler::StylesheetHandler(
			XSLTEngineImpl&					processor,
			Stylesheet&						stylesheetTree,
			StylesheetConstructionContext&	constructionContext) :
	FormatterListener(OUTPUT_METHOD_OTHER),
	m_includeBase(stylesheetTree.getBaseIdentifier()),
	m_pendingException(),
	m_exceptionPending(false),
	m_processor(processor),
	m_stylesheet(stylesheetTree),
	m_constructionContext(constructionContext),
	m_elemStack(),
	m_strayElements(),
	m_whiteSpaceElems(),
	m_pTemplate(0),
	m_lastPopped(0),
	m_inTemplate(false),
	m_foundStylesheet(false),
	m_foundNotImport(false),
	m_inLXSLTScript(false),
	m_LXSLTScriptBody(),
	m_LXSLTScriptLang(),
	m_LXSLTScriptSrcURL(),
	m_pLXSLTExtensionNSH(0)
{
}



StylesheetHandler::~StylesheetHandler()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up the element stack vector
	for_each(m_elemStack.begin(),
			 m_elemStack.end(),
			 DeleteFunctor<ElemTemplateElement>());

	// Clean up the whitespace elements.
	for_each(m_whiteSpaceElems.begin(),
			 m_whiteSpaceElems.end(),
			 DeleteFunctor<ElemTemplateElement>());

	// Clean up the stray elements.
	for_each(m_strayElements.begin(),
			 m_strayElements.end(),
			 DeleteFunctor<ElemTemplateElement>());
}



void StylesheetHandler::setDocumentLocator(const Locator* const		locator)
{
	m_processor.pushLocatorOnStack(locator);
}


void StylesheetHandler::startDocument()
{
	m_exceptionPending = false;

	clear(m_pendingException);
}


void StylesheetHandler::endDocument()
{
	m_processor.popLocatorStack();

	if (m_exceptionPending == true)
	{
		throw SAXException(toCharArray(m_pendingException));
	}
}


bool
StylesheetHandler::isAttrOK(
			const XalanDOMChar*		attrName,
			const AttributeList&	atts,
			int						which)
{
	return m_stylesheet.isAttrOK(attrName, atts, which, m_constructionContext);
}



bool
StylesheetHandler::processSpaceAttr(
			const XalanDOMChar*		aname,
			const AttributeList&	atts,
			int						which)
{
	const bool	isSpaceAttr = equals(aname, XALAN_STATIC_UCODE_STRING("xml:space"));

	if(isSpaceAttr)
	{
		const XalanDOMChar*	const	spaceVal = atts.getValue(which);

		if(equals(spaceVal, XALAN_STATIC_UCODE_STRING("default")))
		{
			m_stylesheet.setDefaultSpaceProcessing(true);
		}
		else if(equals(spaceVal, XALAN_STATIC_UCODE_STRING("preserve")))
		{
			m_stylesheet.setDefaultSpaceProcessing(false);
		}
		else
		{
			throw SAXException("xml:space has an illegal value");
		}
	}

	return isSpaceAttr;
}



void
StylesheetHandler::startElement (const XMLCh* const name, AttributeList& atts)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

	try
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		// Clean up the whitespace elements.
		for_each(m_whiteSpaceElems.begin(),
				 m_whiteSpaceElems.end(),
				 DeleteFunctor<ElemTemplateElement>());

		m_whiteSpaceElems.clear();

		const Locator* const	locator = m_processor.getLocatorFromStack();

		const int	lineNumber = 0 != locator ? locator->getLineNumber() : 0;
		const int	columnNumber = 0 != locator ? locator->getColumnNumber() : 0;

		// First push namespaces
		m_stylesheet.pushNamespaces(atts);

		const XalanDOMString	ns = m_stylesheet.getNamespaceFromStack(name);

		const unsigned int		nameLength = length(name);
		const unsigned int		index = indexOf(name,':');

		const XalanDOMString	localName = index == nameLength ? XalanDOMString(name) : substring(name, index + 1);

		if(length(ns) == 0 && nameLength != length(localName))
		{
			// Warn that there is a prefix that was not resolved...
			m_constructionContext.warn(XalanDOMString("Could not resolve prefix ") + name);
		}

		ElemTemplateElement* elem = 0;

		const unsigned	origStackSize = m_elemStack.size();

		if(equals(ns, m_constructionContext.getXSLTNamespaceURI()))
		{
			if(!isEmpty(m_stylesheet.getXSLTNamespaceURI()))
				m_stylesheet.setXSLTNamespaceURI(ns);

			if(false == m_foundStylesheet)
			{
				m_stylesheet.getStylesheetRoot().initDefaultRule(m_constructionContext);
				m_stylesheet.setWrapperless(false);
			}

			XSLTEngineImpl::AttributeKeysMapType::const_iterator iter=
				XSLTEngineImpl::getElementKeys().find(localName);

			int xslToken = iter!= XSLTEngineImpl::getElementKeys().end() ? (*iter).second : -2;

			if(!m_inTemplate)
			{
				if(m_foundStylesheet && Constants::ELEMNAME_IMPORT != xslToken)
				{
					m_foundNotImport = true;
				}

				switch(xslToken)
				{
				case Constants::ELEMNAME_TEMPLATE:
					m_pTemplate = new ElemTemplate(m_constructionContext,
												m_stylesheet,
												name, atts, lineNumber, columnNumber);
					m_elemStack.push_back(m_pTemplate);
					m_inTemplate = true;
					m_stylesheet.addTemplate(m_pTemplate);
					break;

				case Constants::ELEMNAME_EXTENSION:
					if(!equalsIgnoreCase(ns, m_processor.getXalanXSLNameSpaceURL()))
					{
						m_constructionContext.warn("Old syntax: the functions instruction should use a url of " + m_processor.getXalanXSLNameSpaceURL());
					}
					// m_constructionContext.handleFunctionsInstruction((Element)child);
				break;

				case Constants::ELEMNAME_VARIABLE:
				case Constants::ELEMNAME_PARAMVARIABLE:
				{
					ElemVariable* varelem = (Constants::ELEMNAME_PARAMVARIABLE == xslToken) 
									   ? new ElemParam(m_constructionContext,
													   m_stylesheet,
													   name, atts, 
													   lineNumber, columnNumber)
										 : new ElemVariable(m_constructionContext,
															m_stylesheet,
															name, atts, 
															lineNumber, columnNumber);

					m_elemStack.push_back(varelem);
					m_inTemplate = true; // fake it out
					m_stylesheet.setTopLevelVariable(varelem);
					varelem->setTopLevel(true);
				}
				break;

				case Constants::ELEMNAME_LOCALE:
					m_processor.warn(XALAN_STATIC_UCODE_STRING("xsl:locale not yet supported!"));
					break;

				case Constants::ELEMNAME_PRESERVESPACE:
				case Constants::ELEMNAME_STRIPSPACE:
				{
					// $$$ ToDo: We should separate this out into a separate function.
					ElemEmpty nsNode(m_constructionContext, m_stylesheet, name, lineNumber, columnNumber);

					const unsigned int	nAttrs = atts.getLength();

					bool foundIt = false;

					for(unsigned int i = 0; i < nAttrs; i++)
					{
						const XalanDOMChar* const	aname = atts.getName(i);

						if(equals(aname, Constants::ATTRNAME_ELEMENTS))
						{
							foundIt = true;

							StringTokenizer		tokenizer(atts.getValue(i),
														  XALAN_STATIC_UCODE_STRING(" \t\n\r"));

							while(tokenizer.hasMoreTokens())
							{
								// Use only the root, at least for right now.
								const XalanDOMString	wildcardName = tokenizer.nextToken();

								/**
								 * Creating a match pattern is too much overhead, but it's a reasonably 
								 * easy and safe way to do this right now.  TODO: Validate the pattern 
								 * to make sure it's a WildcardName.
								 */
								const XPath* const	matchPat =
										m_constructionContext.createMatchPattern(wildcardName, nsNode);

								if(Constants::ELEMNAME_PRESERVESPACE == xslToken)
								{
									m_stylesheet.getStylesheetRoot().pushWhitespacePreservingElement(matchPat);
								}
								else
								{
									m_stylesheet.getStylesheetRoot().pushWhitespaceStrippingElement(matchPat);
								}
							}
						}
						else if(!isAttrOK(aname, atts, i))
						{
								m_constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
						}
					}

					if(!foundIt)
					{
						XalanDOMString msg("(StylesheetHandler) " + XalanDOMString(name) +
						" requires a " + Constants::ATTRNAME_ELEMENTS + " attribute!");

						throw SAXException(toCharArray(msg));
					}
				}
				break;

				case Constants::ELEMNAME_KEY:
				{
					ElemEmpty nsContext(m_constructionContext, m_stylesheet, name, lineNumber, columnNumber);

					m_stylesheet.processKeyElement(&nsContext, atts, m_constructionContext);
				}
				break;

				case Constants::ELEMNAME_DEFINEATTRIBUTESET:
				{
					m_inTemplate = true; // fake it out

					ElemAttributeSet* attrSet = new ElemAttributeSet(m_constructionContext,
															   m_stylesheet,
															   name,
															   atts,
															   lineNumber,
															   columnNumber);
					m_elemStack.push_back(attrSet);
				}
				break;

				case Constants::ELEMNAME_INCLUDE:
					processInclude(name, atts);
					break;

				case Constants::ELEMNAME_IMPORT:
					processImport(name, atts);
					break;

				case Constants::ELEMNAME_OUTPUT:
					m_stylesheet.getStylesheetRoot().processOutputSpec(name, atts, m_constructionContext);
					break;

				case Constants::ELEMNAME_DECIMALFORMAT:
					{
						ElemDecimalFormat* const	edf =
							new ElemDecimalFormat(m_constructionContext,
												  m_stylesheet,
												  name,
												  atts,
												  lineNumber,
												  columnNumber);

						m_stylesheet.processDecimalFormatElement(edf, atts, m_constructionContext);
					}
					break;

				case Constants::ELEMNAME_NSALIAS:
					{
						m_stylesheet.processNSAliasElement(name, atts, m_constructionContext);
					}
					break;

				case Constants::ELEMNAME_WITHPARAM:
				case Constants::ELEMNAME_ATTRIBUTE:
				case Constants::ELEMNAME_APPLY_TEMPLATES:
				case Constants::ELEMNAME_USE:
				case Constants::ELEMNAME_CHILDREN:
				case Constants::ELEMNAME_CHOOSE:
				case Constants::ELEMNAME_COMMENT:
				case Constants::ELEMNAME_CONSTRUCT:
				case Constants::ELEMNAME_CONTENTS:
				case Constants::ELEMNAME_COPY:
				case Constants::ELEMNAME_COPY_OF:
				case Constants::ELEMNAME_DISPLAYIF:
				case Constants::ELEMNAME_EVAL:
				case Constants::ELEMNAME_EXPECTEDCHILDREN:
				case Constants::ELEMNAME_FOREACH:
				case Constants::ELEMNAME_IF:
				case Constants::ELEMNAME_CALLTEMPLATE:
				case Constants::ELEMNAME_MESSAGE:
				case Constants::ELEMNAME_NUMBER:
				case Constants::ELEMNAME_OTHERWISE:
				case Constants::ELEMNAME_PI:
				case Constants::ELEMNAME_REMOVEATTRIBUTE:
				case Constants::ELEMNAME_SORT:
				case Constants::ELEMNAME_TEXT:
				case Constants::ELEMNAME_VALUEOF:
				case Constants::ELEMNAME_WHEN:
				case Constants::ELEMNAME_ELEMENT:
				case Constants::ELEMNAME_COUNTER:
				case Constants::ELEMNAME_COUNTERS:
				case Constants::ELEMNAME_COUNTERINCREMENT:
				case Constants::ELEMNAME_COUNTERRESET:
				case Constants::ELEMNAME_COUNTERSCOPE:
				case Constants::ELEMNAME_APPLY_IMPORTS:
				{
					XalanDOMString msg("(StylesheetHandler) " + XalanDOMString(name) + " not allowed inside a stylesheet!");

					throw SAXException(toCharArray(msg));
				}
				// break;

				case Constants::ELEMNAME_STYLESHEET:
				{
					m_stylesheet.setWrapperless(false);
					m_foundStylesheet = true;

					const unsigned int	nAttrs = atts.getLength();

					bool				fVersionFound = false;

					// bool didSpecifiyIndent = false;	//doesn't seem to be used

					for(unsigned int i = 0; i < nAttrs; i++)
					{
						const XalanDOMChar* const	aname = atts.getName(i);

						if(equals(aname, XALAN_STATIC_UCODE_STRING("result-ns")))
						{
							throw SAXException("result-ns no longer supported!  Use xsl:output instead.");
						}
						else if(equals(aname, Constants::ATTRNAME_DEFAULTSPACE))
						{
							throw SAXException("default-space no longer supported!  Use xsl:strip-space or xsl:preserve-space instead.");
						}
						else if(equals(aname, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
						{
							m_constructionContext.warn(Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES
									+ " not supported yet!");

						/*
							@@ TODO: implement this ---
							StringToStringTable excluded =
								m_stylesheet.getExcludeResultPrefixes();
							excluded =
								m_stylesheet.processExcludeResultPrefixes(atts.getValue(i),
										excluded);
							m_stylesheet.setExcludeResultPrefixes(excluded);
						*/
						}
						else if(equals(aname, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
						{
							StringTokenizer tokenizer(atts.getValue(i),
													  XALAN_STATIC_UCODE_STRING(" \t\n\r"),
													  false);

							while(tokenizer.hasMoreTokens() == true)
							{
								const XalanDOMString	prefix = tokenizer.nextToken();
								// SANJIVA: ask Scott: is the line below correct?

								const XalanDOMString extns = m_stylesheet.getNamespaceForPrefixFromStack(prefix);

								ExtensionNSHandler* const	nsh = new ExtensionNSHandler(m_processor, extns);
								m_stylesheet.addExtensionNamespace(extns, nsh);
							}
						}
						else if(equals(aname, XALAN_STATIC_UCODE_STRING("id")))
						{
							//
						}
						else if(equals(aname, XALAN_STATIC_UCODE_STRING("indent-result")))
						{
							throw SAXException("indent-result no longer supported!  Use xsl:output instead.");
						}
						else if(equals(aname, XALAN_STATIC_UCODE_STRING("version")))
						{
							const XalanDOMChar* const	versionStr = atts.getValue(i);

							m_stylesheet.setXSLTVerDeclared(WideStringToDouble(versionStr));

							fVersionFound = true;
						}
						else if(!(isAttrOK(aname, atts, i) || processSpaceAttr(aname, atts, i)))
						{
							if(false == m_stylesheet.isWrapperless())
							{
								XalanDOMString msg("(StylesheetHandler) " + XalanDOMString(name) + 
											  " has an illegal attribute: " + aname);

								throw SAXException(toCharArray(msg));
							}
						}

						if(!m_stylesheet.getNamespaces().empty())
						{
							m_stylesheet.setNamespaceDecls(m_stylesheet.getNamespaces().back());
						}
					}

					if (fVersionFound == false)
					{
						const XalanDOMString		msg("The stylesheet element did not specify a version attribute!");

						throw SAXException(toCharArray(msg));
					}
				}
				break;

				default:
				{
					XalanDOMString msg("Unknown XSL element: " + localName);

					throw SAXException(toCharArray(msg));
				}
				break;

			}
		}
		else
		{
			switch(xslToken)
			{            
			case Constants::ELEMNAME_APPLY_TEMPLATES:
				elem = new ElemApplyTemplates(m_constructionContext,
											m_stylesheet,
											name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_CALLTEMPLATE:
				elem = new ElemCallTemplate(m_constructionContext,
										  m_stylesheet,
										  name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_WITHPARAM:
				elem = new ElemWithParam(m_constructionContext,
									   m_stylesheet,
									   name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_FOREACH:
				elem = new ElemForEach(m_constructionContext,
									 m_stylesheet,
									 name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_SORT:
				{
					ElemForEach* foreach = static_cast<ElemForEach*>(m_elemStack.back());

					ElemSort* sortElem = new ElemSort(m_constructionContext,
												 m_stylesheet,
												 name, atts, lineNumber, columnNumber);

					// Note: deleted in ElemForEach destructor
					foreach->getSortElems().push_back(sortElem);

					sortElem->setParentNodeElem(foreach);
				}
				break;

			case Constants::ELEMNAME_APPLY_IMPORTS:
				elem = new ElemApplyImport(m_constructionContext,
										 m_stylesheet,
										 name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_VALUEOF:
				elem = new ElemValueOf(m_constructionContext,
									 m_stylesheet,
									 name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_NUMBER:
				elem = new ElemNumber(m_constructionContext,
									m_stylesheet,
									name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_VARIABLE:
				elem = new ElemVariable(m_constructionContext,
									  m_stylesheet,
									  name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_PARAMVARIABLE:
				elem = new ElemParam(m_constructionContext,
								   m_stylesheet,
								   name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_IF:
				elem = new ElemIf(m_constructionContext,
								m_stylesheet,
								name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_CHOOSE:
				elem = new ElemChoose(m_constructionContext,
									m_stylesheet,
									name, atts, lineNumber, columnNumber);
				break;
          
			case Constants::ELEMNAME_WHEN:
				{
					ElemTemplateElement* const	parent = m_elemStack.back();

					if(Constants::ELEMNAME_CHOOSE == parent->getXSLToken())
					{
						ElemTemplateElement* const	lastChild = parent->getLastChildElem();

						if(0 == lastChild ||
							Constants::ELEMNAME_WHEN == lastChild->getXSLToken())
						{
							elem = new ElemWhen(m_constructionContext,
										m_stylesheet,
										name, atts, lineNumber, columnNumber);
						}
						else
						{
							throw SAXException("(StylesheetHandler) misplaced xsl:when!");
						}
					}
					else
					{
						throw SAXException("(StylesheetHandler) xsl:when not parented by xsl:choose!");
					}
				}
				break;
          
			case Constants::ELEMNAME_OTHERWISE:
				{
					ElemTemplateElement* parent = m_elemStack.back();

					if(Constants::ELEMNAME_CHOOSE == parent->getXSLToken())
					{
						ElemTemplateElement* lastChild = parent->getLastChildElem();

						if(0 == lastChild || 
							Constants::ELEMNAME_WHEN == lastChild->getXSLToken())
						{
							elem = new ElemOtherwise(m_constructionContext,
											 m_stylesheet,
											 name, atts, lineNumber, columnNumber);
						}
						else
						{
							throw SAXException("(StylesheetHandler) misplaced xsl:otherwise!");
						}
					}
					else
					{
						throw SAXException("(StylesheetHandler) xsl:otherwise not parented by xsl:choose!");
					}
				}
				break;

			case Constants::ELEMNAME_COPY_OF:
				elem = new ElemCopyOf(m_constructionContext,
									m_stylesheet,
									name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_COPY:
				elem = new ElemCopy(m_constructionContext,
								  m_stylesheet,
								  name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_TEXT:
			  // Just push the element on the stack to signal
			  // that space should be preserved.
				m_elemStack.push_back(new ElemText(m_constructionContext,
										m_stylesheet,
										name, atts, lineNumber, columnNumber));
				break;

			case Constants::ELEMNAME_USE:
				elem = new ElemUse(m_constructionContext,
								 m_stylesheet,
								 name, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_ATTRIBUTE:
				elem = new ElemAttribute(m_constructionContext,
									   m_stylesheet,
									   name, atts, lineNumber, columnNumber);
				break;

			case Constants::ELEMNAME_ELEMENT:
				elem = new ElemElement(m_constructionContext,
									 m_stylesheet,
									 name, atts, lineNumber, columnNumber);
			  break;
          
			case Constants::ELEMNAME_PI:
				elem = new ElemPI(m_constructionContext,
								m_stylesheet,
								name, atts, lineNumber, columnNumber);
			  break;

			case Constants::ELEMNAME_COMMENT:
				elem = new ElemComment(m_constructionContext,
									 m_stylesheet,
									 name, atts, lineNumber, columnNumber);
			  break;
          
			case Constants::ELEMNAME_MESSAGE:
				elem = new ElemMessage(m_constructionContext,
									 m_stylesheet,
									 name, atts, lineNumber, columnNumber);

				break;
          
			case Constants::ELEMNAME_TEMPLATE:
			case Constants::ELEMNAME_LOCALE:
			case Constants::ELEMNAME_DEFINEATTRIBUTESET:
			case Constants::ELEMNAME_DEFINESCRIPT:
			case Constants::ELEMNAME_EXTENSION:
			case Constants::ELEMNAME_EXTENSIONHANDLER:
			case Constants::ELEMNAME_KEY:
			case Constants::ELEMNAME_IMPORT:
			case Constants::ELEMNAME_INCLUDE:
			case Constants::ELEMNAME_PRESERVESPACE:
			case Constants::ELEMNAME_STRIPSPACE:
				{
					XalanDOMString msg("(StylesheetHandler) " + XalanDOMString(name) + " is not allowed inside a template!");
					throw SAXException(toCharArray(msg));
				}
				break;

			default:
				{
					XalanDOMString msg("Unknown XSL element: " + localName);

					// If this stylesheet is declared to be of a higher version than the one
					// supported, don't flag an error.
					if(XSLTEngineImpl::getXSLTVerSupported() < m_stylesheet.getXSLTVerDeclared())
					{
						m_constructionContext.warn(msg);
					}
					else
					{
						throw SAXException(toCharArray(msg));
					}
				}
			}
		  }
		}
		// BEGIN SANJIVA CODE
		else if (!m_inTemplate && startsWith(ns,m_processor.getXalanXSLNameSpaceURL()))
		{
			if (equals(localName, XALAN_STATIC_UCODE_STRING("component")))
			{
				XalanDOMString prefix;
				XalanDOMString elements;
				XalanDOMString functions;

				const int nAttrs = atts.getLength();

				for (int i = 0; i < nAttrs; i++)
				{
					const XalanDOMChar* const	aname = atts.getName (i);

					if (equals(aname, XALAN_STATIC_UCODE_STRING("prefix")))
					{
						prefix = atts.getValue (i);
					}
					else if (equals(aname, XALAN_STATIC_UCODE_STRING("elements")))
					{
						elements = atts.getValue (i);
					}
					else if (equals(aname, XALAN_STATIC_UCODE_STRING("functions")))
					{
						functions = atts.getValue (i);
					}
					else if(!isAttrOK(aname, atts, i))
					{
						m_constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
					}
				}

				if (isEmpty(prefix)) 
				{
					XalanDOMString msg("StylesheetHandler) " + XalanDOMString(name) + " attribute 'prefix' is missing");

					throw SAXException(toCharArray(msg));
				}

				// SCOTT: is the line below correct?
				XalanDOMString extns = m_stylesheet.getNamespaceForPrefixFromStack (prefix);
				ExtensionNSHandler* nsh = m_stylesheet.lookupExtensionNSHandler(extns);

				if (nsh == 0) 
				{
					XalanDOMString msg("(StylesheetHandler) " + XalanDOMString(name) + " extension namespace prefix '" + prefix + "' unknown");

					throw SAXException(toCharArray(msg));
				}

				if (!isEmpty(elements)) 
				{
					nsh->setElements(elements);
				}

				if (!isEmpty(functions)) 
				{
					nsh->setFunctions(functions);
				}

				m_pLXSLTExtensionNSH = nsh; // hang on to it for processing 
				// endElement on lxslt:script
			}
			else if (equals(localName, XALAN_STATIC_UCODE_STRING("script"))) 
			{
				// process this in end element so that I can see whether I had 
				// a body as well. The default pushing logic will save the 
				// attributes for me. The body will be accumulated into the
				// following string buffer
				m_inLXSLTScript = true;
				m_LXSLTScriptBody = XalanDOMString();

				const int	nAttrs = atts.getLength();

				for (int i = 0; i < nAttrs; i++) 
				{

					const XalanDOMChar* const	aname = atts.getName(i);

					if (equals(aname, XALAN_STATIC_UCODE_STRING("lang")))
					{
						m_LXSLTScriptLang = atts.getValue (i);
					}
					else if (equals(aname, XALAN_STATIC_UCODE_STRING("src")))
					{
						m_LXSLTScriptSrcURL = atts.getValue (i);
					}
					else if(!isAttrOK(aname, atts, i))
					{
						m_constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
					}
				}
			}
			else 
			{
				// other xslt4j: element. Not my business.
			}
		}
		// END SANJIVA CODE
		else
		{
			if(!m_inTemplate && !m_foundStylesheet)
			{
				elem = initWrapperless(name, atts, lineNumber, columnNumber);
			}
			else
			{
				// BEGIN SANJIVA CODE
				// is this an extension element call?
				ExtensionNSHandler* nsh = 0;

				if (!isEmpty(ns) && 
					((nsh = m_stylesheet.lookupExtensionNSHandler (ns)) != 0)) 
				{
					elem = new ElemExtensionCall (m_constructionContext,
											m_stylesheet,
											name,
											atts,
											lineNumber,
											columnNumber,
											*nsh,
											localName);
				}
				else 
				{
					elem = new ElemLiteralResult(m_constructionContext,
										   m_stylesheet,
										   name,
										   atts,
										   lineNumber,
										   columnNumber);
				}
				// BEGIN SANJIVA CODE
			}
			// END SANJIVA CODE
		}

		if(m_inTemplate && 0 != elem)
		{
			if(!m_elemStack.empty())
			{
				ElemTemplateElement* const	parent = m_elemStack.back();
				parent->appendChildElem(elem);
			}

			m_elemStack.push_back(elem);
		}

		// If for some reason something didn't get pushed, push an empty 
		// object.
		if(origStackSize == m_elemStack.size())
		{
			m_elemStack.push_back(new ElemEmpty(m_constructionContext,
									 m_stylesheet,
									 name, lineNumber, columnNumber));
		}

	} // end try

	// Here's the story.  startElement throws exceptions for certain malformed constructs.  These
	// exceptions need to reach the end user.  But the parser eats all exceptions and we lose
	// the error messages and exit silently.  So, I'll eat the exceptions first, store the message
	// and then throw the exception during endDocument
	catch(SAXException& e)
	{
		m_exceptionPending = true;

		m_pendingException = e.getMessage();

		m_elemStack.clear();
	}
	catch(...)
	{
		// $$$ ToDo: This probably should't happen, but it does...
		m_exceptionPending = true;

		m_elemStack.clear();

		throw;
	}
}


ElemTemplateElement* StylesheetHandler::initWrapperless (const XalanDOMString& name,
	const AttributeList& atts, int lineNumber, int columnNumber)
{
	m_stylesheet.getStylesheetRoot().initDefaultRule(m_constructionContext);

	AttributeListImpl templateAttrs;

	templateAttrs.addAttribute(c_wstr(XALAN_STATIC_UCODE_STRING("name")),
							   c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")),
							   c_wstr(XALAN_STATIC_UCODE_STRING("simple")));

	m_pTemplate = new ElemTemplate(m_constructionContext,
								   m_stylesheet,
								   XALAN_STATIC_UCODE_STRING("xsl:template"),
								   templateAttrs,
								   lineNumber,
								   columnNumber);

	ElemTemplateElement* const	pElem =
				new ElemLiteralResult(m_constructionContext,
									  m_stylesheet,
									  name,
									  atts,
									  lineNumber,
									  columnNumber);

	m_pTemplate->appendChildElem(pElem);
	m_inTemplate = true;
	
	m_stylesheet.setWrapperlessTemplate(m_pTemplate);

	m_foundStylesheet = true;
	m_stylesheet.setWrapperless(true);

	if(equalsIgnoreCase(name, Constants::ELEMNAME_HTML_STRING) == true)
	{
		m_stylesheet.getStylesheetRoot().setIndentResult(true);
		m_stylesheet.getStylesheetRoot().setOutputMethod(OUTPUT_METHOD_HTML);
	}

	return pElem;
}



static bool 
stackContains(
			const Stylesheet::URLStackType&		stack, 
			const XMLURL&						url)
{
	const int	n = stack.size();

	bool		contains = false;

	for(int i = 0; i < n && contains == false; i++)
	{
		if(*stack[i] == url)
		{
			contains = true;
		}
	}

	return contains;	
}


void
StylesheetHandler::processImport(
			const XalanDOMChar*		name,
			const AttributeList&	atts)
{
	const unsigned int	nAttrs = atts.getLength();

	bool				foundIt = false;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_HREF))
		{
			foundIt = true;
			
			if(m_foundNotImport)
			{
				throw SAXException("Imports can only occur as the first elements in the stylesheet!");
			}
			
			const XalanDOMString	saved_XSLNameSpaceURL = m_stylesheet.getXSLTNamespaceURI();

			const XalanDOMString	href = atts.getValue(i);

			Stylesheet::URLStackType& includeStack = m_stylesheet.getIncludeStack();
			assert(includeStack.size() > 0);

			typedef StylesheetConstructionContext::URLAutoPtrType	URLAutoPtrType;

			URLAutoPtrType	hrefUrl = m_constructionContext.getURLFromString(href, includeStack.back()->getURLText());
			assert(hrefUrl.get() != 0);

			Stylesheet::URLStackType& importStack = m_stylesheet.getStylesheetRoot().getImportStack();

			if(stackContains(importStack, *hrefUrl.get()))
			{
				XalanDOMString msg(XalanDOMString(hrefUrl->getURLText()) + " is directly or indirectly importing itself!");

				throw SAXException(toCharArray(msg));
			}

			importStack.push_back(hrefUrl.get());
			hrefUrl.release();

			const XalanDOMString	theImportURI(hrefUrl->getURLText());

			Stylesheet* pImportedStylesheet = new Stylesheet(
				m_stylesheet.getStylesheetRoot(), 
				theImportURI,
				m_constructionContext);

			StylesheetHandler tp(m_processor, *pImportedStylesheet, m_constructionContext);

//			pImportedStylesheet->setBaseIdentifier();

			m_processor.parseXML(*hrefUrl, &tp, pImportedStylesheet);

			// I'm going to insert the elements in backwards order, 
			// so I can walk them 0 to n.
			m_stylesheet.getImports().insert(m_stylesheet.getImports().begin(),
				pImportedStylesheet);

			importStack.pop_back();
			
			m_stylesheet.setXSLTNamespaceURI(saved_XSLNameSpaceURL);
		}
		else if(!isAttrOK(aname, atts, i))
		{
			m_constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
		}
	}

	if(!foundIt)
	{
		XalanDOMString msg("Could not find href attribute for " + XalanDOMString(name));
		throw SAXException(toCharArray(msg));
	}
}


void
StylesheetHandler::processInclude(
			const XalanDOMChar*		name,
			const AttributeList&	atts)
{
	const unsigned int	nAttrs = atts.getLength();

	bool				foundIt = false;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_HREF))
		{
			foundIt = true;
			
			PushPopIncludeState		theStateHandler(*this);

			const XalanDOMString	href = atts.getValue(i);

			typedef StylesheetConstructionContext::URLAutoPtrType	URLAutoPtrType;

			assert(m_stylesheet.getIncludeStack().back() != 0);
			URLAutoPtrType	hrefUrl = m_constructionContext.getURLFromString(href, m_stylesheet.getIncludeStack().back()->getURLText());

			if(stackContains(m_stylesheet.getIncludeStack(), *hrefUrl.get()))
			{
				XalanDOMString msg(XalanDOMString(hrefUrl->getURLText()) + " is directly or indirectly including itself!");

				throw SAXException(toCharArray(msg));
			}

			m_stylesheet.getIncludeStack().push_back(hrefUrl.get());
			hrefUrl.release();

			m_processor.parseXML(*hrefUrl, this, &m_stylesheet);

			m_stylesheet.getIncludeStack().pop_back();
		}
		else if(!isAttrOK(aname, atts, i))
		{
			m_constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
		}
	}

	if(!foundIt)
	{
		XalanDOMString msg("Could not find href attribute for " + XalanDOMString(name));

		throw SAXException(toCharArray(msg));
	}
}



void StylesheetHandler::endElement(const XMLCh* const name) 
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

	m_stylesheet.popNamespaces();

	m_lastPopped = m_elemStack.back();
	m_elemStack.pop_back();
	m_lastPopped->setFinishedConstruction(true);

	const int tok = m_lastPopped->getXSLToken();

	if(Constants::ELEMNAME_TEMPLATE == tok)
	{
		m_inTemplate = false;
	}
	else if((Constants::ELEMNAME_PARAMVARIABLE == tok) ||
		Constants::ELEMNAME_VARIABLE == tok)
	{
		ElemVariable* const		var = static_cast<ElemVariable*>(m_lastPopped);

		if(var->isTopLevel())
		{
			// Top-level param or variable
			m_inTemplate = false;
		}
	}
	else if(Constants::ELEMNAME_DEFINEATTRIBUTESET == tok)
	{
		m_inTemplate = false;
	}
	else if (tok == Constants::ELEMNAME_UNDEFINED ||
		tok == Constants::ELEMNAME_TEXT)
	{
		// These are stray elements, so stuff them away
		// to be deleted when we're finished...
		m_strayElements.push_back(m_lastPopped);
	}

	// BEGIN SANJIVA CODE
	if (m_inLXSLTScript) 
	{
		if (isEmpty(m_LXSLTScriptLang)) 
		{
			XalanDOMString msg(XalanDOMString(name) + " attribute \'lang\' is missing");

			throw SAXException(toCharArray(msg));
		}
		if (m_pLXSLTExtensionNSH == 0) 
		{
			XalanDOMString msg("(StylesheetHandler) misplaced " + XalanDOMString(name) + " element?? Missing container element " + "'component'");

			throw SAXException(toCharArray(msg));
		}

		m_pLXSLTExtensionNSH->setScript(m_LXSLTScriptLang, m_LXSLTScriptSrcURL, m_LXSLTScriptBody);
		
		// reset state
		m_inLXSLTScript = false;
		m_LXSLTScriptLang = 0;
		m_LXSLTScriptSrcURL = 0;
		m_LXSLTScriptBody = 0;
		m_pLXSLTExtensionNSH = 0;
	}
	// END SANJIVA CODE
}


void StylesheetHandler::characters (const XMLCh* const chars, const unsigned int length)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

	if(m_inTemplate)
	{
		ElemTemplateElement*	parent = m_elemStack.back();

		bool					preserveSpace = false;
		bool					disableOutputEscaping = false;

		if(Constants::ELEMNAME_TEXT == parent->getXSLToken())
		{
			disableOutputEscaping = static_cast<ElemText*>(parent)->getDisableOutputEscaping();
			parent = m_elemStack[m_elemStack.size()-2];
			preserveSpace = true;
		}

		const Locator* const	locator = m_processor.getLocatorFromStack();

		const int				lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
		const int				columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;

		ElemTextLiteral *elem = new ElemTextLiteral(m_constructionContext,
			m_stylesheet,
			lineNumber, columnNumber,
			chars, 0, length, 
			false, preserveSpace, 
			disableOutputEscaping);

		const bool isWhite = isWhiteSpace(chars, 0, length);

		if(preserveSpace || (!preserveSpace && !isWhite))
		{
			while(!m_whiteSpaceElems.empty())
			{
				parent->appendChildElem(m_whiteSpaceElems.back());

				m_whiteSpaceElems.pop_back();
			}

			parent->appendChildElem(elem);

			elem = 0;
		}
		else if(isWhite)
		{
			bool						shouldPush = true;

			ElemTemplateElement* const	last = parent->getLastChildElem();

			if(0 != last)
			{
				// If it was surrounded by xsl:text, it will count as an element.
				bool isPrevCharData =
					Constants::ELEMNAME_TEXTLITERALRESULT == last->getXSLToken();
				bool isLastPoppedXSLText = (m_lastPopped != 0) &&
					(Constants::ELEMNAME_TEXT == m_lastPopped->getXSLToken());

				if(isPrevCharData && ! isLastPoppedXSLText)
				{
					parent->appendChildElem(elem);

					shouldPush = false;
				}
			}

			if(shouldPush)
				m_whiteSpaceElems.push_back(elem);
		}
	}
	// BEGIN SANJIVA CODE
	else if (m_inLXSLTScript)
	{
		XalanDOMString tmpStr(chars, length);
		append(m_LXSLTScriptBody,tmpStr);
	}
	// END SANJIVA CODE
	// TODO: Flag error if text inside of stylesheet

//	m_lastPopped = 0;
}



void StylesheetHandler::cdata(const XMLCh* const chars, const unsigned int length)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

	if(m_inTemplate)
	{
		ElemTemplateElement*	parent = m_elemStack.back();

		bool					preserveSpace = false;
		bool					disableOutputEscaping = false;

		if(Constants::ELEMNAME_TEXT == parent->getXSLToken())
		{
			disableOutputEscaping = static_cast<ElemText*>(parent)->getDisableOutputEscaping();
			parent = m_elemStack[m_elemStack.size()-2];
			preserveSpace = true;
		}

		const Locator* const	locator = m_processor.getLocatorFromStack();

		const int lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
		const int columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;

		ElemTextLiteral* elem = new ElemTextLiteral(m_constructionContext,
			m_stylesheet,
			lineNumber, columnNumber,
			chars, 0, length,
			true, preserveSpace, 
			disableOutputEscaping);

		bool isWhite = isWhiteSpace(chars, 0, length);

		if(preserveSpace || (!preserveSpace && !isWhite))
		{
			while(!m_whiteSpaceElems.empty())
			{
				parent->appendChildElem(m_whiteSpaceElems.back());

				m_whiteSpaceElems.pop_back();
			}

			parent->appendChildElem(elem);
		}
		else if(isWhite)
		{
			bool						shouldPush = true;

			ElemTemplateElement* const	last = parent->getLastChildElem();

			if(0 != last)
			{
				// If it was surrounded by xsl:text, it will count as an element.
				bool isPrevCharData =
					Constants::ELEMNAME_TEXTLITERALRESULT == last->getXSLToken();
				bool isLastPoppedXSLText = (m_lastPopped != 0) &&
						(Constants::ELEMNAME_TEXT == m_lastPopped->getXSLToken());

				if(isPrevCharData && ! isLastPoppedXSLText)
				{
					parent->appendChildElem(elem);

					shouldPush = false;
				}
			}

			if(shouldPush)
				m_whiteSpaceElems.push_back(elem);
		}
	}
	// BEGIN SANJIVA CODE
	else if (m_inLXSLTScript)
	{
		append(m_LXSLTScriptBody, chars);
	}
	// END SANJIVA CODE
	// TODO: Flag error if text inside of stylesheet

	m_lastPopped = 0;
}


void StylesheetHandler::ignorableWhitespace (const XMLCh* const /*chars*/, const unsigned int /*length*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

  // Ignore!

	m_lastPopped = 0;
}


void StylesheetHandler::processingInstruction (const XMLCh* const /*target*/, const XMLCh* const /*data*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

	m_lastPopped = 0;
}


void StylesheetHandler::comment(const XMLCh* const /*data*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

  // No action for the moment.
}


void StylesheetHandler::entityReference(const XMLCh* const /*name*/)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

  // No action for the moment.
}


void StylesheetHandler::resetDocument()
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

  // No action for the moment.
}


void
StylesheetHandler::charactersRaw(const XMLCh* const /* chars */, const unsigned int	/* length */)
{
	// if we have apending exception, we don't want to even try to process this
	if (m_exceptionPending == true)
		return;

  // No action for the moment.
}



StylesheetHandler::PushPopIncludeState::PushPopIncludeState(StylesheetHandler&	theHandler) :
	m_handler(theHandler),
	m_elemStack(theHandler.m_elemStack),
	m_pTemplate(theHandler.m_pTemplate),
	m_lastPopped(theHandler.m_lastPopped),
	m_inTemplate(theHandler.m_inTemplate),
	m_foundStylesheet(theHandler.m_foundStylesheet),
	m_XSLNameSpaceURL(theHandler.m_stylesheet.getXSLTNamespaceURI()),
	m_foundNotImport(theHandler.m_foundNotImport)
{
	m_handler.m_elemStack.clear();
	m_handler.m_pTemplate = 0;
	m_handler.m_lastPopped = 0;
	m_handler.m_inTemplate = false;
	m_handler.m_foundStylesheet = false;
	m_handler.m_foundNotImport = false;
}



StylesheetHandler::PushPopIncludeState::~PushPopIncludeState()
{
	// Clean up the element stack vector
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up the element stack vector
	for_each(m_handler.m_elemStack.begin(),
			 m_handler.m_elemStack.end(),
			 DeleteFunctor<ElemTemplateElement>());

	m_handler.m_elemStack = m_elemStack;
	m_handler.m_pTemplate = m_pTemplate;
	m_handler.m_lastPopped = m_lastPopped;
	m_handler.m_inTemplate = m_inTemplate;
	m_handler.m_foundStylesheet = m_foundStylesheet;
	m_handler.m_stylesheet.setXSLTNamespaceURI(m_XSLNameSpaceURL);
	m_handler.m_foundNotImport = m_foundNotImport;
}
