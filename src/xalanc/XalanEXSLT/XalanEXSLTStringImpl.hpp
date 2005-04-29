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
#if !defined(EXSLT_STRINGIMPL_HEADER_GUARD_1357924680)
#define EXSLT_STRINGIMPL_HEADER_GUARD_1357924680



#include "XalanEXSLTDefinitions.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAlign : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionAlign()
    {
    }


    virtual
    ~XalanEXSLTFunctionAlign()
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
    virtual XalanEXSLTFunctionAlign*
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
                    XalanMessages::EXSLTFunctionAcceptsTwoOrThreeArguments_1Param,
                    "align()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionAlign&
    operator=(const XalanEXSLTFunctionAlign&);

    bool
    operator==(const XalanEXSLTFunctionAlign&) const;


    // Data members...
    static const XalanDOMChar   s_centerString[];
    static const XalanDOMChar   s_rightString[];
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionConcat : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionConcat() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionConcat()
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
    virtual XalanEXSLTFunctionConcat*
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
                    XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,
                    "concat()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionConcat&
    operator=(const XalanEXSLTFunctionConcat&);

    bool
    operator==(const XalanEXSLTFunctionConcat&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionPadding : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionPadding(MemoryManagerType&  theManager) :
        Function(),
        m_space(s_spaceString, theManager)
    {
    }

    // A dummy constructor for use internally.  Do not use this one!!!!
    XalanEXSLTFunctionPadding(
                MemoryManagerType&  theManager,
                int                 /* theDummy */) :
        Function(),
        m_space(theManager)
    {
    }

    XalanEXSLTFunctionPadding(
                const XalanEXSLTFunctionPadding&    other,
                MemoryManagerType&                  theManager) :
        Function(other),
        m_space(s_spaceString, theManager)
    {
    }

    virtual
    ~XalanEXSLTFunctionPadding()
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
    virtual XalanEXSLTFunctionPadding*
#endif
    clone(MemoryManagerType&    theManager) const
    {
        return XalanCopyConstruct(theManager, *this, theManager);
    }

protected:

    const XalanDOMString&
    getError(XalanDOMString&    theBuffer) const
    {
        return XalanMessageLoader::getMessage(
                    theBuffer,
                    XalanMessages::EXSLTFunctionAccepts1Or2Argument_1Param,
                    "padding()");
    }

private:

    // Not implemented...
    XalanEXSLTFunctionPadding(const XalanEXSLTFunctionPadding&);

    XalanEXSLTFunctionPadding&
    operator=(const XalanEXSLTFunctionPadding&);

    bool
    operator==(const XalanEXSLTFunctionPadding&) const;


    // Data members...
    const XalanDOMString        m_space;

    static const XalanDOMChar   s_spaceString[];
};


class XALAN_EXSLT_EXPORT XalanEXSLTFunctionEncodeURI : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionEncodeURI() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionEncodeURI()
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
    virtual XalanEXSLTFunctionEncodeURI*
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
                    XalanMessages::EXSLTFunctionAcceptsTwoOrThreeArguments_1Param,
                    "encode-uri()");
    }

    const XalanDOMString&
    escapedOctet(
                XalanDOMChar        theChar,
                XalanDOMString&     theBuffer) const;

    static const XalanDOMChar               s_reservedChars[];
    static const XalanDOMString::size_type  s_reservedCharsSize;

    static const XalanDOMChar               s_excludedChars[];
    static const XalanDOMString::size_type  s_excludedCharsSize;

private:

    // Not implemented...
    XalanEXSLTFunctionEncodeURI&
    operator=(const XalanEXSLTFunctionEncodeURI&);

    bool
    operator==(const XalanEXSLTFunctionEncodeURI&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionDecodeURI : public Function
{
public:

    typedef Function    ParentType;

    XalanEXSLTFunctionDecodeURI() :
        Function()
    {
    }

    virtual
    ~XalanEXSLTFunctionDecodeURI()
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
    virtual XalanEXSLTFunctionDecodeURI*
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
                    XalanMessages::EXSLTFunctionAccepts1Or2Argument_1Param,
                    "decode-uri()");

    }

    XalanDOMChar
    hexCharsToByte( 
            XPathExecutionContext&      executionContext,
            XalanNode*                  context,
            const LocatorType*          locator,
            const XalanDOMChar          highHexChar,
            const XalanDOMChar          lowHexChar) const;

    static const XalanDOMString::size_type s_octetSize;

private:

    // Not implemented...
    XalanEXSLTFunctionDecodeURI&
    operator=(const XalanEXSLTFunctionDecodeURI&);

    bool
    operator==(const XalanEXSLTFunctionDecodeURI&) const;
};

XALAN_CPP_NAMESPACE_END



#endif  // EXSLT_STRINGIMPL_HEADER_GUARD_1357924680
