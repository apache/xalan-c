/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file...
#include "XPathExecutionContextDefault.hpp"



#include <xercesc/sax/Locator.hpp>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/PrefixResolver.hpp>



#include <DOMSupport/DOMSupport.hpp>



#include "FoundIndex.hpp"
#include "XObjectFactory.hpp"
#include "XalanQName.hpp"
#include "XPathEnvSupport.hpp"



const NodeRefList	XPathExecutionContextDefault::s_dummyList;



XPathExecutionContextDefault::XPathExecutionContextDefault(
			XPathEnvSupport&		theXPathEnvSupport,
			DOMSupport&				theDOMSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode,
			const NodeRefListBase*	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	XPathExecutionContext(),
	m_xpathEnvSupport(&theXPathEnvSupport),
	m_domSupport(&theDOMSupport),
	m_xobjectFactory(&theXObjectFactory),
	m_currentNode(theCurrentNode),
	m_contextNodeList(theContextNodeList == 0 ? &s_dummyList : theContextNodeList),
	m_prefixResolver(thePrefixResolver),
	m_throwFoundIndex(false),
	m_nodeListCache(eNodeListCacheListSize),
	m_stringCache()
{
}



XPathExecutionContextDefault::XPathExecutionContextDefault(
			XalanNode*				theCurrentNode,
			const NodeRefListBase*	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	XPathExecutionContext(),
	m_xpathEnvSupport(0),
	m_domSupport(0),
	m_xobjectFactory(0),
	m_currentNode(theCurrentNode),
	m_contextNodeList(theContextNodeList == 0 ? &s_dummyList : theContextNodeList),
	m_prefixResolver(thePrefixResolver),
	m_throwFoundIndex(false),
	m_nodeListCache(eNodeListCacheListSize),
	m_stringCache()
{
}




XPathExecutionContextDefault::~XPathExecutionContextDefault()
{
	reset();
}



void
XPathExecutionContextDefault::reset()
{
	if (m_xpathEnvSupport != 0)
	{
		m_xpathEnvSupport->reset();
	}

	if (m_domSupport != 0)
	{
		m_domSupport->reset();
	}

	if (m_xobjectFactory != 0)
	{
		m_xobjectFactory->reset();
	}

	m_currentNode = 0;
	m_contextNodeList = &s_dummyList;
	m_prefixResolver = 0;
	m_throwFoundIndex = false;

	m_nodeListCache.reset(),

	m_stringCache.reset();
}



XalanNode*
XPathExecutionContextDefault::getCurrentNode() const
{
	return m_currentNode;
}



void
XPathExecutionContextDefault::setCurrentNode(XalanNode*		theCurrentNode)
{
	m_currentNode = theCurrentNode;
}



XObjectFactory&
XPathExecutionContextDefault::getXObjectFactory() const
{
	assert(m_xobjectFactory != 0);

	return *m_xobjectFactory;
}



XObjectPtr
XPathExecutionContextDefault::createNodeSet(XalanNode&	theNode)
{
	assert(m_xobjectFactory != 0);

	// This list will hold the node...
	BorrowReturnMutableNodeRefList	theNodeList(*this);

	theNodeList->addNode(&theNode);

	return m_xobjectFactory->createNodeSet(theNodeList);
}



bool
XPathExecutionContextDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return m_domSupport->isNodeAfter(node1, node2);
}



const NodeRefListBase&
XPathExecutionContextDefault::getContextNodeList() const
{
	return *m_contextNodeList;
}



void	
XPathExecutionContextDefault::setContextNodeList(const NodeRefListBase&	theList)
{
	m_contextNodeList = &theList;
}



XPathExecutionContextDefault::size_type
XPathExecutionContextDefault::getContextNodeListLength() const
{
	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	return m_contextNodeList->getLength();
}



XPathExecutionContextDefault::size_type
XPathExecutionContextDefault::getContextNodeListPosition(const XalanNode&	contextNode) const
{
	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	// Get the index of the node...
	const size_type		theIndex = m_contextNodeList->indexOf(&contextNode);

	// If not found return 0.  Otherwise, return the index + 1
	return theIndex == NodeRefListBase::npos ? 0 : theIndex + 1;
}



bool
XPathExecutionContextDefault::elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->elementAvailable(theNamespace, elementName);
}



bool
XPathExecutionContextDefault::functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->functionAvailable(theNamespace, functionName);
}



const XObjectPtr
XPathExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName, 
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const Locator*					locator)
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->extFunction(*this, theNamespace, functionName, context, argVec, locator);
}



XalanDocument*
XPathExecutionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->parseXML(urlString, base);
}



MutableNodeRefList*
XPathExecutionContextDefault::borrowMutableNodeRefList()
{
	return m_nodeListCache.get();
}



bool
XPathExecutionContextDefault::returnMutableNodeRefList(MutableNodeRefList*	theList)
{
	return m_nodeListCache.release(theList);
}



MutableNodeRefList*
XPathExecutionContextDefault::createMutableNodeRefList() const
{
	return new MutableNodeRefList;
}



XalanDOMString&
XPathExecutionContextDefault::getCachedString()
{
	return m_stringCache.get();
}



bool
XPathExecutionContextDefault::releaseCachedString(XalanDOMString&	theString)
{
	return m_stringCache.release(theString);
}



void
XPathExecutionContextDefault::getNodeSetByKey(
			XalanNode*				/* doc */,
			const XalanDOMString&	/* name */,
			const XalanDOMString&	/* ref */,
			const PrefixResolver&	/* resolver */,
			MutableNodeRefList&		/* nodelist */)
{
}



const XObjectPtr
XPathExecutionContextDefault::getVariable(
			const XalanQName&		name,
			const Locator*			/* locator */)
{
	assert(m_xobjectFactory != 0);

	return m_xobjectFactory->createUnknown(name.getLocalPart());
}



const PrefixResolver*
XPathExecutionContextDefault::getPrefixResolver() const
{
	return m_prefixResolver;
}



void
XPathExecutionContextDefault::setPrefixResolver(const PrefixResolver*	thePrefixResolver)
{
	m_prefixResolver = thePrefixResolver;
}



const XalanDOMString*
XPathExecutionContextDefault::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	assert(m_prefixResolver != 0);

	return m_prefixResolver->getNamespaceForPrefix(prefix);
}



XalanDOMString
XPathExecutionContextDefault::findURIFromDoc(const XalanDocument*	owner) const
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->findURIFromDoc(owner);
}



const XalanDOMString&
XPathExecutionContextDefault::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	return m_domSupport->getUnparsedEntityURI(theName, theDocument);
}



bool
XPathExecutionContextDefault::shouldStripSourceNode(const XalanNode&	/* node */)
{
	return false;
}



void
XPathExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	assert(m_xpathEnvSupport != 0);

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eError,
			m_prefixResolver, 
			sourceNode,
			msg,
			0,
			-1,
			-1) == true)
	{
		throw XalanXPathException(msg, 0);
	}
}



void
XPathExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const Locator*			locator) const
{
	assert(m_xpathEnvSupport != 0);

	int				lineNumber = -1;
	int				columnNumber = -1;

	XalanDOMString	uri;

	if (locator != 0)
	{
		lineNumber = locator->getLineNumber();
		columnNumber = locator->getColumnNumber();

		const XalanDOMChar*		id =
			locator->getPublicId();

		if (id != 0)
		{
			uri = id;
		}
		else
		{
			id = locator->getSystemId();

			if (id != 0)
			{
				uri = id;
			}
		}
	}

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eError,
			m_prefixResolver, 
			sourceNode,
			msg,
			c_wstr(uri),
			lineNumber,
			columnNumber) == true)
	{
		throw XalanXPathException(msg, uri, lineNumber, columnNumber);
	}
}



void
XPathExecutionContextDefault::error(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
XPathExecutionContextDefault::error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
XPathExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	assert(m_xpathEnvSupport != 0);

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eWarning,
			m_prefixResolver, 
			sourceNode,
			msg,
			0,
			-1,
			-1) == true)
	{
		throw XalanXPathException(msg, sourceNode);
	}
}



void
XPathExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const Locator* 			locator) const
{
	assert(m_xpathEnvSupport != 0);

	int					lineNumber = -1;
	int					columnNumber = -1;

	XalanDOMString	uri;

	if (locator != 0)
	{
		lineNumber = locator->getLineNumber();
		columnNumber = locator->getColumnNumber();

		const XalanDOMChar*		id =
			locator->getPublicId();

		if (id != 0)
		{
			uri = id;
		}
		else
		{
			id = locator->getSystemId();

			if (id != 0)
			{
				uri = id;
			}
		}
	}

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eWarning,
			m_prefixResolver, 
			sourceNode,
			msg,
			c_wstr(uri),
			lineNumber,
			columnNumber) == true)
	{
		throw XalanXPathException(msg, uri, lineNumber, columnNumber);
	}
}



void
XPathExecutionContextDefault::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
XPathExecutionContextDefault::warn(
			const char*			msg,
			const XalanNode*	sourceNode,
			const Locator* 		locator) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
XPathExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	assert(m_xpathEnvSupport != 0);

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eMessage,
			m_prefixResolver, 
			sourceNode,
			msg,
			0,
			-1,
			-1) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XalanXPathException(msg);
	}
}



void
XPathExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const Locator* 			locator) const
{
	assert(m_xpathEnvSupport != 0);

	int					lineNumber = -1;
	int					columnNumber = -1;

	XalanDOMString	uri;

	if (locator != 0)
	{
		lineNumber = locator->getLineNumber();
		columnNumber = locator->getColumnNumber();

		const XalanDOMChar*		id =
			locator->getPublicId();

		if (id != 0)
		{
			uri = id;
		}
		else
		{
			id = locator->getSystemId();

			if (id != 0)
			{
				uri = id;
			}
		}
	}

	if (m_xpathEnvSupport->problem(
			XPathEnvSupport::eXPATHProcessor, 
			XPathEnvSupport::eMessage,
			m_prefixResolver, 
			sourceNode,
			msg,
			c_wstr(uri),
			lineNumber,
			columnNumber) == true)
	{
		throw XalanXPathException(msg, uri, lineNumber, columnNumber);
	}
}



void
XPathExecutionContextDefault::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const XalanNode*	styleNode) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
XPathExecutionContextDefault::message(
			const char*			msg,
			const XalanNode*	sourceNode,
			const Locator* 		locator) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}

			
			
bool
XPathExecutionContextDefault::getThrowFoundIndex() const
{
	return m_throwFoundIndex;
}



void
XPathExecutionContextDefault::setThrowFoundIndex(bool 	fThrow)
{
	m_throwFoundIndex = fThrow;
}



XalanDocument*
XPathExecutionContextDefault::getSourceDocument(const XalanDOMString&	theURI) const
{
	assert(m_xpathEnvSupport != 0);

	return m_xpathEnvSupport->getSourceDocument(theURI);
}



void
XPathExecutionContextDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	assert(m_xpathEnvSupport != 0);

	m_xpathEnvSupport->setSourceDocument(theURI, theDocument);
}



const XalanDecimalFormatSymbols*
XPathExecutionContextDefault::getDecimalFormatSymbols(const XalanQName&		/* qname */)
{
	return 0;
}
