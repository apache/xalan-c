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

#include "XalanEncodingPropertyCache.hpp"
#include "XalanTranscodingServices.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanEncodingPropertyCache::XalanEncodingPropertyCache(
			size_t							theCacheSize,
			const XalanOutputTranscoder*	theTranscoder) :
	m_transcoder(theTranscoder),
	m_presentBitmap(theCacheSize),
	m_valueBitmap(theCacheSize)
{
}



XalanEncodingPropertyCache::~XalanEncodingPropertyCache()
{
}



bool
XalanEncodingPropertyCache::canEncodeCharacter(unsigned int 	theChar) const
{
	if (theChar > m_presentBitmap.getSize())
	{
		return m_transcoder == 0 ? true : m_transcoder->canTranscodeTo(theChar);
	}
	else if (m_presentBitmap.isSet(theChar) == true)
	{
		return m_valueBitmap.isSet(theChar);
	}
	else
	{
		if (m_transcoder == 0)
		{
			return true;
		}
		else
		{
#if defined(XALAN_NO_MUTABLE)
			((XalanEncodingPropertyCache*)this)->m_presentBitmap.set(theChar);
#else
			m_presentBitmap.set(theChar);
#endif

			const bool	fResult = m_transcoder->canTranscodeTo(theChar);

			if (fResult == true)
			{
#if defined(XALAN_NO_MUTABLE)
				((XalanEncodingPropertyCache*)this)->m_valueBitmap.set(theChar);
#else
				m_valueBitmap.set(theChar);
#endif
			}

			return fResult;
		}
	}
}



XALAN_CPP_NAMESPACE_END
