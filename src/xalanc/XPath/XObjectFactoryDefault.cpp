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
// Class header file...
#include "XObjectFactoryDefault.hpp"



#include <algorithm>
#include <memory>



#include "XBoolean.hpp"
#include "XNodeSet.hpp"
#include "XNumber.hpp"
#include "XString.hpp"
#include "XStringAdapter.hpp"
#include "XStringCached.hpp"
#include "XStringReference.hpp"
#include "XUnknown.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XObjectFactoryDefault::XObjectFactoryDefault(
            MemoryManager&  theManager,
            size_type       theXStringBlockSize,
            size_type       theXNumberBlockSize,
            size_type       theXNodeSetBlockSize,
            size_type       theXNodeSetNodeProxyBlockSize) : 
    XObjectFactory(theManager),
    m_xstringAdapterAllocator(theManager, theXStringBlockSize),
    m_xstringAllocator(theManager, theXStringBlockSize),
    m_xstringCachedAllocator(theManager, theXStringBlockSize),
    m_xstringReferenceAllocator(theManager, theXStringBlockSize),
    m_xnumberAllocator(theManager, theXNumberBlockSize),
    m_xnodesetAllocator(theManager, theXNodeSetBlockSize),
    m_xnodesetNodeProxyAllocator(theManager, theXNodeSetNodeProxyBlockSize),
    m_xtokenNumberAdapterAllocator(theManager, theXNumberBlockSize),
    m_xtokenStringAdapterAllocator(theManager, theXStringBlockSize),
    m_xobjects(theManager),
    m_xnumberCache(theManager),
    m_xnodesetCache(theManager),
    m_xstringCache(theManager),
    m_xbooleanFalse(false, theManager),
    m_xbooleanTrue(true, theManager),
    m_references(theManager)
{
}



XObjectFactoryDefault*
XObjectFactoryDefault::create(
                              MemoryManager&    theManager,
                              size_type         theXStringBlockSize ,
                              size_type         theXNumberBlockSize ,
                              size_type         theXNodeSetBlockSize,
                              size_type         theXNodeSetNodeProxyBlockSize)
{
    typedef XObjectFactoryDefault Type;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(Type)));

    Type* theResult =
        new (theGuard.get()) Type(
                                theManager, 
                                theXStringBlockSize,
                                theXNumberBlockSize,
                                theXNodeSetBlockSize,
                                theXNodeSetNodeProxyBlockSize);

    theGuard.release();

    return theResult;
}



XObjectFactoryDefault::~XObjectFactoryDefault()
{
    reset();
}



bool
XObjectFactoryDefault::doReturnObject(
            XObject*    theXObject,
            bool        fInReset)
{
    assert(theXObject != 0);

    bool bStatus = false;   

    const XObject::eObjectType  theType = getRealType(*theXObject);

    switch(theType)
    {
    case XObject::eTypeBoolean:
    case XObject::eTypeNull:
        {       
            bStatus = true;
        }

    case XObject::eTypeStringAdapter:
        {
            XStringAdapter* const       theXStringAdapter =
                static_cast<XStringAdapter*>(theXObject);

            bStatus = m_xstringAdapterAllocator.destroy(theXStringAdapter);
        }
        break;

    case XObject::eTypeXTokenNumberAdapter:
        {
            XTokenNumberAdapter* const  theAdapter =
                static_cast<XTokenNumberAdapter*>(theXObject);

            bStatus = m_xtokenNumberAdapterAllocator.destroy(theAdapter);
        }
        break;

    case XObject::eTypeXTokenStringAdapter:
        {
            XTokenStringAdapter* const  theAdapter =
                static_cast<XTokenStringAdapter*>(theXObject);

            bStatus = m_xtokenStringAdapterAllocator.destroy(theAdapter);
        }
        break;

    case XObject::eTypeString:
        {
            XString* const  theXString =
                static_cast<XString*>(theXObject);

            if (m_xstringCache.size() < eXStringCacheMax)
            {
                m_xstringCache.push_back(theXString);

                bStatus = true;
            }
            else
            {
                bStatus = m_xstringAllocator.destroy(theXString);
            }
        }
        break;

    case XObject::eTypeStringCached:
        {
            XStringCached* const    theXStringCached =
                static_cast<XStringCached*>(theXObject);

            bStatus = m_xstringCachedAllocator.destroy(theXStringCached);
        }
        break;

    case XObject::eTypeStringReference:
        {
            XStringReference* const     theXStringReference =
                static_cast<XStringReference*>(theXObject);

            bStatus = m_xstringReferenceAllocator.destroy(theXStringReference);
        }
        break;

    case  XObject::eTypeNumber:
        {
            XNumber* const  theXNumber =
                static_cast<XNumber*>(theXObject);

            if (m_xnumberCache.size() < eXNumberCacheMax)
            {
                m_xnumberCache.push_back(theXNumber);

                bStatus = true;
            }
            else
            {
                bStatus = m_xnumberAllocator.destroy(theXNumber);
            }
        }
        break;

    case XObject::eTypeNodeSet:
        {
            XNodeSet* const     theXNodeSet =
                static_cast<XNodeSet*>(theXObject);

            if (m_xnodesetCache.size() < eXNodeSetCacheMax)
            {
                theXNodeSet->release();

                m_xnodesetCache.push_back(theXNodeSet);

                bStatus = true;
            }
            else
            {
                bStatus = m_xnodesetAllocator.destroy(theXNodeSet);
            }
        }
        break;

    case XObject::eTypeNodeSetNodeProxy:
        {
            XNodeSetNodeProxy* const    theXNodeSet =
                static_cast<XNodeSetNodeProxy*>(theXObject);

            bStatus = m_xnodesetNodeProxyAllocator.destroy(theXNodeSet);
        }
        break;

    default:
        {
            XALAN_USING_STD(find)

            const XObjectCollectionType::iterator   i =
                    find(m_xobjects.begin(), m_xobjects.end(), theXObject);

            if (i != m_xobjects.end())
            {
                if (fInReset == false)
                {
                    m_xobjects.erase(i);
                }

                deleteObject(theXObject);

                bStatus = true;
            }
        }
        break;
    }
    
    return bStatus;
}



const XObjectPtr
XObjectFactoryDefault::createBoolean(bool   theValue)
{
    if (theValue == true)
    {
        return XObjectPtr(&m_xbooleanTrue);
    }
    else
    {
        return XObjectPtr(&m_xbooleanFalse);
    }
}



const XObjectPtr
XObjectFactoryDefault::createUnknown(const XalanDOMString&  theValue)
{
    XUnknown* const theXUnknown = XUnknown::create(theValue, getMemoryManager());

    m_xobjects.push_back(theXUnknown);

    theXUnknown->setFactory(this);

    return XObjectPtr(theXUnknown);
}



const XObjectPtr
XObjectFactoryDefault::createNumber(double  theValue)
{
    if (m_xnumberCache.empty() == false)
    {
        XNumber* const  theXNumber = m_xnumberCache.back();

        m_xnumberCache.pop_back();

        theXNumber->set(theValue);

        return XObjectPtr(theXNumber);
    }
    else
    {
        m_xnumberCache.reserve(eXNumberCacheMax);

        XObject* const  theXObject = m_xnumberAllocator.createNumber(theValue);

        theXObject->setFactory(this);

        return XObjectPtr(theXObject);
    }
}



const XObjectPtr
XObjectFactoryDefault::createNumber(const XToken&   theValue)
{
    XObject*    theXObject = m_xtokenNumberAdapterAllocator.create(theValue);

    theXObject->setFactory(this);

    return XObjectPtr(theXObject);
}



const XObjectPtr
XObjectFactoryDefault::createNodeSet(BorrowReturnMutableNodeRefList&    theValue)
{
    if (m_xnodesetCache.empty() == false)
    {
        XNodeSet* const     theXObject = m_xnodesetCache.back();

        m_xnodesetCache.pop_back();

        theXObject->set(theValue);

        return XObjectPtr(theXObject);
    }
    else
    {
        m_xnodesetCache.reserve(eXNodeSetCacheMax);

        XNodeSet* const     theXObject = m_xnodesetAllocator.createNodeSet(theValue);

        theXObject->setFactory(this);

        return XObjectPtr(theXObject);
    }
}



const XObjectPtr
XObjectFactoryDefault::createNodeSet(XalanNode*     theValue)
{
    XNodeSetNodeProxy* const    theNodeSet =
        m_xnodesetNodeProxyAllocator.create(theValue);

    theNodeSet->setFactory(this);

    return XObjectPtr(theNodeSet);
}



const XObjectPtr
XObjectFactoryDefault::createString(const XalanDOMString&   theValue)
{
    if (m_xstringCache.empty() == false)
    {
        XString* const  theXString = m_xstringCache.back();

        m_xstringCache.pop_back();

        theXString->set(theValue);

        return XObjectPtr(theXString);
    }
    else
    {
        m_xstringCache.reserve(eXStringCacheMax);

        XString* const  theXString = m_xstringAllocator.createString(theValue);

        theXString->setFactory(this);

        return XObjectPtr(theXString);
    }
}



const XObjectPtr
XObjectFactoryDefault::createString(const XalanDOMChar*     theValue)
{
    XString* const  theXString = m_xstringAllocator.createString(theValue);

    theXString->setFactory(this);

    return XObjectPtr(theXString);
}



const XObjectPtr
XObjectFactoryDefault::createString(
            const XalanDOMChar*     theValue,
            XalanSize_t             theLength)
{
    XString* const  theXString = m_xstringAllocator.createString(theValue, theLength);

    theXString->setFactory(this);

    return XObjectPtr(theXString);
}



const XObjectPtr
XObjectFactoryDefault::createString(const XToken&   theValue)
{
    XObject*    theXObject = m_xtokenStringAdapterAllocator.create(theValue);

    theXObject->setFactory(this);

    return XObjectPtr(theXObject);
}



const XObjectPtr
XObjectFactoryDefault::createStringReference(const XalanDOMString&  theValue)
{
    XStringReference* const theXStringReference = m_xstringReferenceAllocator.createString(theValue);

    theXStringReference->setFactory(this);

    return XObjectPtr(theXStringReference);
}



const XObjectPtr
XObjectFactoryDefault::createStringAdapter(
            const XObjectPtr&       theValue,
            XPathExecutionContext&  theExecutionContext)
{
    XStringAdapter* const   theXStringAdapter =
        m_xstringAdapterAllocator.createString(theValue, theExecutionContext);

    theXStringAdapter->setFactory(this);

    return XObjectPtr(theXStringAdapter);
}



const XObjectPtr
XObjectFactoryDefault::createString(GetCachedString&    theValue)
{
    XStringCached* const    theXStringCached = m_xstringCachedAllocator.createString(theValue);

    theXStringCached->setFactory(this);

    return XObjectPtr(theXStringCached);
}



void
XObjectFactoryDefault::holdReference(XObjectPtr    theValue)
{
    m_references.push_back(theValue);
}



void
XObjectFactoryDefault::reset()
{
    m_references.clear();

    m_xstringAdapterAllocator.reset();

    m_xstringAllocator.reset();

    m_xstringCachedAllocator.reset();

    m_xstringReferenceAllocator.reset();

    m_xnumberAllocator.reset();

    m_xnodesetAllocator.reset();

    m_xnodesetNodeProxyAllocator.reset();

    m_xtokenNumberAdapterAllocator.reset();

    m_xtokenStringAdapterAllocator.reset();

#if !defined(XALAN_NO_STD_NAMESPACE)
    using std::for_each;
#endif

    for_each(m_xobjects.begin(),
             m_xobjects.end(),
             DeleteXObjectFunctor(*this, true));

    m_xobjects.clear();

    m_xnumberCache.clear();

    m_xnodesetCache.clear();

    m_xstringCache.clear();
}



XALAN_CPP_NAMESPACE_END
