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
#include <cassert>
#include <cstdio>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include "ICUResHandler.hpp"
#include "IndexFileData.hpp"
#include "MsgCreator.hpp"



static const char* INDEX_FILE_NAME="LocalMsgIndex.hpp";



// ---------------------------------------------------------------------------
//  SAX2Handler: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2Handler::SAX2Handler( ) :
							m_numberOfRecords(0),
							m_locale(0),
							m_startCollectingCharacters(false),
							m_fIndexOutputStream(INDEX_FILE_NAME)
{

}

SAX2Handler::~SAX2Handler()
{
	if (m_locale != 0)
	{
		XMLString::release(&m_locale);
	}
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
}



void SAX2Handler::createBottomForIndexFile ()
{
	printToIndexFile ( szEndIndexFile );
}


void SAX2Handler::printToIndexFile( const char* sArrayOfStrins[] )
{
	if ( sArrayOfStrins == NULL)
		return;

	for (int i = 0; sArrayOfStrins[i] != NULL; i++)
	{
		m_fIndexOutputStream.writeAsASCII(sArrayOfStrins[i],XMLString::stringLen(sArrayOfStrins[i]));
	}
}



void SAX2Handler::startElement(const   XMLCh* const    ,
									const   XMLCh* const    localname,
									const   XMLCh* const    ,
                                    const   Attributes&		attributes)
{
	if(!XMLString::compareString(localname,s_transUnitXMLCh))
	{
		unsigned int len = attributes.getLength();
		
		++m_numberOfRecords;
		
		for (unsigned int index = 0; index < len; index++)
		{
			const XMLCh* name = attributes.getQName(index) ;
			
			if (name != NULL && !XMLString::compareString(name,s_idXMLCh)	)
			{
				const XMLCh* val = attributes.getValue(index);
				if ( val != NULL )
				{
					if ( m_numberOfRecords != 1)
						printBeginOfIndexLine();
										
					m_fIndexOutputStream.writeAsASCII(val,XMLString::stringLen(val));
					
					char buff[100];
					
					sprintf( buff, "		 = %d \n",(m_numberOfRecords - 1));
					
					m_fIndexOutputStream.writeAsASCII(buff,XMLString::stringLen(buff));
					
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


void SAX2Handler::setLocale( const char* localName)
{
	assert(localName != 0);

	if (m_locale != 0)
	{
		XMLString::release(&m_locale);
	}

	m_locale = XMLString::transcode(localName);
}


void SAX2Handler::startDocument()
{
	createHeaderForIndexFile ( );
}



void SAX2Handler::endDocument()
{
	createBottomForIndexFile ( );
}


