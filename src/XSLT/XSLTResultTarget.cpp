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
 *
 * $ Id: $
 *
 */

// Base include file.  Must be first.
#include "XSLTResultTarget.hpp"



#include <cassert>



XSLTResultTarget::XSLTResultTarget() :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
}



XSLTResultTarget::XSLTResultTarget(const XalanDOMString&	fileName) :
	m_fileName(fileName),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
}



XSLTResultTarget::XSLTResultTarget(const char*	fileName) :
	m_fileName(fileName),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
}



#if defined(XALAN_NO_NAMESPACES)
XSLTResultTarget::XSLTResultTarget(ostream*		theStream) :
#else
XSLTResultTarget::XSLTResultTarget(std::ostream*	theStream) :
#endif
	m_fileName(),
	m_byteStream(theStream),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
	assert(theStream != 0);
}



#if defined(XALAN_NO_NAMESPACES)
XSLTResultTarget::XSLTResultTarget(ostream&		theStream) :
#else
XSLTResultTarget::XSLTResultTarget(std::ostream&	theStream) :
#endif
	m_fileName(),
	m_byteStream(&theStream),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
}



XSLTResultTarget::XSLTResultTarget(Writer*	characterStream) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(characterStream),
	m_document(0),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
	assert(characterStream != 0);
}



XSLTResultTarget::XSLTResultTarget(XalanDocument*	document) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(document),
	m_documentFragment(0),
	m_element(0),
	m_formatterListener(0)
{
	assert(document != 0);
}



XSLTResultTarget::XSLTResultTarget(XalanDocumentFragment*	documentFragment) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(documentFragment),
	m_element(0),
	m_formatterListener(0)
{
	assert(documentFragment != 0);
}



XSLTResultTarget::XSLTResultTarget(XalanElement*	element) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_document(0),
	m_documentFragment(0),
	m_element(element),
	m_formatterListener(0)
{
	assert(element != 0);
}
