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


