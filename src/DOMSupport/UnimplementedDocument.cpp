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
#include "UnimplementedDocument.hpp"



#include <cassert>



UnimplementedDocument::UnimplementedDocument() :
	DocumentImpl()
{
}



UnimplementedDocument::UnimplementedDocument(
			const UnimplementedDocument&	theOther) :
	DocumentImpl(theOther)
{
}



UnimplementedDocument::~UnimplementedDocument()
{
}



void
UnimplementedDocument::referenced()
{
}



void
UnimplementedDocument::unreferenced()
{
}



NodeImpl*
UnimplementedDocument::appendChild(NodeImpl*	/* newChild */)
{
	assert(false);

	return 0;
}


void
UnimplementedDocument::changed()
{
	assert(false);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
NodeImpl*
#else
UnimplementedDocument*
#endif
UnimplementedDocument::	cloneNode(bool /* deep */)
{
	assert(false);

	return 0;
}


bool
UnimplementedDocument::isDocumentImpl()
{
	return true;
}



NamedNodeMapImpl*
UnimplementedDocument::getAttributes()
{
	assert(false);

	return 0;
}



NodeListImpl*
UnimplementedDocument::	getChildNodes()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocument::getFirstChild()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocument::	getLastChild()
{
	assert(false);

	return 0;
}


unsigned int
UnimplementedDocument::getLength()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocument::getNextSibling()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedDocument::getNodeName()
{
	assert(false);

	return DOMString();
}


short
UnimplementedDocument::getNodeType()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedDocument::getNodeValue()
{
	assert(false);

	return DOMString();
}



DocumentImpl*
UnimplementedDocument::getOwnerDocument()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedDocument::getParentNode()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedDocument::getPreviousSibling()
{
	assert(false);

	return 0;
}


void*
UnimplementedDocument::getUserData()
{
	assert(false);

	return 0;
}



bool
UnimplementedDocument::hasChildNodes()
{
	assert(false);

	return false;
}



NodeImpl*
UnimplementedDocument::insertBefore(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* refChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedDocument::item(unsigned long	/* index */)
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocument::removeChild(NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedDocument::replaceChild(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}


void
UnimplementedDocument::setNodeValue(const DOMString&	/* value */)
{
	assert(false);
}


void
UnimplementedDocument::setReadOnly(
			bool	/* readOnly */,
			bool	/* deep */)
{
	assert(false);
}


void
UnimplementedDocument::setUserData(void*	/* value */)
{
	assert(false);
}


DOMString
UnimplementedDocument::toString()
{
	assert(false);

	return DOMString();
}



AttrImpl*
UnimplementedDocument::createAttribute(const DOMString&		/* name */)
{
	assert(false);

	return 0;
}



CDATASectionImpl*
UnimplementedDocument::createCDATASection(const DOMString&	/* data */)
{
	assert(false);

	return 0;
}



CommentImpl*
UnimplementedDocument::createComment(const DOMString&	/* data */)
{
	assert(false);

	return 0;
}



DocumentFragmentImpl*
UnimplementedDocument::createDocumentFragment()
{
	assert(false);

	return 0;
}



DocumentTypeImpl*
UnimplementedDocument::createDocumentType(const DOMString&	/* name */)
{
	assert(false);

	return 0;
}



ElementImpl*
UnimplementedDocument::createElement(const DOMString&	/* tagName */)
{
	assert(false);

	return 0;
}



ElementImpl*
UnimplementedDocument::createElement(const XMLCh*	/* tagName */)
{
	assert(false);

	return 0;
}



EntityImpl*
UnimplementedDocument::createEntity(const DOMString&	/* name */)
{
	assert(false);

	return 0;
}



EntityReferenceImpl*
UnimplementedDocument::createEntityReference(const DOMString&	/* name */)
{
	assert(false);

	return 0;
}



NotationImpl*
UnimplementedDocument::createNotation(const DOMString&	/* name */)
{
	assert(false);

	return 0;
}



ProcessingInstructionImpl*
UnimplementedDocument::createProcessingInstruction(
			const DOMString&	/* target */,
			const DOMString&	/* data */)
{
	assert(false);

	return 0;
}



TextImpl*
UnimplementedDocument::createTextNode(const DOMString&	/* data */)
{
	assert(false);

	return 0;
}



DocumentTypeImpl*
UnimplementedDocument::getDoctype()
{
	return 0;
}



ElementImpl*
UnimplementedDocument::getDocumentElement()
{
	assert(false);

	return 0;
}



DeepNodeListImpl*
UnimplementedDocument::getElementsByTagName(const DOMString&	/* tagname */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedDocument::importNode(
			NodeImpl*	/* source */,
			bool		/* deep */)
{
	assert(false);

	return 0;
}




DOM_UnimplementedDocument::DOM_UnimplementedDocument(UnimplementedDocument*		theDocument) :
	DOM_Document(theDocument)
{
}



DOM_UnimplementedDocument::DOM_UnimplementedDocument() :
	DOM_Document()
{
}



DOM_UnimplementedDocument::DOM_UnimplementedDocument(const DOM_UnimplementedDocument&	theDocument) :
	DOM_Document(theDocument)
{
}



DOM_UnimplementedDocument::~DOM_UnimplementedDocument()
{
}
