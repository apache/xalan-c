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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file...
#include "XPathExecutionContextDefault.hpp"



#include "ElementPrefixResolverProxy.hpp"
#include "FoundIndex.hpp"
#include "PrefixResolver.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



XPathExecutionContextDefault::XPathExecutionContextDefault(
			XPathEnvSupport&		theXPathEnvSupport,
			XPathSupport&			theXPathSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode,
			const NodeRefListBase&	theContextNodeList,
			const PrefixResolver*	thePrefixResolver) :
	XPathExecutionContext(),
	m_xpathEnvSupport(theXPathEnvSupport),
	m_xpathSupport(theXPathSupport),
	m_xobjectFactory(theXObjectFactory),
	m_currentNode(theCurrentNode),
	m_contextNodeList(theContextNodeList),
	m_prefixResolver(thePrefixResolver),
	m_throwFoundIndex(false)
{
}



XPathExecutionContextDefault::~XPathExecutionContextDefault()
{
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
	return m_xobjectFactory;
}



XalanDOMString
XPathExecutionContextDefault::getNamespaceOfNode(const XalanNode&	n) const
{
	return m_xpathSupport.getNamespaceOfNode(n);
}



XalanDOMString
XPathExecutionContextDefault::getLocalNameOfNode(const XalanNode&	n) const
{
	return m_xpathSupport.getLocalNameOfNode(n);
}



XalanNode*
XPathExecutionContextDefault::getParentOfNode(const XalanNode&	n) const
{
	return m_xpathSupport.getParentOfNode(n);
}



bool
XPathExecutionContextDefault::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const
{
	return m_xpathSupport.isNodeAfter(node1, node2);
}



XalanDOMString
XPathExecutionContextDefault::getNodeData(const XalanNode&	n) const
{
	return m_xpathSupport.getNodeData(n);
}



XalanElement*
XPathExecutionContextDefault::getElementByID(
			const XalanDOMString&	id,
			const XalanDocument&	doc) const
{
	return m_xpathSupport.getElementByID(id, doc);
}



const NodeRefListBase&
XPathExecutionContextDefault::getContextNodeList() const
{
	return m_contextNodeList;
}



void	
XPathExecutionContextDefault::setContextNodeList(const NodeRefListBase&	theList)
{
	m_contextNodeList = theList;
}



int
XPathExecutionContextDefault::getContextNodeListLength() const
{
	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	return m_contextNodeList.getLength();
}



int
XPathExecutionContextDefault::getContextNodeListPosition(const XalanNode&	contextNode) const
{
	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	int pos = 0;

	const int	nNodes = m_contextNodeList.getLength();

	for(int i = 0; i < nNodes; i++)
	{
		if(m_contextNodeList.item(i) == &contextNode)
		{
			pos = i + 1; // for 1-based XSL count.

			break;
		}
	}

	return pos;
}



bool
XPathExecutionContextDefault::elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const
{
	return m_xpathEnvSupport.elementAvailable(theNamespace, elementName);
}



bool
XPathExecutionContextDefault::functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const
{
	return m_xpathEnvSupport.functionAvailable(theNamespace, functionName);
}



XObject*
XPathExecutionContextDefault::extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName, 
			XalanNode*						context,
			const XObjectArgVectorType&		argVec)
{
	return m_xpathEnvSupport.extFunction(*this, theNamespace, functionName, context, argVec);
}



XLocator*
XPathExecutionContextDefault::getXLocatorFromNode(const XalanNode*	node) const
{
	return m_xpathEnvSupport.getXLocatorFromNode(node);
}



void
XPathExecutionContextDefault::associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator)
{
	m_xpathEnvSupport.associateXLocatorToNode(node, xlocator);
}



XalanDocument*
XPathExecutionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const
{
	return m_xpathEnvSupport.parseXML(urlString, base);
}



MutableNodeRefList
XPathExecutionContextDefault::createMutableNodeRefList() const
{
	return MutableNodeRefList(&m_xpathSupport);
}



bool
XPathExecutionContextDefault::getProcessNamespaces() const
{
	return m_xpathSupport.getProcessNamespaces();
}



const NodeRefListBase*
XPathExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const XalanElement&		nscontext)
{
	return getNodeSetByKey(doc,
						   name,
						   ref,
						   ElementPrefixResolverProxy(&nscontext, m_xpathEnvSupport, m_xpathSupport));
}



const NodeRefListBase*
XPathExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref)
{
	assert(m_prefixResolver != 0);

	return getNodeSetByKey(doc,
						   name,
						   ref,
						   *m_prefixResolver);
}



const NodeRefListBase*
XPathExecutionContextDefault::getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver)
{
	return m_xpathEnvSupport.getNodeSetByKey(doc,
											 name,
											 ref,
											 resolver,
											 *this);
}



XObject*
XPathExecutionContextDefault::getVariable(const QName&	name) const
{
	return m_xpathEnvSupport.getVariable(m_xobjectFactory, name); 
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



XalanDOMString
XPathExecutionContextDefault::getNamespaceForPrefix(const XalanDOMString&	prefix) const
{
	assert(m_prefixResolver != 0);

	return m_prefixResolver->getNamespaceForPrefix(prefix);
}



XalanDOMString
XPathExecutionContextDefault::findURIFromDoc(const XalanDocument*	owner) const
{
	return m_xpathEnvSupport.findURIFromDoc(owner);
}



XalanDOMString
XPathExecutionContextDefault::getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const
{
	return m_xpathSupport.getUnparsedEntityURI(theName, theDocument);
}



bool
XPathExecutionContextDefault::shouldStripSourceNode(const XalanNode&	node)
{
	return m_xpathEnvSupport.shouldStripSourceNode(*this, node);
}



void
XPathExecutionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	XalanDOMString			emsg;

	const XalanDOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = XalanDOMString("pattern = '") +
			  theCurrentPattern +
			  XalanDOMString("'\n");
	}

	emsg += msg;

	if (m_xpathEnvSupport.problem(XPathEnvSupport::eXPATHProcessor, 
								  XPathEnvSupport::eError,
								  m_prefixResolver, 
								  sourceNode,
								  emsg,
								  0,
								  0) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XPathException(emsg, 0);
	}
}



void
XPathExecutionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	XalanDOMString	emsg;

	const XalanDOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = XalanDOMString("pattern = '") +
			  theCurrentPattern +
			  XalanDOMString("'\n");
	}

	emsg += msg;

	if (m_xpathEnvSupport.problem(XPathEnvSupport::eXPATHProcessor, 
								  XPathEnvSupport::eWarning,
								  m_prefixResolver, 
								  sourceNode,
								  emsg,
								  0,
								  0) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XPathException(msg, 0);
	}
}



void
XPathExecutionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode,
			const XalanNode*		/* styleNode */) const
{
	XalanDOMString	emsg;

	const XalanDOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = XalanDOMString("pattern = '") +
			  theCurrentPattern +
			  XalanDOMString("'\n");
	}

	emsg += msg;

	if (m_xpathEnvSupport.problem(XPathEnvSupport::eXPATHProcessor, 
								  XPathEnvSupport::eMessage,
								  m_prefixResolver, 
								  sourceNode,
								  emsg,
								  0,
								  0) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XPathException(msg);
	}
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


void
XPathExecutionContextDefault::setCurrentPattern(const XalanDOMString&	thePattern)
{
	m_currentPattern = thePattern;
}


XalanDOMString
XPathExecutionContextDefault::getCurrentPattern() const
{
	return m_currentPattern;
}



XalanDocument*
XPathExecutionContextDefault::getSourceDocument(const XalanDOMString&	theURI) const
{
	return m_xpathEnvSupport.getSourceDocument(theURI);
}



void
XPathExecutionContextDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_xpathEnvSupport.setSourceDocument(theURI, theDocument);
}



const XalanDecimalFormatSymbols*
XPathExecutionContextDefault::getDecimalFormatSymbols(const XalanDOMString&		/* name */)
{
	return 0;
}
