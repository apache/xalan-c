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
#if !defined(LOGGER_HEADER_GUARD_1357924680)
#define LOGGER_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <iostream>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_USING_STD(ostream)
XALAN_USING_XALAN(XalanDOMString)



class Logger
{
public:

	Logger(ostream & stream);

	typedef enum { eMessage = 0, eWarning = 1, eError = 2} eLogType;

	ostream& message();
	ostream& warning();
	ostream& error();
	
	ostream& log(eLogType logType);

protected:

	char * logText[3];

	ostream& m_stream;
};



#endif LOGGER_HEADER_GUARD_1357924680