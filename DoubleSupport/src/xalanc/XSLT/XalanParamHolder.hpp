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

#if !defined(XALAN_PARAMHOLDER_HEADER_GUARD)
#define XALAN_PARAMHOLDER_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



class XalanParamHolder
{
public:

    XalanParamHolder(MemoryManager&     theMemoryManager) :
        m_expression(theMemoryManager),
        m_value()
    {
    }

    XalanParamHolder(
                const XalanParamHolder&     theRHS,
                MemoryManager&              theMemoryManager) :
        m_expression(theRHS.m_expression, theMemoryManager),
        m_value(theRHS.m_value)
    {
    }

    XalanParamHolder(
                MemoryManager&          theMemoryManager,
                const XalanDOMString&   theString) :
        m_expression(theString, theMemoryManager),
        m_value()
    {
    }

    XalanParamHolder(
               MemoryManager&  theMemoryManager,
               XObjectPtr      theXObject) :
        m_expression(theMemoryManager),
        m_value(theXObject)
    {
    }

    XalanDOMString  m_expression;

    XObjectPtr      m_value;

private:

    // These are not implemented...
    XalanParamHolder();

    XalanParamHolder(const XalanParamHolder&    theRHS);
};

XALAN_USES_MEMORY_MANAGER(XalanParamHolder)



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_PARAMHOLDER_HEADER_GUARD
