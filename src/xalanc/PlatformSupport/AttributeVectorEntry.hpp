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
#if !defined(ATTRIBUTEVECTORENTRY_HEADER_GUARD_1357924680)
#define ATTRIBUTEVECTORENTRY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/Include/XalanVector.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT AttributeVectorEntry
{
public:

	typedef XalanVector<XMLCh>		XMLChVectorType;

	AttributeVectorEntry(
			const XMLChVectorType&	theName,
			const XMLChVectorType&	theValue,
			const XMLChVectorType&	theType,
            MemoryManagerType&      theManager) :
		m_Name(theName,theManager),
		m_Value(theValue,theManager),
		m_Type(theType,theManager)
	{
	}

	AttributeVectorEntry(
			const XMLCh*	        theName,
			const XMLCh*	        theValue,
			const XMLCh*	        theType,
            MemoryManagerType&      theManager) :
		m_Name(theName, theName + length(theName) + 1, theManager),
		m_Value(theValue, theValue + length(theValue) + 1, theManager),
		m_Type(theType, theType + length(theType) + 1, theManager)
	{
	}

	AttributeVectorEntry(MemoryManagerType&      theManager) :
		m_Name(theManager),
		m_Value(theManager),
		m_Type(theManager)
	{
	}

    static AttributeVectorEntry*
    create( const XMLCh*	theName,
			const XMLCh*	theValue,
			const XMLCh*	theType,
            MemoryManagerType&      theManager)
    {
        typedef AttributeVectorEntry ThisType;
        
        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theName, theValue, theType, theManager);

        theGuard.release();

        return theResult;
    }

    virtual
	~AttributeVectorEntry()
	{
	}

	void
	clear()
	{
		m_Name.clear();
		m_Value.clear();
		m_Type.clear();
	}

	XMLChVectorType		m_Name;
	XMLChVectorType		m_Value;
	XMLChVectorType		m_Type;

	// A convenience function to find the length of a null-terminated
	// array of XMLChs
	static const XMLCh*
	endArray(const XMLCh*	data)
	{
		assert(data != 0);

		while(*data)
		{
			++data;
		}

		return data;
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// ATTRIBUTEVECTORENTRY_HEADER_GUARD_1357924680
