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
#include "ElemAttribute.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemAttribute::ElemAttribute(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_ATTRIBUTE),
	m_nameAVT(0),	
	m_namespaceAVT(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_nameAVT =
				constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_NAMESPACE))
		{
			m_namespaceAVT =
				constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				Constants::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			this);
	} 
}



ElemAttribute::~ElemAttribute()
{
}



const XalanDOMString&
ElemAttribute::getElementName() const
{
	return Constants::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING;
}



void
ElemAttribute::execute(StylesheetExecutionContext&	executionContext) const
{
	assert(m_nameAVT != 0);

	ElemTemplateElement::execute(executionContext);

	StylesheetExecutionContext::GetAndReleaseCachedString	attrNameGuard(executionContext);

	XalanDOMString&		attrName = attrNameGuard.get();

	m_nameAVT->evaluate(attrName, *this, executionContext);

	if(XalanQName::isValidQName(attrName) == false)
	{
		executionContext.warn(
			XalanMessageLoader::getMessage(XalanMessages::AttributeNameNotValidQName_1Param, attrName),
			executionContext.getCurrentNode(),
			getLocator());
	}
	else
	{
		// save original attribute name
		StylesheetExecutionContext::GetAndReleaseCachedString	origAttrNameGuard(executionContext);

		XalanDOMString&		origAttrName = origAttrNameGuard.get();

		assign(origAttrName, attrName);

		const XalanDOMString::size_type		origAttrNameLength = length(origAttrName);

		XalanDOMString::size_type			indexOfNSSep = 0;

		StylesheetExecutionContext::GetAndReleaseCachedString	attrNameSpaceGuard(executionContext);

		XalanDOMString&		attrNameSpace = attrNameSpaceGuard.get();

		if(0 != m_namespaceAVT)
		{
			m_namespaceAVT->evaluate(attrNameSpace, *this, executionContext);

			indexOfNSSep = indexOf(origAttrName, XalanUnicode::charColon);

			if(isEmpty(attrNameSpace))
			{
				// If there's no namespace, but the attribute has a
				// prefix, then we must strip the prefix off.
				if (indexOfNSSep < origAttrNameLength)
				{
					substring(origAttrName, attrName, indexOfNSSep + 1);
				}

				// We set this to indicate that there is no prefix any
				// longer.
				indexOfNSSep = origAttrNameLength;
			}
			else
			{
				// See if the namespace already exists.  If it does, we'll get the
				// prefix that was used when it was declared.
				const XalanDOMString*  const	prefix =
					executionContext.getResultPrefixForNamespace(attrNameSpace);

				// If there is already a prefix for the namespace, and it's length
				// is not 0, and there is no prefix on the attribute name, or
				// it's equal to the prefix on the attribute, then go ahead
				// and use that prefix.
				if(prefix != 0 &&
				   length(*prefix) != 0 &&
				   (indexOfNSSep == origAttrNameLength ||
				    equals(c_wstr(*prefix), c_wstr(attrName), indexOfNSSep) == true))
				{
					if(indexOfNSSep < origAttrNameLength)
					{
						reserve(
							attrName,
							length(attrName) - (indexOfNSSep + 1) + DOMServices::s_XMLNamespaceSeparatorStringLength + length(*prefix) + 1);

						attrName.erase(0, indexOfNSSep + 1);
					}
					else
					{
						reserve(
							attrName,
							length(attrName) + DOMServices::s_XMLNamespaceSeparatorStringLength + length(*prefix) + 1);
					}

					insert(attrName, 0, DOMServices::s_XMLNamespaceSeparatorString);
					insert(attrName, 0, *prefix);
				}
				else
				{
					StylesheetExecutionContext::GetAndReleaseCachedString	newPrefixGuard(executionContext);

					XalanDOMString&		newPrefix = newPrefixGuard.get();

					// If the prefix on the QName is xmlns, we cannot use it.
					const bool			fPrefixIsXMLNS =
						startsWith(origAttrName, DOMServices::s_XMLNamespaceWithSeparator);

					// If there's a prefix, and it's not xmlns, then use
					// the prefix that's provided.
					if(indexOfNSSep < origAttrNameLength &&
					    fPrefixIsXMLNS == false)
					{
						substring(origAttrName, newPrefix, 0, indexOfNSSep);

						// OK, make sure that the prefix provided maps to
						// the same namespace as the one the user requested,
						// and see if it's in use...
						const XalanDOMString* const	theNamespace =
							executionContext.getResultNamespaceForPrefix(newPrefix);

						if (theNamespace != 0 &&
							equals(*theNamespace, attrNameSpace) == false &&
							executionContext.isPendingResultPrefix(newPrefix) == true)
						{
							// It doesn't, so we'll need to manufacture a
							// prefix.
							clear(newPrefix);

							// Strip the user-supplied prefix from the name...
							substring(origAttrName, attrName, indexOfNSSep + 1);
						}
					}

					if (length(newPrefix) == 0)
					{
						// If there's a prefix, and it's xmlns, then strip it
						// off...
						if (fPrefixIsXMLNS == true)
						{
							attrName.erase(0, indexOfNSSep + 1);
						}

						// Get a new, unique namespace prefix...
						executionContext.getUniqueNamespaceValue(newPrefix);

						// Reserve some space in the string.
						reserve(
							attrName,
							length(attrName) + DOMServices::s_XMLNamespaceSeparatorStringLength + length(newPrefix) + 1);

						insert(attrName, 0, DOMServices::s_XMLNamespaceSeparatorString);
						insert(attrName, 0, newPrefix);
					}

					// OK, now we have to generate a namespace declaration...
					StylesheetExecutionContext::GetAndReleaseCachedString	nsDeclGuard(executionContext);

					XalanDOMString&		nsDecl = nsDeclGuard.get();

					reserve(nsDecl, DOMServices::s_XMLNamespaceWithSeparatorLength + length(newPrefix) + 1);

					assign(nsDecl, DOMServices::s_XMLNamespaceWithSeparator);

					append(nsDecl, newPrefix);

					// Add the namespace declaration...
					executionContext.addResultAttribute(nsDecl, attrNameSpace);
				}
			}
		}
      // Note we are using original attribute name for these tests. 
		else if(executionContext.isElementPending() == true &&
				!equals(origAttrName, DOMServices::s_XMLNamespace))
		{
			// Don't try to create a namespace declaration for anything that
			// starts with xml:
			if (startsWith(origAttrName, DOMServices::s_XMLString) == true)
			{
				// This just fakes out the test below.  It would be better if
				// we had a better way of testing this...
				indexOfNSSep = origAttrNameLength;
			}
			else
			{
				// make sure that if a prefix is specified on the attribute name, it is valid
				indexOfNSSep = indexOf(origAttrName, XalanUnicode::charColon);

				if(indexOfNSSep < origAttrNameLength)
				{
					StylesheetExecutionContext::GetAndReleaseCachedString	nsprefixGuard(executionContext);

					XalanDOMString&		nsprefix = nsprefixGuard.get();

					substring(origAttrName, nsprefix, 0, indexOfNSSep);

					const XalanDOMString* const		theNamespace =
						getNamespaceForPrefix(nsprefix);

					if (theNamespace != 0)
					{
						assign(attrNameSpace, *theNamespace);
					}

					if (isEmpty(attrNameSpace))
					{
						// Could not resolve prefix
						executionContext.warn(
							XalanMessageLoader::getMessage(XalanMessages::CouldNotResolvePrefix),
							executionContext.getCurrentNode(),
							getLocator());
					}
					else
					{
						// Check to see if there's already a namespace declaration in scope...
						const XalanDOMString* const		prefix =
							executionContext.getResultPrefixForNamespace(attrNameSpace);

						if (prefix == 0)
						{
							// We need to generate a namespace declaration...
							StylesheetExecutionContext::GetAndReleaseCachedString	nsDeclGuard(executionContext);

							XalanDOMString&		nsDecl = nsDeclGuard.get();

							reserve(nsDecl, DOMServices::s_XMLNamespaceWithSeparatorLength + length(nsprefix) + 1);

							assign(nsDecl, DOMServices::s_XMLNamespaceWithSeparator);

							append(nsDecl, nsprefix);

							// Add the namespace declaration...
							executionContext.addResultAttribute(nsDecl, attrNameSpace);
						}
					}
				}
			}
		}
		else
		{
			executionContext.warn(
				XalanMessageLoader::getMessage(XalanMessages::AttributesCannotBeAdded),
				executionContext.getCurrentNode(),
				getLocator());
		}

		// If there was no namespace, or the namespace was resolved, process
		// the result attribute.
		if (indexOfNSSep == origAttrNameLength || !isEmpty(attrNameSpace))
		{
			StylesheetExecutionContext::SetAndRestoreCopyTextNodesOnly	theSetAndRestore(executionContext, true);

			childrenToResultAttribute(
				executionContext,
				attrName);
		}
	}
}



bool
ElemAttribute::childTypeAllowed(int		xslToken) const
{
	bool	fResult = false;

	switch(xslToken)
	{
		// char-instructions 
	case StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT:
	case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
	case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
	case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
	case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
	case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
	case StylesheetConstructionContext::ELEMNAME_COPY_OF:
	case StylesheetConstructionContext::ELEMNAME_NUMBER:
	case StylesheetConstructionContext::ELEMNAME_CHOOSE:
	case StylesheetConstructionContext::ELEMNAME_IF:
	case StylesheetConstructionContext::ELEMNAME_TEXT:
	case StylesheetConstructionContext::ELEMNAME_COPY:
	case StylesheetConstructionContext::ELEMNAME_VARIABLE:
	case StylesheetConstructionContext::ELEMNAME_MESSAGE:
		fResult = true;
		break;

	default:
		break;
	}

	return fResult;
}



XALAN_CPP_NAMESPACE_END
