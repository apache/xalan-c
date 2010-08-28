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
#include "FunctionNodeSet.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XNodeSetBase.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XalanDocumentFragmentNodeRefListBaseProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDocumentFragmentXNodeSetBaseProxy : public XNodeSetBase
{
public:

    XalanDocumentFragmentXNodeSetBaseProxy(
                XPathExecutionContext&  theExecutionContext,
                const XObjectPtr&       theXObject) :
        XNodeSetBase(theExecutionContext.getMemoryManager()),
        m_xobject(theXObject),
        m_proxy(theXObject->rtree())
    {
    }

    static XalanDocumentFragmentXNodeSetBaseProxy*
    create(
                XPathExecutionContext&  theExecutionContext,
                const XObjectPtr&       theXObject)
    {
        XalanDocumentFragmentXNodeSetBaseProxy*     theResult;

        return XalanConstruct(
                    theExecutionContext.getMemoryManager(),
                    theResult,
                    theExecutionContext,
                    theXObject);
    }

    XalanDocumentFragmentXNodeSetBaseProxy(
                MemoryManager&                                  theManager,
                const XalanDocumentFragmentXNodeSetBaseProxy&   theSource) :
        XNodeSetBase(theSource, theManager),
        m_xobject(theSource.m_xobject),
        m_proxy(theSource.m_proxy)
    {
    }

    virtual
    ~XalanDocumentFragmentXNodeSetBaseProxy()
    {
    }

    virtual const NodeRefListBase&
    nodeset(XPathExecutionContext&  /* theExecutionContext */) const
    {
        return m_proxy;
    }

    virtual const NodeRefListBase&
    nodeset() const
    {
        return m_proxy;
    }

    virtual void
    dereferenced()
    {
        XalanDestroy(getMemoryManager(), *this);
    }

    virtual XalanNode*
    item(size_type  index) const
    {
        return m_proxy.item(index);
    }

    virtual size_type
    getLength() const
    {
        return m_proxy.getLength();
    }

private:

    const XObjectPtr                                    m_xobject;

    const XalanDocumentFragmentNodeRefListBaseProxy     m_proxy;
};



FunctionNodeSet::FunctionNodeSet(bool   convertString) :
    m_convertString(convertString)
{
}



FunctionNodeSet::~FunctionNodeSet()
{
}



XObjectPtr
FunctionNodeSet::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    const XObject::eObjectType  theType = args[0]->getType();

    if (theType == XObject::eTypeResultTreeFrag ||
        (theType == XObject::eTypeString && m_convertString == true))
    {
        const XObjectPtr    theResult(
                        XalanDocumentFragmentXNodeSetBaseProxy::create(
                            executionContext,
                            args[0]));
        executionContext.getXObjectFactory().holdReference(theResult);

        return theResult;
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     theMessage = theGuard.get();

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eWarning,
            getInvalidArgumentTypeError(theMessage),
            locator,
            context);

        return args[0];
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNodeSet*
#endif
FunctionNodeSet::clone(MemoryManager&   theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionNodeSet::getError(XalanDOMString&   theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionAcceptsOneArgument_1Param,
                "nodeset");
}



const XalanDOMString&
FunctionNodeSet::getInvalidArgumentTypeError(XalanDOMString&    theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::InvalidArgumentTypeFunction_1Param,
                "nodeset");
}



XALAN_CPP_NAMESPACE_END
