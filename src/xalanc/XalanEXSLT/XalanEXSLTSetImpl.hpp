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
#if !defined(EXSLT_SETIMPL_HEADER_GUARD_1357924680)
#define EXSLT_SETIMPL_HEADER_GUARD_1357924680



#include "XalanEXSLTDefinitions.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/Function.hpp>



#include <xalanc/XalanExtensions/FunctionDifference.hpp>
#include <xalanc/XalanExtensions/FunctionDistinct.hpp>
#include <xalanc/XalanExtensions/FunctionIntersection.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionDifference : public FunctionDifference
{
public:

    XalanEXSLTFunctionDifference() :
        FunctionDifference()
    {
    }

    virtual
    ~XalanEXSLTFunctionDifference()
    {
    }

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionDifference*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "difference()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionDifference&
    operator=(const XalanEXSLTFunctionDifference&);

    bool
    operator==(const XalanEXSLTFunctionDifference&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionDistinct : public FunctionDistinct
{
public:

    XalanEXSLTFunctionDistinct() :
        FunctionDistinct()
    {
    }

    virtual
    ~XalanEXSLTFunctionDistinct()
    {
    }

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionDistinct*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer ,
                    XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                    "distinct()");
    }


private:

    // Not implemented...
    XalanEXSLTFunctionDistinct&
    operator=(const XalanEXSLTFunctionDistinct&);

    bool
    operator==(const XalanEXSLTFunctionDistinct&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionHasSameNode : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionHasSameNode() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionHasSameNode()
    {
    }

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const LocatorType*              locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
    using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionHasSameNode*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "has-same-node()");
    }



private:

    // Not implemented...
    XalanEXSLTFunctionHasSameNode&
    operator=(const XalanEXSLTFunctionHasSameNode&);

    bool
    operator==(const XalanEXSLTFunctionHasSameNode&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionIntersection : public FunctionIntersection
{
public:

    XalanEXSLTFunctionIntersection() :
        FunctionIntersection()
    {
    }

    virtual
    ~XalanEXSLTFunctionIntersection()
    {
    }

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionIntersection*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "distinct()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionIntersection&
    operator=(const XalanEXSLTFunctionIntersection&);

    bool
    operator==(const XalanEXSLTFunctionIntersection&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLeading : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionLeading() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionLeading()
    {
    }

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const LocatorType*              locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
    using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionLeading*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "leading()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionLeading&
    operator=(const XalanEXSLTFunctionLeading&);

    bool
    operator==(const XalanEXSLTFunctionLeading&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionTrailing : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionTrailing() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionTrailing()
    {
    }

    // These methods are inherited from Function ...

    virtual XObjectPtr
    execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const LocatorType*              locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
    using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual Function*
#else
    virtual XalanEXSLTFunctionTrailing*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    virtual const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "trailing()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionTrailing&
    operator=(const XalanEXSLTFunctionTrailing&);

    bool
    operator==(const XalanEXSLTFunctionTrailing&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // EXSLT_SETIMPL_HEADER_GUARD_1357924680
