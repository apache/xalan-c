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
#include "InMemHandler.hpp"
#include <cstdio>
#include <cassert>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include "MsgCreator.hpp"
#include "InMemData.hpp"


// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
InMemHandler::InMemHandler(const char* fileName):
	SAX2Handler(),
	m_fStream(fileName),
	m_isTheFirstLineInArray(true)
{
}

InMemHandler::~InMemHandler()
{

}


void InMemHandler::printToDataFile( const char* sArrayOfStrins[] )
{
	if ( sArrayOfStrins == 0)
		return;


	for (int i = 0; sArrayOfStrins[i] != 0; i++)
	{

		m_fStream.writeAsASCII(sArrayOfStrins[i], strlen(sArrayOfStrins[i]));

	}
}


void InMemHandler::endDocument()
{
	createBottomForDataFile ( );

	SAX2Handler::endDocument( );
}




void InMemHandler::characters(	const   XMLCh* const    chars
						, const unsigned int    length)
{

	if ( m_startCollectingCharacters == true )
	{		

		char buffer[20];

		for( unsigned int i = 0; i < length ; i++)
		{
			buffer[0] = '\0';

			sprintf(buffer," %#4x, ",chars[i]);

			m_fStream.writeAsASCII(buffer, strlen(buffer));

		}
		
		

	}
}

void InMemHandler::startDocument()
{
	createHeaderForDataFile ( );

	SAX2Handler::startDocument();

}

void InMemHandler::endElement(const XMLCh* const ,
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

void InMemHandler::startElement(const   XMLCh* const    uri,
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

		
		m_startCollectingCharacters = true;	
		
		printBeginOfDataLine();

	}
	
}

void InMemHandler::createHeaderForDataFile ()
{
	printToDataFile( szApacheLicense );
	printToDataFile( szStartDataFile );
}



void InMemHandler::printBeginOfDataLine ()
{

	char buff[50];

	buff[0] = 0;

	sprintf(buff,"%s%d[] = {",s_szVariableName, m_numberOfRecords);

	m_fStream.writeAsASCII(buff,strlen(buff));

}


void InMemHandler::printEndOfDataLine ()
{
	printToDataFile( szEndOfLineInDataFile  );
}


void InMemHandler::createBottomForDataFile ()
{

	printToDataFile( szArraySizeVar );

	char buff[100];
	
	sprintf( buff, " %d ;",m_numberOfRecords);
		
	m_fStream.writeAsASCII(buff,strlen(buff));

	m_fStream.writeAsASCII("\nstatic const XalanDOMChar* msgArray[]={ ",40);

	buff[0] = 0;

	
	for(int i = 0; i < m_numberOfRecords; i++)
	{
		sprintf(buff,"%s%d ",s_szSimpleVariableName,i+1);

		m_fStream.writeAsASCII(buff, strlen(buff));

		if(i != ( m_numberOfRecords-1))
		{
			m_fStream.writeAsASCII(",",1);
		}
		else
		{
			m_fStream.writeAsASCII("};",2);
		}
		buff[0] = 0;

	}
	printToDataFile( szEndDataFile );	

}
