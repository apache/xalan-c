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
#include "XSpan.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XSpan::XSpan(BorrowReturnMutableNodeRefList&	value) :
	XNodeSet(value),
	m_start(-1),
	m_end(-1)
{
}



XSpan::XSpan(const XSpan&	source) :
	XNodeSet(source),
	m_start(source.m_start),
	m_end(source.m_end)
{
}



XSpan::~XSpan()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XSpan*
#endif
XSpan::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XSpan(*this);
	}
	else
	{
		return new (theAddress) XSpan(*this);
	}
}



int
XSpan::getStart() const
{
	return m_start;
}



void
XSpan::setStart(int 	start)
{
	m_start = start;
}


int
XSpan::getEnd() const
{
	return m_end;
}



void
XSpan::setEnd(int	end)
{
	m_end = end;
}



void
XSpan::setSpan(
			int 	firstNodeOffset,
			int 	lastNodeOffset)
{
	m_start = firstNodeOffset;
	m_end = lastNodeOffset;
}



XALAN_CPP_NAMESPACE_END
