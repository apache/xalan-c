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

#include "XalanSpaceNodeTester.hpp"



#include "xalanc/PlatformSupport/DOMStringHelper.hpp"
#include "xalanc/PlatformSupport/XalanMessageLoader.hpp"



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSpaceNodeTester::XalanSpaceNodeTester() :
    m_nodeTester(),
    m_matchScore(XPath::eMatchScoreNone)
{
}
    


static const XalanDOMString     s_emptyString;



XalanSpaceNodeTester::XalanSpaceNodeTester(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMString&           theNameTest,
            const PrefixResolver&           thePrefixResolver,
            const LocatorType*              theLocator) :
    m_nodeTester(),
    m_matchScore(XPath::eMatchScoreNone)
{
	const XalanDOMString::size_type     theLength =
                length(theNameTest);

    if (theLength == 1 && theNameTest[0] == XPath::PSEUDONAME_ANY[0])
    {
        m_nodeTester = XPath::NodeTester(s_emptyString, s_emptyString, m_matchScore);
    }
    else
    {
        const XalanDOMString::size_type     theIndex =
                indexOf(theNameTest, XalanUnicode::charColon);

        // If there's no ':', it's an NCName...
        if (theIndex == theLength)
        {
            if (XalanQName::isValidNCName(theNameTest) == false)
            {
		        theConstructionContext.error(
                    XalanMessageLoader::getMessage(
                        XalanMessages::IsNotValidQName_1Param,
                        theNameTest),
                        0,
                        theLocator);
            }
            else
            {
                m_nodeTester =
                    XPath::NodeTester(
                        s_emptyString,
                        theConstructionContext.getPooledString(theNameTest),
                        m_matchScore);
            }
        }
        else
        {
            StylesheetConstructionContext::GetAndReleaseCachedString    scratchGuard(theConstructionContext);

	        XalanDOMString&		theScratchString = scratchGuard.get();

            theScratchString.assign(theNameTest, 0, theIndex);

            // Get the namespace URI for the prefix...
            const XalanDOMString* const     theNamespaceURI =
                thePrefixResolver.getNamespaceForPrefix(theScratchString);

            if (theNamespaceURI == 0)
            {
		        theConstructionContext.error(
                    XalanMessageLoader::getMessage(
                        XalanMessages::UndeclaredNamespacePrefix_1Param,
                        theScratchString),
                        0,
                        theLocator);
            }
            else
            {
                // OK, now we have a namespace URI...
                if (XalanQName::isValidNCName(theScratchString) == false)
                {
		            theConstructionContext.error(
                        XalanMessageLoader::getMessage(
                            XalanMessages::IsNotValidQName_1Param,
                            theNameTest),
                            0,
                            theLocator);
                }
                else if (theIndex == theLength - 2 &&
                         theNameTest[theIndex + 1] == XPath::PSEUDONAME_ANY[0])
                {
                    // It's of the form "NCName:*"
                    m_nodeTester =
                        XPath::NodeTester(
                            theConstructionContext.getPooledString(*theNamespaceURI),
                            s_emptyString,
                            m_matchScore);
                }
                else
                {
                    theScratchString.assign(theNameTest, theIndex + 1, theLength - theIndex - 1);

                    if (XalanQName::isValidNCName(theScratchString) == false)
                    {
		                theConstructionContext.error(
                            XalanMessageLoader::getMessage(
                                XalanMessages::IsNotValidQName_1Param,
                                theNameTest),
                                0,
                                theLocator);
                    }
                    else
                    {
                        // It's of the form "NCName:NCName"
                        m_nodeTester =
                            XPath::NodeTester(
                                theConstructionContext.getPooledString(*theNamespaceURI),
                                theConstructionContext.getPooledString(theScratchString),
                                m_matchScore);
                    }
                }
            }
        }
    }

    assert(m_matchScore != XPath::eMatchScoreNone);
}
    


XalanSpaceNodeTester::~XalanSpaceNodeTester()
{
}
    


XALAN_CPP_NAMESPACE_END
