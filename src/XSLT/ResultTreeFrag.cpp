/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
#include "ResultTreeFrag.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



const XalanDOMString	ResultTreeFrag::s_emptyString;



ResultTreeFrag::ResultTreeFrag(XalanSourceTreeDocumentFragment*		theDocumentFragment) :
	ResultTreeFragBase(),
	XalanNodeList(),
	m_documentFragment(theDocumentFragment)
{
}



ResultTreeFrag::ResultTreeFrag(
			const ResultTreeFrag&	theSource,
			bool					deepClone) :
	ResultTreeFragBase(theSource),
	m_documentFragment(theSource.m_documentFragment == 0 ? 0 : theSource.m_documentFragment->clone(deepClone))
{
}



ResultTreeFrag::~ResultTreeFrag()
{
}



XalanNode*
ResultTreeFrag::getNodesetRoot() const
{
	if (m_documentFragment != 0)
	{
		return m_documentFragment;
	}
	else
	{
		// I hate to do this, but since all of the non-const
		// member functions will throw exceptions, I can live
		// with it.
#if defined(XALAN_OLD_STYLE_CASTS)
		return (ResultTreeFrag*)this;
#else
		return const_cast<ResultTreeFrag*>(this);
#endif
	}
}



void
ResultTreeFrag::clear()
{
	if (m_documentFragment != 0)
	{
		m_documentFragment->clearChildren();
	}
}



const XalanDOMString&
ResultTreeFrag::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getNodeValue() const
{
	return s_emptyString;
}



ResultTreeFrag::NodeType
ResultTreeFrag::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
ResultTreeFrag::getParentNode() const
{
	return 0;
}



const XalanNodeList*
ResultTreeFrag::getChildNodes() const
{
	return this;
}



XalanNode*
ResultTreeFrag::getFirstChild() const
{
	if (m_documentFragment != 0)
	{
		return m_documentFragment->getFirstChild();
	}
	else
	{
		return 0;
	}
}



XalanNode*
ResultTreeFrag::getLastChild() const
{
	if (m_documentFragment != 0)
	{
		return m_documentFragment->getLastChild();
	}
	else
	{
		return 0;
	}
}



XalanNode*
ResultTreeFrag::getPreviousSibling() const
{
	return 0;
}



XalanNode*
ResultTreeFrag::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
ResultTreeFrag::getAttributes() const
{
	return 0;
}



XalanDocument*
ResultTreeFrag::getOwnerDocument() const
{
	if (m_documentFragment != 0)
	{
		return m_documentFragment->getOwnerDocument();
	}
	else
	{
		return 0;
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
ResultTreeFrag*
#endif
ResultTreeFrag::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return new ResultTreeFrag(
		*this,
		deep);
#else
	return clone(deep);
#endif
}



XalanNode*
ResultTreeFrag::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
ResultTreeFrag::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
ResultTreeFrag::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
ResultTreeFrag::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}


bool
ResultTreeFrag::hasChildNodes() const
{
	if (m_documentFragment != 0)
	{
		return m_documentFragment->hasChildNodes();
	}
	else
	{
		return false;
	}
}



void
ResultTreeFrag::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
ResultTreeFrag::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
ResultTreeFrag::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
ResultTreeFrag::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
ResultTreeFrag::getLocalName() const
{
	return s_emptyString;
}



void
ResultTreeFrag::setPrefix(const XalanDOMString&		/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
ResultTreeFrag::isIndexed() const
{
	return false;
}



unsigned long
ResultTreeFrag::getIndex() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
ResultTreeFragBase*
#else
ResultTreeFrag*
#endif
ResultTreeFrag::clone(bool	deep) const
{
	return new ResultTreeFrag(*this, deep);
}



XalanNode*
ResultTreeFrag::item(unsigned int	index) const
{
	assert(index < getLength());

	if (m_documentFragment == 0)
	{
		return 0;
	}
	else
	{
		XalanNode*	theCurrentChild = m_documentFragment->getFirstChild();

		for(unsigned int i = 0; i < index && theCurrentChild != 0; ++i)
		{
			theCurrentChild = theCurrentChild->getNextSibling();
		}

		return theCurrentChild;
	}
}



unsigned int
ResultTreeFrag::getLength() const
{
	if (m_documentFragment == 0)
	{
		return 0;
	}
	else
	{
		unsigned int	theLength = 0;

		XalanNode*	theCurrentChild = m_documentFragment->getFirstChild();

		while(theCurrentChild != 0)
		{
			++theLength;
			theCurrentChild = theCurrentChild->getNextSibling();
		}

		return theLength;
	}
}
