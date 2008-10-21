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
#if !defined(XALAN_SPACENODETESTER_HEADER_GUARD_1357924680)
#define XALAN_SPACENODETESTER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include "xalanc/XPath/XPath.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class PrefixResolver;
class StylesheetConstructionContext;



class XalanSpaceNodeTester : public XPath::NodeTester
{
public:

    typedef XPath::NodeTester   ParentType;

    enum eType
    {
        eStrip,
        ePreserve
    };

    XalanSpaceNodeTester();

    XalanSpaceNodeTester(const XalanSpaceNodeTester&    theSource);

    XalanSpaceNodeTester(
            eType                           theType,
            StylesheetConstructionContext&  theContext,
            const XalanDOMString&           theNameTest,
            const PrefixResolver&           thePrefixResolver,
            const Locator*                  theLocator);

    ~XalanSpaceNodeTester();

    XPath::eMatchScore
    getMatchScore() const
    {
        return m_matchScore;
    }

    eType
    getType() const
    {
        return m_type;
    }

    XalanSpaceNodeTester&
    operator=(const XalanSpaceNodeTester&   theRHS)
    {
        m_matchScore = theRHS.m_matchScore;
        m_type = theRHS.m_type;

        ParentType::operator=(theRHS);

        return *this;
    }

private:

    XPath::eMatchScore  m_matchScore;

    eType               m_type;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALAN_SPACENODETESTER_HEADER_GUARD_1357924680)
