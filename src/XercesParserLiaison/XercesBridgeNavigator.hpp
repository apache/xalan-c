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
#if !defined(XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680)
#define XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



class DOM_Attr;
class DOM_Node;
class XercesDocumentBridge;
class XalanAttr;
class XalanElement;
class XalanNode;
class XalanText;
class DOM_Text;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesBridgeNavigator
{
public:

	XercesBridgeNavigator(XercesDocumentBridge*		theOwnerDocument);

	XercesBridgeNavigator(const XercesBridgeNavigator&	theSource);

	virtual
	~XercesBridgeNavigator();

	XercesBridgeNavigator&
	operator=(const XercesBridgeNavigator&	theRHS);


	virtual XercesDocumentBridge*
	getOwnerDocument() const;

	virtual XalanNode*
	mapNode(const DOM_Node&		theXercesNode) const;

	virtual XalanAttr*
	mapNode(const DOM_Attr&		theXercesNode) const;

	virtual DOM_Node
	mapNode(const XalanNode*	theXalanNode) const;

	virtual DOM_Attr
	mapNode(const XalanAttr*	theXercesNode) const;

	virtual XalanNode*
	getParentNode(const DOM_Node&	theXercesNode) const;

	virtual XalanNode*
	getPreviousSibling(const DOM_Node&	theXercesNode) const;

	virtual XalanNode*
	getNextSibling(const DOM_Node&	theXercesNode) const;

	virtual XalanNode*
	getFirstChild(const DOM_Node&	theXercesNode) const;

	virtual XalanNode*
	getLastChild(const DOM_Node&	theXercesNode) const;

	virtual XalanNode*
	insertBefore(
			DOM_Node&	theXercesParent,
			XalanNode*	newChild,
			XalanNode*	refChild) const;

	virtual XalanNode*
	replaceChild(
			DOM_Node&	theXercesParent,
			XalanNode*	newChild,
			XalanNode*	oldChild) const;

	virtual XalanNode*
	removeChild(
			DOM_Node&	theXercesParent,
			XalanNode*	oldChild) const;

	virtual XalanNode*
	appendChild(
			DOM_Node&	theXercesParent,
			XalanNode*	newChild) const;

	virtual XalanElement*
	getOwnerElement(const DOM_Attr&		theXercesAttr) const;

	virtual XalanNode*
	cloneNode(
			const XalanNode*	theXalanNode,
			const DOM_Node&		theXercesNode,
			bool				deep) const;

	virtual XalanText*
	splitText(
			DOM_Text&		theXercesText,
			unsigned int	offset) const;

private:

	// Not implemented...
	bool
	operator==(const XercesBridgeNavigator&	theRHS) const;

	// Data members...
	XercesDocumentBridge* const		m_ownerDocument;
};



#endif	// !defined(XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680)
