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

#if !defined(XALAN_XSLTINPUTSOURCE_HEADER_GUARD)
#define XALAN_XSLTINPUTSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iosfwd>
#endif



#include "xercesc/sax/InputSource.hpp"



#include "xalanc/Include/XalanMemoryManagement.hpp"



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER BinInputStream	BinInputStreamType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER InputSource		InputSourceType;
XALAN_USING_XERCES(MemoryManager)


class XalanDOMString;
class XalanNode;



class XALAN_XSLT_EXPORT XSLTInputSource : public InputSourceType
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef istream			StreamType;
#else
	typedef std::istream	StreamType;
#endif

	explicit
    XSLTInputSource(MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Copy constructor.
	 */
	XSLTInputSource(
            const XSLTInputSource&  theSource,
            MemoryManager&          theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Assignment operator.
	 */
	XSLTInputSource&
	operator=(const XSLTInputSource&	theRHS);

	/**
	 * Create a new input source with a system identifier.
	 *
	 * <p>Applications may use setPublicId to include a public identifier as
	 * well, or setEncoding to specify the character encoding, if known.</p>
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId  system identifier (URI)
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            const XMLCh*    systemId,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a system identifier.
	 *
	 * <p>Applications may use setPublicId to include a public identifier as
	 * well, or setEncoding to specify the character encoding, if known.</p>
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId  system identifier (URI)
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            const XalanDOMString&	systemId,
            MemoryManager&          theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a system identifier.
	 *
	 * <p>Applications may use setPublicId to include a public identifier as
	 * well, or setEncoding to specify the character encoding, if known.</p>
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId  system identifier (URI)
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            const char*		systemId,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a system identifier and a public
	 * identifier.
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId system identifier (URI)
	 * @param publicId public identifier
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
			const char*		systemId,
			const char*		publicId,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a system identifier and a public
	 * identifier.
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId system identifier (URI)
	 * @param publicId public identifier
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
			const XMLCh*	systemId,
			const XMLCh*	publicId,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a system identifier and a public
	 * identifier.
	 *
	 * <p>If the system identifier is a URL, it must be full resolved.</p>
	 *
	 * @param systemId system identifier (URI)
	 * @param publicId public identifier
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
			const XalanDOMString&	systemId,
			const XalanDOMString&	publicId,
            MemoryManager&          theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with a DOM node.
	 *
	 * <p>Application writers may use setSystemId() to provide a base for
	 * resolving relative URIs, and setPublicId to include a public
	 * identifier.</p>
	 *
	 * @param node DOM node that is root of the document
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            XalanNode*	    node,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with std stream.
	 *
	 * <p>Application writers may use setSystemId() to provide a base for
	 * resolving relative URIs, and setPublicId to include a public
	 * identifier.</p>
	 *
	 * @param stream the input stream...
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            StreamType*		stream,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Create a new input source with std stream.
	 *
	 * <p>Application writers may use setSystemId() to provide a base for
	 * resolving relative URIs, and setPublicId to include a public
	 * identifier.</p>
	 *
	 * @param stream the input stream...
	 * @param theMemoryManager  The MemoryManager instance to use.
	 */
	XSLTInputSource(
            StreamType&     stream,
            MemoryManager&  theMemoryManager = XalanMemMgrs::getDefault());

	/**
	 * Makes the byte stream for this input source.
	 *
	 * <p>The SAX parser will ignore this if there is also a character
	 * stream specified, but it will use a byte stream in preference
	 * to opening a URI connection itself.</p>
	 *
	 *
	 * @return pointer to byte stream created
	 */
	virtual BinInputStreamType*
	makeStream() const;

	/**
	 * Set the DOM node for this input source.
	 *
	 * @param node DOM node that is root of the document
	 */
	void
	setNode(XalanNode*	node)
	{
		m_node = node;
	}

	/**
	 * Retrieve the DOM node for this input source.
	 *
	 * @return DOM node that is root of the document
	 */
	XalanNode*
	getNode() const
	{
		return m_node;
	}

	StreamType*
	getStream() const
	{
		return m_stream;
	}

	void
	setStream(StreamType*	stream)
	{
		m_stream = stream;
	}

private:

	StreamType*		m_stream;

	XalanNode*		m_node;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_XSLTINPUTSOURCE_HEADER_GUARD
