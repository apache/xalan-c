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

#include "StylesheetConstructionContextDefault.hpp"



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include "XSLTEngineImpl.hpp"



StylesheetConstructionContextDefault::StylesheetConstructionContextDefault(
			XSLTEngineImpl&		processor,
			XPathEnvSupport&	xpathEnvSupport,
			XObjectFactory&		xobjectFactory,
			XPathFactory&		xpathFactory) :
	StylesheetConstructionContext(),
	m_processor(processor),
	m_xpathEnvSupport(xpathEnvSupport),
	m_xobjectFactory(xobjectFactory),
	m_xpathFactory(xpathFactory),
	m_xpathProcessor(new XPathProcessorImpl)
{
}



StylesheetConstructionContextDefault::~StylesheetConstructionContextDefault()
{
}



void
StylesheetConstructionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_processor.error(msg, styleNode, sourceNode);
}



void
StylesheetConstructionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_processor.warn(msg, styleNode, sourceNode);
}



void
StylesheetConstructionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const XalanNode*		styleNode) const
{
	m_processor.message(msg, styleNode, sourceNode);
}



int
StylesheetConstructionContextDefault::getAttrTok(const XalanDOMString&	name) const
{
	return m_processor.getAttrTok(name);
}



XMLURL*
StylesheetConstructionContextDefault::getURLFromString(const XalanDOMString&	urlString)
{
	return m_processor.getURLFromString(urlString);
}



XMLURL*
StylesheetConstructionContextDefault::getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return m_processor.getURLFromString(urlString, base);
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXSLNameSpaceURLPre() const
{
	return m_processor.getXSLNameSpaceURLPre();
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXSLNameSpaceURL() const
{
	return m_processor.getXSLNameSpaceURL();
}



XPath*
StylesheetConstructionContextDefault::createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initMatchPattern(*xpath,
									   str,
									   resolver,
									   m_xobjectFactory,
									   m_xpathEnvSupport);

	return xpath;
}



XPath*
StylesheetConstructionContextDefault::createXPath(
			const XalanDOMString&		str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	m_xpathProcessor->initXPath(*xpath,
								str,
								resolver,
								m_xobjectFactory,
								m_xpathEnvSupport);

	return xpath;
}
