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
#include "ElemElement.hpp"



#include <sax/AttributeList.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemElement::ElemElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			Constants::ELEMNAME_ELEMENT),
	m_nameAVT(0),
	m_namespaceAVT(0)
{
	// Namespace aliases are not used for xsl:element, so
	// turn them off...
	m_namespacesHandler.setProcessNamespaceAliaises(false);

	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_nameAVT = new AVT(aname,	atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_NAMESPACE))
		{
			m_namespaceAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext); 
		}
		else if(!(processUseAttributeSets(constructionContext, aname, atts, i) || processSpaceAttr(aname, atts, i) ||
			isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_nameAVT)
	{
		constructionContext.error(Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING + " must have a name attribute.");
	}
	
}



ElemElement::~ElemElement()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (AVT*)m_nameAVT;

	delete (AVT*)m_namespaceAVT;
#else
	delete m_nameAVT;

	delete m_namespaceAVT;
#endif
}



const XalanDOMString&
ElemElement::getElementName() const
{
	return Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING;
}



void
ElemElement::execute(StylesheetExecutionContext&		executionContext) const
{
	StylesheetExecutionContext::GetAndReleaseCachedString	elemNameGuard(executionContext);

	XalanDOMString&		elemName = elemNameGuard.get();

	XalanNode* sourceNode = executionContext.getCurrentNode();

	m_nameAVT->evaluate(elemName, sourceNode, *this, executionContext);

	StylesheetExecutionContext::GetAndReleaseCachedString	elemNameSpaceGuard(executionContext);

	XalanDOMString&		elemNameSpace = elemNameSpaceGuard.get();

	if (m_namespaceAVT != 0)
	{
		m_namespaceAVT->evaluate(elemNameSpace, sourceNode, *this, executionContext);
	}

	bool				isIllegalElement = false;

	bool				hasUnresolvedPrefix = false;

	unsigned int		len = length(elemName);

	const unsigned int	indexOfNSSep = indexOf(elemName, XalanUnicode::charColon);

	StylesheetExecutionContext::GetAndReleaseCachedString	prefixGuard(executionContext);

	XalanDOMString&		prefix = prefixGuard.get();

	const bool			haveNamespace = indexOfNSSep == len ? false : true;

	if(haveNamespace == true)
	{
		if (indexOfNSSep + 1 == len ||
			isValidNCName(substring(elemName, indexOfNSSep + 1)) == false)
		{
			isIllegalElement = true;
		}
	}
	else if(len == 0 || isValidNCName(elemName) == false)
	{
		isIllegalElement = true;
	}

	if (isIllegalElement == true)
	{
		executionContext.warn("Illegal element name: \"" + elemName + "\"", this, sourceNode);

		clear(elemName);
	}
	else if (haveNamespace == true)
	{
		prefix = substring(elemName, 0, indexOfNSSep);

		const XalanDOMString* const		theNamespace =
			executionContext.getResultNamespaceForPrefix(prefix);

		if (theNamespace == 0)
		{
			const XalanDOMString* const		theNamespace =
				m_namespacesHandler.getNamespace(prefix);

			if(theNamespace == 0 && isEmpty(elemNameSpace))
			{
				executionContext.warn("Could not resolve prefix: " + prefix, this, sourceNode);

				if (m_namespaceAVT != 0)
				{
					hasUnresolvedPrefix = true;

					elemName = substring(elemName, indexOfNSSep + 1);
				}
				else
				{
					isIllegalElement = true;
				}
			}
		}
	}

	if (isIllegalElement == false)
	{
		executionContext.startElement(c_wstr(elemName));   

		m_namespacesHandler.outputResultNamespaces(executionContext);

		// OK, now let's check to make sure we don't have to change the default namespace...
		const XalanDOMString* const		theCurrentDefaultNamespace =
				executionContext.getResultNamespaceForPrefix(s_emptyString);

		if (theCurrentDefaultNamespace != 0)
		{
			const XalanDOMString* const		theElementDefaultNamespace =
					m_namespacesHandler.getNamespace(s_emptyString);

			if (hasUnresolvedPrefix == true || theElementDefaultNamespace == 0)
			{
				// There was no default namespace, so we have to turn the
				// current one off.
				executionContext.addResultAttribute(DOMServices::s_XMLNamespace, s_emptyString);
			}
			else if (equals(*theCurrentDefaultNamespace, *theElementDefaultNamespace) == false)
			{
				executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
			}
		}

		if(0 != m_namespaceAVT)
		{
			if(isEmpty(elemNameSpace) == false || hasUnresolvedPrefix == false)
			{
				if(indexOfNSSep == len)
				{
					executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
				}
				else
				{
					const XalanDOMString* const		theNamespace =
						executionContext.getResultNamespaceForPrefix(prefix);

					if (theNamespace == 0 ||
						equals(*theNamespace, elemNameSpace) == false)
					{
						insert(prefix, 0, DOMServices::s_XMLNamespaceWithSeparator);

						executionContext.addResultAttribute(prefix, elemNameSpace);
					}
				}
			}
		}
	}

	ElemUse::execute(executionContext);

	doExecuteChildren(executionContext, sourceNode, isIllegalElement);

	if (isIllegalElement == false)
	{
		executionContext.endElement(c_wstr(elemName));
	}
}



void
ElemElement::doExecuteChildren(
			StylesheetExecutionContext&		executionContext,			
			XalanNode*						sourceNode,
			bool							skipAttributeChildren) const
{
	if (skipAttributeChildren == false)
	{
		// If we should execute all children, then just call
		// executeChildren()...
		executeChildren(executionContext);
	}
	else
	{
		StylesheetExecutionContext::PushAndPopElementFrame	thePushAndPop(executionContext, this);

		for (ElemTemplateElement* node = getFirstChildElem(); node != 0; node = node->getNextSiblingElem()) 
		{
			if (node->getXSLToken() != Constants::ELEMNAME_ATTRIBUTE)
			{
				node->execute(executionContext);
			}
		}
	}
}
