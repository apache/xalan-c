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
#include "StdBinInputStream.hpp"



#include <cassert>

#if !defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream>
#endif



XALAN_CPP_NAMESPACE_BEGIN



StdBinInputStream::StdBinInputStream(StreamType&	theStream) :
	ParentType(),
	m_stream(theStream)
{
}



StdBinInputStream::~StdBinInputStream()
{
}



unsigned int
StdBinInputStream::curPos() const
{
	return m_stream.tellg();
}



unsigned int
StdBinInputStream::readBytes(
			XMLByte* const      toFill,
			const unsigned int	maxToRead)
{
	assert(sizeof(XMLByte) == sizeof(char));

	if (!m_stream)
	{
		return 0;
	}
	else
	{
#if !defined(XALAN_CLASSIC_IOSTREAMS)
#if defined(XALAN_OLD_STYLE_CASTS)
		m_stream.read((char*)toFill, maxToRead);
#else
		m_stream.read(reinterpret_cast<char*>(toFill), maxToRead);
#endif
#else
		m_stream.read(toFill, maxToRead);
#endif

		return m_stream.gcount();
	}
}



XALAN_CPP_NAMESPACE_END
