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
// Class header file.
#include "ResultTreeFrag.hpp"



#include <cassert>



#include <include/DOMHelper.hpp>



ResultTreeFrag::ResultTreeFrag(
			const DOM_Document&		theFactory,
			XPathSupport&			theSupport) :
	ResultTreeFragBase(theFactory),
	m_document(theFactory),
	m_children(&theSupport),
	m_surrogate(m_children)
{
}



ResultTreeFrag::ResultTreeFrag(const ResultTreeFrag&	theSource,
							   bool						deepClone) :
	ResultTreeFragBase(theSource),
	m_document(theSource.m_document),
	m_children(deepClone == false ? theSource.m_children : MutableNodeRefList()),
	m_surrogate(m_children)
{
	if (deepClone == true)
	{
		const int	theLength = theSource.getLength();

		for (int i = 0; i < theLength; i++)
		{
			m_children.addNode(theSource.item(i).cloneNode(true));
		}
	}
}



ResultTreeFrag::~ResultTreeFrag()
{
}



NodeImpl*
ResultTreeFrag::item(int index)
{
	return XALAN_DOM_NodeHack(m_children.item(index)).getImplementationObject();
};



int
ResultTreeFrag::getLength()
{
	return m_children.getLength();
};



NodeImpl*
ResultTreeFrag::appendChild(NodeImpl*	newChild)
{
	XALAN_DOM_NodeHack	theHack(newChild);

	theHack = appendChild(theHack);

	return theHack.getImplementationObject();
}



void
ResultTreeFrag::changed()
{
}



NodeImpl*
ResultTreeFrag::cloneNode(bool	deep)
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return dynamic_cast<ResultTreeFrag*>(clone(deep));
#else
	clone(deep);
#endif
}



NamedNodeMapImpl*
ResultTreeFrag::getAttributes()
{
	return 0;
}



NodeListImpl*
ResultTreeFrag::getChildNodes()
{
	return &m_surrogate;
}



NodeImpl*
ResultTreeFrag::getFirstChild()
{
	return XALAN_DOM_NodeHack(m_children.item(0)).getImplementationObject();
}



NodeImpl*
ResultTreeFrag::getLastChild()
{
	return XALAN_DOM_NodeHack(m_children.item(m_children.getLength() - 1)).getImplementationObject();
}



DocumentImpl*
ResultTreeFrag::getOwnerDocument()
{
	XALAN_DOM_NodeHack	theHack(m_document);

	return dynamic_cast<DocumentImpl*>(theHack.getImplementationObject());
}



bool
ResultTreeFrag::hasChildNodes()
{
	return const_cast<const ResultTreeFrag*>(this)->hasChildNodes();
}



NodeImpl*
ResultTreeFrag::insertBefore(
			NodeImpl*	newChild,
			NodeImpl*	refChild)
{
	XALAN_DOM_NodeHack	theHack(insertBefore(XALAN_DOM_NodeHack(newChild),
											 XALAN_DOM_NodeHack(refChild)));

	return theHack.getImplementationObject();
}



NodeImpl*
ResultTreeFrag::removeChild(NodeImpl*	oldChild)
{
	XALAN_DOM_NodeHack	theHack(removeChild(XALAN_DOM_NodeHack(oldChild)));

	return theHack.getImplementationObject();
}



NodeImpl*
ResultTreeFrag::replaceChild(
			NodeImpl*	newChild,
			NodeImpl*	oldChild)
{
	XALAN_DOM_NodeHack	theHack(replaceChild(XALAN_DOM_NodeHack(newChild),
											 XALAN_DOM_NodeHack(oldChild)));

	return theHack.getImplementationObject();
}



void
ResultTreeFrag::setReadOnly(
			bool	/* readOnly */,
			bool	/* deep */)
{
}


DOMString
toString()
{
	// $$$ ToTo: Fix this!!!
	return DOMString();
}



DOM_Node
ResultTreeFrag::getFirstChild() const
{
	return m_children.getLength() > 0 ? m_children.item(0) : DOM_Node();
}



DOM_Node
ResultTreeFrag::getLastChild() const
{
	const int	theLength = m_children.getLength();

	return  theLength > 0 ? m_children.item(theLength - 1) : DOM_Node();
}



DOM_Document
ResultTreeFrag::getOwnerDocument() const
{
	return m_document;
}



DOM_Node
ResultTreeFrag::insertBefore(
			const DOM_Node&		newChild, 
			const DOM_Node&		refChild)
{
	const int	refIndex = 0 == refChild ? m_children.getLength() :
										   m_children.indexOf(refChild);

	assert(refIndex >= 0);

	m_children.insertNode(newChild, refIndex);

	return newChild;
}



DOM_Node
ResultTreeFrag::replaceChild(
			const DOM_Node&		newChild,
			const DOM_Node&		oldChild)
{
	const int	refIndex =
		0 == oldChild ? -1 : m_children.indexOf(oldChild);

	if(refIndex > -1)
	{
		const int	newChildIndex = m_children.indexOf(newChild);

		// Set the new child first, then erase it from
		// the old position. if it's there.
		m_children.setNode(refIndex, newChild);

		if(newChildIndex > -1)
		{
			m_children.removeNode(newChildIndex);
		}
	}

	return oldChild;
}



DOM_Node
ResultTreeFrag::removeChild(const DOM_Node&		oldChild)
{
	m_children.removeNode(oldChild);

	return oldChild;
}



DOM_Node
ResultTreeFrag::appendChild(const DOM_Node&		newChild)
{
	m_children.addNode(newChild);

	return newChild;
}



const NodeRefListBase&
ResultTreeFrag::getChildNodesAsNodeRefList() const
{
	return m_children;
}



bool
ResultTreeFrag::hasChildNodes() const
{
	return m_children.getLength() > 0 ? true : false;
}



int
ResultTreeFrag::getLength() const
{
	return m_children.getLength();
};



DOM_Node
ResultTreeFrag::item(int index) const
{
	return m_children.item(index);
};



DOM_Node
ResultTreeFrag::cloneNode(bool	deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	return DOM_ResultTreeFragBase(dynamic_cast<ResultTreeFrag*>(clone(deep)));
#else
	return DOM_ResultTreeFragBase(clone(deep));
#endif
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Cloneable*
#else
ResultTreeFrag*
#endif
ResultTreeFrag::clone(bool	deep) const
{
	return new ResultTreeFrag(*this,
							  deep);
}



DOMString
ResultTreeFrag::toString()
{
	return DOMString();
}
