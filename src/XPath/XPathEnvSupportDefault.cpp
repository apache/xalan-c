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



XPathEnvSupportDefault::XPathEnvSupportDefault() :
	XPathEnvSupport(),
	m_extendedSupport(0),
	m_sourceDocs()
{
}



XPathEnvSupportDefault::~XPathEnvSupportDefault()
{
}



void
XPathEnvSupportDefault::reset()
{
	if (m_extendedSupport != 0)
	{
		m_extendedSupport->reset();
	}
}



const NodeRefListBase*
XPathEnvSupportDefault::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const DOM_Element&		nscontext,
			XPathExecutionContext&	executionContext) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getNodeSetByKey(doc,
												  name,
												  ref,
												  nscontext,
												  executionContext);
	}
	else
	{
		return 0;
	}
}



const NodeRefListBase*
XPathEnvSupportDefault::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getNodeSetByKey(doc,
												  name,
												  ref,
												  resolver,
												  executionContext);
	}
	else
	{
		return 0;
	}
}



XObject*
XPathEnvSupportDefault::getVariable(
			XObjectFactory&		xobjectFactory,
			const QName&		name) const

{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getVariable(xobjectFactory,
											  name);
	}
	else
	{
		return xobjectFactory.createUnknown(name.getLocalPart());
	}
}



DOM_Document
XPathEnvSupportDefault::parseXML(
			const DOMString&	/* urlString */,
			const DOMString&	/* base */) const
{
	return DOM_Document();
}



DOM_Document
XPathEnvSupportDefault::getSourceDocument(const DOMString&	theURI) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getSourceDocument(theURI);
	}
	else
	{
		const SourceDocsTableType::const_iterator	i =
			m_sourceDocs.find(theURI);

		if (i == m_sourceDocs.end())
		{
			return DOM_Document();
		}
		else
		{
			return i->second;
		}
	}
}



void
XPathEnvSupportDefault::setSourceDocument(
			const DOMString&		theURI,
			const DOM_Document&		theDocument)
{
	if (m_extendedSupport != 0)
	{
		m_extendedSupport->setSourceDocument(theURI, theDocument);
	}
	else
	{
		m_sourceDocs[theURI] = theDocument;
	}
}



DOMString
XPathEnvSupportDefault::findURIFromDoc(const DOM_Document&	owner) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->findURIFromDoc(owner);
	}
	else
	{
		SourceDocsTableType::const_iterator		i =
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

		return fFound == false ? "Unknown" : i->first;
	}
}



DOM_Document
XPathEnvSupportDefault::getDOMFactory() const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getDOMFactory();
	}
	else
	{
		return DOM_Document();
	}
}



bool
XPathEnvSupportDefault::functionAvailable(
			const DOMString&	theNamespace,
			const DOMString&	extensionName) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->functionAvailable(theNamespace,
													extensionName);
	}
	else
	{
		return false;
	}
}



XObject*
XPathEnvSupportDefault::extFunction(
			XPathExecutionContext&			executionContext,
			const DOMString&				theNamespace,
			const DOMString&				extensionName,
			const std::vector<XObject*>&	argVec) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->extFunction(executionContext,
											  theNamespace,
											  extensionName,
											  argVec);
	}
	else
	{
		return executionContext.getXObjectFactory().createNull();
	}
}



XLocator*
XPathEnvSupportDefault::getXLocatorFromNode(const DOM_Node&	 node) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->getXLocatorFromNode(node);
	}
	else
	{
		return SimpleNodeLocator::getDefaultInstance();
	}
}



void
XPathEnvSupportDefault::associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator) const
{
	if (m_extendedSupport != 0)
	{
		m_extendedSupport->associateXLocatorToNode(node,
												   xlocator);
	}
}



bool
XPathEnvSupportDefault::shouldStripSourceNode(const DOM_Node&	node) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->shouldStripSourceNode(node);
	}
	else
	{
		return false;
	}
}



bool
XPathEnvSupportDefault::problem(
			eSource				where,
			eClassification		classification,
			const DOM_Node&		styleNode,
			const DOM_Node&		sourceNode,
			const DOMString&	msg,
			int					lineNo,
			int					charOffset) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->problem(where,
										  classification,
										  styleNode,
										  sourceNode,
										  msg,
										  lineNo,
										  charOffset);
	}
	else
	{
		std::cerr << msg
				  << ", at line number "
				  << static_cast<long>(lineNo)
				  << " at offset "
				  << static_cast<long>(charOffset)
				  << std::endl;

		return classification == XPathEnvSupport::eError ? true : false;
	}
}



bool
XPathEnvSupportDefault::problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const DOM_Node&			sourceNode,
			const DOMString&		msg,
			int						lineNo,
			int						charOffset) const
{
	if (m_extendedSupport != 0)
	{
		return m_extendedSupport->problem(where,
										  classification,
										  resolver,
										  sourceNode,
										  msg,
										  lineNo,
										  charOffset);
	}
	else
	{
		return classification == XPathEnvSupport::eError ? true :false;
	}
}



XPathEnvSupport*
XPathEnvSupportDefault::GetExtendedEnvSupport() const
{
	return m_extendedSupport;
}



XPathEnvSupport*
XPathEnvSupportDefault::SetExtendedEnvSupport(XPathEnvSupport*	theExtendedSupport)
{
	XPathEnvSupport* const	theOldSupport = m_extendedSupport;

	m_extendedSupport = theExtendedSupport;

	return theOldSupport;
}
