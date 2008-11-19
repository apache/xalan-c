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

#include "XPathEnvSupportDefault.hpp"



#include <algorithm>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XalanXPathException.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPath.hpp"
#include "XPathExecutionContext.hpp"



XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)



XALAN_CPP_NAMESPACE_BEGIN



XPathEnvSupportDefault::NamespaceFunctionTablesType     XPathEnvSupportDefault::s_externalFunctions(XalanMemMgrs::getDummyMemMgr());

const XalanDOMString                                    XPathEnvSupportDefault::s_emptyString(XalanMemMgrs::getDummyMemMgr());

void
XPathEnvSupportDefault::initialize(MemoryManager&   theManager)
{
    XPathEnvSupportDefault::NamespaceFunctionTablesType tmpMap(theManager);

    s_externalFunctions.swap(tmpMap);
}



void
XPathEnvSupportDefault::terminate()
{
    XALAN_USING_STD(for_each)

    // Clean up the extension namespaces vector
    for_each(s_externalFunctions.begin(),
             s_externalFunctions.end(),
             NamespaceFunctionTableDeleteFunctor(s_externalFunctions.getMemoryManager()));

    NamespaceFunctionTablesType temp(XalanMemMgrs::getDummyMemMgr());
    temp.swap(s_externalFunctions);
}



XPathEnvSupportDefault::XPathEnvSupportDefault(MemoryManager&   theManager) :
    XPathEnvSupport(),
    m_sourceDocs(theManager),
    m_externalFunctions(theManager),
    m_memoryManager(theManager),
    m_pw(0)
{
}



XPathEnvSupportDefault::~XPathEnvSupportDefault()
{
    XALAN_USING_STD(for_each)

    // Clean up the extension namespaces vector
    for_each(m_externalFunctions.begin(),
             m_externalFunctions.end(),
             NamespaceFunctionTableDeleteFunctor(m_externalFunctions.getMemoryManager()));

    NamespaceFunctionTablesType temp(XalanMemMgrs::getDummyMemMgr());
    temp.swap(m_externalFunctions);
}



void
XPathEnvSupportDefault::updateFunctionTable(
            NamespaceFunctionTablesType&    theTable,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            const Function*                 function)
{
    // See if there's a table for that namespace...
    const NamespaceFunctionTablesType::iterator     i =
        theTable.find(theNamespace);

    if (i == theTable.end())
    {
        // The namespace was not found.  If function is not
        // 0, then add a clone of the function.
        if (function != 0)
        {
            theTable[theNamespace][functionName] =
                function->clone(theTable.getMemoryManager());
        }
    }
    else
    {
        // There is already a table for the namespace,
        // so look for the function...
        const FunctionTableType::iterator   j =
            (*i).second.find(functionName);

        if (j == (*i).second.end())
        {
            // The function was not found.  If function is not
            // 0, then add a clone of the function.
            if (function != 0)
            {
                (*i).second[functionName] = function->clone(theTable.getMemoryManager());
            }
        }
        else
        {
            // Found it, so delete the function...
            const_cast<Function*>((*j).second)->~Function();

            MemoryManager&  theManager = theTable.getMemoryManager();

            theManager.deallocate((void*)(*j).second);

            // If function is not 0, then we update
            // the entry.  Otherwise, we erase it...
            if (function != 0)
            {
                // Update it...
                (*j).second = function->clone(theTable.getMemoryManager());
            }
            else
            {
                // Erase it...
                (*i).second.erase(j);
            }
        }
    }
}



void
XPathEnvSupportDefault::installExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function)
{
    updateFunctionTable(s_externalFunctions, theNamespace, functionName, &function);
}



void
XPathEnvSupportDefault::uninstallExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName)
{
    updateFunctionTable(s_externalFunctions, theNamespace, functionName, 0);
}



void
XPathEnvSupportDefault::installExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function)
{
    updateFunctionTable(m_externalFunctions, theNamespace, functionName, &function);
}



void
XPathEnvSupportDefault::uninstallExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName)
{
    updateFunctionTable(m_externalFunctions, theNamespace, functionName, 0);
}



void
XPathEnvSupportDefault::reset()
{
    m_sourceDocs.clear();
}



XalanDocument*
XPathEnvSupportDefault::parseXML(
            MemoryManager&      /* theManager */,
            const XalanDOMString&   /* urlString */,
            const XalanDOMString&   /* base */,
            ErrorHandler*           /* theErrorHandler */)
{
    return 0;
}



XalanDocument*
XPathEnvSupportDefault::getSourceDocument(const XalanDOMString&     theURI) const
{
    const SourceDocsTableType::const_iterator   i =
            m_sourceDocs.find(theURI);

    if (i == m_sourceDocs.end())
    {
        return 0;
    }
    else
    {
        return (*i).second;
    }
}



void
XPathEnvSupportDefault::setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument)
{
    m_sourceDocs[theURI] = theDocument;
}



const XalanDOMString&
XPathEnvSupportDefault::findURIFromDoc(const XalanDocument*     owner) const
{
    SourceDocsTableType::const_iterator i =
            m_sourceDocs.begin();

    bool    fFound = false;

    while(i != m_sourceDocs.end() && fFound == false)
    {
        if ((*i).second == owner)
        {
            fFound = true;
        }
        else
        {
            ++i;
        }
    }

    return fFound == false ? s_emptyString : (*i).first;
}



bool
XPathEnvSupportDefault::elementAvailable(
            const XalanDOMString&   /* theNamespace */,
            const XalanDOMString&   /* elementName */) const
{
    return false;
}



bool
XPathEnvSupportDefault::functionAvailable(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName) const
{
    bool    theResult = false;

    // Any function without a namespace prefix is considered
    // to be an intrinsic function.
    if (theNamespace.empty() == true)
    {
        theResult = XPath::isInstalledFunction(functionName);
    }
    else
    {
        const Function* const   theFunction =
            findFunction(
                theNamespace,
                functionName);

        if (theFunction != 0)
        {
            theResult = true;
        }
    }

    return theResult;
}



const Function*
XPathEnvSupportDefault::findFunction(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName) const
{
    // First, look locally...
    const Function*     theFunction = findFunction(
            m_externalFunctions,
            theNamespace,
            functionName);

    if (theFunction == 0)
    {
        // Not found, so look in the global space...
        theFunction = findFunction(
            s_externalFunctions,
            theNamespace,
            functionName);
    }

    return theFunction;
}



const Function*
XPathEnvSupportDefault::findFunction(
            const NamespaceFunctionTablesType&  theTable,
            const XalanDOMString&               theNamespace,
            const XalanDOMString&               functionName) const
{
    const Function* theFunction = 0;

    // See if there's a table for that namespace...
    const NamespaceFunctionTablesType::const_iterator   i =
        theTable.find(theNamespace);

    if (i != theTable.end())
    {
        // There is a table for the namespace,
        // so look for the function...
        const FunctionTableType::const_iterator     j =
            (*i).second.find(functionName);

        if (j != (*i).second.end())
        {
            // Found the function...
            assert((*j).second != 0);

            theFunction = (*j).second;
        }
    }

    return theFunction;
}



XObjectPtr
XPathEnvSupportDefault::extFunction(
            XPathExecutionContext&          executionContext,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator) const
{
    const Function* const   theFunction = findFunction(theNamespace, functionName);

    if (theFunction != 0)
    {
        return theFunction->execute(
                    executionContext,
                    context,
                    argVec,
                    locator);
    }
    else
    {
        XalanDOMString  theFunctionName(executionContext.getMemoryManager());

        if(theNamespace.empty() == false)
        {
            theFunctionName += theNamespace;
            theFunctionName += DOMServices::s_XMLNamespaceSeparatorString;
        }

        theFunctionName += functionName;

        const XPathExecutionContext::GetCachedString    theGuard(executionContext);

        throw XPathExceptionFunctionNotAvailable(
                theFunctionName,
                theGuard.get(),
                locator);

        // dummy return value...
        return XObjectPtr();
    }
}



void
XPathEnvSupportDefault::setPrintWriter(PrintWriter*     pw)
{
    m_pw = pw;
}



void
XPathEnvSupportDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    if (m_pw != 0)
    {
        defaultFormat(
            *m_pw,
            source,
            classification,
            msg,
            locator,
            sourceNode);
    }

    if (classification == eError)
    {
        throw XalanXPathException(
                msg,
                getMemoryManager(),
                locator);
    }
}



void
XPathEnvSupportDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    if (m_pw != 0)
    {
        defaultFormat(
            *m_pw,
            source,
            classification,
            msg,
            sourceNode);
    }

    if (classification == eError)
    {
        throw XalanXPathException(
                msg,
                getMemoryManager(),
                0);
    }
}



XPathEnvSupportDefault::NamespaceFunctionTableDeleteFunctor::NamespaceFunctionTableDeleteFunctor(MemoryManager&     theManager) :
    m_memoryManager(theManager)
{
}



void
XPathEnvSupportDefault::NamespaceFunctionTableDeleteFunctor::operator()(const NamespaceFunctionTablesInnerType::value_type& thePair) const
{
    FunctionTableInnerType::const_iterator  i = thePair.second.begin();

    while (i != thePair.second.end())
    {
        XalanDestroy(
            m_memoryManager,
            const_cast<Function*>((*i).second));

        ++i;
    }
}



XALAN_CPP_NAMESPACE_END
