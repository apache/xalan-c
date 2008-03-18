/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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

#if !defined(INMEMHANDLER_1357924680)
#define INMEMHANDLER_1357924680



#include "xalanc/Include/PlatformDefinitions.hpp"

#include "SAX2Handler.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class InMemHandler : public SAX2Handler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    InMemHandler(
            const char*     fileName,
            const char*     indexFileName);

    virtual 
    ~InMemHandler();

    // -----------------------------------------------------------------------
    //  Implementations of the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    virtual void
    endDocument();

    virtual void
    endElement(
            const XMLCh* const,
            const XMLCh* const  localname,
            const XMLCh* const);

    virtual void
    startDocument();

    virtual void 
    startElement(
            const XMLCh* const  uri,
            const XMLCh* const  localname,
            const XMLCh* const  qname,
            const Attributes&   attributes);

    virtual void 
    characters(
            const XMLCh* const  chars,
            const XalanSize_t   length);

protected:

    MsgFileOutputStream     m_stream;

    // -----------------------------------------------------------------------
    //  functions for formatting the output file
    // -----------------------------------------------------------------------
    virtual void
    createHeaderForDataFile();
    
    virtual void
    createBottomForDataFile();

    virtual void
    printBeginOfDataLine();
    
    virtual void
    printEndOfDataLine();

    void
    printToDataFile(const char*     sArrayOfStrings[]);

private:

    bool    m_isTheFirstLineInArray;

    // Not implemented...
    InMemHandler&
    operator=(const InMemHandler&);

    InMemHandler(const InMemHandler&);

    bool
    operator==(const InMemHandler&) const;
};



XALAN_CPP_NAMESPACE_END



#endif /// INMEMHANDLER_1357924680
