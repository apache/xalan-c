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



#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_Entity.hpp>

// $$$ ToDo:  Eventually, we shouldn't have to do this, but we need ignorable
// whitespace support
#include <internal/XMLReader.hpp>


#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <DOMSupport/DOMServices.hpp>
#include <DOMSupport/DOMSupport.hpp>
#include "XPathException.hpp"



XPathSupportDefault::XPathSupportDefault(DOMSupport&	theDOMSupport) :
	XPathSupport(),
	DOMServices::WhitespaceSupport(),
	m_DOMSupport(theDOMSupport)
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



DOMString
XPathSupportDefault::getNamespaceForPrefix(
			const DOMString&	prefix, 
			const DOM_Element&	namespaceContext) const
{
	return DOMServices::getNamespaceForPrefix(prefix,
											  namespaceContext);
}



DOMString
XPathSupportDefault::getNamespaceOfNode(const DOM_Node&		n) const
{
	return m_DOMSupport.getNamespaceOfNode(n);
}



DOMString
XPathSupportDefault::getLocalNameOfNode(const DOM_Node&		n) const
{
	return DOMServices::getLocalNameOfNode(n);
}



DOMString
XPathSupportDefault::getNodeData(const DOM_Node&	node) const
{
	return DOMServices::getNodeData(node,
									*this);
}



/**
 * I have to write this silly, and expensive function, 
 * because the DOM WG decided that attributes don't 
 * have parents.  If XPath is used with a DOM implementation
 * that reuses attribute nodes, this will not work correctly.
 */
DOM_Node
XPathSupportDefault::getParentOfNode(const DOM_Node&	node) const
{
	return DOMServices::getParentOfNode(node);
}



bool
XPathSupportDefault::isIgnorableWhitespace(const DOM_Text&	node) const
{
// $$$ ToDo:  Eventually, we shouldn't have to do this, but we need ignorable
// whitespace support
	const DOMString		theData(node.getData());

	return XMLReader::isAllSpaces(toCharArray(theData), length(theData));
}



DOM_Document
XPathSupportDefault::parseXML(
			const DOMString&	/* urlString */,
			const DOMString&	/* base */) const
{
	return DOM_Document();
}



DOM_Document
XPathSupportDefault::parseXML(
			const URL&				/* url */,
			DocumentHandler&		/* docHandler */,
			const DOM_Document&		/* styleDoc */) const
{
	return DOM_Document();
}



DOM_Element
XPathSupportDefault::getElementByID(
			const DOMString&	/* id */,
			const DOM_Document&	/* doc */) const
{
	return DOM_Element();
}



URL*
XPathSupportDefault::getURLFromString(
			const DOMString&		/* urlString */,
			const DOMString&		/* base */) const
{
	return 0;
}



void
XPathSupportDefault::setProcessNamespaces(bool	/* processNamespaces */)
{
}



bool
XPathSupportDefault::getProcessNamespaces() const
{
	return false;
}



DOMString
XPathSupportDefault::getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const
{
	DOMString				theURI;

	const DOM_DocumentType	theDoctype = theDocument.getDoctype();

	if(theDoctype != 0)
	{
		DOM_NamedNodeMap	theEntities = theDoctype.getEntities();

		if (theEntities != 0)
		{
			const DOM_Node		theNode = theEntities.getNamedItem(theName);

			if (theNode != 0 && theNode.getNodeType() == DOM_Node::ENTITY_NODE)
			{
				const DOM_Entity&	theEntity =
					static_cast<const DOM_Entity&>(theNode);

				const DOMString		theNotationName = theEntity.getNotationName();

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
					theURI = theEntity.getSystemId();

					if(isEmpty(theURI) == true)
					{
						theURI = theEntity.getPublicId();
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
