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
#include "XPathSupportDefault.hpp"



#include <algorithm>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanDocumentType.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanEntity.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <DOMSupport/DOMServices.hpp>
#include <DOMSupport/DOMSupport.hpp>
#include "XPathException.hpp"



XPathSupportDefault::XPathSupportDefault(DOMSupport&	theDOMSupport) :
	XPathSupport(),
	m_DOMSupport(theDOMSupport),
	m_whitespaceSupport(),
	m_processNamespaces(true)
{
}



XPathSupportDefault::~XPathSupportDefault()
{
}



void
XPathSupportDefault::reset()
{
	m_DOMSupport.reset();
}



bool
XPathSupportDefault::isIgnorableWhitespace(const XalanText&		node) const
{
	return m_whitespaceSupport.isIgnorableWhitespace(node);
}



const XalanDOMString&
XPathSupportDefault::getNamespaceForPrefix(
			const XalanDOMString&	prefix, 
			const XalanElement&		namespaceContext) const
{
	return DOMServices::getNamespaceForPrefix(prefix,
											  namespaceContext);
}



const XalanDOMString&
XPathSupportDefault::getNamespaceOfNode(const XalanNode&	n) const
{
	return m_DOMSupport.getNamespaceOfNode(n);
}



const XalanDOMString&
XPathSupportDefault::getNameOfNode(const XalanNode&		n) const
{
	return DOMServices::getNameOfNode(n);
}



const XalanDOMString&
XPathSupportDefault::getLocalNameOfNode(const XalanNode&	n) const
{
	return DOMServices::getLocalNameOfNode(n);
}



void
XPathSupportDefault::getNodeData(
			const XalanNode&	n,
			XalanDOMString&		s) const
{
	DOMServices::getNodeData(n, s);
}



XalanNode*
XPathSupportDefault::getParentOfNode(const XalanNode&	node) const
{
	return DOMServices::getParentOfNode(node);
}



bool
XPathSupportDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return DOMServices::isNodeAfter(node1, node2);
}



XalanElement*
XPathSupportDefault::getElementByID(
			const XalanDOMString&	id,
			const XalanDocument&	doc) const
{
	return doc.getElementById(id);
}



void
XPathSupportDefault::setProcessNamespaces(bool	processNamespaces)
{
	m_processNamespaces = processNamespaces;
}



bool
XPathSupportDefault::getProcessNamespaces() const
{
	return m_processNamespaces;
}



XalanDOMString
XPathSupportDefault::getUnparsedEntityURI(
			const XalanDOMString&		theName,
			const XalanDocument&		theDocument) const
{
	XalanDOMString					theURI;

	const XalanDocumentType* const	theDoctype =
		theDocument.getDoctype();

	if(theDoctype != 0)
	{
		const XalanNamedNodeMap* const	theEntities =
			theDoctype->getEntities();

		if (theEntities != 0)
		{
			const XalanNode* const	theNode =
				theEntities->getNamedItem(theName);

			if (theNode != 0 && theNode->getNodeType() == XalanNode::ENTITY_NODE)
			{
				const XalanEntity*	theEntity =
#if defined(XALAN_OLD_STYLE_CASTS)
					(const XalanEntity*)theNode;
#else
					static_cast<const XalanEntity*>(theNode);
#endif

				const XalanDOMString		theNotationName(theEntity->getNotationName());

				if(isEmpty(theNotationName) == false) // then it's unparsed
				{
					// The draft says: "The XSLT processor may use the public
					// identifier to generate a URI for the entity instead of the URI
					// specified in the system identifier. If the XSLT processor does
					// not use the public identifier to generate the URI, it must use
					// the system identifier; if the system identifier is a relative
					// URI, it must be resolved into an absolute URI using the URI of
					// the resource containing the entity declaration as the base
					// URI [RFC2396]."
					// So I'm falling a bit short here.
					theURI = theEntity->getSystemId();

					if(isEmpty(theURI) == true)
					{
						theURI = theEntity->getPublicId();
					}
					else
					{
						// This should be resolved to an absolute URL, but that's hard
						// to do from here.
					}
				}
			}
		}
	}

	return theURI;
}
