/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <cassert>
#include <cstdio>



#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>



#include "MsgCreator.h"
#include "IndexFileData.hpp"



// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------

static const char* INDEX_FILE_NAME="LocalMsgIndex.hpp";



// ---------------------------------------------------------------------------
//  SAX2Handler: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2Handler::SAX2Handler( ) :
							m_fIndexFormatter(INDEX_FILE_NAME),
							m_numberOfRecords(0),
							m_XML_lang(0),
							m_startCollectingCharacters(false)
{

}

SAX2Handler::~SAX2Handler()
{
	if (m_XML_lang != 0)
	{
		XMLString::release(&m_XML_lang);
	}
}


bool SAX2Handler::translateCharToXMLByteArr ( XMLByte* buffer, int iBufLen, const char* szSource)const
{
	bool bResult = false;

	if ( iBufLen == 0 || szSource == 0 || buffer == 0 )
	{
		return bResult;
	}
	int inpLen = XMLString::stringLen(szSource);

	if ( inpLen > iBufLen )
	{
		buffer[0] = 0;
		return bResult;
	}


	for ( int i = 0; i < inpLen; i++ )
	{
		buffer[i] = szSource[i];
	}
	buffer[inpLen+1] = 0;
	
	bResult = true;

	return bResult;
}



void SAX2Handler::createHeaderForIndexFile ()
{
	printToIndexFile( szApacheLicense );
	
	printToIndexFile ( szStartIndexFile );
}

void SAX2Handler::printBeginOfIndexLine ()
{
	printToIndexFile ( szBeginIndexLine );
}

	
void SAX2Handler::printEndOfIndexLine ()
{
//	printToIndexFile ( szEndIndexLine );
}



void SAX2Handler::createBottomForIndexFile ()
{
	printToIndexFile ( szEndIndexFile );
}


void SAX2Handler::printToIndexFile( const char* sArrayOfStrins[] )
{
	if ( sArrayOfStrins == NULL)
		return;

	XMLByte buffer[1024];
	buffer[0] = 0;

	for (int i = 0; sArrayOfStrins[i] != NULL; i++){

		translateCharToXMLByteArr(buffer, 1024, sArrayOfStrins[i]);


		m_fIndexFormatter.writeChars(buffer,XMLString::stringLen(sArrayOfStrins[i] ),NULL);
		m_fIndexFormatter.flush();

	}
}



void SAX2Handler::startElement(const   XMLCh* const    uri,
									const   XMLCh* const    localname,
									const   XMLCh* const    qname,
                                    const   Attributes&		attributes)
{
	if(!XMLString::compareString(localname,trans_unitXMLCh))
	{
		unsigned int len = attributes.getLength();
		
		++m_numberOfRecords;
		
		for (unsigned int index = 0; index < len; index++)
		{
			const XMLCh* name = attributes.getQName(index) ;
			
			if (name != NULL && !XMLString::compareString(name,idXMLCh)	)
			{
				const XMLCh* val = attributes.getValue(index);
				if ( val != NULL )
				{
					if ( m_numberOfRecords != 1)
						printBeginOfIndexLine();
					
					int valLen = XMLString::stringLen(val);
					
					
					char* szTempString = XMLString::transcode(val);
					
					XMLByte* xmlByteBufferPtr = new XMLByte[valLen+10];
					
					translateCharToXMLByteArr(xmlByteBufferPtr, valLen, szTempString);
					
					m_fIndexFormatter.writeChars(xmlByteBufferPtr,XMLString::stringLen(szTempString),NULL);
					
					m_fIndexFormatter.flush();
					
					XMLString::release( &szTempString );
					delete[] xmlByteBufferPtr;
					
					
					char buff[100];
					XMLByte xmlByteBuffer[100];
					
					sprintf( buff, "		 = %d \n",(m_numberOfRecords - 1));
					
					translateCharToXMLByteArr(xmlByteBuffer, 100, buff);
					
					m_fIndexFormatter.writeChars(xmlByteBuffer,XMLString::stringLen(buff),NULL);
					
					m_fIndexFormatter.flush();
					
					printEndOfIndexLine ();
					
					
				}
			}
			
			
		}
		
	}
}


XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)


// ---------------------------------------------------------------------------
//  
// ---------------------------------------------------------------------------
void SAX2Handler::error(const SAXParseException& e)
{
    cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAX2Handler::fatalError(const SAXParseException& e)
{
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAX2Handler::warning(const SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}


void SAX2Handler::setXML_Lang( const char* localName)
{
	assert(localName != 0);

	if (m_XML_lang != 0)
	{
		XMLString::release(&m_XML_lang);
	}

	m_XML_lang = XMLString::transcode(localName);
}


void SAX2Handler::startDocument()
{
	createHeaderForIndexFile ( );
}



void SAX2Handler::endDocument()
{
	createBottomForIndexFile ( );
}


