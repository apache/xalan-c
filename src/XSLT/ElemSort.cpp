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
#include "ElemSort.hpp"



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"



ElemSort::ElemSort(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_SORT),
	m_selectPattern(0),
	m_langAVT(0),
	m_dataTypeAVT(0),
	m_orderAVT(0),
	m_caseOrderAVT(0)
{
	const unsigned int nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern 
				= constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if(equals(aname, Constants::ATTRNAME_LANG))
		{			
			m_langAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_DATATYPE))
		{
			m_dataTypeAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_ORDER))
		{
			m_orderAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(equals(aname, Constants::ATTRNAME_CASEORDER))
		{
			constructionContext.warn("Xalan C++ does not yet handle the 'case-order' attribute!");

			m_caseOrderAVT = new AVT(getLocator(), aname, atts.getType(i), atts.getValue(i),
				*this, constructionContext);
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(Constants::ELEMNAME_SORT_WITH_PREFIX_STRING + " has an illegal attribute: " + aname);
		}
	}

	if(0 == m_dataTypeAVT)
	{
		m_dataTypeAVT = new AVT(getLocator(), c_wstr(Constants::ATTRNAME_DATATYPE), c_wstr(Constants::ATTRTYPE_CDATA), c_wstr(Constants::ATTRVAL_DATATYPE_TEXT), 
			*this, constructionContext);
	}

	if(0 == m_orderAVT)
	{
		m_orderAVT = new AVT(getLocator(), c_wstr(Constants::ATTRNAME_ORDER),	c_wstr(Constants::ATTRTYPE_CDATA), c_wstr(Constants::ATTRVAL_ORDER_ASCENDING),
			*this, constructionContext);
	}

	if(0 == m_selectPattern)
	{
		m_selectPattern = constructionContext.createXPath(getLocator(), StaticStringToDOMString(XALAN_STATIC_UCODE_STRING(".")), *this);
	}
}


ElemSort::~ElemSort()
{
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (AVT*)m_langAVT;

	delete (AVT*)m_dataTypeAVT;

	delete (AVT*)m_orderAVT;

	delete (AVT*)m_caseOrderAVT;
#else
	delete m_langAVT;

	delete m_dataTypeAVT;

	delete m_orderAVT;

	delete m_caseOrderAVT;
#endif
}


const XalanDOMString&
ElemSort::getElementName() const
{
	return Constants::ELEMNAME_SORT_WITH_PREFIX_STRING;
}

