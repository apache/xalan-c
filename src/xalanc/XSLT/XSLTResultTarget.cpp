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

// Base include file.  Must be first.
#include "XSLTResultTarget.hpp"



#include <cassert>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <ostream>
#endif



XALAN_CPP_NAMESPACE_BEGIN



XSLTResultTarget::XSLTResultTarget() :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const XalanDOMString&	fileName) :
	m_fileName(fileName),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const XalanDOMChar*	fileName) :
	m_fileName(fileName),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(const char*	fileName) :
	m_fileName(fileName),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(StreamType*	theStream) :
	m_fileName(),
	m_byteStream(theStream),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
	assert(theStream != 0);
}



XSLTResultTarget::XSLTResultTarget(StreamType&	theStream) :
	m_fileName(),
	m_byteStream(&theStream),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(0)
{
}



XSLTResultTarget::XSLTResultTarget(Writer*	characterStream) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(characterStream),
	m_formatterListener(0),
	m_stream(0)
{
	assert(characterStream != 0);
}



XSLTResultTarget::XSLTResultTarget(FILE*	stream) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(0),
	m_stream(stream)
{
	assert(stream != 0);
}



XSLTResultTarget::XSLTResultTarget(FormatterListener&	flistener) :
	m_fileName(),
	m_byteStream(0),
	m_encoding(),
	m_characterStream(0),
	m_formatterListener(&flistener),
	m_stream(0)
{
}



XSLTResultTarget::~XSLTResultTarget()
{
}



XALAN_CPP_NAMESPACE_END
