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
			const DOM_Node&			theCurrentNode,
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



DOM_Node
XPathExecutionContextDefault::getCurrentNode() const
{
	return m_currentNode;
}



void
XPathExecutionContextDefault::setCurrentNode(const DOM_Node&	theCurrentNode)
{
	m_currentNode = theCurrentNode;
}



XObjectFactory&
XPathExecutionContextDefault::getXObjectFactory() const
{
	return m_xobjectFactory;
}



DOMString
XPathExecutionContextDefault::getNamespaceOfNode(const DOM_Node&	n) const
{
	return m_xpathSupport.getNamespaceOfNode(n);
}



DOMString
XPathExecutionContextDefault::getLocalNameOfNode(const DOM_Node&	n) const
{
	return m_xpathSupport.getLocalNameOfNode(n);
}



DOM_Node
XPathExecutionContextDefault::getParentOfNode(const DOM_Node&	n) const
{
	return m_xpathSupport.getParentOfNode(n);
}



DOMString
XPathExecutionContextDefault::getNodeData(const DOM_Node&	n) const
{
	return m_xpathSupport.getNodeData(n);
}



DOM_Element
XPathExecutionContextDefault::getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const
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
XPathExecutionContextDefault::getContextNodeListPosition(const DOM_Node&	contextNode) const
{
	assert(contextNode != 0);

	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	int pos = 0;

	const int	nNodes = m_contextNodeList.getLength();

	for(int i = 0; i < nNodes; i++)
	{
		if(m_contextNodeList.item(i) == contextNode)
		{
			pos = i + 1; // for 1-based XSL count.

			break;
		}
	}

	return pos;
}



bool
XPathExecutionContextDefault::functionAvailable(
			const DOMString&	theNamespace, 
			const DOMString&	extensionName) const
{
	return m_xpathEnvSupport.functionAvailable(theNamespace, extensionName);
}



XObject*
XPathExecutionContextDefault::extFunction(
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec)
{
	return m_xpathEnvSupport.extFunction(*this, theNamespace, extensionName, argVec);
}



XLocator*
XPathExecutionContextDefault::getXLocatorFromNode(const DOM_Node&		node) const
{
	return m_xpathEnvSupport.getXLocatorFromNode(node);
}



void
XPathExecutionContextDefault::associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator)
{
	m_xpathEnvSupport.associateXLocatorToNode(node, xlocator);
}



DOM_Document
XPathExecutionContextDefault::parseXML(
			const DOMString&	urlString,
			const DOMString&	base) const
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
			const DOM_Node&		doc,
			const DOMString&	name,
			const DOMString&	ref,
			const DOM_Element&	nscontext)
{
	return getNodeSetByKey(doc,
						   name,
						   ref,
						   ElementPrefixResolverProxy(nscontext, m_xpathEnvSupport, m_xpathSupport));
}



const NodeRefListBase*
XPathExecutionContextDefault::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref)
{
	assert(m_prefixResolver != 0);

	return getNodeSetByKey(doc,
						   name,
						   ref,
						   *m_prefixResolver);
}



const NodeRefListBase*
XPathExecutionContextDefault::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver)
{
	return m_xpathEnvSupport.getNodeSetByKey(doc,
											 name,
											 ref,
											 resolver,
											 *this);
}



XObject*
XPathExecutionContextDefault::getVariable(
			const QName&			name) const
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



DOMString
XPathExecutionContextDefault::getNamespaceForPrefix(const DOMString&	prefix) const
{
	assert(m_prefixResolver != 0);

	return m_prefixResolver->getNamespaceForPrefix(prefix);
}



DOMString
XPathExecutionContextDefault::findURIFromDoc(const DOM_Document&	owner) const
{
	return m_xpathEnvSupport.findURIFromDoc(owner);
}



DOMString
XPathExecutionContextDefault::getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const
{
	return m_xpathSupport.getUnparsedEntityURI(theName, theDocument);
}



bool
XPathExecutionContextDefault::shouldStripSourceNode(const DOM_Node&	node) const
{
	return m_xpathEnvSupport.shouldStripSourceNode(node);
}



void
XPathExecutionContextDefault::error(
			const DOMString&	msg,
			const DOM_Node&		sourceNode,
			const DOM_Node&		/* styleNode */) const
{
	DOMString			emsg;

	const DOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = DOMString("pattern = '") +
			  theCurrentPattern +
			  DOMString("'\n");
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
		throw XPathException(msg, DOM_Node());
	}
}



void
XPathExecutionContextDefault::warn(
			const DOMString&	msg,
			const DOM_Node&		sourceNode,
			const DOM_Node&		/* styleNode */) const
{
	DOMString	emsg;

	const DOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = DOMString("pattern = '") +
			  theCurrentPattern +
			  DOMString("'\n");
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
		throw XPathException(msg, DOM_Node());
	}
}



void
XPathExecutionContextDefault::message(
			const DOMString&	msg,
			const DOM_Node&		sourceNode,
			const DOM_Node&		/* styleNode */) const
{
	DOMString	emsg;

	const DOMString		theCurrentPattern(getCurrentPattern());

	if (length(theCurrentPattern) != 0)
	{
	   emsg = DOMString("pattern = '") +
			  theCurrentPattern +
			  DOMString("'\n");
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
		throw XPathException(msg, DOM_Node());
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
XPathExecutionContextDefault::setCurrentPattern(const DOMString&	thePattern)
{
	m_currentPattern = thePattern;
}


DOMString
XPathExecutionContextDefault::getCurrentPattern() const
{
	return m_currentPattern;
}



DOM_Document
XPathExecutionContextDefault::getSourceDocument(const DOMString&	theURI) const
{
	return m_xpathEnvSupport.getSourceDocument(theURI);
}



void
XPathExecutionContextDefault::setSourceDocument(
			const DOMString&		theURI,
			const DOM_Document&		theDocument)
{
	m_xpathEnvSupport.setSourceDocument(theURI, theDocument);
}
