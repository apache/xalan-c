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

#if !defined(ICURESHANDLER_MSGCREATOR_1357924680)
#define ICURESHANDLER_MSGCREATOR_1357924680

#include "SAX2Handler.hpp"


XERCES_CPP_NAMESPACE_USE

class ICUResHandler : public SAX2Handler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    ICUResHandler(const char* fileName);

    virtual 
    ~ICUResHandler();
	// -----------------------------------------------------------------------
    //  Implementations of the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
	virtual void 
	endDocument();
	virtual void 
	endElement(const XMLCh* const ,
								 const XMLCh* const localname,
								 const XMLCh* const )	;
	virtual void 
	startDocument();

	virtual void 
	startElement(const   XMLCh* const    uri,
								const   XMLCh* const    localname,
								const   XMLCh* const    qname,
                                const   Attributes&		attributes);

	virtual void 
	characters(	const   XMLCh* const    chars
						, const unsigned int    length);

protected:
	virtual void 
	createHeaderForDataFile ();
	
	virtual void 
	createBottomForDataFile ();

	virtual void 
	printBeginOfDataLine ();
	
	virtual void 
	printEndOfDataLine ();

	void 
	printToDataFile( const char* sArrayOfStrins[] );


	XalanFileOutputStream	m_fStream;

private:
	// Not implemented...
	ICUResHandler&
	operator=(const ICUResHandler&);

	ICUResHandler(const ICUResHandler&);

	bool
	operator==(const ICUResHandler&) const;
};

#endif // ICURESHANDLER_MSGCREATOR_1357924680

