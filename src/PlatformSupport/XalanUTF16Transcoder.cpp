/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#include "XalanUTF16Transcoder.hpp"



#include <cassert>



XalanUTF16Transcoder::XalanUTF16Transcoder() :
	XalanOutputTranscoder()
{
}



XalanUTF16Transcoder::~XalanUTF16Transcoder()
{
}



XalanUTF16Transcoder::eCode
XalanUTF16Transcoder::transcode(
			const XalanDOMChar*		theSourceData,
			unsigned int			theSourceCount,
			XalanXMLByte*			theTarget,
			unsigned int			theTargetSize,
			unsigned int&			theSourceCharsTranscoded,
			unsigned int&			theTargetBytesUsed)
{
	unsigned int	theSourceEaten = 0;
	unsigned int	theTargetPosition = 0;

	while(theSourceEaten < theSourceCount)
	{
		// Swap bytes to big endian...
		if (theTargetPosition + 1 >= theTargetSize)
		{
			break;
		}
		else
		{
			const XalanXMLByte	theHighByte = XalanXMLByte((theSourceData[theSourceEaten] & 0xFF00) >> 8);
			const XalanXMLByte	theLowByte = XalanXMLByte(theSourceData[theSourceEaten] & 0x00FF);

#if defined(XALAN_LITTLE_ENDIAN)
			theTarget[theTargetPosition++] = theLowByte;
			theTarget[theTargetPosition++] = theHighByte;
#elif defined(XALAN_BIG_ENDIAN)
			theTarget[theTargetPosition++] = theHighByte;
			theTarget[theTargetPosition++] = theLowByte;
#else
#error The platform must define the byte order!
#endif

			++theSourceEaten;
		}
	}

	theSourceCharsTranscoded = theSourceEaten;

	theTargetBytesUsed = theTargetPosition;

	return XalanTranscodingServices::OK;
}



XalanUTF16Transcoder::eCode
XalanUTF16Transcoder::transcode(
			const XalanXMLByte*		theSourceData,
			unsigned int			theSourceCount,
			XalanDOMChar*			theTarget,
			unsigned int			theTargetSize,
			unsigned int&			theSourceCharsTranscoded,
			unsigned int&			theTargetBytesUsed,
			unsigned char*			theCharSizes)
{
	unsigned int	theSourceEaten = 0;
	unsigned int	theTargetPosition = 0;

	while(theSourceEaten < theSourceCount)
	{
		// Swap bytes to big endian...
		if (theTargetPosition + 1 >= theTargetSize)
		{
			break;
		}
		else
		{
#if defined(XALAN_LITTLE_ENDIAN)
			const XalanXMLByte	theLowByte = theSourceData[theSourceCount++];
			const XalanXMLByte	theHighByte = theSourceData[theSourceCount++];
#elif defined(XALAN_BIG_ENDIAN)
			const XalanXMLByte	theHighByte = theSourceData[theSourceCount++];
			const XalanXMLByte	theLowByte = theSourceData[theSourceCount++];
#else
#error The platform must define the byte order!
#endif

			theTarget[theTargetPosition++] = XalanDOMChar((theHighByte << 8) | theLowByte);

			*theCharSizes++ = 2;
		}
	}

	theSourceCharsTranscoded = theSourceEaten;

	theTargetBytesUsed = theTargetPosition;

	return XalanTranscodingServices::OK;
}
