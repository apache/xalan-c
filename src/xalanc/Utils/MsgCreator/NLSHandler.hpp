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

#if !defined(NLSHANDLER_MSGCREATOR_1357924680)
#define NLSHANDLER_MSGCREATOR_1357924680

#include "xalanc/Include/PlatformDefinitions.hpp"

#include "ICUResHandler.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XERCES_CPP_NAMESPACE_USE



class NLSHandler : public ICUResHandler 
{
public:

    NLSHandler(
            const char*     fileName,
            const char*     indexFileName,
            bool            createUnicode = false);

    virtual 
    ~NLSHandler(){}

    virtual void 
    createHeaderForDataFile();

    virtual void 
    createBottomForDataFile();

    virtual void 
    printBeginOfDataLine();

    virtual void 
    printEndOfDataLine();

    void 
    characters(
            const XMLCh* const  chars,
            const XalanSize_t   length);

    void 
    startDocument();

protected:

    // ASCII version of print
    void 
    printToDataFileAsASCII(const char*  sArrayOfStrings[]);

private:

    int         m_runningNumber;

    const bool  m_createUnicode;


    // Not implemented...
    NLSHandler&
    operator=(const NLSHandler&);

    NLSHandler(const NLSHandler&);

    bool
    operator==(const NLSHandler&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  //NLSHANDLER_MSGCREATOR_1357924680
