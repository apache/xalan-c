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



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemAttribute::ElemAttribute(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString&			name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						name,
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
			m_pNameAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname,Constants::ATTRNAME_NAMESPACE))
		{
			m_pNamespaceAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) || 
			processSpaceAttr(aname, atts, i)))
		{
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_pNameAVT)
	{
#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (AVT*)m_pNamespaceAVT;
#else
		delete m_pNamespaceAVT;
#endif

		constructionContext.error(name + " must have a name attribute.");
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



void
ElemAttribute::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	assert(m_pNameAVT != 0);

	ElemTemplateElement::execute(executionContext, sourceTree, sourceNode, mode);

	XalanDOMString attrName;

	m_pNameAVT->evaluate(attrName, sourceNode, *this, executionContext);

	if(!isEmpty(attrName))
	{
		const XalanDOMString	origAttrName = attrName;      // save original attribute name

		const unsigned int		origAttrNameLength = length(origAttrName);

		unsigned int			indexOfNSSep = 0;

		XalanDOMString			attrNameSpace;

		if(0 != m_pNamespaceAVT)
		{
			m_pNamespaceAVT->evaluate(attrNameSpace, sourceNode, *this, executionContext);

			if(!isEmpty(attrNameSpace))
			{
				XalanDOMString prefix = executionContext.getResultPrefixForNamespace(attrNameSpace);

				if(isEmpty(prefix))
				{
					prefix = executionContext.getUniqueNameSpaceValue();

					XalanDOMString nsDecl = XalanDOMString(DOMServices::s_XMLNamespaceWithSeparator) + prefix;

					executionContext.addResultAttribute(nsDecl, attrNameSpace);
				}

				indexOfNSSep = indexOf(origAttrName, ':');

				if(indexOfNSSep < origAttrNameLength)
				{
					attrName = substring(attrName, indexOfNSSep + 1);
				}

				attrName = prefix + XalanDOMString(XALAN_STATIC_UCODE_STRING(":")) + attrName;
			}
		}
      // Note we are using original attribute name for these tests. 
		else if(!isEmpty(executionContext.getPendingElementName())
				&& !equals(origAttrName, DOMServices::s_XMLNamespace))
		{
			// make sure that if a prefix is specified on the attribute name, it is valid
			indexOfNSSep = indexOf(origAttrName, ':');

			if(indexOfNSSep < origAttrNameLength)
			{
				const XalanDOMString	nsprefix = substring(origAttrName, 0, indexOfNSSep);

				attrNameSpace = getNamespaceForPrefix(nsprefix);

				if (isEmpty(attrNameSpace))
				{
					// Could not resolve prefix
					executionContext.warn(XalanDOMString("Warning: Could not resolve prefix ") + nsprefix, sourceNode, this);
				}
			}
		}
		else
		{
			//warn(templateChild, sourceNode, "Trying to add attribute after element child has been added, ignoring...");
			executionContext.warn("Warning: Trying to add attribute after element child has been added, ignoring...", sourceNode, this);
		}

		// If there was no namespace, or the namespace was resolved, process
		// the result attribute.
		if (indexOfNSSep == origAttrNameLength || !isEmpty(attrNameSpace))
		{  
			const XalanDOMString	val =
				childrenToString(executionContext,
								 sourceTree,
								 sourceNode,
								 mode);

			executionContext.addResultAttribute(attrName, val);
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
