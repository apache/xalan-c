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
#include "UnimplementedNode.hpp"



#include <cassert>



UnimplementedNode::UnimplementedNode(DocumentImpl*	ownerDoc) :
	NodeImpl(ownerDoc,
			 DOMString(),
			 0,
			 false,
			 DOMString())
{
}



UnimplementedNode::UnimplementedNode(
			const UnimplementedNode&	theOther,
			bool						deep) :
	NodeImpl(theOther,
			 deep)
{
}



UnimplementedNode::~UnimplementedNode()
{
}



void
UnimplementedNode::referenced()
{
}



void
UnimplementedNode::unreferenced()
{
}



NodeImpl*
UnimplementedNode::appendChild(NodeImpl*	/* newChild */)
{
	assert(false);

	return 0;
}


void
UnimplementedNode::changed()
{
	assert(false);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
NodeImpl*
#else
UnimplementedNode*
#endif
UnimplementedNode::	cloneNode(bool /* deep */)
{
	assert(false);

	return 0;
}



NamedNodeMapImpl*
UnimplementedNode::getAttributes()
{
	assert(false);

	return 0;
}



NodeListImpl*
UnimplementedNode::	getChildNodes()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedNode::getFirstChild()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedNode::	getLastChild()
{
	assert(false);

	return 0;
}


int
UnimplementedNode::getLength()
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedNode::getNextSibling()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedNode::getNodeName()
{
	assert(false);

	return DOMString();
}


short
UnimplementedNode::getNodeType()
{
	assert(false);

	return 0;
}



DOMString
UnimplementedNode::getNodeValue()
{
	assert(false);

	return DOMString();
}



DocumentImpl*
UnimplementedNode::getOwnerDocument()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedNode::getParentNode()
{
	assert(false);

	return 0;
}


NodeImpl* 
UnimplementedNode::getPreviousSibling()
{
	assert(false);

	return 0;
}


void*
UnimplementedNode::getUserData()
{
	assert(false);

	return 0;
}



bool
UnimplementedNode::hasChildNodes()
{
	assert(false);

	return false;
}



NodeImpl*
UnimplementedNode::insertBefore(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* refChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedNode::item(int /* index */)
{
	assert(false);

	return 0;
}


NodeImpl*
UnimplementedNode::removeChild(NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}



NodeImpl*
UnimplementedNode::replaceChild(
			NodeImpl*	/* newChild */,
			NodeImpl*	/* oldChild */)
{
	assert(false);

	return 0;
}


void
UnimplementedNode::setNodeValue(const DOMString&	/* value */)
{
	assert(false);
}


void
UnimplementedNode::setReadOnly(
			bool	/* readOnly */,
			bool	/* deep */)
{
	assert(false);
}


void
UnimplementedNode::setUserData(void*	/* value */)
{
	assert(false);
}


DOMString
UnimplementedNode::toString()
{
	assert(false);

	return DOMString();
}



DOM_UnimplementedNode::DOM_UnimplementedNode(UnimplementedNode*	theNode) :
	DOM_Node(theNode)
{
}



DOM_UnimplementedNode::DOM_UnimplementedNode() :
	DOM_Node()
{
}



DOM_UnimplementedNode::DOM_UnimplementedNode(const DOM_UnimplementedNode&	theNode) :
	DOM_Node(theNode)
{
}



DOM_UnimplementedNode::~DOM_UnimplementedNode()
{
}
