/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
// Class header file...
#include "XalanDocumentPrefixResolver.hpp"



#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanDocumentPrefixResolver::XalanDocumentPrefixResolver(
			const XalanDocument*	theDocument,
			const XalanDOMString&	theURI) :
	m_namespaces(),
	m_uri(theURI)
{
	assert(theDocument != 0);

	NamespaceNodesTreeWalker	theWalker(m_namespaces);

	theWalker.traverse(theDocument);
}



XalanDocumentPrefixResolver::~XalanDocumentPrefixResolver()
{
}



const XalanDOMString*
XalanDocumentPrefixResolver::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	const NamespacesMapType::const_iterator		i = m_namespaces.find(&prefix);

	if (i == m_namespaces.end())
	{
		return 0;
	}
	else
	{
		const AttributeVectorType&	theVector = (*i).second;
		assert(theVector.empty() == false);

		if (theVector.size() == 1)
		{
			assert(theVector.front() != 0);

			return &(theVector.front()->getNodeValue());
		}
		else
		{
			return duplicateBinding(theVector);
		}
	}
}



const XalanDOMString&
XalanDocumentPrefixResolver::getURI() const
{
	return m_uri;
}



const XalanDOMString*
XalanDocumentPrefixResolver::duplicateBinding(const AttributeVectorType&	theVector) const
{
	assert(theVector.front() != 0);

	return &(theVector.front()->getNodeValue());
}



XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::NamespaceNodesTreeWalker(NamespacesMapType& 	theMap) :
	TreeWalker(),
	m_map(theMap)
{
}



XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::~NamespaceNodesTreeWalker()
{
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::startNode(const XalanNode*	node)
{
	assert(node != 0);

	switch(node->getNodeType())
	{
	case XalanNode::ELEMENT_NODE:
		{
			const XalanElement*	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanElement*)node;
#else
				static_cast<const XalanElement*>(node);
#endif

			const XalanNamedNodeMap* const	atts = theElementNode->getAttributes();
			assert(atts != 0);

			const unsigned int	theSize = atts->getLength();

			for (unsigned int i = 0; i < theSize; ++i)
			{
				assert(atts->item(i) != 0 && atts->item(i)->getNodeType() == XalanNode::ATTRIBUTE_NODE);

				const XalanAttr* const	theAttr =
#if defined(XALAN_OLD_STYLE_CASTS)
					(const XalanAttr*)atts->item(i);
#else
					static_cast<const XalanAttr*>(atts->item(i));
#endif

				if (DOMServices::isNamespaceDeclaration(*theAttr) == true)
				{
					m_map[&theAttr->getLocalName()].push_back(theAttr);
				}
			}
		}
		break;

	default:
		// Do nothing...
		break;
	}

	return false;
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::startNode(XalanNode*		node)
{
	assert(node != 0);

#if defined(XALAN_OLD_STYLE_CASTS)
	return startNode((const XalanNode*)node);
#else
	return startNode(const_cast<const XalanNode*>(node));
#endif
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::endNode(const XalanNode*		/* node */)
{
	return false;
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::endNode(XalanNode*	/* node */)
{
	return false;
}



XALAN_CPP_NAMESPACE_END
