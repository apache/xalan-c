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



#include <vector>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT AttributeVectorEntry
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XMLCh>		XMLChVectorType;
#else
	typedef std::vector<XMLCh>	XMLChVectorType;
#endif

	AttributeVectorEntry(
			const XMLChVectorType&	theName,
			const XMLChVectorType&	theValue,
			const XMLChVectorType&	theType) :
		m_Name(theName),
		m_Value(theValue),
		m_Type(theType)
	{
	}

	AttributeVectorEntry(
			const XMLCh*	theName,
			const XMLCh*	theValue,
			const XMLCh*	theType) :
		m_Name(theName, theName + length(theName) + 1),
		m_Value(theValue, theValue + length(theValue) + 1),
		m_Type(theType, theType + length(theType) + 1)
	{
	}

	AttributeVectorEntry() :
		m_Name(),
		m_Value(),
		m_Type()
	{
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
