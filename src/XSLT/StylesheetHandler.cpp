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
#include "StylesheetHandler.hpp"



#include <algorithm>



#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax/SAXParseException.hpp>



#include <Include/STLHelper.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/XalanLocator.hpp>



#include <DOMSupport/DOMServices.hpp>



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
#include "ElemFallback.hpp"
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



#include <Include/XalanAutoPtr.hpp>



StylesheetHandler::StylesheetHandler(
			Stylesheet&						stylesheetTree,
			StylesheetConstructionContext&	constructionContext) :
	FormatterListener(OUTPUT_METHOD_OTHER),
	m_stylesheet(stylesheetTree),
	m_constructionContext(constructionContext),
	m_elemStack(),
	m_elemStackParentedElements(),
	m_strayElements(),
	m_whiteSpaceElems(),
	m_pTemplate(0),
	m_lastPopped(0),	
	m_inTemplate(false),
	m_foundStylesheet(false),
	m_foundNotImport(false),
	m_elementLocalName(),
	m_accumulateText(),
	m_includeBase(stylesheetTree.getBaseIdentifier()),
	m_inExtensionElementStack(),
	m_inLXSLTScript(false),
	m_LXSLTScriptBody(),
	m_LXSLTScriptLang(),
	m_LXSLTScriptSrcURL(),
	m_pLXSLTExtensionNSH(0),
	m_locatorsPushed(0),
	m_globalVariableNames(),
	m_inScopeVariableNamesStack()
{
	m_inScopeVariableNamesStack.reserve(eVariablesStackDefault);
}



StylesheetHandler::~StylesheetHandler()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up the element stack vector...
	ElemTemplateStackType::const_iterator	i = m_elemStack.begin();

	while(i != m_elemStack.end())
	{
		// See if the element is in the set of elements that have
		// already been parented, so we don't try to delete it after
		// it's parent has already deleted it...
		const ElemTemplateSetType::iterator		j =
				m_elemStackParentedElements.find(*i);

		if (j == m_elemStackParentedElements.end())
		{
			// Not found, so delete it...
			delete *i;
		}
		else
		{
			// Found, so erase it from the set...
			m_elemStackParentedElements.erase(j);
		}

		++i;
	}

	doCleanup();

	// Clean up the whitespace elements.
	for_each(m_whiteSpaceElems.begin(),
			 m_whiteSpaceElems.end(),
			 DeleteFunctor<ElemTextLiteral>());

	// Clean up the stray elements.
	for_each(m_strayElements.begin(),
			 m_strayElements.end(),
			 DeleteFunctor<ElemTemplateElement>());

	// Clean up any template that's left over...
	if (m_pTemplate != m_stylesheet.getWrapperlessTemplate())
	{
		delete m_pTemplate;
	}

	m_elemStackParentedElements.clear();
}



void StylesheetHandler::setDocumentLocator(const Locator* const		locator)
{
	m_constructionContext.pushLocatorOnStack(locator);

	++m_locatorsPushed;
}


void StylesheetHandler::startDocument()
{
}


void StylesheetHandler::endDocument()
{
	m_constructionContext.popLocatorStack();

	if (m_locatorsPushed > 0)
	{
		--m_locatorsPushed;
	}

	m_inExtensionElementStack.clear();
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
			const XalanDOMChar*				aname,
			const AttributeList&			atts,
			int								which,
			const Locator*					locator,
			bool&							fPreserve)
{
	if(m_constructionContext.isXMLSpaceAttribute(aname, m_stylesheet, locator) == false)
	{
		fPreserve = false;

		return false;
	}
	else
	{
		const XalanDOMChar*	const	spaceVal = atts.getValue(which);

		if(equals(spaceVal, Constants::ATTRVAL_DEFAULT))
		{
			fPreserve = false;
		}
		else if(equals(spaceVal, Constants::ATTRVAL_PRESERVE))
		{
			fPreserve = true;
		}
		else
		{
			error("xml:space has an illegal value", locator);
		}

		return true;
	}
}



inline int
getLineNumber(const Locator*	theLocator)
{
	return theLocator == 0 ? -1 : theLocator->getLineNumber();
}



inline int
getColumnNumber(const Locator*	theLocator)
{
	return theLocator == 0 ? -1 : theLocator->getColumnNumber();
}



bool
StylesheetHandler::processSpaceAttr(
			const AttributeList&			atts,
			const Locator*					locator,
			bool&							fPreserve)
{
	const unsigned int	len = atts.getLength();

	for (unsigned int i = 0; i < len; ++i)
	{
		if (processSpaceAttr(atts.getName(i), atts, i, locator, fPreserve) == true)
		{
			return true;
		}
	}

	return false;
}



void
StylesheetHandler::startElement(
			const XMLCh* const	name,
			AttributeList&		atts)
{
	m_inExtensionElementStack.push_back(false);

	if (m_preserveSpaceStack.empty() == true)
	{
		m_preserveSpaceStack.push_back(false);
	}
	else
	{
		m_preserveSpaceStack.push_back(m_preserveSpaceStack.back());
	}

	try
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::for_each;
#endif

		// By default, space is not preserved...
		bool	fPreserveSpace = false;
		bool	fSpaceAttrProcessed = false;

		processAccumulatedText();

		// Clean up the whitespace elements.
		for_each(m_whiteSpaceElems.begin(),
				 m_whiteSpaceElems.end(),
				 DeleteFunctor<ElemTextLiteral>());

		m_whiteSpaceElems.clear();

		const Locator* const	locator = m_constructionContext.getLocatorFromStack();

		const XalanLocator::size_type	lineNumber = getLineNumber(locator);
		const XalanLocator::size_type	columnNumber = getColumnNumber(locator);

		// First push namespaces
		m_stylesheet.pushNamespaces(atts);

		const XalanDOMString&	ns = getNamespaceFromStack(name);

		const XalanDOMString::size_type		nameLength = length(name);
		const XalanDOMString::size_type		index = indexOf(name, XalanUnicode::charColon);

		if(length(ns) == 0 && index < nameLength)
		{
			error("Could not resolve prefix.", locator);
		}

		if (index < nameLength)
		{
			m_elementLocalName.assign(name + index + 1, nameLength - index - 1);
		}
		else
		{
			m_elementLocalName.assign(name, nameLength);
		}

		ElemTemplateElement* elem = 0;

		const ElemTemplateStackType::size_type	origStackSize = m_elemStack.size();

		if(equals(ns, m_constructionContext.getXSLTNamespaceURI()))
		{
			if(!isEmpty(m_stylesheet.getXSLTNamespaceURI()))
				m_stylesheet.setXSLTNamespaceURI(ns);

			if(false == m_foundStylesheet)
			{
				m_stylesheet.getStylesheetRoot().initDefaultRule(m_constructionContext);
				m_stylesheet.setWrapperless(false);
			}

			const int	xslToken = m_constructionContext.getElementToken(m_elementLocalName);

			if(!m_inTemplate)
			{
				processTopLevelElement(name, m_elementLocalName, ns, atts, xslToken, locator, fPreserveSpace, fSpaceAttrProcessed);
			}
			else
			{
				switch(xslToken)
				{            
				case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
					elem = new ElemApplyTemplates(m_constructionContext,
												m_stylesheet,
												atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
					elem = new ElemCallTemplate(m_constructionContext,
											  m_stylesheet,
											  atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_WITH_PARAM:
					elem = new ElemWithParam(m_constructionContext,
										   m_stylesheet,
										   atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
					elem = new ElemForEach(m_constructionContext,
										 m_stylesheet,
										 atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_SORT:
					{
						if (m_elemStack.empty() == true)
						{
							error("Misplaced xsl:sort.", locator);
						}

						ElemTemplateElement* const	theElement =
								m_elemStack.back();
						assert(theElement != 0);

						const int	xslToken = theElement->getXSLToken();

						if (xslToken != StylesheetConstructionContext::ELEMNAME_FOR_EACH &&
							xslToken != StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES)
						{
							error("Misplaced xsl:sort.", locator);
						}

						ElemForEach* foreach =
#if defined(XALAN_OLD_STYLE_CASTS)
								(ElemForEach*)theElement;
#else
								static_cast<ElemForEach*>(theElement);
#endif

						ElemSort* sortElem = new ElemSort(m_constructionContext,
														 m_stylesheet,
														 atts, lineNumber, columnNumber);

						// Note: deleted in ElemForEach destructor
						foreach->getSortElems().push_back(sortElem);

						sortElem->setParentNodeElem(foreach);

						m_elemStackParentedElements.insert(foreach);

						m_elemStack.push_back(new ElemEmpty(m_constructionContext,
									 m_stylesheet,
									 lineNumber, columnNumber, &Constants::ELEMNAME_SORT_WITH_PREFIX_STRING));
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
					{
						if (m_elemStack.empty() == true)
						{
							error("xsl:apply-imports is not allowed at this position in the stylesheet", locator);
						}

						ElemTemplateElement* const	theElement =
								m_elemStack.back();
						assert(theElement != 0);

						const int	xslToken = theElement->getXSLToken();

						if (xslToken == StylesheetConstructionContext::ELEMNAME_FOR_EACH)
						{
							error("xsl:apply-imports is not allowed at this position in the stylesheet", locator);
						}

						elem = new ElemApplyImport(m_constructionContext,
												 m_stylesheet,
												 atts, lineNumber, columnNumber);
					}
					break;
          
				case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
					elem = new ElemValueOf(m_constructionContext,
										 m_stylesheet,
										 atts, lineNumber, columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_NUMBER:
					elem = new ElemNumber(
							m_constructionContext,
							m_stylesheet,
							atts,
							lineNumber,
							columnNumber,
							m_stylesheet.getStylesheetRoot().getNextElemNumberID());
					break;
          
				case StylesheetConstructionContext::ELEMNAME_VARIABLE:
					{
						XalanAutoPtr<ElemVariable>	newVar(
							new ElemVariable(
									m_constructionContext,
									m_stylesheet,
									atts,
									lineNumber,
									columnNumber));

						checkForOrAddVariableName(newVar->getName(), locator);

						elem = newVar.release();
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_PARAM:
					elem = new ElemParam(m_constructionContext,
									   m_stylesheet,
									   atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_IF:
					elem = new ElemIf(m_constructionContext,
									m_stylesheet,
									atts, lineNumber, columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_FALLBACK:
					elem = new ElemFallback(
							m_constructionContext,
							m_stylesheet,
							atts,
							lineNumber,
							columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_CHOOSE:
					elem = new ElemChoose(m_constructionContext,
										m_stylesheet,
										atts, lineNumber, columnNumber);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_WHEN:
					{
						ElemTemplateElement* const	parent = m_elemStack.back();

						if(StylesheetConstructionContext::ELEMNAME_CHOOSE == parent->getXSLToken())
						{
							ElemTemplateElement* const	lastChild = parent->getLastChildElem();

							if(0 == lastChild ||
								StylesheetConstructionContext::ELEMNAME_WHEN == lastChild->getXSLToken() ||
								lastChild->isWhitespace() == true)
							{
								elem = new ElemWhen(m_constructionContext,
											m_stylesheet,
											atts, lineNumber, columnNumber);
							}
							else
							{
								error("(StylesheetHandler) misplaced xsl:when.", locator);
							}
						}
						else
						{
							error("(StylesheetHandler) xsl:when not parented by xsl:choose.", locator);
						}
					}
					break;
          
				case StylesheetConstructionContext::ELEMNAME_OTHERWISE:
					{
						ElemTemplateElement* parent = m_elemStack.back();

						if(StylesheetConstructionContext::ELEMNAME_CHOOSE == parent->getXSLToken())
						{
							ElemTemplateElement* lastChild = parent->getLastChildElem();

							if(0 == lastChild ||
								StylesheetConstructionContext::ELEMNAME_WHEN == lastChild->getXSLToken() ||
								lastChild->isWhitespace() == true)
							{
								elem = new ElemOtherwise(m_constructionContext,
												 m_stylesheet,
												 atts, lineNumber, columnNumber);
							}
							else
							{
								error("Misplaced xsl:otherwise.", locator);
							}
						}
						else
						{
							error("xsl:otherwise not parented by xsl:choose.", locator);
						}
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_COPY_OF:
					elem = new ElemCopyOf(m_constructionContext,
										m_stylesheet,
										atts, lineNumber, columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_COPY:
					elem = new ElemCopy(m_constructionContext,
									  m_stylesheet,
									  atts, lineNumber, columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_TEXT:
				  // Just push the element on the stack to signal
				  // that space should be preserved.
					m_elemStack.push_back(new ElemText(m_constructionContext,
											m_stylesheet,
											atts, lineNumber, columnNumber));
					break;

				case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
					elem = new ElemAttribute(m_constructionContext,
										   m_stylesheet,
										   atts, lineNumber, columnNumber);
					break;

				case StylesheetConstructionContext::ELEMNAME_ELEMENT:
					elem = new ElemElement(m_constructionContext,
										 m_stylesheet,
										 atts, lineNumber, columnNumber);
				  break;
          
				case StylesheetConstructionContext::ELEMNAME_PI:
					elem = new ElemPI(m_constructionContext,
									m_stylesheet,
									atts, lineNumber, columnNumber);
				  break;

				case StylesheetConstructionContext::ELEMNAME_COMMENT:
					elem = new ElemComment(m_constructionContext,
										 m_stylesheet,
										 atts, lineNumber, columnNumber);
				  break;
          
				case StylesheetConstructionContext::ELEMNAME_MESSAGE:
					elem = new ElemMessage(m_constructionContext,
										 m_stylesheet,
										 atts, lineNumber, columnNumber);

					break;
          
				case StylesheetConstructionContext::ELEMNAME_TEMPLATE:
				case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET:
				case StylesheetConstructionContext::ELEMNAME_EXTENSION:
				case StylesheetConstructionContext::ELEMNAME_EXTENSION_HANDLER:
				case StylesheetConstructionContext::ELEMNAME_KEY:
				case StylesheetConstructionContext::ELEMNAME_IMPORT:
				case StylesheetConstructionContext::ELEMNAME_INCLUDE:
				case StylesheetConstructionContext::ELEMNAME_PRESERVE_SPACE:
				case StylesheetConstructionContext::ELEMNAME_STRIP_SPACE:
					{
						const XalanDOMString	msg(XalanDOMString(name) + " is not allowed inside a template.");

						error(msg, locator);
					}
					break;

				default:
					{
						const XalanDOMString	msg("Unknown XSL element: " + m_elementLocalName);

						// If this stylesheet is declared to be of a higher version than the one
						// supported, don't flag an error.
						if(m_constructionContext.getXSLTVersionSupported() < m_stylesheet.getXSLTVerDeclared())
						{
							m_constructionContext.warn(msg);
						}
						else
						{
							error(msg, locator);
						}
					}
				}

				m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());
			}
		}
		else if (!m_inTemplate && startsWith(ns, m_constructionContext.getXalanXSLNameSpaceURL()))
		{
			processExtensionElement(name, m_elementLocalName, atts, locator);
		}
		else
		{
			if(!m_inTemplate)
			{
				// If it's a top level 
				if (!m_foundStylesheet)
				{
					elem = initWrapperless(name, atts, lineNumber, columnNumber);
				}
				else if (length(ns) == 0 && m_elemStack.size() == 1)
				{
					error("Illegal top level element", locator);
				}
				else
				{
					m_inExtensionElementStack.back() = true;
				}
			}
			else
			{
				m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());

				// BEGIN SANJIVA CODE
				// is this an extension element call?
				ExtensionNSHandler*		nsh = 0;

				if (!isEmpty(ns) && 
					((nsh = m_stylesheet.lookupExtensionNSHandler(ns)) != 0)) 
				{
					elem = new ElemExtensionCall (m_constructionContext,
											m_stylesheet,
											name,
											atts,
											lineNumber,
											columnNumber,
											*nsh,
											m_elementLocalName);

					assert(m_inExtensionElementStack.empty() == false);

					m_inExtensionElementStack.back() = true;
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
			}
		}

		if(m_inTemplate && 0 != elem)
		{
			if(!m_elemStack.empty())
			{
				// Guard against an exception in appendChildElem()...
				XalanAutoPtr<ElemTemplateElement>	theGuard(elem);

				appendChildElementToParent(elem, locator);

				// The element is parented and will now be
				// deleted when the parent is delete...
				theGuard.release();
			}

			m_elemStack.push_back(elem);
		}

		// If we haven't processed an xml:space attribute already, look for one...
		if (fSpaceAttrProcessed == false)
		{
			fSpaceAttrProcessed = processSpaceAttr(atts, locator, fPreserveSpace);
		}

		// Only update the stack if we actually processed an xml:space attribute...
		if (fSpaceAttrProcessed == true)
		{
			// Set the preserve value...
			m_preserveSpaceStack.back() = fPreserveSpace;
		}

		// If for some reason something didn't get pushed, push an empty 
		// object.
		if(origStackSize == m_elemStack.size())
		{
			m_elemStack.push_back(new ElemEmpty(m_constructionContext,
									 m_stylesheet,
									 lineNumber, columnNumber));

			if (elem != 0)
			{
				delete elem;

				m_elemStackParentedElements.erase(elem);
			}
		}
	} // end try
	catch(...)
	{
		doCleanup();

		throw;
	}
}



ElemTemplateElement*
StylesheetHandler::initWrapperless(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			int						lineNumber,
			int						columnNumber)
{
	m_stylesheet.getStylesheetRoot().initDefaultRule(m_constructionContext);

	AttributeListImpl	templateAttrs;

	templateAttrs.addAttribute(c_wstr(Constants::ATTRNAME_NAME),
							   c_wstr(Constants::ATTRTYPE_CDATA),
							   c_wstr(Constants::ATTRVAL_SIMPLE));

	assert(m_pTemplate == 0);

	m_pTemplate = new ElemTemplate(m_constructionContext,
								   m_stylesheet,
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

	m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());

	m_stylesheet.setWrapperlessTemplate(m_pTemplate);

	m_foundStylesheet = true;
	m_stylesheet.setWrapperless(true);

	// This attempts to optimize for a literal result element with
	// the name HTML, so we don't have to switch on-the-fly.
	if(equalsIgnoreCaseASCII(name, Constants::ELEMNAME_HTML_STRING) == true)
	{
		// If there's a default namespace, then we must output XML.
		// Otherwise, we'll set the output method to HTML.
		if (atts.getValue(c_wstr(DOMServices::s_XMLNamespace)) == 0)
		{
			m_stylesheet.getStylesheetRoot().setIndentResult(true);
			m_stylesheet.getStylesheetRoot().setOutputMethod(OUTPUT_METHOD_HTML);
		}
	}

	return pElem;
}



const XalanDOMString&
StylesheetHandler::getNamespaceFromStack(const XalanDOMChar*	theName) const
{
	const XalanDOMString* const		theNamespace =
		m_stylesheet.getNamespaceFromStack(theName);

	if (theNamespace == 0)
	{
		return s_emptyString;
	}
	else
	{
		return *theNamespace;
	}
}



const XalanDOMString&
StylesheetHandler::getNamespaceForPrefixFromStack(const XalanDOMString&		thePrefix) const
{
	const XalanDOMString* const		theNamespace =
		m_stylesheet.getNamespaceForPrefixFromStack(thePrefix);

	if (theNamespace == 0)
	{
		return s_emptyString;
	}
	else
	{
		return *theNamespace;
	}
}



void
StylesheetHandler::processTopLevelElement(
			const XalanDOMChar*		name,
			const XalanDOMString&	localName,
			const XalanDOMString&	ns,
			const AttributeList&	atts,
			int						xslToken,
			const Locator*			locator,
			bool&					fPreserveSpace,
			bool&					fSpaceAttrProcessed)
{
	if(m_foundStylesheet && StylesheetConstructionContext::ELEMNAME_IMPORT != xslToken)
	{
		m_foundNotImport = true;
	}

	const int	lineNumber = getLineNumber(locator);
	const int	columnNumber = getColumnNumber(locator);

	switch(xslToken)
	{
	case StylesheetConstructionContext::ELEMNAME_TEMPLATE:
		assert(m_pTemplate == 0);

		m_pTemplate = new ElemTemplate(
				m_constructionContext,
				m_stylesheet,
				atts,
				lineNumber,
				columnNumber);
	
		m_elemStack.push_back(m_pTemplate);
		m_elemStackParentedElements.insert(m_pTemplate);
		m_inTemplate = true;
		m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());
		break;

	case StylesheetConstructionContext::ELEMNAME_EXTENSION:
		if(!equalsIgnoreCaseASCII(ns, m_constructionContext.getXalanXSLNameSpaceURL()))
		{
			m_constructionContext.warn("Old syntax: the functions instruction should use a url of " + m_constructionContext.getXalanXSLNameSpaceURL());
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_VARIABLE:
	case StylesheetConstructionContext::ELEMNAME_PARAM:
		{
			ElemVariable* varelem = (StylesheetConstructionContext::ELEMNAME_PARAM == xslToken) 
									   ? new ElemParam(m_constructionContext,
													   m_stylesheet,
													   atts, 
													   lineNumber, columnNumber)
										 : new ElemVariable(m_constructionContext,
															m_stylesheet,
															atts, 
															lineNumber, columnNumber);

			XalanAutoPtr<ElemVariable>	newVar(varelem);

			checkForOrAddVariableName(varelem->getName(), locator);

			m_elemStack.push_back(varelem);
			m_inTemplate = true; // fake it out
			m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());
			m_stylesheet.setTopLevelVariable(varelem);
			m_elemStackParentedElements.insert(varelem);

			newVar.release();

			varelem->setTopLevel(true);
		}
	break;

	case StylesheetConstructionContext::ELEMNAME_PRESERVE_SPACE:
	case StylesheetConstructionContext::ELEMNAME_STRIP_SPACE:
		processPreserveStripSpace(name, atts, locator, xslToken);
		break;

	case StylesheetConstructionContext::ELEMNAME_KEY:
		{
			ElemEmpty nsContext(m_constructionContext, m_stylesheet, lineNumber, columnNumber);

			m_stylesheet.processKeyElement(&nsContext, atts, m_constructionContext);
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET:
		{
			m_inTemplate = true; // fake it out
			m_inScopeVariableNamesStack.push_back(QNameSetVectorType::value_type());

			ElemAttributeSet* attrSet = new ElemAttributeSet(m_constructionContext,
															   m_stylesheet,
															   atts,
															   lineNumber,
															   columnNumber);
		
			m_elemStack.push_back(attrSet);
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_INCLUDE:
		processInclude(name, atts, locator);
		break;

	case StylesheetConstructionContext::ELEMNAME_IMPORT:
		processImport(name, atts, locator);
		break;

	case StylesheetConstructionContext::ELEMNAME_OUTPUT:
		m_stylesheet.getStylesheetRoot().processOutputSpec(name, atts, m_constructionContext);
		break;

	case StylesheetConstructionContext::ELEMNAME_DECIMAL_FORMAT:
		m_stylesheet.processDecimalFormatElement(
					new ElemDecimalFormat(
							m_constructionContext,
							m_stylesheet,
							atts,
							lineNumber,
							columnNumber));
		break;

	case StylesheetConstructionContext::ELEMNAME_NAMESPACE_ALIAS:
		m_stylesheet.processNSAliasElement(name, atts, m_constructionContext);
		break;

	case StylesheetConstructionContext::ELEMNAME_WITH_PARAM:
	case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
	case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
	case StylesheetConstructionContext::ELEMNAME_CHOOSE:
	case StylesheetConstructionContext::ELEMNAME_COMMENT:
	case StylesheetConstructionContext::ELEMNAME_COPY:
	case StylesheetConstructionContext::ELEMNAME_COPY_OF:
	case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
	case StylesheetConstructionContext::ELEMNAME_IF:
	case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
	case StylesheetConstructionContext::ELEMNAME_MESSAGE:
	case StylesheetConstructionContext::ELEMNAME_NUMBER:
	case StylesheetConstructionContext::ELEMNAME_OTHERWISE:
	case StylesheetConstructionContext::ELEMNAME_PI:
	case StylesheetConstructionContext::ELEMNAME_SORT:
	case StylesheetConstructionContext::ELEMNAME_TEXT:
	case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
	case StylesheetConstructionContext::ELEMNAME_WHEN:
	case StylesheetConstructionContext::ELEMNAME_ELEMENT:
	case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
		if (inExtensionElement() == false)
		{
			const XalanDOMString	msg("(StylesheetHandler) " + XalanDOMString(name) + " not allowed inside a stylesheet.");
				
			error(msg, locator);
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_STYLESHEET:
		processStylesheet(name, atts, locator, fPreserveSpace, fSpaceAttrProcessed);
		break;

	default:
		if (inExtensionElement() == false)
		{
			const XalanDOMString	msg("Unknown XSL element: " + localName);

			error(msg, locator);
		}
		break;
	}
}


void
StylesheetHandler::processStylesheet(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			const Locator*			locator,
			bool&					fPreserveSpace,
			bool&					fSpaceAttrProcessed)
{
	m_stylesheet.setWrapperless(false);
	m_foundStylesheet = true;

	const unsigned int	nAttrs = atts.getLength();

	bool				fVersionFound = false;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
		{
			m_stylesheet.processExcludeResultPrefixes(m_constructionContext, atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
		{
			StringTokenizer tokenizer(atts.getValue(i),
									  Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

			XalanDOMString	prefix;

			while(tokenizer.hasMoreTokens() == true)
			{
				tokenizer.nextToken(prefix);

				const XalanDOMString&	extns = getNamespaceForPrefixFromStack(prefix);

				ExtensionNSHandler* const	nsh = new ExtensionNSHandler(extns);

				m_stylesheet.addExtensionNamespace(m_constructionContext, extns, nsh);
			}
		}
 		else if(equals(aname, Constants::ATTRNAME_ID))
 		{
 			//
 		}
		else if(equals(aname, Constants::ATTRNAME_VERSION))
		{
			const XalanDOMChar* const	versionStr = atts.getValue(i);
			assert(versionStr != 0);

			m_stylesheet.setXSLTVerDeclared(DoubleSupport::toDouble(versionStr));

			fVersionFound = true;
		}
		else if(processSpaceAttr(aname, atts, i, locator, fPreserveSpace) == true)
		{
			fSpaceAttrProcessed = true;
		}
		else if(isAttrOK(aname, atts, i) == false)
		{
			if(false == m_stylesheet.isWrapperless())
			{
				const XalanDOMString	msg(
					"(StylesheetHandler) " +
					XalanDOMString(name) + 
					" has an illegal attribute: " +
					aname);

				error(msg, locator);
			}
		}

		if(!m_stylesheet.getNamespaces().empty())
		{
			m_stylesheet.setNamespaceDecls(m_stylesheet.getNamespaces().back());
		}
	}

	if (fVersionFound == false)
	{
		error("The stylesheet element did not specify a version attribute.", locator);
	}
}



void
StylesheetHandler::processExtensionElement(
			const XalanDOMChar*		/* name */,
			const XalanDOMString&	/* localName */,
			const AttributeList&	/* atts */,
			const Locator*			/* locator */)
{
}



void
StylesheetHandler::checkForOrAddVariableName(
			const XalanQName&	theVariableName,
			const Locator*		theLocator)
{
	XalanQNameByValue	theLocalVariableName(theVariableName);

	if (m_inTemplate == false)
	{
		assert(m_inScopeVariableNamesStack.empty() == true);

		if (m_globalVariableNames.find(theLocalVariableName) != m_globalVariableNames.end())
		{
			error("A global variable with this name has already been declared", theLocator);
		}
		else
		{
			m_globalVariableNames.insert(theLocalVariableName);
		}
	}
	else
	{
		assert(m_inScopeVariableNamesStack.empty() == false);

		QNameSetVectorType::iterator		theCurrent = m_inScopeVariableNamesStack.begin();
		const QNameSetVectorType::iterator	theEnd = m_inScopeVariableNamesStack.end();

		while(theCurrent != theEnd)
		{
			QNameSetVectorType::value_type	theLocalScope = *theCurrent;

			if (theLocalScope.find(theLocalVariableName) != theLocalScope.end())
			{
				error("A variable with this name has already been declared in this template", theLocator);
			}

			++theCurrent;
		}

		assert(theCurrent == theEnd);

		m_inScopeVariableNamesStack.back().insert(theLocalVariableName);
	}
}



void
StylesheetHandler::processPreserveStripSpace(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			const Locator*			locator,
			int						xslToken)
{
	ElemEmpty nsNode(m_constructionContext, m_stylesheet, getLineNumber(locator), getColumnNumber(locator));

	const unsigned int	nAttrs = atts.getLength();

	bool foundIt = false;

	const bool	isPreserveSpace = StylesheetConstructionContext::ELEMNAME_PRESERVE_SPACE == xslToken? true : false;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_ELEMENTS))
		{
			foundIt = true;

			StringTokenizer		tokenizer(atts.getValue(i),
										  Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

			while(tokenizer.hasMoreTokens())
			{
				// Use only the root, at least for right now.
				const XalanDOMString	wildcardName = tokenizer.nextToken();

				/**
				 * Creating a match pattern is too much overhead, but it's a reasonably 
				 * easy and safe way to do this right now.
				 */
				const XPath* const	matchPat =
						m_constructionContext.createMatchPattern(0, wildcardName, nsNode);

				if(isPreserveSpace == true)
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
			const XalanDOMString	msg(XalanDOMString(name) + " has an illegal attribute: " + aname);

			error(msg, locator);
		}
	}

	if(!foundIt && inExtensionElement() == false)
	{
		const XalanDOMString	msg("(StylesheetHandler) " + XalanDOMString(name) +
			" requires a " + Constants::ATTRNAME_ELEMENTS + " attribute.");

		error(msg, locator);
	}
}



void
StylesheetHandler::appendChildElementToParent(
			ElemTemplateElement*	parent,
			ElemTemplateElement*	elem)
{
	assert(elem != 0);

	appendChildElementToParent(parent, elem, elem->getLocator());
}



void
StylesheetHandler::appendChildElementToParent(
			ElemTemplateElement*	elem,
			const Locator*			locator)
{
	appendChildElementToParent(m_elemStack.back(), elem, locator);
}



void
StylesheetHandler::appendChildElementToParent(
			ElemTemplateElement*	parent,
			ElemTemplateElement*	elem,
			const Locator*			locator)
{
	assert(parent != 0 && elem != 0);

	try
	{
		parent->appendChildElem(elem);
	}
	catch(const XalanDOMException&	e)
	{
		if (e.getExceptionCode() == XalanDOMException::HIERARCHY_REQUEST_ERR)
		{
			// $$$ ToDo: There is a bug in the version of gcc that
			// we're using when the optimizer is enabled.  Constructing
			// this error message from the parameter results in an internal
			// compiler error, so I'm using a local variable instead.
#if defined(__GNUC__)
			const ElemTemplateElement* const	localElem = elem;

			XalanDOMString	theMessage(localElem->getElementName());

#else
			XalanDOMString	theMessage(elem->getElementName());
#endif

			append(theMessage, " is not allowed at this position in the stylesheet");

			error(theMessage, locator);
		}

		throw;
	}

	m_elemStackParentedElements.insert(elem);
}



void
StylesheetHandler::doCleanup()
{
	if (m_locatorsPushed > 0)
	{
		m_constructionContext.popLocatorStack();

		--m_locatorsPushed;
	}

	// Pop anything that's not an empty element...
	while(m_elemStack.empty() == false &&
		  m_elemStack.back()->getXSLToken() != StylesheetConstructionContext::ELEMNAME_UNDEFINED)
	{
		m_elemStackParentedElements.erase(m_elemStack.back());
		m_elemStack.pop_back();
	}
}



static bool
stackContains(
			const Stylesheet::URLStackType&		stack, 
			const XalanDOMString&				urlString)
{
	const Stylesheet::URLStackType::size_type	n = stack.size();

	bool				contains = false;

	for(Stylesheet::URLStackType::size_type i = 0; i < n && contains == false; ++i)
	{
		if(equals(stack[i], urlString))
		{
			contains = true;
		}
	}

	return contains;	
}



void
StylesheetHandler::processImport(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			const Locator*			locator)
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
				error("Imports can only occur as the first elements in the stylesheet.", locator);
			}

			const XalanDOMString	saved_XSLNameSpaceURL = m_stylesheet.getXSLTNamespaceURI();

			const XalanDOMString	href(atts.getValue(i));

			Stylesheet::URLStackType&	includeStack = m_stylesheet.getIncludeStack();
			assert(includeStack.empty() == false);

			const XalanDOMString	hrefUrl = m_constructionContext.getURLStringFromString(href, includeStack.back());
			assert(length(hrefUrl) != 0);

			Stylesheet::URLStackType&	importStack = m_stylesheet.getStylesheetRoot().getImportStack();

			if(stackContains(importStack, hrefUrl))
			{
				const XalanDOMString	msg(hrefUrl + " is directly or indirectly importing itself.");

				error(msg, locator);
			}

			importStack.push_back(hrefUrl);
			
			// This will take care of cleaning up the stylesheet if an exception
			// is thrown.
			XalanAutoPtr<Stylesheet>	importedStylesheet( 
				m_constructionContext.create(
				m_stylesheet.getStylesheetRoot(), 
				hrefUrl));

			StylesheetHandler tp(*importedStylesheet.get(), m_constructionContext);

			m_constructionContext.parseXML(hrefUrl, &tp, 0);

			// Add it to the front of the imports, releasing the XalanAutoPtr...
			m_stylesheet.addImport(importedStylesheet.release(), true);

			assert(equals(importStack.back(), hrefUrl));
			importStack.pop_back();		

			m_stylesheet.setXSLTNamespaceURI(saved_XSLNameSpaceURL);
		}
		else if(!isAttrOK(aname, atts, i))
		{
			const XalanDOMString	msg(XalanDOMString(name) + " has an illegal attribute: " + aname);

			error(msg, locator);
		}
	}

	if(!foundIt)
	{
		const XalanDOMString	msg("Could not find href attribute for " + XalanDOMString(name));

		error(msg, locator);
	}
}


void
StylesheetHandler::processInclude(
			const XalanDOMChar*		name,
			const AttributeList&	atts,
			const Locator*			locator)
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

			const XalanDOMString	href(atts.getValue(i));
		
			assert(c_wstr(m_stylesheet.getIncludeStack().back()) != 0);
			const XalanDOMString	hrefUrl = m_constructionContext.getURLStringFromString(href, m_stylesheet.getIncludeStack().back());

			if(stackContains(m_stylesheet.getIncludeStack(), hrefUrl))
			{
				XalanDOMString msg(hrefUrl + " is directly or indirectly including itself.");

				error(msg, locator);
			}

			m_stylesheet.getIncludeStack().push_back(hrefUrl);

			m_constructionContext.parseXML(hrefUrl, this, 0);

			assert(equals(m_stylesheet.getIncludeStack().back(), hrefUrl));
			m_stylesheet.getIncludeStack().pop_back();
		}
		else if(!isAttrOK(aname, atts, i))
		{
			const XalanDOMString	msg(XalanDOMString(name) + " has an illegal attribute: " + aname);

			error(msg, locator);
		}
	}

	if(!foundIt)
	{
		const XalanDOMString	msg("Could not find href attribute for " + XalanDOMString(name));

		error(msg, locator);
	}
}



void
StylesheetHandler::endElement(const XMLCh* const name)
{
	const Locator* const	locator = m_constructionContext.getLocatorFromStack();

#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	processAccumulatedText();

	// Clean up the whitespace elements.
	for_each(m_whiteSpaceElems.begin(),
			 m_whiteSpaceElems.end(),
			 DeleteFunctor<ElemTextLiteral>());

	m_whiteSpaceElems.clear();

	m_stylesheet.popNamespaces();

	assert(m_elemStack.empty() == false);

	m_lastPopped = m_elemStack.back();
	m_elemStack.pop_back();
	m_elemStackParentedElements.erase(m_lastPopped);
	m_lastPopped->finishedConstruction();

	const int tok = m_lastPopped->getXSLToken();

	if (m_inTemplate == true)
	{
		assert(m_inScopeVariableNamesStack.empty() == false);

		m_inScopeVariableNamesStack.pop_back();
	}

	if(StylesheetConstructionContext::ELEMNAME_TEMPLATE == tok)
	{
		m_inTemplate = false;
		m_stylesheet.addTemplate(m_pTemplate, m_constructionContext);
		m_pTemplate = 0;
	}
	else if((StylesheetConstructionContext::ELEMNAME_PARAM == tok) ||
			 StylesheetConstructionContext::ELEMNAME_VARIABLE == tok)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		const ElemVariable* const	var = (const ElemVariable*)m_lastPopped;
#else
		const ElemVariable* const	var = static_cast<const ElemVariable*>(m_lastPopped);
#endif

		if(var->isTopLevel() == true)
		{
			// Top-level param or variable
			m_inTemplate = false;
		}
	}
	else if(StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET == tok)
	{
		m_inTemplate = false;
	}
	else if (tok == StylesheetConstructionContext::ELEMNAME_UNDEFINED ||
			 tok == StylesheetConstructionContext::ELEMNAME_TEXT)
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
			const XalanDOMString	msg(XalanDOMString(name) + " attribute \'lang\' is missing");

			error(msg, locator);
		}
		if (m_pLXSLTExtensionNSH == 0) 
		{
			const XalanDOMString	msg("(StylesheetHandler) misplaced " + XalanDOMString(name) + " element?? Missing container element " + "'component'");

			error(msg, locator);
		}

		m_pLXSLTExtensionNSH->setScript(m_LXSLTScriptLang, m_LXSLTScriptSrcURL, m_LXSLTScriptBody);
		
		// reset state
		m_inLXSLTScript = false;
		clear(m_LXSLTScriptLang);
		clear(m_LXSLTScriptSrcURL);
		clear(m_LXSLTScriptBody);
		m_pLXSLTExtensionNSH = 0;
	}
	// END SANJIVA CODE

	assert(m_inExtensionElementStack.empty() == false);

	m_inExtensionElementStack.pop_back();

	assert(m_preserveSpaceStack.empty() == false);

	m_preserveSpaceStack.pop_back();
}



void
StylesheetHandler::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	accumulateText(chars, length);
}



void
StylesheetHandler::cdata(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	accumulateText(chars, length);

	processText(chars, length);

	m_lastPopped = 0;
}



void
StylesheetHandler::ignorableWhitespace(
			const XMLCh* const	/*chars*/,
			const unsigned int	/*length*/)
{
	// Ignore!
	m_lastPopped = 0;
}



void
StylesheetHandler::processingInstruction(
			const XMLCh* const	/*target*/,
			const XMLCh* const	/*data*/)
{
	if (isXMLWhitespace(m_accumulateText) == false)
	{
		processAccumulatedText();
	}
	else
	{
		clear(m_accumulateText);
	}
}



void
StylesheetHandler::comment(const XMLCh* const /*data*/)
{
	processAccumulatedText();
}



void
StylesheetHandler::entityReference(const XMLCh* const /*name*/)
{
	processAccumulatedText();
}



void
StylesheetHandler::resetDocument()
{
	clear(m_accumulateText);
}



void
StylesheetHandler::charactersRaw(
			const XMLCh* const	/* chars */,
			const unsigned int	/* length */)
{
  // No action for the moment.
}



void
StylesheetHandler::processText(
			const XMLCh*				chars,
			XalanDOMString::size_type	length)
{
	if(m_inTemplate)
	{
		ElemTemplateElement*	parent = m_elemStack.back();
		assert(parent != 0);

		assert(m_preserveSpaceStack.empty() == false);

		bool	preserveSpace = m_preserveSpaceStack.back();
		bool	disableOutputEscaping = false;

		if (preserveSpace == false && parent->getXSLToken() == StylesheetConstructionContext::ELEMNAME_TEXT)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			disableOutputEscaping = ((ElemText*)parent)->getDisableOutputEscaping();
#else
			disableOutputEscaping = static_cast<ElemText*>(parent)->getDisableOutputEscaping();
#endif
			preserveSpace = true;

			parent = m_elemStack[m_elemStack.size() - 2];
		}

		const Locator* const	locator = m_constructionContext.getLocatorFromStack();

		const XalanLocator::size_type	lineNumber = (0 != locator) ? locator->getLineNumber() : 0;
		const XalanLocator::size_type	columnNumber = (0 != locator) ? locator->getColumnNumber() : 0;

		XalanAutoPtr<ElemTextLiteral>	elem(new ElemTextLiteral(m_constructionContext,
			m_stylesheet,
			lineNumber,
			columnNumber,
			chars,
			0,
			length,
			true,
			preserveSpace, 
			disableOutputEscaping));

		const bool	isWhite = elem->isWhitespace();

		if(preserveSpace || (!preserveSpace && !isWhite))
		{
			while(!m_whiteSpaceElems.empty())
			{
				assert(m_whiteSpaceElems.back() != 0);

				appendChildElementToParent(
					parent,
					m_whiteSpaceElems.back());

				m_whiteSpaceElems.pop_back();
			}

			appendChildElementToParent(
				parent,
				elem.get());

			elem.release();
		}
		else if(isWhite)
		{
			bool						shouldPush = true;

			ElemTemplateElement* const	last = parent->getLastChildElem();

			if(0 != last)
			{
				// If it was surrounded by xsl:text, it will count as an element.
				const bool	isPrevCharData =
					StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT == last->getXSLToken();

				const bool	isLastPoppedXSLText = (m_lastPopped != 0) &&
						(StylesheetConstructionContext::ELEMNAME_TEXT == m_lastPopped->getXSLToken());

				if(isPrevCharData && ! isLastPoppedXSLText)
				{
					appendChildElementToParent(
						parent,
						elem.get());

					elem.release();

					shouldPush = false;
				}
			}

			if(shouldPush)
			{
				m_whiteSpaceElems.push_back(elem.get());

				elem.release();
			}
		}
	}
	else if (m_inLXSLTScript)
	{
		append(m_LXSLTScriptBody, chars);
	}

	// TODO: Flag error if text inside of stylesheet
}



void
StylesheetHandler::accumulateText(
			const XMLCh*				chars,
			XalanDOMString::size_type	length)
{	
	if(m_inTemplate)
	{
		append(m_accumulateText, chars, length);
	}
}



void
StylesheetHandler::processAccumulatedText()
{
	if (isEmpty(m_accumulateText) == false)
	{
		processText(m_accumulateText.c_str(), length(m_accumulateText));

		clear(m_accumulateText);
	}	
}



bool
StylesheetHandler::inExtensionElement() const
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	if (find(
			m_inExtensionElementStack.rbegin(),
			m_inExtensionElementStack.rend(),
			true) != m_inExtensionElementStack.rend())
	{
		return true;
	}
	else
	{
		return false;
	}
}



void
StylesheetHandler::error(
			const char*		theMessage,
			const Locator*	theLocator) const
{
	m_constructionContext.error(theMessage, 0, theLocator);
}



void
StylesheetHandler::error(
			const XalanDOMString&	theMessage,
			const Locator*			theLocator) const
{
	m_constructionContext.error(theMessage, 0, theLocator);
}



StylesheetHandler::PushPopIncludeState::PushPopIncludeState(StylesheetHandler&	theHandler) :
	m_handler(theHandler),
	m_elemStack(theHandler.m_elemStack),
	m_elemStackParentedElements(theHandler.m_elemStackParentedElements),
	m_pTemplate(theHandler.m_pTemplate),
	m_lastPopped(theHandler.m_lastPopped),
	m_inTemplate(theHandler.m_inTemplate),
	m_foundStylesheet(theHandler.m_foundStylesheet),
	m_XSLNameSpaceURL(theHandler.m_stylesheet.getXSLTNamespaceURI()),
	m_foundNotImport(theHandler.m_foundNotImport),
	m_namespaceDecls(),
	m_namespaces(),
	m_namespacesHandler(),
	m_inExtensionElementStack()
{
	clear(m_handler.m_accumulateText);
	m_handler.m_elemStack.clear();
	m_handler.m_pTemplate = 0;
	m_handler.m_lastPopped = 0;
	m_handler.m_inTemplate = false;
	m_handler.m_foundStylesheet = false;
	m_handler.m_foundNotImport = false;

	// This is much more efficient, since we're just swapping
	// underlying data.  This clears out the stack as well...
	m_namespaceDecls.swap(theHandler.m_stylesheet.getNamespaceDecls());
	m_namespaces.swap(theHandler.m_stylesheet.getNamespaces());
	m_namespacesHandler.swap(theHandler.m_stylesheet.getNamespacesHandler());
	m_inExtensionElementStack.swap(theHandler.m_inExtensionElementStack);
	m_preserveSpaceStack.swap(theHandler.m_preserveSpaceStack);
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

	clear(m_handler.m_accumulateText);
	m_handler.m_elemStack = m_elemStack;
	m_handler.m_elemStackParentedElements = m_elemStackParentedElements;
	m_handler.m_pTemplate = m_pTemplate;
	m_handler.m_lastPopped = m_lastPopped;
	m_handler.m_inTemplate = m_inTemplate;
	m_handler.m_foundStylesheet = m_foundStylesheet;
	m_handler.m_stylesheet.setXSLTNamespaceURI(m_XSLNameSpaceURL);
	m_handler.m_foundNotImport = m_foundNotImport;

	// This is much more efficient, since we're just swapping
	// underlying data.
	m_handler.m_stylesheet.getNamespaceDecls().swap(m_namespaceDecls);
	m_handler.m_stylesheet.getNamespaces().swap(m_namespaces);
	m_handler.m_stylesheet.getNamespacesHandler().swap(m_namespacesHandler);
	m_handler.m_inExtensionElementStack.swap(m_inExtensionElementStack);
	m_handler.m_preserveSpaceStack.swap(m_preserveSpaceStack);
}



const XalanDOMString			StylesheetHandler::s_emptyString;



void
StylesheetHandler::initialize()
{
}



void
StylesheetHandler::terminate()
{
}
