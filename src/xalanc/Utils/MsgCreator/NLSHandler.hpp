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

#if !defined(NLSHANDLER_MSGCREATOR_1357924680)
#define NLSHANDLER_MSGCREATOR_1357924680

#include "ICUResHandler.hpp"


XERCES_CPP_NAMESPACE_USE

class NLSHandler : public ICUResHandler 
{
public:
	NLSHandler(const char* fileName, bool bCreateUnicode = false);

	virtual 
	~NLSHandler(){}

	virtual void 
	createHeaderForDataFile ();
	
	virtual void 
	createBottomForDataFile ();

	virtual void 
	printBeginOfDataLine ();
	
	virtual void 
	printEndOfDataLine ();

	void 
	characters(	const   XMLCh* const    chars
						, const unsigned int    length);
	void 
	startDocument();

protected:

	// ASCII version of print
	void 
	printToDataFileasASCII( const char* sArrayOfStrins[] );

private:
	int m_RunningNumber;
	
	bool m_bCreateUnicode;

	// Not implemented...
	NLSHandler&
	operator=(const NLSHandler&);

	NLSHandler(const NLSHandler&);

	bool
	operator==(const NLSHandler&) const;
};




#endif  //NLSHANDLER_MSGCREATOR_1357924680


