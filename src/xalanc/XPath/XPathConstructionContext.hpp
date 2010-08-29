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
#if !defined(XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680)
#define XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include "xalanc/PlatformSupport/ProblemListenerBase.hpp"



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator  LocatorType;
XALAN_USING_XERCES(Locator)



class XalanNode;



//
// An abstract class which provides support for constructing the internal
// representation  of a stylesheet.
//
class XALAN_XPATH_EXPORT XPathConstructionContext : public ProblemListenerBase
{
public:

    XPathConstructionContext(MemoryManager&     theManager);

    virtual
    ~XPathConstructionContext();


    // These interfaces are inherited from ProblemListenerBase...
    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode) = 0;

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode) = 0;


    // These interfaces are new...
    /**
     * Reset the instance.  Any existing objects
     * created by the instance will be destroyed.
     */
    virtual void
    reset() = 0;

    /**
     * Get a pooled string given the source string.  If
     * the string already exists in the pool, no copy
     * will be made.  If not, a copy will be made and
     * kept for later use.
     *
     * @param theString The source string
     * @return a const reference to a pooled string.
     */
    virtual const XalanDOMString&
    getPooledString(const XalanDOMString&   theString) = 0;

    /**
     * Get a pooled string given the source character
     * array.  If the string already exists in the pool,
     * no copy will be made.  If not, a copy will be made
     * and kept for later use.
     *
     * @param theString The source character array
     * @param theLength The length of the character array
     * @return a const reference to a pooled string.
     */
    virtual const XalanDOMString&
    getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength = XalanDOMString::npos) = 0;

    /**
     * Get a cached string for temporary use.
     *
     * @return A reference to the string
     */
    virtual XalanDOMString&
    getCachedString() = 0;

    /**
     * Return a cached string.
     *
     * @param theString The string to release.
     *
     * @return true if the string was released successfully.
     */
    virtual bool
    releaseCachedString(XalanDOMString&     theString) = 0;

    class GetCachedString
    {
    public:

        GetCachedString(XPathConstructionContext&     theConstructionContext) :
            m_constructionContext(&theConstructionContext),
            m_string(&theConstructionContext.getCachedString())
        {
        }

        ~GetCachedString()
        {
            assert(m_string != 0);

            m_constructionContext->releaseCachedString(*m_string);
        }

        XalanDOMString&
        get() const
        {
            assert(m_string != 0);

            return *m_string;
        }

        XPathConstructionContext&
        getConstructionContext() const
        {
            assert(m_constructionContext != 0);

            return *m_constructionContext;
        }

    private:

        // Not implemented...
        GetCachedString();

        GetCachedString(const GetCachedString&);

        GetCachedString&
        operator=(const GetCachedString&);


        // Data members...
        XPathConstructionContext*   m_constructionContext;

        XalanDOMString*             m_string;
    };

    typedef GetCachedString     GetAndReleaseCachedString;

    MemoryManager&
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    MemoryManager&
    getMemoryManager()
    {
        return m_memoryManager;
    }

private:

    MemoryManager&  m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680
