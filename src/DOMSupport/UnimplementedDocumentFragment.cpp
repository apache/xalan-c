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
#include "UnimplementedDocumentFragment.hpp"



#include <cassert>



UnimplementedDocumentFragment::UnimplementedDocumentFragment(DocumentImpl*		ownerDoc) :
	DocumentFragmentImpl(ownerDoc)
{
}



UnimplementedDocumentFragment::UnimplementedDocumentFragment(
			const UnimplementedDocumentFragment&	theOther) :
	DocumentFragmentImpl(theOther)
{
}



UnimplementedDocumentFragment::~UnimplementedDocumentFragment()
{
}



void
UnimplementedDocumentFragment::referenced()
{
}



void
UnimplementedDocumentFragment::unreferenced()
{
}



NodeImpl*
UnimplementedDocumentFragment::appendChild(NodeImpl*	/* newChild */)
{
	assert(false);

	return 0;
}


void
UnimplementedDocumentFragment::changed()
{
	assert(false);
}



NodeImpl*
UnimplementedDocumentFragment::	cloneNode(bool /* deep */)
{
	assert(false);

	return 0;
}


bool
UnimplementedDocumentFragment::isDocumentFragmentImpl()
{
	return true;
}



NamedNodeMapImpl*
UnimplementedDocumentFragment::getAttributes()
{
	assert(false);

	return 0;
}



NodeListImpl*
UnimplementedDocumentFragment::	getChildNodes()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocumentFragment::getFirstChild()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocumentFragment::	getLastChild()
{
	assert(false);

	return 0;
}


unsigned int
UnimplementedDocumentFragment::getLength()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocumentFragment::getNextSibling()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedDocumentFragment::getNodeName()
{
	assert(false);

	return DOMString();
}


short
UnimplementedDocumentFragment::getNodeType()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedDocumentFragment::getNodeValue()
{
	assert(false);

	return DOMString();
}



DocumentImpl*
UnimplementedDocumentFragment::getOwnerDocument()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedDocumentFragment::getParentNode()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedDocumentFragment::getPreviousSibling()
{
	assert(false);

	return 0;
}


bool
UnimplementedDocumentFragment::hasChildNodes()
{
	assert(false);

	return false;
}



NodeImpl*
UnimplementedDocumentFragment::insertBefore(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* refChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedDocumentFragment::item(unsigned long	/* index */)
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedDocumentFragment::removeChild(NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedDocumentFragment::replaceChild(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}



void
UnimplementedDocumentFragment::setNodeValue(const DOMString&	/* value */)
{
}



void
UnimplementedDocumentFragment::setReadOnly(
			bool	/* readOnly */,
			bool	/* deep */)
{
	assert(false);
}



DOMString
UnimplementedDocumentFragment::toString()
{
	assert(false);

	return DOMString();
}



DOM_UnimplementedDocumentFragment::DOM_UnimplementedDocumentFragment(UnimplementedDocumentFragment*		theDocument) :
	DOM_DocumentFragment(theDocument)
{
}



DOM_UnimplementedDocumentFragment::DOM_UnimplementedDocumentFragment() :
	DOM_DocumentFragment()
{
}



DOM_UnimplementedDocumentFragment::DOM_UnimplementedDocumentFragment(const DOM_UnimplementedDocumentFragment&	theDocument) :
	DOM_DocumentFragment(theDocument)
{
}



DOM_UnimplementedDocumentFragment::~DOM_UnimplementedDocumentFragment()
{
}
