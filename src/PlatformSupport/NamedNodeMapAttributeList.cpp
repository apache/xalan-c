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
#include "NamedNodeMapAttributeList.hpp"



#include <dom/DOM_Attr.hpp>
#include <util/Janitor.hpp>


#include "DOMStringHelper.hpp"



NamedNodeMapAttributeList::NamedNodeMapAttributeList(DOM_NamedNodeMap&	theMap) :
	m_nodeMap(theMap),
	m_lastIndex(theMap.getLength() - 1),
	m_cachedData()
{
}



NamedNodeMapAttributeList::~NamedNodeMapAttributeList()
{
}



unsigned int
NamedNodeMapAttributeList::getLength() const
{
	return m_nodeMap.getLength();
}



const XMLCh*
NamedNodeMapAttributeList::getName(const unsigned int index) const
{
	// We have to return a pointer to a something, but the Xerces
	// DOM classes return strings by value, so we have to get
	// the value from the node and store the DOMString somewhere
	// safe, so we have a vector of DOMStrings to hold everything.
	const DOM_Node		theNode = m_nodeMap.item(m_lastIndex - index);

	const DOM_Attr&		theAttr = 
		static_cast<const DOM_Attr&>(theNode);

	m_cachedData.push_back(theAttr.getName());

	return c_wstr(m_cachedData.back());
}



// This is out here so we don't have to worry about multithreading issues.
static const DOMString	theType(XALAN_STATIC_UCODE_STRING("CDATA"));



const XMLCh*
NamedNodeMapAttributeList::getType(const unsigned int /* index */) const
{
	return c_wstr(theType);
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const unsigned int index) const
{
	// We have to return a pointer to a something, but the Xerces
	// DOM classes return strings by value, so we have to get
	// the value from the node and store the DOMString somewhere
	// safe, so we have a vector of DOMStrings to hold everything.
	const DOM_Node		theNode = m_nodeMap.item(m_lastIndex - index);

	const DOM_Attr&		theAttr = 
		static_cast<const DOM_Attr&>(theNode);

	m_cachedData.push_back(theAttr.getValue());

	return c_wstr(m_cachedData.back());
}



const XMLCh*
NamedNodeMapAttributeList::getType(const XMLCh* const /* name */) const
{
	return c_wstr(theType);
}



const XMLCh*
NamedNodeMapAttributeList::getValue(const XMLCh* const name) const
{
	// We have to return a pointer to a something, but the Xerces
	// DOM classes return strings by value, so we have to get
	// the value from the node and store the DOMString somewhere
	// safe, so we have a vector of DOMStrings to hold everything.
	const DOM_Node		theNode = m_nodeMap.getNamedItem(name);

	const DOM_Attr&		theAttr = 
		static_cast<const DOM_Attr&>(theNode);

	m_cachedData.push_back(theAttr.getValue());

	return c_wstr(m_cachedData.back());
}



const XMLCh* 
NamedNodeMapAttributeList::getValue(const char* const name) const
{
	XMLCh* const	theTranscodedName =
		XMLString::transcode(name);

	ArrayJanitor<XMLCh>	theJanitor(theTranscodedName);

	return getValue(theTranscodedName);
}
