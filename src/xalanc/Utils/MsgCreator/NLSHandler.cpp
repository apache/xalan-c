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
#include "NLSData.hpp"
#include "NLSHandler.hpp"

#include <cstdio>

// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
NLSHandler::NLSHandler(const char* fileName, bool bCreateUnicode ):
	ICUResHandler(fileName),
	m_RunningNumber(2),
	m_bCreateUnicode(bCreateUnicode)
{
}

void NLSHandler::startDocument()
{
	if (m_bCreateUnicode)
	{
		ICUResHandler::startDocument();
	}
	else
	{
		createHeaderForDataFile ( );
		
		SAX2Handler::startDocument();
	}

}

void NLSHandler::characters(	const   XMLCh* const    chars
						, const unsigned int    length)
{

	if ( m_startCollectingCharacters == true )
	{
		if( m_bCreateUnicode)
		{
			m_fStream.write(chars,length);
		}
		else
		{
			m_fStream.writeAsASCII(chars,length);
		}
	}
}

void NLSHandler::createHeaderForDataFile ()
{
	if (m_bCreateUnicode )
	{
		printToDataFile( szApacheLicense );
		printToDataFile( szStartDataFile );
	}
	else
	{
		printToDataFileasASCII( szApacheLicense );
		printToDataFileasASCII( szStartDataFile );
	}

}

void NLSHandler::printToDataFileasASCII( const char* sArrayOfStrins[] )
{
	if ( sArrayOfStrins == NULL)
		return;

	for (int i = 0; sArrayOfStrins[i] != NULL; i++)
	{
		m_fStream.writeAsASCII(sArrayOfStrins[i],strlen(sArrayOfStrins[i]));
	}
}

void NLSHandler::printBeginOfDataLine ()
{
	char szNumb[20];

	sprintf(szNumb,"%d ^", m_RunningNumber);

	m_RunningNumber++;

	if ( m_bCreateUnicode )
	{
		m_fStream.write(szNumb,strlen(szNumb));
	}
	else
	{
		m_fStream.writeAsASCII(szNumb,strlen(szNumb));
	}
}



void NLSHandler::createBottomForDataFile ()
{


}


void NLSHandler::printEndOfDataLine ()
{
	if ( m_bCreateUnicode )
	{
		m_fStream.write("^\n",2);
	}
	else
	{
		m_fStream.writeAsASCII("^\n",2);
	}
}
