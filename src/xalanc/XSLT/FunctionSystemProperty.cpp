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
#include "FunctionSystemProperty.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanQName.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



FunctionSystemProperty::FunctionSystemProperty(MemoryManager&   theManager) :
    Function(),
    m_xsltNamespaceURI("http://www.w3.org/1999/XSL/Transform", theManager),
    m_versionPropertyString("version", theManager),
    m_vendorPropertyString("vendor", theManager),
    m_vendorURLPropertyString("vendor-url", theManager),
    m_vendorString("Apache Software Foundation", theManager),
    m_vendorURLString("http://xml.apache.org/xalan-c", theManager)
{
}



FunctionSystemProperty::FunctionSystemProperty(
            const FunctionSystemProperty&   other, 
            MemoryManager&              theManager) :
    Function(other),
    m_xsltNamespaceURI(other.m_xsltNamespaceURI, theManager),
    m_versionPropertyString(other.m_versionPropertyString, theManager),
    m_vendorPropertyString(other.m_vendorPropertyString, theManager),
    m_vendorURLPropertyString(other.m_vendorURLPropertyString, theManager),
    m_vendorString(other.m_vendorString, theManager),
    m_vendorURLString(other.m_vendorURLString, theManager)
{
}



FunctionSystemProperty::~FunctionSystemProperty()
{
}




inline void
validateNCName(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const Locator*          locator,
            const XalanDOMString&   ncname)
{
    if (XalanQName::isValidNCName(ncname) == false)
    {
        const Function::GetCachedString     theGuard(executionContext);

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::PropertyIsNotValidQName_1Param,
                "system-property()"),
            locator,
            context);
    }
}



XObjectPtr
FunctionSystemProperty::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const Locator*          locator) const
{
    assert(arg1.null() == false);

    const XalanDOMString&               fullName = arg1->str(executionContext);
    const XalanDOMString::size_type     fullNameLength = fullName.length();
    const XalanDOMString::size_type     indexOfNSSep = indexOf(fullName, XalanUnicode::charColon);

    if(indexOfNSSep < fullNameLength)
    {
        const GetCachedString   guard(executionContext);

        XalanDOMString&     theBuffer = guard.get();

        substring(fullName, theBuffer, 0, indexOfNSSep);

        validateNCName(executionContext, context, locator, theBuffer);

        const XalanDOMString* const     nspace = executionContext.getNamespaceForPrefix(theBuffer);

        if (nspace == 0)
        {
            const GetCachedString   theGuard(executionContext);

            executionContext.problem(
                XPathExecutionContext::eXPath,
                XPathExecutionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::PrefixIsNotDeclared_1Param,
                    theBuffer),
                locator,
                context);
        }
        else
        {
            if(*nspace == m_xsltNamespaceURI)
            {
                substring(fullName, theBuffer, indexOfNSSep + 1);

                validateNCName(executionContext, context, locator, theBuffer);

                if(equals(theBuffer, m_versionPropertyString))
                {
                    return executionContext.getXObjectFactory().createNumber(1.0);
                }
                else if(equals(theBuffer, m_vendorPropertyString))
                {
                    return executionContext.getXObjectFactory().createStringReference(m_vendorString);
                }
                else if(equals(theBuffer, m_vendorURLPropertyString))
                {
                    return executionContext.getXObjectFactory().createStringReference(m_vendorURLString);
                }
                else
                {
                    return executionContext.getXObjectFactory().createStringReference(s_emptyString);
                }
            }
        }
    }
    else
    {
        validateNCName(executionContext, context, locator, fullName);

        XalanDOMString::CharVectorType theResultVect(executionContext.getMemoryManager());

        TranscodeToLocalCodePage(fullName, theResultVect, true);

        const char* const   theEnvString =
#if defined(XALAN_STRICT_ANSI_HEADERS)
            std::getenv(c_str(theResultVect));
#else
            getenv(c_str(theResultVect));
#endif

        if (theEnvString != 0)
        {
            GetCachedString     theResult(executionContext);

            XalanDOMString&     theString = theResult.get();

            TranscodeFromLocalCodePage(theEnvString, theString);

            return executionContext.getXObjectFactory().createString(theResult);
        }
    }

    return executionContext.getXObjectFactory().createStringReference(s_emptyString);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSystemProperty*
#endif
FunctionSystemProperty::clone(MemoryManager&    theManager) const
{
    return XalanCopyConstruct(theManager, *this, theManager);
}



const XalanDOMString&
FunctionSystemProperty::getError(XalanDOMString&    theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionAcceptsOneArgument_1Param,
                "system-property()");
}



XALAN_CPP_NAMESPACE_END
