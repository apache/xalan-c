/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "XPathEnvSupportDefault.hpp"



#include <algorithm>
#include <iostream>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include "QName.hpp"
#include "SimpleNodeLocator.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPathExecutionContext.hpp"



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif
		


XPathEnvSupportDefault::XPathEnvSupportDefault() :
	XPathEnvSupport(),
	m_sourceDocs()
{
}



XPathEnvSupportDefault::~XPathEnvSupportDefault()
{
}



void
XPathEnvSupportDefault::reset()
{
	m_sourceDocs.clear();
}



const NodeRefListBase*
XPathEnvSupportDefault::getNodeSetByKey(
			const XalanNode&		/* doc */,
			const XalanDOMString&	/* name */,
			const XalanDOMString&	/* ref */,
			const PrefixResolver&	/* resolver */,
			XPathExecutionContext&	/* executionContext */) const
{
	return 0;
}



XObject*
XPathEnvSupportDefault::getVariable(
			XObjectFactory&		xobjectFactory,
			const QName&		name) const

{
	return xobjectFactory.createUnknown(name.getLocalPart());
}



XalanDocument*
XPathEnvSupportDefault::parseXML(
			const XalanDOMString&	/* urlString */,
			const XalanDOMString&	/* base */)
{
	return 0;
}



XalanDocument*
XPathEnvSupportDefault::getSourceDocument(const XalanDOMString&		theURI) const
{
	const SourceDocsTableType::const_iterator	i =
			m_sourceDocs.find(theURI);

	if (i == m_sourceDocs.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}



void
XPathEnvSupportDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_sourceDocs[theURI] = theDocument;
}



XalanDOMString
XPathEnvSupportDefault::findURIFromDoc(const XalanDocument*		owner) const
{
	SourceDocsTableType::const_iterator	i =
			m_sourceDocs.begin();

	bool	fFound = false;

	while(i != m_sourceDocs.end() && fFound == false)
	{
		if (i->second == owner)
		{
			fFound = true;
		}
		else
		{
			++i;
		}
	}

	return fFound == false ? XalanDOMString() : i->first;
}



XalanDocument*
XPathEnvSupportDefault::getDOMFactory() const
{
	return 0;
}



bool
XPathEnvSupportDefault::elementAvailable(
			const XalanDOMString&	/* theNamespace */,
			const XalanDOMString&	/* extensionName */) const
{
	return false;
}



bool
XPathEnvSupportDefault::functionAvailable(
			const XalanDOMString&	/* theNamespace */,
			const XalanDOMString&	/* extensionName */) const
{
	return false;
}



XObject*
XPathEnvSupportDefault::extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			/* theNamespace */,
			const XalanDOMString&			/* extensionName */,
			const XObjectArgVectorType&		/* argVec */) const
{
	return executionContext.getXObjectFactory().createNull();
}



XLocator*
XPathEnvSupportDefault::getXLocatorFromNode(const XalanNode*	/* node */) const
{
	return SimpleNodeLocator::getDefaultInstance();
}



void
XPathEnvSupportDefault::associateXLocatorToNode(
			const XalanNode*	/* node */,
			XLocator*			/* xlocator */)
{
}



bool
XPathEnvSupportDefault::shouldStripSourceNode(const XalanNode&	/* node */) const
{
	return false;
}



bool
XPathEnvSupportDefault::problem(
			eSource					/* where */,
			eClassification			classification,
			const XalanNode*		/* styleNode */,
			const XalanNode*		/* sourceNode */,
			const XalanDOMString&	msg,
			int						lineNo,
			int						charOffset) const
{
	cerr << msg
		 << ", at line number "
		 << static_cast<long>(lineNo)
		 << " at offset "
		 << static_cast<long>(charOffset)
		 << endl;

	return classification == XPathEnvSupport::eError ? true : false;
}



bool
XPathEnvSupportDefault::problem(
			eSource					/* where */,
			eClassification			classification,
			const PrefixResolver*	/* resolver */,
			const XalanNode*		/* sourceNode */,
			const XalanDOMString&	msg,
			int						lineNo,
			int						charOffset) const
{
	cerr << msg
		 << ", at line number "
		 << static_cast<long>(lineNo)
		 << " at offset "
		 << static_cast<long>(charOffset)
		 << endl;

	return classification == XPathEnvSupport::eError ? true :false;
}
