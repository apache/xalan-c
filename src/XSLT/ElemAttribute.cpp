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

#include "ElemPriv.hpp"

ElemAttribute::ElemAttribute(
	XSLTEngineImpl& processor,
	Stylesheet& stylesheetTree,
	const DOMString& name, 
	const AttributeList& atts,
	int lineNumber, 
	int columnNumber) :
		ElemTemplateElement(processor, stylesheetTree, name,  lineNumber, columnNumber),
		m_pNameAVT(0),	
		m_pNamespaceAVT(0)
{
	const int nAttrs = atts.getLength();
	
	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname(atts.getName(i));

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			m_pNameAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, processor);
		}
		else if(equals(aname,Constants::ATTRNAME_NAMESPACE))
		{
			m_pNamespaceAVT = new AVT(aname, atts.getType(i), atts.getValue(i),
				*this, processor);
		}
		else if(!(isAttrOK(aname, atts, i) || 
			processSpaceAttr(aname, atts, i)))
		{
			processor.error(name + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_pNameAVT)
	{
		processor.error(name + " must have a name attribute.");
	} 
	
}

ElemAttribute::~ElemAttribute()
{
	delete m_pNameAVT;
	m_pNameAVT = 0;

	delete m_pNamespaceAVT;
	m_pNamespaceAVT = 0;
}

int ElemAttribute::getXSLToken() const 
{
	return Constants::ELEMNAME_ATTRIBUTE;
}

void ElemAttribute::execute(
	XSLTEngineImpl& processor, 
	const DOM_Node& sourceTree, 
	const DOM_Node& sourceNode,
	const QName& mode)
{
	ElemTemplateElement::execute(processor, sourceTree, sourceNode, mode);

	assert(m_pNameAVT);

	DOMString attrName;
	m_pNameAVT->evaluate(attrName, sourceNode, *this, 
		processor.getContextNodeList());

	if(!isEmpty(attrName))
	{
		if(0 != m_pNamespaceAVT)
		{
			DOMString attrNameSpace;
			m_pNamespaceAVT->evaluate(attrNameSpace, sourceNode, 
				*this, processor.getContextNodeList());

			if(!isEmpty(attrNameSpace))
			{
				DOMString prefix = processor.getResultPrefixForNamespace(attrNameSpace);

				if(isEmpty(prefix))
				{
					prefix = DOMString("ns") + LongToDOMString(processor.getUniqueNSValue());
					DOMString nsDecl = DOMString("xmlns:") + prefix;
					processor.addResultAttribute(processor.getPendingAttributes(), 
						nsDecl, attrNameSpace);
				}
				attrName = (prefix + DOMString(":") + attrName);
			}
		}
		
		if(!isEmpty(processor.getPendingElementName()))
		{
			DOMString val = childrenToString(processor, sourceTree, sourceNode, mode);
			
			processor.addResultAttribute(processor.getPendingAttributes(), attrName, val);
		}
		else
		{
			//warn(templateChild, sourceNode, "Trying to add attribute after element child has been added, ignoring...");
		}
	}
}



NodeImpl* ElemAttribute::appendChild(NodeImpl* newChild)
{
	assert(newChild != 0);
	
	const int	type = dynamic_cast<ElemTemplateElement*>(newChild)->getXSLToken();
	
	switch(type)
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
		break;
		
	default:
		error("Can not add " + dynamic_cast<ElemTemplateElement*>(newChild)->getTagName() + " to " + getTagName());
		break;
	}
	
	return ElemTemplateElement::appendChild(newChild);
}
