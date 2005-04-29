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
#if !defined(EXSLT_MATHIMPL_HEADER_GUARD_1357924680)
#define EXSLT_MATHIMPL_HEADER_GUARD_1357924680



#include "XalanEXSLTDefinitions.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAbs : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAbs()
    {
    }

    virtual
    ~XalanEXSLTFunctionAbs()
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
    virtual XalanEXSLTFunctionAbs*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionAbs&
    operator=(const XalanEXSLTFunctionAbs&);

    bool
    operator==(const XalanEXSLTFunctionAbs&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionRandom : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionRandom()
    {
    }

    virtual
    ~XalanEXSLTFunctionRandom()
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
    virtual XalanEXSLTFunctionRandom*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionRandom&
    operator=(const XalanEXSLTFunctionRandom&);

    bool
    operator==(const XalanEXSLTFunctionRandom&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAcos : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAcos()
    {
    }

    virtual
    ~XalanEXSLTFunctionAcos()
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
    virtual XalanEXSLTFunctionAcos*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionAcos&
    operator=(const XalanEXSLTFunctionAcos&);

    bool
    operator==(const XalanEXSLTFunctionAcos&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAsin : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAsin()
    {
    }

    virtual
    ~XalanEXSLTFunctionAsin()
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
    virtual XalanEXSLTFunctionAsin*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionAsin&
    operator=(const XalanEXSLTFunctionAsin&);

    bool
    operator==(const XalanEXSLTFunctionAsin&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAtan : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAtan()
    {
    }

    virtual
    ~XalanEXSLTFunctionAtan()
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
    virtual XalanEXSLTFunctionAtan*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionAtan&
    operator=(const XalanEXSLTFunctionAtan&);

    bool
    operator==(const XalanEXSLTFunctionAtan&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAtan2 : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAtan2()
    {
    }

    virtual
    ~XalanEXSLTFunctionAtan2()
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
    virtual XalanEXSLTFunctionAtan2*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionAtan2&
    operator=(const XalanEXSLTFunctionAtan2&);

    bool
    operator==(const XalanEXSLTFunctionAtan2&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionConstant : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionConstant()
    {
    }

    virtual
    ~XalanEXSLTFunctionConstant()
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
    virtual XalanEXSLTFunctionConstant*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAcceptsTwoArguments_1Param,
                    "constant()");
    }

private:

    static const XalanDOMChar   s_eString[];
    static const XalanDOMChar   s_ln10String[];
    static const XalanDOMChar   s_ln2String[];
    static const XalanDOMChar   s_log2EString[];
    static const XalanDOMChar   s_piString[];
    static const XalanDOMChar   s_sqrt1_2String[];
    static const XalanDOMChar   s_sqrt2String[];

    static const double     s_eValues[];
    static const double     s_ln10Values[];
    static const double     s_ln2Values[];
    static const double     s_log2EValues[];
    static const double     s_piValues[];
    static const double     s_sqrt1_2Values[];
    static const double     s_sqrt2Values[];


    // Not implemented...
    XalanEXSLTFunctionConstant&
    operator=(const XalanEXSLTFunctionConstant&);

    bool
    operator==(const XalanEXSLTFunctionConstant&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionCos : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionCos()
    {
    }

    virtual
    ~XalanEXSLTFunctionCos()
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
    virtual XalanEXSLTFunctionCos*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionCos&
    operator=(const XalanEXSLTFunctionCos&);

    bool
    operator==(const XalanEXSLTFunctionCos&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionExp : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionExp()
    {
    }

    virtual
    ~XalanEXSLTFunctionExp()
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
    virtual XalanEXSLTFunctionExp*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionExp&
    operator=(const XalanEXSLTFunctionExp&);

    bool
    operator==(const XalanEXSLTFunctionExp&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionHighest : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionHighest()
    {
    }

    virtual
    ~XalanEXSLTFunctionHighest()
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
    virtual XalanEXSLTFunctionHighest*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionHighest&
    operator=(const XalanEXSLTFunctionHighest&);

    bool
    operator==(const XalanEXSLTFunctionHighest&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLog : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionLog()
    {
    }

    virtual
    ~XalanEXSLTFunctionLog()
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
    virtual XalanEXSLTFunctionLog*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionLog&
    operator=(const XalanEXSLTFunctionLog&);

    bool
    operator==(const XalanEXSLTFunctionLog&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLowest : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionLowest()
    {
    }

    virtual
    ~XalanEXSLTFunctionLowest()
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
    virtual XalanEXSLTFunctionLowest*
#endif
    clone(MemoryManagerType&  theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionLowest&
    operator=(const XalanEXSLTFunctionLowest&);

    bool
    operator==(const XalanEXSLTFunctionLowest&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionMax : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionMax()
    {
    }

    virtual
    ~XalanEXSLTFunctionMax()
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
    virtual XalanEXSLTFunctionMax*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionMax&
    operator=(const XalanEXSLTFunctionMax&);

    bool
    operator==(const XalanEXSLTFunctionMax&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionMin : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionMin()
    {
    }

    virtual
    ~XalanEXSLTFunctionMin()
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
    virtual XalanEXSLTFunctionMin*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionMin&
    operator=(const XalanEXSLTFunctionMin&);

    bool
    operator==(const XalanEXSLTFunctionMin&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionPower : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionPower()
    {
    }

    virtual
    ~XalanEXSLTFunctionPower()
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
    virtual XalanEXSLTFunctionPower*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionPower&
    operator=(const XalanEXSLTFunctionPower&);

    bool
    operator==(const XalanEXSLTFunctionPower&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionSin : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionSin()
    {
    }

    virtual
    ~XalanEXSLTFunctionSin()
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
    virtual XalanEXSLTFunctionSin*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionSin&
    operator=(const XalanEXSLTFunctionSin&);

    bool
    operator==(const XalanEXSLTFunctionSin&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionSqrt : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionSqrt()
    {
    }

    virtual
    ~XalanEXSLTFunctionSqrt()
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
    virtual XalanEXSLTFunctionSqrt*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionSqrt&
    operator=(const XalanEXSLTFunctionSqrt&);

    bool
    operator==(const XalanEXSLTFunctionSqrt&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionTan : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionTan()
    {
    }

    virtual
    ~XalanEXSLTFunctionTan()
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
    virtual XalanEXSLTFunctionTan*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const;

private:

    // Not implemented...
    XalanEXSLTFunctionTan&
    operator=(const XalanEXSLTFunctionTan&);

    bool
    operator==(const XalanEXSLTFunctionTan&) const;
};



XALAN_CPP_NAMESPACE_END



#endif  // EXSLT_MATHIMPL_HEADER_GUARD_1357924680
