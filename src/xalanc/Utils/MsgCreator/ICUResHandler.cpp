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
#include "ICUResHandler.hpp"
#include <cstdio>
#include <cassert>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include "ICUResData.hpp"


// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
ICUResHandler::ICUResHandler(const char* fileName):
	SAX2Handler(),
	m_fStream(fileName)
{
}

ICUResHandler::~ICUResHandler()
{

}




void ICUResHandler::endDocument()
{
	createBottomForDataFile ( );

	SAX2Handler::endDocument( );
}


// ICU genrb doesnt' like " and {} chars
// change tham with \", \{ and /}

void ICUResHandler::characters(	const   XMLCh* const    chars
						, const unsigned int    length)
{

	if ( m_startCollectingCharacters == true )
	{
		XMLCh buffer[512];

		assert( length < 512); 

		int j = 0;

		for ( int i=0 ; i < length ; i++ , j++)
		{
			if( chars[i] == chDoubleQuote || chars[i] == chOpenCurly || chars[i] == chCloseCurly)
			{
				buffer[j] = chBackSlash;

				j++;

				buffer[j] = chars[i]; 
			}
			else
			{
				buffer[j] = chars[i];
			}
		}

		buffer[j] = chNull;
		m_fStream.write(buffer,j);	
	}
}

void ICUResHandler::startDocument()
{
	m_fStream.writeUTFprefix();

	createHeaderForDataFile ( );

	SAX2Handler::startDocument();

}

void ICUResHandler::endElement(const XMLCh* const uri,
					const XMLCh* const localname,
					const XMLCh* const qname)
{
	if ( m_startCollectingCharacters == false)
		return;


    if(!XMLString::compareString(localname,targetXMLCh))
	{
		m_startCollectingCharacters = false;

		printEndOfDataLine();
	}
}

void ICUResHandler::startElement(const   XMLCh* const    uri,
								const   XMLCh* const    localname,
								const   XMLCh* const    qname,
								const   Attributes&		attributes)
{
	
	if(!XMLString::compareString(localname,trans_unitXMLCh))
	{
		// this is an elemente, SAX2Handler class is responsible to handle:
		// creating Index file, commom for all localization styles
		SAX2Handler::startElement(uri, localname, qname, attributes);
	}
	else if(!XMLString::compareString(localname,targetXMLCh))
	{
		if ( m_XML_lang != 0 )
		{
			m_startCollectingCharacters = true;	
			
			printBeginOfDataLine();
		
		}
	}
	
}

void ICUResHandler::createHeaderForDataFile ()
{
	printToDataFile( szApacheLicense );

	if ( m_XML_lang != 0)
	{
		m_fStream.write(m_XML_lang,XMLString::stringLen(m_XML_lang));

	}

	printToDataFile( szStartDataFile );


}



void ICUResHandler::printBeginOfDataLine ()
{
//	if ( m_isTheFirstLineInArray ){
//		m_isTheFirstLineInArray = false;
//	}
//	else
	{
		printToDataFile( szBeginOfLineInDataFile );
	}
}



void ICUResHandler::createBottomForDataFile ()
{

	printToDataFile( szEndDataFile );

}


void ICUResHandler::printEndOfDataLine ()
{
	printToDataFile( szEndOfLineInDataFile  );
}

void ICUResHandler::printToDataFile( const char* sArrayOfStrins[] )
{
	if ( sArrayOfStrins == 0)
		return;

	for (int i = 0; sArrayOfStrins[i] != 0; i++)
	{
		m_fStream.write(sArrayOfStrins[i],strlen(sArrayOfStrins[i]));
	}
}



