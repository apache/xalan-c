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

#if !defined(SAX2HANDLER_MSGCREATOR_1357924680)
#define SAX2HANDLER_MSGCREATOR_1357924680

#include "MsgFileOutputStream.hpp"

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLUniDefs.hpp>


XERCES_CPP_NAMESPACE_USE



static const XMLCh s_transUnitXMLCh[] = 
{ 
	chLatin_t, 
	chLatin_r,
	chLatin_a,	
	chLatin_n, 
	chLatin_s, 
	chDash, 
	chLatin_u, 
	chLatin_n,
	chLatin_i, 
	chLatin_t,

	chNull
};


static const XMLCh s_sourceXMLCh[] = 
{ 
	chLatin_s, 
	chLatin_o,
	chLatin_u,	
	chLatin_r, 
	chLatin_c, 
	chLatin_e, 

	chNull
};


static const XMLCh s_targetXMLCh[] = 
{ 
	chLatin_t, 
	chLatin_a,
	chLatin_r,	
	chLatin_g, 
	chLatin_e, 
	chLatin_t, 

	chNull
};


static const XMLCh s_xmlLangXMLCh[] = 
{ 
	chLatin_x, 
	chLatin_m,
	chLatin_l,	
	chColon, 
	chLatin_l, 
	chLatin_a, 
	chLatin_n, 
	chLatin_g,
	chNull
};



static const XMLCh s_idXMLCh[] =
{
	chLatin_i,
	chLatin_d,
	chNull
};

static const XMLCh s_textXMLCh[] = 
{ 
	chLatin_T, 
	chLatin_e, 
	chLatin_x, 
	chLatin_t, 
	chNull
};


// Common class for the all system: creates index file ( common for all localization systems)
// For creation data file responsible subclasses


class SAX2Handler : public DefaultHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    SAX2Handler();
    virtual ~SAX2Handler();

public:
    virtual void 
    startElement(const   XMLCh* const    ,
									const   XMLCh* const    localname,
									const   XMLCh* const    ,
                                    const   Attributes&		attributes);

	virtual void 
	startDocument();

	virtual void 
	endDocument();

   // -----------------------------------------------------------------------
    //  Implementations of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
	virtual void 
	error(const SAXParseException& e); 
	
	virtual void 
	fatalError(const SAXParseException& e);
	
	virtual void 
	warning(const SAXParseException& e);

	void 
	setLocale( const char* localeName);

	const XMLCh*
	getLocale () const
	{
		return m_locale;
	}

protected:
	bool translateCharToXMLByteArr ( XMLByte* buffer, int iBufLen, const char* szSource)const;

	virtual void 
	createHeaderForDataFile ()=0;
	
	virtual void 
	createBottomForDataFile ()=0;

	virtual void 
	printBeginOfDataLine ()=0;
	
	virtual void 
	printEndOfDataLine ()=0;

	virtual void 
	printToDataFile( const char* sArrayOfStrins[] ) = 0;
	
	void 
	printToIndexFile( const char* sArrayOfStrins[] );

	void 
	printNumbOfRecords ();



private:

	void createHeaderForIndexFile ();
	void createBottomForIndexFile ();

	void printBeginOfIndexLine ();
	void printEndOfIndexLine ();

protected :
	int	m_numberOfRecords;

	XMLCh* m_locale;

	bool m_startCollectingCharacters;

    // -----------------------------------------------------------------------
    //  SAX2 Handler is responsible for creating index file, so keep
    //  index file FormatTarget as private data member
    // -----------------------------------------------------------------------

private:
	XalanFileOutputStream    m_fIndexOutputStream;

	// Not implemented...
	SAX2Handler&
	operator=(const SAX2Handler&);

	SAX2Handler(const SAX2Handler&);

	bool
	operator==(const SAX2Handler&) const;

};


#endif //SAX2HANDLER_MSGCREATOR_1357924680


