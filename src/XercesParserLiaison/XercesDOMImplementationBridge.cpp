/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include "XercesDOMImplementationBridge.hpp"



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_DOMImplementation.hpp>
#else
#include <xercesc/dom/DOM_DOMImplementation.hpp>
#endif



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XercesDOMException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMImplementationBridge::XercesDOMImplementationBridge(DOM_DOMImplementationType&	theXercesDOMImplementation) :
	XalanDOMImplementation(),
	m_xercesNode(theXercesDOMImplementation)
{
}



XercesDOMImplementationBridge::~XercesDOMImplementationBridge()
{
}



XercesDOMImplementationBridge::XercesDOMImplementationBridge(const XercesDOMImplementationBridge&	theSource) :
	XalanDOMImplementation(theSource),
	m_xercesNode(theSource.m_xercesNode)
{
}



bool
XercesDOMImplementationBridge::hasFeature(
			const XalanDOMString&	feature,
			const XalanDOMString&	version)
{
	return m_xercesNode.hasFeature(c_wstr(feature), c_wstr(version));
}



XalanDocumentType*
XercesDOMImplementationBridge::createDocumentType(
			const XalanDOMString&	qualifiedName,
			const XalanDOMString&	publicId,
			const XalanDOMString&	systemId)
{
	// $$$ ToDo: Fix this!!!
	throw XercesDOMException(XercesDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanDocument*
XercesDOMImplementationBridge::createDocument(
			const XalanDOMString&		namespaceURI,
			const XalanDOMString&		qualifiedName,
			const XalanDocumentType&	doctype)
{
	// $$$ ToDo: Fix this!!!
	throw XercesDOMException(XercesDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XALAN_CPP_NAMESPACE_END
