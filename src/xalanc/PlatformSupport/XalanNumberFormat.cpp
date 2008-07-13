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
#include "XalanNumberFormat.hpp"



#include <xalanc/Include/XalanMemMngArrayAllocate.hpp>


#include <xalanc/Include/STLHelper.hpp>



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	XalanNumberFormat::s_defaultGroupingSeparator[] = 
{
	XalanUnicode::charComma,
	0
};



XalanNumberFormat::XalanNumberFormat(MemoryManagerType&     theManager) :
	m_isGroupingUsed(false),
	m_groupingSeparator(s_defaultGroupingSeparator, theManager),
	m_groupingSize(3)	// Default to US values
{
}



XalanNumberFormat::~XalanNumberFormat()
{
}



XalanDOMString&
XalanNumberFormat::format(
			double				theValue,
			XalanDOMString&		theResult)
{
	DoubleToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

    return theResult;
}




XalanDOMString&
XalanNumberFormat::format(
			int					theValue,
			XalanDOMString&		theResult)
{
	LongToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

	return theResult;
}




XalanDOMString&
XalanNumberFormat::format(
			unsigned int		theValue,
			XalanDOMString&		theResult)
{
	UnsignedLongToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

	return theResult;
}





XalanDOMString&
XalanNumberFormat::format(
			long				theValue,
			XalanDOMString&		theResult)
{
	LongToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

	return theResult;
}





XalanDOMString&
XalanNumberFormat::format(
			unsigned long		theValue,
			XalanDOMString&		theResult)
{
	UnsignedLongToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

    return theResult;
}



XalanDOMString&
XalanNumberFormat::format(
			XALAN_INT64	        theValue,
			XalanDOMString&		theResult)
{
	NumberToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

    return theResult;
}



XalanDOMString&
XalanNumberFormat::format(
			XALAN_UINT64	    theValue,
			XalanDOMString&		theResult)
{
	NumberToDOMString(theValue, theResult);

	applyGrouping(theResult, theResult);

    return theResult;
}



/*
 * Convert a string value using the currently active values for grouping size
 * and separator; returns the converted string
 */
void
XalanNumberFormat::applyGrouping(
			const XalanDOMString&	value,
			XalanDOMString&			result)
{
	if (m_isGroupingUsed == false ||
		m_groupingSize == 0)
	{
		result = value;
	}
	else
	{
		const XalanDOMString::size_type		len = length(value);

		if (len == 0)
		{
			result = value;
		}
		else
		{
			// Add two, so we leave one byte at the beginning as empty space
			const XalanDOMString::size_type		bufsize = len + len / m_groupingSize + 2;

            typedef XalanMemMngArrayAllocate<XalanDOMChar> XalanDOMCharHeapAllocator;

			XalanDOMChar* const		buffer = XalanDOMCharHeapAllocator::allocate( bufsize, 
                                                                                 result.getMemoryManager());

			XalanMemMgrAutoPtrArray<XalanDOMChar>		theGuard(result.getMemoryManager(),
                                                                  buffer,
                                                                  bufsize);

			XalanDOMChar*			p = buffer + bufsize - 1;

			*p-- = 0;	// null terminate

			for (XalanDOMString::size_type i = 0, ix = len - 1; i < len && p > buffer; i++, ix--)
			{
				const XalanDOMChar		c = charAt(value, ix);

				if (i && !(i% m_groupingSize))
				{
					// Could be a multiple character separator??
					for (long j = long(m_groupingSeparator.length() - 1); j >= 0 && p > buffer; j--)
						*p-- = charAt(m_groupingSeparator, j);
				}

				*p-- = c;
			}

			result = ++p;
		}
	}
}



bool
XalanNumberFormat::isGroupingUsed() const
{
	return m_isGroupingUsed;
}



void
XalanNumberFormat::setGroupingUsed(bool bUsed)
{
	m_isGroupingUsed = bUsed;
}



void
XalanNumberFormat::setGroupingSize(unsigned long	size)
{
	assert(size > 0);

	m_groupingSize = size;
}



void
XalanNumberFormat::setGroupingSeparator(const XalanDOMString&	s)
{
	m_groupingSeparator = s;
}



XALAN_CPP_NAMESPACE_END
