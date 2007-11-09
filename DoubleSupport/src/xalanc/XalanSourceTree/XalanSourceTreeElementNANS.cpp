/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "XalanSourceTreeElementNANS.hpp"


#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeElementNANS::XalanSourceTreeElementNANS(
            MemoryManagerType&          theManager,
			const XalanDOMString&		theTagName,
			const XalanDOMString&		theLocalName,
			const XalanDOMString&		theNamespaceURI,
			const XalanDOMString&		thePrefix,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			IndexType					theIndex) :
	XalanSourceTreeElementNA(
        theManager,
		theTagName,
		theOwnerDocument,
		theParentNode,
		thePreviousSibling,
		theNextSibling,
		theIndex),
	m_localName(theLocalName),
	m_prefix(thePrefix),
	m_namespaceURI(theNamespaceURI)
{
}



XalanSourceTreeElementNANS::~XalanSourceTreeElementNANS()
{
}



XalanSourceTreeElementNANS::XalanSourceTreeElementNANS(
            MemoryManagerType&                  theManager,
			const XalanSourceTreeElementNANS&	theSource,
			bool								deep) :
	XalanSourceTreeElementNA(theManager, theSource, deep),
	m_localName(theSource.m_localName),
	m_prefix(theSource.m_prefix),
	m_namespaceURI(theSource.m_namespaceURI)
{
}

XalanSourceTreeElementNANS*
XalanSourceTreeElementNANS::create(
            MemoryManagerType&                  theManager,
			const XalanSourceTreeElementNANS&	theSource,
			bool								deep) 
{
    typedef XalanSourceTreeElementNANS ThisType;

    XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

    ThisType* theResult = theGuard.get();

    new (theResult) ThisType(theManager,
                            theSource,
                            deep);

    theGuard.release();

    return theResult;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElementNANS*
#endif
XalanSourceTreeElementNANS::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



const XalanDOMString&
XalanSourceTreeElementNANS::getNamespaceURI() const
{
	return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeElementNANS::getPrefix() const
{
	return m_prefix;
}



const XalanDOMString&
XalanSourceTreeElementNANS::getLocalName() const
{
	return m_localName;
}



XALAN_CPP_NAMESPACE_END
