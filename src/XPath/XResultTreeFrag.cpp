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
#include "XResultTreeFrag.hpp"



#include <XalanDOM/XalanNodeList.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include "NodeRefListBase.hpp"
#include "ResultTreeFragBase.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathSupport.hpp"



XResultTreeFrag::XResultTreeFrag(
			XPathEnvSupport&			envSupport,
			XPathSupport&				support,
			const ResultTreeFragBase&	val,
			bool						deepClone) :
	XObject(&envSupport, &support),
	NodeRefListBase(),
	m_value(val.clone(deepClone)),
	m_cachedStringValue(),
	m_cachedNumberValue(0.0)
{
}



XResultTreeFrag::XResultTreeFrag(
			const XResultTreeFrag&	source,
			bool					deepClone) :
	XObject(source),
	NodeRefListBase(source),
	m_value(source.m_value->clone(deepClone)),
	m_cachedStringValue(source.m_cachedStringValue),
	m_cachedNumberValue(source.m_cachedNumberValue)
{
}



XResultTreeFrag::~XResultTreeFrag()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XResultTreeFrag*
#endif
XResultTreeFrag::clone(void*	theAddress) const
{
	return theAddress == 0 ? new XResultTreeFrag(*this) : new (theAddress) XResultTreeFrag(*this);
};



XResultTreeFrag::eObjectType
XResultTreeFrag::getType() const
{
	return eTypeResultTreeFrag;
}



XalanDOMString
XResultTreeFrag::getTypeString() const
{
	return XALAN_STATIC_UCODE_STRING("#RESULT_TREE_FRAG");
}



double
XResultTreeFrag::num() const
{
	if (m_cachedNumberValue == 0.0)
	{
#if defined(XALAN_NO_MUTABLE)
		((XNodeSet*)this)->m_cachedNumberValue = DoubleSupport::toDouble(str());
#else
		m_cachedNumberValue = DoubleSupport::toDouble(str());
#endif
	}

	return m_cachedNumberValue;
}



bool
XResultTreeFrag::boolean() const
{
    bool					fResult = false;

	const XalanNode*	theCurrentNode = m_value->getFirstChild();

    while(theCurrentNode != 0)
    {
		if(XalanNode::TEXT_NODE == theCurrentNode->getNodeType())
		{
			const XalanText* const	theTextNode =
				static_cast<const XalanText*>(theCurrentNode);

			if (m_support->isIgnorableWhitespace(*theTextNode) ||
			    length(trim(theTextNode->getData())) == 0)
			{
				continue;
			}
      
			fResult = true;
			break;
		}

		theCurrentNode = theCurrentNode->getNextSibling();
	}

    return fResult;
}



XalanDOMString
XResultTreeFrag::str() const
{
	if (isEmpty(m_cachedStringValue) == true)
	{
#if defined(XALAN_NO_MUTABLE)
		((XResultTreeFrag*)this)->m_cachedStringValue = m_support->getNodeData(*m_value.get());
#else
		m_cachedStringValue = m_support->getNodeData(*m_value.get());
#endif
	}

	return m_cachedStringValue;
}



const ResultTreeFragBase&
XResultTreeFrag::rtree() const
{
	return *m_value.get();
}



ResultTreeFragBase&
XResultTreeFrag::rtree()
{
	return *m_value.get();
}



const NodeRefListBase&
XResultTreeFrag::nodeset() const
{
	return *this;
}



const MutableNodeRefList&
XResultTreeFrag::mutableNodeset() const
{
	error("Can't cast XResultTreeFrag to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



MutableNodeRefList&
XResultTreeFrag::mutableNodeset()
{
	error("Can't cast XResultTreeFrag to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.ResultTreeFragment(*this,
										 rtree());
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.ResultTreeFragment(*this,
										 rtree());
}



bool
XResultTreeFrag::equals(const XObject&	theRHS) const
{
	return ::equals(str(), theRHS.str());
}



XalanNode*
XResultTreeFrag::item(unsigned int	index) const
{
	assert(m_value.get() != 0);

	XalanNode*	theCurrentChild = m_value->getFirstChild();

	for(unsigned int i = 0; i < index && theCurrentChild != 0; ++i)
	{
		theCurrentChild = theCurrentChild->getNextSibling();
	}

	return theCurrentChild;
}



unsigned int
XResultTreeFrag::getLength() const
{
	assert(m_value.get() != 0);

	unsigned int	theLength = 0;

	XalanNode*	theCurrentChild = m_value->getFirstChild();

	while(theCurrentChild != 0)
	{
		++theLength;
		theCurrentChild = theCurrentChild->getNextSibling();
	}

	return theLength;
}



unsigned int
XResultTreeFrag::indexOf(const XalanNode*	theNode) const
{
	unsigned	theIndex = 0;
	bool		fFound = false;

	XalanNode*	theCurrentChild = m_value->getFirstChild();

	while(theCurrentChild != 0 && fFound == false)
	{
		if (theCurrentChild == theNode)
		{
			fFound = true;
		}
		else
		{
			theIndex++;
			theCurrentChild = theCurrentChild->getNextSibling();
		}
	}

	return fFound == true ? theIndex : NodeRefListBase::npos;
}



XPathSupport*
XResultTreeFrag::getSupport() const
{
	return m_support;
}
