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

#if !defined(INMEMHANDLER_MSGCREATOR_1357924680)
#define INMEMHANDLER_MSGCREATOR_1357924680

#include "SAX2Handler.hpp"


XERCES_CPP_NAMESPACE_USE

class InMemHandler : public SAX2Handler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    InMemHandler(const char* fileName);

    virtual 
    ~InMemHandler();

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

	XalanFileOutputStream	m_fStream;
    // -----------------------------------------------------------------------
    //  functions for formatting the output file
    // -----------------------------------------------------------------------
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


private:

	bool m_isTheFirstLineInArray;

	// Not implemented...
	InMemHandler&
	operator=(const InMemHandler&);

	InMemHandler(const InMemHandler&);

	bool
	operator==(const InMemHandler&) const;

};

#endif /// INMEMHANDLER_MSGCREATOR_1357924680
