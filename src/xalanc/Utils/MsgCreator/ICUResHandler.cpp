/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "ICUResData.hpp"
#include "ICUResHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <cstdio>
#include <cassert>




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
// change tham with \", \{ and \}

void ICUResHandler::characters(	const   XMLCh* const    chars
						, const unsigned int    length)
{

	if ( m_startCollectingCharacters == true )
	{
		XMLCh buffer[512];

		assert( length < 512); 

		int j = 0;

		for ( unsigned int i=0 ; i < length ; i++ , j++)
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

void ICUResHandler::endElement(const XMLCh* const ,
					const XMLCh* const localname,
					const XMLCh* const )
{
	if ( m_startCollectingCharacters == false)
		return;


    if(!XMLString::compareString(localname,s_targetXMLCh))
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
	
	if(!XMLString::compareString(localname,s_transUnitXMLCh))
	{
		// this is an elemente, SAX2Handler class is responsible to handle:
		// creating Index file, commom for all localization styles
		SAX2Handler::startElement(uri, localname, qname, attributes);
	}
	else if(!XMLString::compareString(localname,s_targetXMLCh))
	{
		if ( m_locale != 0 )
		{
			m_startCollectingCharacters = true;	
			
			printBeginOfDataLine();
		
		}
	}
	
}

void ICUResHandler::createHeaderForDataFile ()
{
	printToDataFile( szApacheLicense );

	if ( m_locale != 0)
	{
		m_fStream.write(m_locale,XMLString::stringLen(m_locale));

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



