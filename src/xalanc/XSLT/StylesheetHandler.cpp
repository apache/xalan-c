/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "StylesheetHandler.hpp"



#include <algorithm>



#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax/SAXParseException.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "Constants.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemTextLiteral.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetRoot.hpp"
#include "XalanSpaceNodeTester.hpp"



#include <xalanc/Include/XalanAutoPtr.hpp>



XALAN_CPP_NAMESPACE_BEGIN



typedef StylesheetConstructionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;



StylesheetHandler::StylesheetHandler(
			Stylesheet&						stylesheetTree,
			StylesheetConstructionContext&	constructionContext) :
	FormatterListener(OUTPUT_METHOD_OTHER),
	m_stylesheet(stylesheetTree),
	m_constructionContext(constructionContext),
	m_elemEmptyAllocator(eElemEmptyAllocatorBlockSize),
	m_elemTextAllocator(eElemTextBlockSize),
	m_elemStack(),
	m_whiteSpaceElems(),
	m_pTemplate(0),
	m_lastPopped(*this),
	m_inTemplate(false),
	m_foundStylesheet(false),
	m_foundNotImport(false),
	m_elementLocalName(),
	m_accumulateText(),
	m_includeBase(stylesheetTree.getBaseIdentifier()),
	m_inExtensionElementStack(),
	m_locatorsPushed(0),
	m_globalVariableNames(),
	m_inScopeVariableNamesStack()
{
	m_inScopeVariableNamesStack.reserve(eVariablesStackDefault);
}



StylesheetHandler::~StylesheetHandler()
{
	doCleanup();
}



void StylesheetHandler::setDocumentLocator(const LocatorType* const		locator)
{
	m_constructionContext.pushLocatorOnStack(locator);

	++m_locatorsPushed;
}



void
StylesheetHandler::startDocument()
{
}



void
StylesheetHandler::endDocument()
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
			const XalanDOMChar*			attrName,
			const AttributeListType&	atts,
			int							which)
{
	return m_stylesheet.isAttrOK(attrName, atts, which, m_constructionContext);
}



bool
StylesheetHandler::processSpaceAttr(
			const XalanDOMChar*			aname,
			const AttributeListType&	atts,
			int							which,
			const LocatorType*			locator,
			bool&						fPreserve)
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
			error(
				XalanMessageLoader::getMessage(
					XalanMessages::AttributeHasIllegalValue_1Param,
					Constants::ATTRNAME_XMLSPACE),
				locator);
		}

		return true;
	}
}



bool
StylesheetHandler::processSpaceAttr(
			const AttributeListType&	atts,
			const LocatorType*			locator,
			bool&						fPreserve)
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
			AttributeListType&	atts)
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
		// By default, space is not preserved...
		bool	fPreserveSpace = false;
		bool	fSpaceAttrProcessed = false;

		processAccumulatedText();

		m_whiteSpaceElems.clear();

		const LocatorType* const	locator = m_constructionContext.getLocatorFromStack();

		// First push namespaces
		m_stylesheet.pushNamespaces(atts);

		const XalanDOMString::size_type		nameLength = length(name);
		const XalanDOMString::size_type		index = indexOf(name, XalanUnicode::charColon);

		const XalanDOMString*	ns = getNamespaceFromStack(name);

		if(ns == 0)
		{
			if (index < nameLength)
			{
				error(XalanMessageLoader::getMessage(XalanMessages::CannotResolvePrefix_1Param, name), locator);
			}
			else
			{
				ns = &s_emptyString;
			}
		}
		assert(ns != 0);

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

		if(equals(*ns, m_constructionContext.getXSLTNamespaceURI()))
		{
			if(!isEmpty(m_stylesheet.getXSLTNamespaceURI()))
				m_stylesheet.setXSLTNamespaceURI(*ns);

			const StylesheetConstructionContext::eElementToken	xslToken =
				m_constructionContext.getElementToken(m_elementLocalName);

			if(!m_inTemplate)
			{
				processTopLevelElement(name, atts, xslToken, locator, fPreserveSpace, fSpaceAttrProcessed);
			}
			else
			{
				switch(xslToken)
				{            
				case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
				case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
				case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
				case StylesheetConstructionContext::ELEMNAME_CHOOSE:
				case StylesheetConstructionContext::ELEMNAME_COMMENT:
				case StylesheetConstructionContext::ELEMNAME_COPY:
				case StylesheetConstructionContext::ELEMNAME_COPY_OF:
				case StylesheetConstructionContext::ELEMNAME_ELEMENT:
				case StylesheetConstructionContext::ELEMNAME_FALLBACK:
				case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
				case StylesheetConstructionContext::ELEMNAME_IF:
				case StylesheetConstructionContext::ELEMNAME_MESSAGE:
				case StylesheetConstructionContext::ELEMNAME_NUMBER:
				case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
				case StylesheetConstructionContext::ELEMNAME_WITH_PARAM:
				case StylesheetConstructionContext::ELEMNAME_PI:
					elem = m_constructionContext.createElement(
												xslToken,
												m_stylesheet,
												atts,
												locator);
					assert(elem != 0);
					break;
          
				case StylesheetConstructionContext::ELEMNAME_PARAM:
					elem = m_constructionContext.createElement(
												xslToken,
												m_stylesheet,
												atts,
												locator);

					checkForOrAddVariableName(elem->getNameAttribute(), locator);

					assert(elem != 0);
					break;

				case StylesheetConstructionContext::ELEMNAME_SORT:
					{
						if (m_elemStack.empty() == true)
						{
							error(XalanMessageLoader::getMessage(XalanMessages::IsNotAllowedInThisPosition_1Param,Constants::ELEMNAME_SORT_WITH_PREFIX_STRING), locator);
						}

						ElemTemplateElement* const	theElement =
								m_elemStack.back();
						assert(theElement != 0);

						theElement->processSortElement(
							m_constructionContext,
							m_stylesheet,
							atts,
							locator);

						m_elemStack.push_back(
							m_elemEmptyAllocator.create(
									m_constructionContext,
									m_stylesheet,
									&Constants::ELEMNAME_SORT_WITH_PREFIX_STRING));
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
					{
						if (m_elemStack.empty() == true)
						{
							error(XalanMessageLoader::getMessage(XalanMessages::IsNotAllowedInThisPosition_1Param,Constants::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING), locator);
						}

						ElemTemplateElement* const	theElement =
								m_elemStack.back();
						assert(theElement != 0);

						const int	parentToken =
							theElement->getXSLToken();

						if (parentToken == StylesheetConstructionContext::ELEMNAME_FOR_EACH)
						{
							error(XalanMessageLoader::getMessage(XalanMessages::IsNotAllowedInThisPosition_1Param,Constants::ELEMNAME_APPLY_IMPORTS_WITH_PREFIX_STRING), locator);
						}

						elem = m_constructionContext.createElement(
													xslToken,
													m_stylesheet,
													atts,
													locator);
						assert(elem != 0);
					}
					break;
          
				case StylesheetConstructionContext::ELEMNAME_VARIABLE:
					{
						elem =
							m_constructionContext.createElement(
												xslToken,
												m_stylesheet,
												atts,
												locator);
						assert(elem != 0);

						checkForOrAddVariableName(elem->getNameAttribute(), locator);
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_WHEN:
					{
						ElemTemplateElement* const	parent = m_elemStack.back();

						if(StylesheetConstructionContext::ELEMNAME_CHOOSE != parent->getXSLToken())
						{
							error(XalanMessageLoader::getMessage(XalanMessages::NotParentedBy_2Param,
								Constants::ELEMNAME_WHEN_WITH_PREFIX_STRING,Constants::ELEMNAME_CHOOSE_WITH_PREFIX_STRING), locator);
						}
						else
						{
							ElemTemplateElement* const	lastChild = parent->getLastChildElem();

							if(0 == lastChild ||
								StylesheetConstructionContext::ELEMNAME_WHEN == lastChild->getXSLToken() ||
								lastChild->isWhitespace() == true)
							{
								elem = m_constructionContext.createElement(
															xslToken,
															m_stylesheet,
															atts,
															locator);
								assert(elem != 0);
							}
							else
							{
								error(XalanMessageLoader::getMessage(XalanMessages::Misplaced_1Param,Constants::ELEMNAME_WHEN_WITH_PREFIX_STRING), locator);
							}
						}
					}
					break;
          
				case StylesheetConstructionContext::ELEMNAME_OTHERWISE:
					{
						ElemTemplateElement* parent = m_elemStack.back();

						if(StylesheetConstructionContext::ELEMNAME_CHOOSE != parent->getXSLToken())
						{
							error(XalanMessageLoader::getMessage(XalanMessages::NotParentedBy_2Param
								,Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING
								,Constants::ELEMNAME_CHOOSE_WITH_PREFIX_STRING), locator);
						}
						else
						{
							ElemTemplateElement* lastChild = parent->getLastChildElem();

							if(0 == lastChild ||
								StylesheetConstructionContext::ELEMNAME_WHEN == lastChild->getXSLToken() ||
								lastChild->isWhitespace() == true)
							{
								elem = m_constructionContext.createElement(
															xslToken,
															m_stylesheet,
															atts,
															locator);
								assert(elem != 0);
							}
							else
							{
								error(XalanMessageLoader::getMessage(XalanMessages::Misplaced_1Param, Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING), locator);
							}
						}
					}
					break;

				case StylesheetConstructionContext::ELEMNAME_TEXT:
					m_elemStack.push_back(
						m_elemTextAllocator.create(
							m_constructionContext,
							m_stylesheet,
							atts,
							XalanLocator::getLineNumber(locator),
							XalanLocator::getColumnNumber(locator)));

                    // This fixes Bugzilla 26354, but it's really a workaround
                    // for the bizarre way we handle literal text in the
                    // stylesheet.  We should examine this strategy, because
                    // an xml:space attribute on an xsl:text element results
                    // in building a stylesheet that tries to parent an
                    // ElenTextLiteral instance to an ElemText instance, which
                    // should not ever happen.
                    fSpaceAttrProcessed = true;
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
				case StylesheetConstructionContext::ELEMNAME_DECIMAL_FORMAT:
					{
						error(XalanMessageLoader::getMessage(XalanMessages::IsNotAllowedInsideTemplate_1Param, name), locator);
					}
					break;

				default:
					{
						// If this stylesheet is declared to be of a higher version than the one
						// supported, don't flag an error.
						if(m_constructionContext.getXSLTVersionSupported() < m_stylesheet.getXSLTVerDeclared())
						{
							warn(XalanMessageLoader::getMessage(XalanMessages::UnknownXSLElement_1Param, name), locator);

							elem = m_constructionContext.createElement(
										StylesheetConstructionContext::ELEMNAME_FORWARD_COMPATIBLE,
										m_stylesheet,
										name,
										atts,
										locator);
						}
						else
						{
							error(XalanMessageLoader::getMessage(XalanMessages::UnknownXSLElement_1Param, name), locator);
						}
					}
				}

				m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);
			}
		}
		else if (!m_inTemplate && startsWith(*ns, m_constructionContext.getXalanXSLNameSpaceURL()))
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
					elem = initWrapperless(name, atts, locator);
				}
				else if (length(*ns) == 0 && m_elemStack.size() == 1)
				{
					error(XalanMessageLoader::getMessage(XalanMessages::IllegalTopLevelElement), locator);
				}
				else
				{
					m_inExtensionElementStack.back() = true;
				}
			}
			else
			{
				m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);

				// BEGIN SANJIVA CODE
				// is this an extension element call?
				ExtensionNSHandler*		nsh = 0;

				if (!isEmpty(*ns) &&
					((nsh = m_stylesheet.lookupExtensionNSHandler(*ns)) != 0))
				{
					elem = m_constructionContext.createElement(
												m_stylesheet,
												name,
												atts,
												*nsh,
												locator);
					assert(m_inExtensionElementStack.empty() == false);

					m_inExtensionElementStack.back() = true;
				}
				else 
				{
					elem = m_constructionContext.createElement(
												StylesheetConstructionContext::ELEMNAME_LITERAL_RESULT,
												m_stylesheet,
												name,
												atts,
												locator);
				}

				assert(elem != 0);
			}
		}

		if(m_inTemplate && 0 != elem)
		{
			if(!m_elemStack.empty())
			{
				appendChildElementToParent(elem, locator);
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
			m_elemStack.push_back(m_elemEmptyAllocator.create(m_constructionContext, m_stylesheet));
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
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator)
{
	assert(m_pTemplate == 0);

	m_pTemplate = m_stylesheet.initWrapperless(m_constructionContext, locator);
	assert(m_pTemplate != 0);

	ElemTemplateElement* const	pElem =
		m_constructionContext.createElement(
			StylesheetConstructionContext::ELEMNAME_LITERAL_RESULT,
			m_stylesheet,
			name,
			atts,
			locator);

	m_pTemplate->appendChildElem(pElem);
	m_inTemplate = true;

	m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);

	m_foundStylesheet = true;

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



const XalanDOMString*
StylesheetHandler::getNamespaceFromStack(const XalanDOMChar*	theName) const
{
	return m_stylesheet.getNamespaceFromStack(theName);
}



const XalanDOMString*
StylesheetHandler::getNamespaceForPrefixFromStack(const XalanDOMString&		thePrefix) const
{
	return m_stylesheet.getNamespaceForPrefixFromStack(thePrefix);
}



void
StylesheetHandler::processTopLevelElement(
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			int							xslToken,
			const LocatorType*			locator,
			bool&						fPreserveSpace,
			bool&						fSpaceAttrProcessed)
{
	if(m_foundStylesheet && StylesheetConstructionContext::ELEMNAME_IMPORT != xslToken)
	{
		m_foundNotImport = true;
	}

	switch(xslToken)
	{
	case StylesheetConstructionContext::ELEMNAME_TEMPLATE:
		assert(m_pTemplate == 0);

		m_pTemplate =
			m_constructionContext.createElement(
									StylesheetConstructionContext::ELEMNAME_TEMPLATE,
									  m_stylesheet,
									  atts,
									  locator);
	
		m_elemStack.push_back(m_pTemplate);
		m_inTemplate = true;
		m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);
		break;

	case StylesheetConstructionContext::ELEMNAME_VARIABLE:
	case StylesheetConstructionContext::ELEMNAME_PARAM:
		{
			ElemTemplateElement* const	elem = m_constructionContext.createElement(
												xslToken,
												m_stylesheet,
												atts,
												locator);
			assert(elem != 0);

			checkForOrAddVariableName(elem->getNameAttribute(), locator);

			m_elemStack.push_back(elem);
			m_inTemplate = true; // fake it out
			m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);
			elem->addToStylesheet(m_constructionContext, m_stylesheet);
		}
	break;

	case StylesheetConstructionContext::ELEMNAME_PRESERVE_SPACE:
	case StylesheetConstructionContext::ELEMNAME_STRIP_SPACE:
		processPreserveStripSpace(name, atts, locator, xslToken);
		break;

	case StylesheetConstructionContext::ELEMNAME_KEY:
		{
			m_stylesheet.processKeyElement(
				XalanQName::PrefixResolverProxy(m_stylesheet.getNamespaces(), m_stylesheet.getURI()),
				atts,
				locator,
				m_constructionContext);
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET:
		{
			m_inTemplate = true; // fake it out
			m_inScopeVariableNamesStack.resize(m_inScopeVariableNamesStack.size() + 1);

			ElemTemplateElement* const	theAttributeSet =
				m_constructionContext.createElement(
					xslToken,
					m_stylesheet,
					atts,
					locator);

			theAttributeSet->addToStylesheet(m_constructionContext, m_stylesheet);

			m_elemStack.push_back(theAttributeSet);
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
							m_constructionContext,
							atts,
							locator);
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
			error(XalanMessageLoader::getMessage(XalanMessages::IsNotAllowedInsideStylesheet_1Param, name), locator);
		}
		break;

	case StylesheetConstructionContext::ELEMNAME_STYLESHEET:
		processStylesheet(name, atts, locator, fPreserveSpace, fSpaceAttrProcessed);
		break;

	default:
		if (inExtensionElement() == false)
		{
			if (m_constructionContext.getXSLTVersionSupported() < m_stylesheet.getXSLTVerDeclared())
			{
				// Forward-compatible mode...
				m_inExtensionElementStack.back() = true;
			}
			else
			{
				error(XalanMessageLoader::getMessage(XalanMessages::UnknownXSLElement_1Param, name), locator);
			}
		}
		break;
	}
}


void
StylesheetHandler::processStylesheet(
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator,
			bool&						fPreserveSpace,
			bool&						fSpaceAttrProcessed)
{
	m_foundStylesheet = true;

	const unsigned int	nAttrs = atts.getLength();

	bool				fVersionFound = false;

	for(unsigned int i = 0; i < nAttrs; ++i)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
		{
			m_stylesheet.processExcludeResultPrefixes(m_constructionContext, atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
		{
			const GetAndReleaseCachedString		theGuard(m_constructionContext);

			XalanDOMString&		prefix = theGuard.get();

			StringTokenizer tokenizer(atts.getValue(i),
									  Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

			while(tokenizer.hasMoreTokens() == true)
			{
				tokenizer.nextToken(prefix);

				const XalanDOMString* const		extns = getNamespaceForPrefixFromStack(prefix);

				if (extns == 0)
				{
					error(
						XalanMessageLoader::getMessage(
							XalanMessages::PrefixMustResolveToNamespace_1Param,
							prefix),
						locator);
				}

				m_stylesheet.processExtensionNamespace(m_constructionContext, *extns);
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
				illegalAttributeError(name, aname, locator);
			}
		}

		if(!m_stylesheet.getNamespaces().empty())
		{
			m_stylesheet.setNamespaceDecls(m_stylesheet.getNamespaces().back());
		}
	}

	if (fVersionFound == false)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::StylesheetAttribDidNotSpecifyVersionAttrib), locator);
	}
}



void
StylesheetHandler::processExtensionElement(
			const XalanDOMChar*			/* name */,
			const XalanDOMString&		/* localName */,
			const AttributeListType&	/* atts */,
			const LocatorType*			/* locator */)
{
}



void
StylesheetHandler::checkForOrAddVariableName(
			const XalanQName&	theVariableName,
			const LocatorType*	theLocator)
{
	if (m_inTemplate == false)
	{
		assert(m_inScopeVariableNamesStack.empty() == true);

		if (m_globalVariableNames.count(theVariableName) != 0)
		{
			error(XalanMessageLoader::getMessage(XalanMessages::VariableHasBeenDeclared), theLocator);
		}
		else
		{
			m_globalVariableNames.insert(theVariableName);
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

			if (theLocalScope.count(theVariableName) != 0)
			{
				error(XalanMessageLoader::getMessage(XalanMessages::VariableHasBeenDeclaredInThisTemplate), theLocator);
			}

			++theCurrent;
		}

		assert(theCurrent == theEnd);

		m_inScopeVariableNamesStack.back().insert(theVariableName);
	}
}



void
StylesheetHandler::processPreserveStripSpace(
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator,
			int							xslToken)
{
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

			const GetAndReleaseCachedString		theGuard(m_constructionContext);

			XalanDOMString&		theNameTest = theGuard.get();

            const XalanQName::PrefixResolverProxy   theProxy(m_stylesheet.getNamespaces(), m_stylesheet.getURI());

			while(tokenizer.hasMoreTokens())
			{
				tokenizer.nextToken(theNameTest);

				m_stylesheet.addWhitespaceElement(
					XalanSpaceNodeTester(
							isPreserveSpace == true ?
								XalanSpaceNodeTester::ePreserve :
								XalanSpaceNodeTester::eStrip,
							m_constructionContext,
							theNameTest,
							theProxy,
							locator));
			}
		}
		else if(!isAttrOK(aname, atts, i))
		{
			illegalAttributeError(name, aname, locator);
		}
	}

	if(!foundIt && inExtensionElement() == false)
	{
		error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				isPreserveSpace == true ? 
				    Constants::ELEMNAME_PRESERVESPACE_WITH_PREFIX_STRING :
					Constants::ELEMNAME_STRIPSPACE_WITH_PREFIX_STRING,
				Constants::ATTRNAME_ELEMENTS),
			locator);
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
			const LocatorType*		locator)
{
	appendChildElementToParent(m_elemStack.back(), elem, locator);
}



void
StylesheetHandler::appendChildElementToParent(
			ElemTemplateElement*	parent,
			ElemTemplateElement*	elem,
			const LocatorType*		locator)
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
			if (elem->getXSLToken() == StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT)
			{
				error(
					XalanMessageLoader::getMessage(XalanMessages::ElemOrLTIsNotAllowed_1Param, elem->getElementName()),
					locator);
			}
			else
			{
				error(
					XalanMessageLoader::getMessage(XalanMessages::ElemIsNotAllowed_1Param, elem->getElementName()),
					locator);
			}
		}
	}
}



void
StylesheetHandler::doCleanup()
{
	if (m_locatorsPushed > 0)
	{
		m_constructionContext.popLocatorStack();

		--m_locatorsPushed;
	}

	m_lastPopped = 0;
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
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator)
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
				error(XalanMessageLoader::getMessage(XalanMessages::ImportCanOnlyOccur), locator);
			}

			const GetAndReleaseCachedString		theGuard1(m_constructionContext);

			XalanDOMString&		saved_XSLNameSpaceURL = theGuard1.get();

			saved_XSLNameSpaceURL = m_stylesheet.getXSLTNamespaceURI();

			const GetAndReleaseCachedString		theGuard2(m_constructionContext);

			XalanDOMString&		href = theGuard2.get();

			href = atts.getValue(i);

			Stylesheet::URLStackType&	includeStack = m_stylesheet.getIncludeStack();
			assert(includeStack.empty() == false);

			const XalanDOMString	hrefUrl = m_constructionContext.getURLStringFromString(href, includeStack.back());
			assert(length(hrefUrl) != 0);

			Stylesheet::URLStackType&	importStack = m_stylesheet.getStylesheetRoot().getImportStack();

			if(stackContains(importStack, hrefUrl))
			{
				error(XalanMessageLoader::getMessage(XalanMessages::ImportingItself_1Param, hrefUrl), locator);
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

			// Add it to the imports, releasing the XalanAutoPtr...
			m_stylesheet.addImport(importedStylesheet.get());

			importedStylesheet.release();

			assert(equals(importStack.back(), hrefUrl));
			importStack.pop_back();		

			m_stylesheet.setXSLTNamespaceURI(saved_XSLNameSpaceURL);
		}
		else if(!isAttrOK(aname, atts, i))
		{
			illegalAttributeError(name, aname, locator);
		}
	}

	if(!foundIt)
	{
		error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				Constants::ELEMNAME_IMPORT_WITH_PREFIX_STRING,
				Constants::ATTRNAME_HREF),
			locator);
	}
}



void
StylesheetHandler::processInclude(
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator)
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

				error(XalanMessageLoader::getMessage(XalanMessages::IncludingItself_1Param,hrefUrl) , locator);
			}

			m_stylesheet.getIncludeStack().push_back(hrefUrl);

			m_constructionContext.parseXML(hrefUrl, this, 0);

			assert(equals(m_stylesheet.getIncludeStack().back(), hrefUrl));
			m_stylesheet.getIncludeStack().pop_back();
		}
		else if(!isAttrOK(aname, atts, i))
		{
			illegalAttributeError(name, aname, locator);
		}
	}

	if(!foundIt)
	{
		error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				Constants::ELEMNAME_INCLUDE_WITH_PREFIX_STRING,
				Constants::ATTRNAME_HREF),
			locator);
	}
}



void
StylesheetHandler::endElement(const XMLCh* const	/* name */)
{
	processAccumulatedText();

	m_whiteSpaceElems.clear();

	m_stylesheet.popNamespaces();

	assert(m_elemStack.empty() == false);

	m_lastPopped = m_elemStack.back();

	assert(m_lastPopped != 0);

	m_elemStack.pop_back();
	m_lastPopped->setFinishedConstruction(true);

	const int tok = m_lastPopped->getXSLToken();

	if (m_inTemplate == true)
	{
		assert(m_inScopeVariableNamesStack.empty() == false);

		m_inScopeVariableNamesStack.pop_back();
	}

	if(StylesheetConstructionContext::ELEMNAME_TEMPLATE == tok)
	{
		m_inTemplate = false;
		m_pTemplate->addToStylesheet(m_constructionContext, m_stylesheet);
		m_pTemplate = 0;
	}
	else if((StylesheetConstructionContext::ELEMNAME_PARAM == tok) ||
			 StylesheetConstructionContext::ELEMNAME_VARIABLE == tok)
	{
		if(m_lastPopped->getParentNodeElem() == 0)
		{
			// Top-level param or variable
			m_inTemplate = false;
		}
	}
	else if(StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET == tok)
	{
		m_inTemplate = false;
	}

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
	if (m_inTemplate == false &&
		inExtensionElement() == false &&
		isXMLWhitespace(chars, 0, length) == false)
	{
		error(
			XalanMessageLoader::getMessage(XalanMessages::CharIsNotAllowedInStylesheet),
			m_constructionContext.getLocatorFromStack());
			  
	}
	else
	{
		accumulateText(chars, length);
	}
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

		const LocatorType* const	locator = m_constructionContext.getLocatorFromStack();

		ElemTemplateElement* const	elem =
			m_constructionContext.createElement(
				m_stylesheet,
				chars,
				length,
				preserveSpace, 
				disableOutputEscaping,
				locator);
		assert(elem != 0);

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
				elem);
		}
		else if(isWhite)
		{
			bool	shouldPush = true;

			ElemTemplateElement* const	last = parent->getLastChildElem();

			if(0 != last)
			{
				// If it was surrounded by xsl:text, it will count as an element.
				const bool	isPrevCharData =
					StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT == last->getXSLToken();

				const bool	isLastPoppedXSLText = (m_lastPopped != 0) &&
						(StylesheetConstructionContext::ELEMNAME_TEXT == m_lastPopped->getXSLToken());

				if(isPrevCharData == true && isLastPoppedXSLText == false)
				{
					appendChildElementToParent(
						parent,
						elem);

					shouldPush = false;
				}
			}

			if(shouldPush)
			{
				m_whiteSpaceElems.push_back(elem);
			}
		}
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
	XALAN_USING_STD(find)

	if (!(find(
			m_inExtensionElementStack.begin(),
			m_inExtensionElementStack.end(),
			true) == m_inExtensionElementStack.end()))
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
			const char*			theMessage,
			const LocatorType*	theLocator) const
{
	m_constructionContext.error(theMessage, 0, theLocator);
}



void
StylesheetHandler::error(
			const XalanDOMString&	theMessage,
			const LocatorType*		theLocator) const
{
	m_constructionContext.error(theMessage, 0, theLocator);
}



void
StylesheetHandler::error(
			const XalanDOMChar*		theMessage1,
			const XalanDOMChar*		theMessage2,
			const LocatorType*		theLocator) const
{
	const GetAndReleaseCachedString		theGuard(m_constructionContext);

	XalanDOMString&		msg = theGuard.get();

	msg = theMessage1;
	msg += theMessage2;

	error(msg, theLocator);
}




void
StylesheetHandler::error(
			const XalanDOMChar*		theMessage1,
			const XalanDOMString&	theMessage2,
			const LocatorType*		theLocator) const
{
	error(theMessage1, theMessage2.c_str(), theLocator);
}



void
StylesheetHandler::error(
			const XalanDOMString&	theMessage1,
			const XalanDOMChar*		theMessage2,
			const LocatorType*		theLocator) const
{
	error(theMessage1.c_str(), theMessage2, theLocator);
}




void
StylesheetHandler::error(
			const XalanDOMString&	theMessage1,
			const XalanDOMString&	theMessage2,
			const LocatorType*		theLocator) const
{
	error(theMessage1.c_str(), theMessage2.c_str(), theLocator);
}



void
StylesheetHandler::warn(
			const XalanDOMChar*		theMessage1,
			const XalanDOMString&	theMessage2,
			const LocatorType*		theLocator) const
{
	warn(theMessage1, theMessage2.c_str(), theLocator);
}

void
StylesheetHandler::warn(
			const XalanDOMString&	theMessage,
			const LocatorType*		theLocator) const
{
	m_constructionContext.warn(theMessage, 0, theLocator);
}



void
StylesheetHandler::warn(
			const XalanDOMChar*		theMessage1,
			const XalanDOMChar*		theMessage2,
			const LocatorType*		theLocator) const
{
	const GetAndReleaseCachedString		theGuard(m_constructionContext);

	XalanDOMString&		msg = theGuard.get();

	msg = theMessage1;
	msg += theMessage2;

	m_constructionContext.warn(msg, 0, theLocator);
}



void
StylesheetHandler::illegalAttributeError(
			const XalanDOMChar*		theElementName,
			const XalanDOMChar*		theAttributeName,
			const LocatorType*		theLocator) const

{
	const GetAndReleaseCachedString		theGuard(m_constructionContext);

	XalanDOMString&		msg = theGuard.get();

	msg = XalanMessageLoader::getMessage(XalanMessages::HasIllegalAttribute_2Param,XalanDOMString(theElementName),XalanDOMString(theAttributeName));


	error(msg, theLocator);
}



StylesheetHandler::PushPopIncludeState::PushPopIncludeState(StylesheetHandler&	theHandler) :
	m_handler(theHandler),
	m_elemStack(theHandler.m_elemStack),
	m_pTemplate(theHandler.m_pTemplate),
	m_lastPopped(theHandler),
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

	m_lastPopped.swap(theHandler.m_lastPopped);

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
	clear(m_handler.m_accumulateText);
	m_handler.m_elemStack = m_elemStack;
	m_handler.m_pTemplate = m_pTemplate;

	m_lastPopped.swap(m_handler.m_lastPopped);

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



void
StylesheetHandler::LastPoppedHolder::cleanup()
{
	if (m_lastPopped != 0)
	{
		const int tok = m_lastPopped->getXSLToken();

		if (tok == StylesheetConstructionContext::ELEMNAME_UNDEFINED)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			m_stylesheetHandler.m_elemEmptyAllocator.destroy((ElemEmpty*)m_lastPopped);
#else
			m_stylesheetHandler.m_elemEmptyAllocator.destroy(static_cast<ElemEmpty*>(m_lastPopped));
#endif
		}
		else if (tok == StylesheetConstructionContext::ELEMNAME_TEXT)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			m_stylesheetHandler.m_elemTextAllocator.destroy((ElemText*)m_lastPopped);
#else
			m_stylesheetHandler.m_elemTextAllocator.destroy(static_cast<ElemText*>(m_lastPopped));
#endif
		}
	}
}



const XalanDOMString	StylesheetHandler::s_emptyString;



void
StylesheetHandler::initialize()
{
}



void
StylesheetHandler::terminate()
{
}



XALAN_CPP_NAMESPACE_END
