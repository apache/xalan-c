/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "XalanToXercesTranscoderWrapper.hpp"



#include <cassert>



#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLException.hpp>



typedef XERCES_CPP_NAMESPACE_QUALIFIER XMLException		XMLExceptionType;



XALAN_CPP_NAMESPACE_BEGIN



XalanToXercesTranscoderWrapper::XalanToXercesTranscoderWrapper(XMLTranscoderType&	theTranscoder) :
	XalanOutputTranscoder(),
	m_transcoder(&theTranscoder)
{
}



XalanToXercesTranscoderWrapper::~XalanToXercesTranscoderWrapper()
{
	delete m_transcoder;
}



XalanToXercesTranscoderWrapper::eCode
XalanToXercesTranscoderWrapper::transcode(
			const XalanDOMChar*		theSourceData,
			size_type				theSourceCount,
			XalanXMLByte*			theTarget,
			size_type				theTargetSize,
			size_type&				theSourceCharsTranscoded,
			size_type&				theTargetBytesUsed)
{
	eCode	theCode = XalanTranscodingServices::OK;

	try
	{
		theTargetBytesUsed = m_transcoder->transcodeTo(
			theSourceData,
			theSourceCount,
			theTarget,
			theTargetSize,
			theSourceCharsTranscoded,
			// $$$ ToDo: Eventually, we're going to want to
			// replace this with UnRep_Throw, and let the
			// caller try to recover.
//			XMLTranscoderType::UnRep_Throw);
			XMLTranscoderType::UnRep_RepChar);
	}
	catch(const XMLExceptionType&)
	{
		theSourceCharsTranscoded = 0;
		theTargetBytesUsed = 0;
		theCode = XalanTranscodingServices::InternalFailure;
	}

	return theCode;
}



XalanToXercesTranscoderWrapper::eCode
XalanToXercesTranscoderWrapper::transcode(
			const XalanXMLByte*		theSourceData,
			size_type				theSourceCount,
			XalanDOMChar*			theTarget,
			size_type				theTargetSize,
			size_type&				theSourceCharsTranscoded,
			size_type&				theTargetBytesUsed,
			unsigned char*			theCharSizes)
{
	eCode	theCode = XalanTranscodingServices::OK;

	try
	{
		theTargetBytesUsed =
				m_transcoder->transcodeFrom(
			theSourceData,
			theSourceCount,
			theTarget,
			theTargetSize,
			theSourceCharsTranscoded,
			theCharSizes);
	}
	catch(const XMLExceptionType&)
	{
		theSourceCharsTranscoded = 0;
		theTargetBytesUsed = 0;
		theCode = XalanTranscodingServices::InternalFailure;
	}

	return theCode;
}



bool
XalanToXercesTranscoderWrapper::canTranscodeTo(UnicodeCharType	theChar) const
{
	return m_transcoder->canTranscodeTo(theChar);
}



XALAN_CPP_NAMESPACE_END
