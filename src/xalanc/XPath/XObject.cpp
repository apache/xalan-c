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
// Class header file.
#include "XObject.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "NodeRefList.hpp"
#include "XObjectFactory.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN


static XalanDOMString   s_localBooleanString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localFalseString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localNodesetString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localNumberString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localResultTreeFragmentString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localStringString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localTrueString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString&   XObject::s_booleanString =
        s_localBooleanString;

const XalanDOMString&   XObject::s_falseString =
        s_localFalseString;

const XalanDOMString&   XObject::s_nodesetString =
        s_localNodesetString;

const XalanDOMString&   XObject::s_numberString =
        s_localNumberString;

const XalanDOMString&   XObject::s_resultTreeFragmentString =
        s_localResultTreeFragmentString;

const XalanDOMString&   XObject::s_stringString =
        s_localStringString;

const XalanDOMString&   XObject::s_trueString =
        s_localTrueString;

const XalanDOMString    XObject::s_emptyString(XalanMemMgrs::getDummyMemMgr());


static const XalanDOMChar   s_boolean[] =
{
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};

static const XalanDOMChar   s_false[] =
{
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    0
};

static const XalanDOMChar   s_number[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};

static const XalanDOMChar   s_nodeset[] =
{
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    0
};

static const XalanDOMChar   s_resultTreeFragment[] =
{
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_e,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_f,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};

static const XalanDOMChar   s_string[] =
{
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

static const XalanDOMChar   s_true[] =
{
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_e,
    0
};


void
XObject::initialize(MemoryManagerType& theManager)
{
    s_localBooleanString.reset(theManager, s_boolean);
    s_localFalseString.reset(theManager,  s_false);
    s_localNodesetString.reset(theManager, s_nodeset);
    s_localNumberString.reset(theManager, s_number);
    s_localResultTreeFragmentString.reset(theManager, s_resultTreeFragment);
    s_localStringString.reset(theManager, s_string);
    s_localTrueString.reset(theManager, s_true);
}



void
XObject::terminate()
{
    releaseMemory(s_localBooleanString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localFalseString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localNodesetString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localNumberString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localResultTreeFragmentString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localStringString, XalanMemMgrs::getDummyMemMgr());
    releaseMemory(s_localTrueString, XalanMemMgrs::getDummyMemMgr());
}



XObject::XObject(eObjectType    theObjectType) :
    m_objectType(theObjectType),
    m_factory(0)
{
}



XObject::XObject(const XObject&     source) :
    m_objectType(source.m_objectType),
    m_factory(0)
{
}



XObject::~XObject()
{
}



XObject::eObjectType
XObject::getRealType() const
{
    return m_objectType;
}



void
XObject::referenced()
{
}



double
XObject::number(
            XPathExecutionContext&  executionContext,
            const XalanNode&        theNode)
{
    XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);

    XalanDOMString&     theString = theGuard.get();

    XObject::string(theNode, theString);

    return XObject::number(theString, executionContext.getMemoryManager());
}



double
XObject::number(
            XPathExecutionContext&  executionContext,
            const NodeRefListBase&  theNodeList)
{
    if (theNodeList.getLength() == 0)
    {
        return number(s_emptyString, executionContext.getMemoryManager());
    }
    else
    {
        assert(theNodeList.item(0) != 0);

        return number(executionContext, *theNodeList.item(0));
    }
}



void 
XObject::dereferenced()
{
    if (m_factory != 0)
    {
        m_factory->returnObject(this);
    }
}



double
XObject::num() const
{
    throw XObjectInvalidConversionException(getType(), eTypeNumber);

    // This is just a dummy value to satisfy the compiler.
    return 0.0;
}



bool
XObject::boolean() const
{    
    throw XObjectInvalidConversionException(getType(), eTypeBoolean);

    // This is just a dummy value to satisfy the compiler.
    return false;
}



const XalanDOMString&
XObject::str() const
{


    throw XObjectInvalidConversionException(getType(), eTypeString);

    // This is just a dummy value to satisfy the compiler.
    return s_emptyString;
}



void
XObject::str(
            FormatterListener&  /* formatterListener */,
            MemberFunctionPtr   /* function */) const
{


        throw XObjectInvalidConversionException(getType(), eTypeString);
}



void
XObject::str(XalanDOMString&    theBuffer) const
{
    append(theBuffer, str());
}



static int foo = 0;

const XalanDocumentFragment&
XObject::rtree() const
{


    throw XObjectInvalidConversionException(getType(), eTypeResultTreeFrag);

    // This is just a dummy value to satisfy the compiler.
#if defined(XALAN_OLD_STYLE_CASTS)
    return (const XalanDocumentFragment&)foo;
#else
    return reinterpret_cast<const XalanDocumentFragment&>(foo);
#endif
}



static const NodeRefList    s_dummyList(XalanMemMgrs::getDummyMemMgr());



const NodeRefListBase&
XObject::nodeset() const
{
    throw XObjectInvalidConversionException(getType(), eTypeNodeSet);

    // error will throw, so this is just a dummy
    // value to satisfy the compiler.
    return s_dummyList;
}



inline void
getStringFromNode(
            const XalanNode&    theNode,
            XalanDOMString&     theString)
{
    DOMServices::getNodeData(theNode, theString);
}



struct
getStringFromNodeFunction
{
    getStringFromNodeFunction(XPathExecutionContext&    executionContext) :
        m_executionContext(executionContext)
    {
    }

    void
    operator()(
            const XalanNode&    theNode,
            XalanDOMString&     theString) const
    {
        getStringFromNode(theNode, theString);
    }

private:

    XPathExecutionContext&  m_executionContext;
};



struct
getNumberFromNodeFunction
{
    getNumberFromNodeFunction(XPathExecutionContext&    executionContext) :
        m_executionContext(executionContext)
    {
    }

    double
    operator()(const XalanNode&     theNode) const
    {
        return getNumberFromNode(theNode);
    }

    void
    operator()(
            const XalanNode&    theNode,
            double&             theNumber) const
    {
        getNumberFromNode(theNode, theNumber);
    }

private:

    double
    getNumberFromNode(const XalanNode&  theNode) const
    {
        XPathExecutionContext::GetAndReleaseCachedString    theString(m_executionContext);

        getStringFromNode(theNode, theString.get());

        return DoubleSupport::toDouble(theString.get(), m_executionContext.getMemoryManager());
    }

    void
    getNumberFromNode(
            const XalanNode&    theNode,
            double&             theNumber) const
    {
        theNumber = getNumberFromNode(theNode);
    }

    XPathExecutionContext&  m_executionContext;
};



struct
equalsDOMString
{
    equalsDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DOMStringEqualsFunction()(theLHS, theRHS);
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DOMStringEqualsFunction()(theLHS.str(), theRHS);
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DOMStringEqualsFunction()(theLHS, theRHS.str());
    }
private:
    MemoryManagerType& m_memoryManager;
};



struct
notEqualsDOMString
{
    notEqualsDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DOMStringNotEqualsFunction()(theLHS, theRHS);
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DOMStringNotEqualsFunction()(theLHS.str(), theRHS);
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DOMStringNotEqualsFunction()(theLHS, theRHS.str());
    }
private:
    MemoryManagerType& m_memoryManager;
};



struct
lessThanDOMString
{
    lessThanDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::lessThan(
                DOMStringToDouble(theLHS, m_memoryManager),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::lessThan(
                theLHS.num(),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DoubleSupport::lessThan(
                DOMStringToDouble(theLHS, m_memoryManager),
                theRHS.num());
    }
private:
    MemoryManagerType& m_memoryManager;
};



struct
lessThanOrEqualDOMString
{
    lessThanOrEqualDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::lessThanOrEqual(
                DOMStringToDouble(theLHS, m_memoryManager),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::lessThanOrEqual(
                theLHS.num(),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DoubleSupport::lessThanOrEqual(
                DOMStringToDouble(theLHS, m_memoryManager),
                theRHS.num());
    }
private:
    MemoryManagerType& m_memoryManager;
};



struct
greaterThanDOMString
{
    greaterThanDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::greaterThan(
                DOMStringToDouble(theLHS, m_memoryManager),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::greaterThan(
                theLHS.num(),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DoubleSupport::greaterThan(
                DOMStringToDouble(theLHS, m_memoryManager),
                theRHS.num());
    }
private:
    MemoryManagerType& m_memoryManager;
};



struct
greaterThanOrEqualDOMString
{
    greaterThanOrEqualDOMString(MemoryManagerType& theManager) :
    m_memoryManager(theManager)
    {
    } 

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::greaterThanOrEqual(
                DOMStringToDouble(theLHS, m_memoryManager),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XObject&          theLHS,
            const XalanDOMString&   theRHS) const
    {
        return DoubleSupport::greaterThanOrEqual(
                theLHS.num(),
                DOMStringToDouble(theRHS, m_memoryManager));
    }

    bool
    operator()(
            const XalanDOMString&   theLHS,
            const XObject&          theRHS) const
    {
        return DoubleSupport::greaterThanOrEqual(
                DOMStringToDouble(theLHS, m_memoryManager),
                theRHS.num());
    }
private:
        MemoryManagerType& m_memoryManager;
};



template<class CompareFunction, class TypeFunction>
inline bool
doCompareNodeSets(
            const NodeRefListBase&  theLHSNodeSet,
            const NodeRefListBase&  theRHSNodeSet,
            const TypeFunction&     theTypeFunction,
            const CompareFunction&  theCompareFunction,
            XPathExecutionContext&  executionContext)
{
    // Excerpt from: 
    //   XML Path Language (XPath) Version 1.0
    //   W3C Recommendation 16 November 1999
    //   http://www.w3.org/TR/xpath#booleans
    //   Editors: 
    //     James Clark <jjc@jclark.com> 
    //     Steve DeRose (Inso Corp. and Brown University) <Steven_DeRose@Brown.edu> 
    // 
    // If both objects to be compared are node-sets, then the comparison 
    // will be true if and only if there is a node in the first node-set 
    // and a node in the second node-set such that the result of performing 
    // the comparison on the string-values of the two nodes is true.

    // Note this little gem from the draft:
    // NOTE: If $x is bound to a node-set, then $x="foo" 
    // does not mean the same as not($x!="foo"): the former 
    // is true if and only if some node in $x has the string-value 
    // foo; the latter is true if and only if all nodes in $x have 
    // the string-value foo.
    bool    theResult = false;

    const NodeRefListBase::size_type    len1 = theLHSNodeSet.getLength();

    if (len1 > 0)
    {
        const NodeRefListBase::size_type    len2 = theRHSNodeSet.getLength();

        if (len2 > 0)
        {
            XPathExecutionContext::GetAndReleaseCachedString    s1(executionContext);

            XPathExecutionContext::GetAndReleaseCachedString    s2(executionContext);

            for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
            {
                const XalanNode* const  theLHSNode = theLHSNodeSet.item(i);
                assert(theLHSNode != 0);

                theTypeFunction(*theLHSNode, s1.get());

                for(NodeRefListBase::size_type k = 0; k < len2 && theResult == false; k++)
                {
                    const XalanNode* const  theRHSNode = theRHSNodeSet.item(k);
                    assert(theRHSNode != 0);

                    theTypeFunction(*theRHSNode, s2.get());

                    if(theCompareFunction(s1.get(), s2.get()) == true)
                    {
                        theResult = true;
                    }

                    clear(s2.get());
                }

                clear(s1.get());
            }
        }
    }

    return theResult;
}



template<class CompareFunction, class StringFunction>
inline bool
doCompareString(
            const NodeRefListBase&  theLHSNodeSet,
            const StringFunction&   theStringFunction,
            const XObject&          theRHS,
            const CompareFunction&  theCompareFunction,
            XPathExecutionContext&  executionContext)
{
    bool                theResult = false;

    const NodeRefListBase::size_type    len1 = theLHSNodeSet.getLength();

    XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);

    XalanDOMString&     theLHS = theGuard.get();

    for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
    {
        const XalanNode* const  theLHSNode = theLHSNodeSet.item(i);
        assert(theLHSNode != 0);

        theStringFunction(*theLHSNode, theLHS);

        if (theCompareFunction(theLHS, theRHS) == true)
        {
            theResult = true;
        }

        clear(theLHS);
    }

    return theResult;
}



template<class CompareFunction, class NumberFunction>
inline bool
doCompareNumber(
            const NodeRefListBase&  theLHSNodeSet,
            const NumberFunction&   theNumberFunction,
            const double            theRHS,
            const CompareFunction&  theCompareFunction)
{
    bool    theResult = false;

    const NodeRefListBase::size_type    len1 = theLHSNodeSet.getLength();

    for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
    {
        const XalanNode* const  theLHSNode = theLHSNodeSet.item(i);
        assert(theLHSNode != 0);

        const double    theLHS = theNumberFunction(*theLHSNode);

        if (theCompareFunction(theLHS, theRHS) == true)
        {
            theResult = true;
        }
    }

    return theResult;
}



template<class StringCompareFunction, class NumberCompareFunction>
inline bool
compareNodeSets(
            const XObject&                  theLHS,
            const XObject&                  theRHS,
            XObject::eObjectType            theRHSType,
            const StringCompareFunction&    theStringCompareFunction,
            const NumberCompareFunction&    theNumberCompareFunction,
            XPathExecutionContext&          executionContext)
{
    bool    theResult = false;

    if(theRHSType == XObject::eTypeNodeSet)
    {
        // Compare as node sets...
        theResult = doCompareNodeSets(
                theLHS.nodeset(),
                theRHS.nodeset(),
                getStringFromNodeFunction(executionContext),
                theStringCompareFunction,
                executionContext);

    }
    else if(theRHSType == XObject::eTypeBoolean)
    {
        // Excerpt from: 
        //   XML Path Language (XPath) Version 1.0
        //   W3C Recommendation 16 November 1999
        //   http://www.w3.org/TR/xpath#booleans
        //   Editors: 
        //     James Clark <jjc@jclark.com> 
        //     Steve DeRose (Inso Corp. and Brown University) <Steven_DeRose@Brown.edu> 
        //
        // If one object to be compared is a node-set and the other is a boolean, 
        // then the comparison will be true if and only if the result of 
        // performing the comparison on the boolean and on the result of 
        // converting the node-set to a boolean using the boolean function 
        // is true.
        const double    num1 = theLHS.boolean() == true ? 1.0 : 0.0;

        theResult = theNumberCompareFunction(num1, theRHS.num());
    }
    else if(theRHSType == XObject::eTypeNumber)
    {
        // Excerpt from: 
        //   XML Path Language (XPath) Version 1.0
        //   W3C Recommendation 16 November 1999
        //   http://www.w3.org/TR/xpath#booleans
        //   Editors: 
        //     James Clark <jjc@jclark.com> 
        //     Steve DeRose (Inso Corp. and Brown University) <Steven_DeRose@Brown.edu> 
        //
        // If one object to be compared is a node-set and the other is a number, 
        // then the comparison will be true if and only if there is a 
        // node in the node-set such that the result of performing the 
        // comparison on the number to be compared and on the result of 
        // converting the string-value of that node to a number using 
        // the number function is true. 

        theResult = doCompareNumber(
                theLHS.nodeset(),
                getNumberFromNodeFunction(executionContext),
                theRHS.num(),
                theNumberCompareFunction);
    }
    else if(theRHSType == XObject::eTypeResultTreeFrag)
    {
        // hmmm... 
        const double    theRHSNumber = theRHS.num();

        if(DoubleSupport::isNaN(theRHSNumber) == false)
        {
            // Compare as number...
            theResult = doCompareNumber(
                    theLHS.nodeset(),
                    getNumberFromNodeFunction(executionContext),
                    theRHS.num(),
                    theNumberCompareFunction);
        }
        else
        {
            // Compare as string...
            theResult = doCompareString(
                    theLHS.nodeset(),
                    getStringFromNodeFunction(executionContext),
                    theRHS,
                    theStringCompareFunction,
                    executionContext);
        }
    }
    else if(theRHSType == XObject::eTypeString)
    {
        // Excerpt from: 
        //   XML Path Language (XPath) Version 1.0
        //   W3C Recommendation 16 November 1999
        //   http://www.w3.org/TR/xpath#booleans
        //   Editors: 
        //     James Clark <jjc@jclark.com> 
        //     Steve DeRose (Inso Corp. and Brown University) <Steven_DeRose@Brown.edu> 
        // 
        // If one object to be compared is a node-set and the other is a 
        // string, then the comparison will be true if and only if there 
        // is a node in the node-set such that the result of performing 
        // the comparison on the string-value of the node and the other 
        // string is true. 
        theResult = doCompareString(
                theLHS.nodeset(),
                getStringFromNodeFunction(executionContext),
                theRHS,
                theStringCompareFunction,
                executionContext);
    }
    else if (theRHSType != XObject::eTypeUnknown)
    {
        assert(false);
    }

    return theResult;
}



inline bool
equalNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                equalsDOMString(executionContext.getMemoryManager()),
                DoubleSupport::equalFunction(),
                executionContext);
}



inline bool
notEqualNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                notEqualsDOMString(executionContext.getMemoryManager()),
                DoubleSupport::notEqualFunction(),
                executionContext);
}



inline bool
lessThanNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                lessThanDOMString(executionContext.getMemoryManager()),
                DoubleSupport::lessThanFunction(),
                executionContext);
}



inline bool
lessThanOrEqualNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                lessThanOrEqualDOMString(executionContext.getMemoryManager()),
                DoubleSupport::lessThanOrEqualFunction(),
                executionContext);
}



inline bool
greaterThanNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                greaterThanDOMString(executionContext.getMemoryManager()),
                DoubleSupport::greaterThanFunction(),
                executionContext);
}



inline bool
greaterThanOrEqualNodeSet(
            const XObject&          theLHS,
            const XObject&          theRHS,
            XObject::eObjectType    theRHSType,
            XPathExecutionContext&  executionContext)
{
    return compareNodeSets(
                theLHS,
                theRHS,
                theRHSType,
                greaterThanOrEqualDOMString(executionContext.getMemoryManager()),
                DoubleSupport::greaterThanOrEqualFunction(),
                executionContext);
}



bool
XObject::equals(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return true;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeUnknown)
        {
            return this == &theRHS ? true : false;
        }
        else if (theLHSType == eTypeNull)
        {
            return theRHS.getType() == eTypeNull ? true : false;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return equalNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else
        {
            const eObjectType   theRHSType = theRHS.getType();

            if (theRHSType == eTypeNodeSet)
            {
                return equalNodeSet(theRHS, *this, theLHSType, executionContext);
            }
            else
            {
                if (theLHSType == eTypeBoolean || theRHSType == eTypeBoolean)
                {
                    return boolean() == theRHS.boolean();
                }
                else if (theLHSType == eTypeNumber || theRHSType == eTypeNumber)
                {
                    return DoubleSupport::equal(num(), theRHS.num());
                }
                else
                {
                    return str() == theRHS.str();
                }
            }
        }
    }
}



bool
XObject::notEquals(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return false;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeUnknown)
        {
            return this == &theRHS ? false : true;
        }
        else if (theLHSType == eTypeNull)
        {
            return theRHS.getType() == eTypeNull ? false : true;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return notEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else
        {
            const eObjectType   theRHSType = theRHS.getType();

            if (theRHSType == eTypeNodeSet)
            {
                return notEqualNodeSet(theRHS, *this, theLHSType, executionContext);
            }
            else
            {
                if (theLHSType == eTypeBoolean || theRHSType == eTypeBoolean)
                {
                    return boolean() != theRHS.boolean();
                }
                else if (theLHSType == eTypeNumber || theRHSType == eTypeNumber)
                {
                    return DoubleSupport::notEqual(num(), theRHS.num());
                }
                else
                {
                    return str() != theRHS.str();
                }
            }
        }
    }
}



bool
XObject::lessThan(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return false;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
        {
            return false;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return lessThanNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else if (theRHS.getType() == eTypeNodeSet)
        {
            return greaterThanNodeSet(theRHS, *this, theLHSType, executionContext);
        }
        else
        {
            return DoubleSupport::lessThan(num(), theRHS.num());
        }
    }
}



bool
XObject::lessThanOrEquals(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return false;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
        {
            return false;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return lessThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else if (theRHS.getType() == eTypeNodeSet)
        {
            return greaterThanOrEqualNodeSet(theRHS, *this, theLHSType, executionContext);
        }
        else
        {
            return DoubleSupport::lessThanOrEqual(num(), theRHS.num());
        }
    }
}



bool
XObject::greaterThan(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return false;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
        {
            return false;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return greaterThanNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else if (theRHS.getType() == eTypeNodeSet)
        {
            return lessThanNodeSet(theRHS, *this, theLHSType, executionContext);
        }
        else
        {
            return DoubleSupport::greaterThan(num(), theRHS.num());
        }
    }
}



bool
XObject::greaterThanOrEquals(
            const XObject&          theRHS,
            XPathExecutionContext&  executionContext) const
{
    if (this == &theRHS)
    {
        return false;
    }
    else
    {
        const eObjectType   theLHSType = getType();

        if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
        {
            return false;
        }
        else if (theLHSType == eTypeNodeSet)
        {
            return greaterThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
        }
        else if (theRHS.getType() == eTypeNodeSet)
        {
            return lessThanOrEqualNodeSet(theRHS, *this, theLHSType, executionContext);
        }
        else
        {
            return DoubleSupport::greaterThanOrEqual(num(), theRHS.num());
        }
    }
}


const XalanDOMChar  XObject::XObjectException::m_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_j,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};


XObject::XObjectException::XObjectException(
                const XalanDOMString&   message,
                MemoryManagerType&      theManager) :
    XalanXPathException(message, theManager)
{
}
XObject::XObjectException::XObjectException( const XObjectException& other):
    XalanXPathException(other)
{
}


XObject::XObjectException::~XObjectException()
{
}

const XalanDOMChar  XObject::XObjectInvalidConversionException::m_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_b,
    XalanUnicode::charLetter_j,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};

XObject::XObjectInvalidConversionException::XObjectInvalidConversionException(
                                eObjectType fromType,
                                eObjectType toType):
            XObjectException(), 
            m_from(fromType),
            m_to(toType)
{
}

XObject::XObjectInvalidConversionException::XObjectInvalidConversionException( 
    const XObjectInvalidConversionException& other) :
    XObjectException(other),
     m_from(other.m_from),
     m_to(other.m_to)
{
}




XObject::XObjectInvalidConversionException::~XObjectInvalidConversionException()
{
}



const XalanDOMString&
XObject::XObjectInvalidConversionException::formatErrorString(
                const XalanDOMString&   fromType,
                const XalanDOMString&   toType,
                XalanDOMString&         theResult)
{

    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::CannotConvertTypetoType_2Param,
                fromType,
                toType);

}



XALAN_CPP_NAMESPACE_END
