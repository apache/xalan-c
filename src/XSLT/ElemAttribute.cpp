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
#include "ElemAttribute.hpp"



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemAttribute::ElemAttribute(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_ATTRIBUTE),
	m_pNameAVT(0),	
	m_pNamespaceAVT(0)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_pNameAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname,Constants::ATTRNAME_NAMESPACE))
		{
			m_pNamespaceAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
				 processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
				"xsl:attribute has an illegal attribute",
				0,
				this);
		}
	}

	if(0 == m_pNameAVT)
	{
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (AVT*)m_pNamespaceAVT;
#else
		delete m_pNamespaceAVT;
#endif

		constructionContext.error(
			"xsl:attribute must have a 'name' attribute",
			0,
			this);
	} 
	
}



ElemAttribute::~ElemAttribute()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (AVT*)m_pNameAVT;

	delete (AVT*)m_pNamespaceAVT;
#else
	delete m_pNameAVT;

	delete m_pNamespaceAVT;
#endif
}



const XalanDOMString&
ElemAttribute::getElementName() const
{
	return Constants::ELEMNAME_ATTRIBUTE_WITH_PREFIX_STRING;
}



void
ElemAttribute::execute(StylesheetExecutionContext&		executionContext) const
{
	assert(m_pNameAVT != 0);

	ElemTemplateElement::execute(executionContext);

	StylesheetExecutionContext::GetAndReleaseCachedString	attrNameGuard(executionContext);

	XalanDOMString&		attrName = attrNameGuard.get();

	XalanNode* sourceNode = executionContext.getCurrentNode();

	m_pNameAVT->evaluate(attrName, sourceNode, *this, executionContext);

	if(XalanQName::isValidQName(attrName) == false)
	{
		executionContext.warn(
			"The attribute name is invalid",
			sourceNode,
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

		if(0 != m_pNamespaceAVT)
		{
			m_pNamespaceAVT->evaluate(attrNameSpace, sourceNode, *this, executionContext);

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
							"Warning: Could not resolve prefix",
							sourceNode,
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
				"Attributes cannot be added after a child has been added.  The attribute(s) will not be added",
				sourceNode,
				getLocator());
		}

		// If there was no namespace, or the namespace was resolved, process
		// the result attribute.
		if (indexOfNSSep == origAttrNameLength || !isEmpty(attrNameSpace))
		{
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
	case Constants::ELEMNAME_TEXTLITERALRESULT:
	case Constants::ELEMNAME_APPLY_TEMPLATES:
	case Constants::ELEMNAME_APPLY_IMPORTS:
	case Constants::ELEMNAME_CALLTEMPLATE:
	case Constants::ELEMNAME_FOREACH:
	case Constants::ELEMNAME_VALUEOF:
	case Constants::ELEMNAME_COPY_OF:
	case Constants::ELEMNAME_NUMBER:
	case Constants::ELEMNAME_CHOOSE:
	case Constants::ELEMNAME_IF:
	case Constants::ELEMNAME_TEXT:
	case Constants::ELEMNAME_COPY:
	case Constants::ELEMNAME_VARIABLE:
	case Constants::ELEMNAME_MESSAGE:		
		// instructions 
		// case Constants.ELEMNAME_PI:
		// case Constants.ELEMNAME_COMMENT:
		// case Constants.ELEMNAME_ELEMENT:
		// case Constants.ELEMNAME_ATTRIBUTE:
		fResult = true;
		break;

	default:
		break;
	}

	return fResult;
}
