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
#include "ElemLiteralResult.hpp"



#include <algorithm>



#include <sax/AttributeList.hpp>
#include <sax/SAXException.hpp>



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemLiteralResult::ElemLiteralResult(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString&			name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ElemUse(constructionContext,
			stylesheetTree,
			name,
			lineNumber,
			columnNumber,
			xslToken),
	m_avts(),
	m_namespacesHandler(stylesheetTree.getNamespacesHandler(),
						stylesheetTree.getNamespaces(),
						stylesheetTree.getXSLTNamespaceURI())
{
	const unsigned int	nAttrs = atts.getLength();

	m_avts.reserve(nAttrs);

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		bool						needToProcess = true;
		const unsigned int			indexOfNSSep = indexOf(aname, XalanUnicode::charColon);

		XalanDOMString				prefix;

		if(indexOfNSSep < length(aname))
		{
			prefix = substring(aname, 0, indexOfNSSep);

			if(!equals(prefix, DOMServices::s_XMLNamespace))
			{
				const XalanDOMString	ns = getNamespaceForPrefix(prefix);

				if(equals(ns, stylesheetTree.getXSLTNamespaceURI()))
				{
					const XalanDOMString localName = substring(aname,indexOfNSSep + 1);

					if(processPrefixControl(constructionContext, stylesheetTree, localName, atts.getValue(i)) == true)
					{
						needToProcess = false;
					}
					else if (equals(localName, Constants::ATTRNAME_VERSION) == true)
					{
						const XalanDOMChar*	const	value = atts.getValue(i);

						stylesheetTree.setXSLTVerDeclared(DoubleSupport::toDouble(value));
					}
				}
			}
			else
			{
				// don't process namespace decls
				needToProcess = false;
			}
		}

		if(needToProcess == true)
		{
			processSpaceAttr(aname, atts, i);

			// Add xmlns attribute(except xmlns:xsl), xml:space, etc... 
			// Ignore anything with xsl:xxx 
			if(! processUseAttributeSets(constructionContext, aname, atts, i) &&
					isAttrOK(aname, atts, i, constructionContext))
			{
				m_avts.push_back(new AVT(aname, atts.getType(i), atts.getValue(i), 	
							*this, constructionContext));
			}
		}
	}

	// Shrink the vector of AVTS, if necessary...
	if (m_avts.capacity() > m_avts.size())
	{
		// Make a copy that's the exact size, and
		// swap the two...
		AVTVectorType(m_avts).swap(m_avts);
	}
}



ElemLiteralResult::~ElemLiteralResult()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the vector.
	for_each(m_avts.begin(),
			 m_avts.end(),
			 DeleteFunctor<AVT>());
}



const NamespacesHandler&
ElemLiteralResult::getNamespacesHandler() const
{
	return m_namespacesHandler;
}



void
ElemLiteralResult::postConstruction(const NamespacesHandler&	theParentHandler)
{
	m_namespacesHandler.postConstruction(getElementName(), &theParentHandler);

	ElemUse::postConstruction(m_namespacesHandler);
}



// $$$ ToDo: Get rid of this when we get rid of Xerces' DOMString!!!
static const XalanDOMChar	theDummy = 0;



void
ElemLiteralResult::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	executionContext.startElement(toCharArray(getElementName()));

	ElemUse::execute(executionContext, sourceTree, sourceNode, mode);

	if(0 != m_avts.size())
	{
		const AVTVectorType::size_type	nAttrs = m_avts.size();

		for(AVTVectorType::size_type i = 0; i < nAttrs; i++)
		{
			const AVT* const	avt = m_avts[i];

			XalanDOMString		thePrefix;

			const XalanDOMString&	theName = avt->getName();

			if (startsWith(theName, DOMServices::s_XMLNamespaceWithSeparator) == true)
			{
				thePrefix = substring(theName, DOMServices::s_XMLNamespaceWithSeparatorLength);
			}

			XalanDOMString	theStringedValue;

			avt->evaluate(theStringedValue, sourceNode, *this, executionContext);

			if (isEmpty(thePrefix) == true ||
			    shouldExcludeResultNamespaceNode(
					thePrefix,
					theStringedValue) == false)
			{
				// $$$ ToDo: Get rid of theDummy when we get rid of Xerces' DOMString!!!
				executionContext.replacePendingAttribute(
						c_wstr(avt->getName()), 
						c_wstr(avt->getType()),
						length(theStringedValue) == 0 ? &theDummy : c_wstr(theStringedValue));
			}
		}
	}

	m_namespacesHandler.outputResultNamespaces(executionContext);

	executeChildren(executionContext, sourceTree, sourceNode, mode);

	executionContext.endElement(toCharArray(getElementName()));
}



bool
ElemLiteralResult::isAttrOK(
			int						tok,
			const XalanDOMChar*		attrName,
			const AttributeList&	atts,
			int						which) const
{
	return ElemUse::isAttrOK(tok, attrName, atts, which);
}



bool
ElemLiteralResult::isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			/* atts */,
			int								/* which */,
			StylesheetConstructionContext&	constructionContext) const
{
    bool	isAttrOK = equals(attrName, DOMServices::s_XMLNamespace) ||
					   startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator);

    if(isAttrOK == false)
    {
		const unsigned int	indexOfNSSep = indexOf(attrName, XalanUnicode::charColon);

		if(indexOfNSSep < length(attrName))
		{
			const XalanDOMString	prefix = substring(attrName, 0, indexOfNSSep);

			const XalanDOMString	ns = getStylesheet().getNamespaceForPrefixFromStack(prefix);

			if (equals(ns, constructionContext.getXSLTNamespaceURI()) == false)
			{
				isAttrOK = true;
			}
		}
		else
		{
			// An empty namespace is OK.
			isAttrOK = true;
		}
    }

    // TODO: Well, process it...
    return isAttrOK;
}



bool
ElemLiteralResult::processPrefixControl(
			StylesheetConstructionContext&	constructionContext,
			const Stylesheet&				stylesheetTree,
			const XalanDOMString&			localName,
			const XalanDOMString&			attrValue)
{
	if(equals(localName, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
	{
		m_namespacesHandler.processExtensionElementPrefixes(c_wstr(attrValue), stylesheetTree.getNamespaces(), constructionContext);

		return true;
	}
	else if (equals(localName, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
	{
		m_namespacesHandler.processExcludeResultPrefixes(c_wstr(attrValue), stylesheetTree.getNamespaces(), constructionContext);

		return true;
	}
	else
	{
		return false;
	}
}



bool
ElemLiteralResult::shouldExcludeResultNamespaceNode(
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theURI) const
{
	return m_namespacesHandler.shouldExcludeResultNamespaceNode(
				getStylesheet().getXSLTNamespaceURI(),
				thePrefix,
				theURI);
}
