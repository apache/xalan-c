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



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/StringTokenizer.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemLiteralResult::ElemLiteralResult(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			xslToken),
	m_elementName(name),
	m_avts()
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
				const XalanDOMString* const		ns =
						getNamespaceForPrefixInternal(prefix, true);

				if(ns != 0 && equals(*ns, stylesheetTree.getXSLTNamespaceURI()))
				{
					const XalanDOMString localName = substring(aname, indexOfNSSep + 1);

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



const XalanDOMString&
ElemLiteralResult::getElementName() const
{
	return m_elementName;
}



void
ElemLiteralResult::postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler)
{
	// OK, now check all attribute AVTs to make sure
	// our NamespacesHandler knows about any prefixes
	// that will need namespace declarations...
	const AVTVectorType::size_type	nAttrs = m_avts.size();

	for(AVTVectorType::size_type i = 0; i < nAttrs; ++i)
	{
		const AVT* const	avt = m_avts[i];

		const XalanDOMString&	theName = avt->getName();

		const unsigned int	theColonIndex = indexOf(theName, XalanUnicode::charColon);

		if (theColonIndex != length(theName))
		{
			m_namespacesHandler.addActivePrefix(substring(theName, 0, theColonIndex));
		}
	}

	// OK, now we can chain-up...
	ElemUse::postConstruction(constructionContext, theParentHandler);
}



inline void
ElemLiteralResult::doAddResultAttribute(
			StylesheetExecutionContext&		executionContext,
			const XalanDOMString&			thePrefix,
			const XalanDOMString&			theName,
			const XalanDOMString&			theValue) const
{
	if (isEmpty(thePrefix) == true ||
	    shouldExcludeResultNamespaceNode(
			thePrefix,
			theValue) == false)
	{
		executionContext.addResultAttribute(
				theName, 
				theValue);
	}
}



void
ElemLiteralResult::execute(StylesheetExecutionContext&		executionContext) const
{
	executionContext.startElement(c_wstr(getElementName()));

	ElemUse::execute(executionContext);

	m_namespacesHandler.outputResultNamespaces(executionContext);

	// OK, now let's check to make sure we don't have to change the default namespace...
	const XalanDOMString* const		theCurrentDefaultNamespace =
				executionContext.getResultNamespaceForPrefix(s_emptyString);

	if (theCurrentDefaultNamespace != 0)
	{
		const XalanDOMString* const		theElementDefaultNamespace =
					m_namespacesHandler.getNamespace(s_emptyString);

		if (theElementDefaultNamespace == 0)
		{
			// There was no default namespace, so we have to turn the
			// current one off.
			executionContext.addResultAttribute(DOMServices::s_XMLNamespace, s_emptyString);
		}
		else if (equals(*theCurrentDefaultNamespace, *theElementDefaultNamespace) == false)
		{
			executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
		}
	}

	if(0 != m_avts.size())
	{
		const AVTVectorType::size_type	nAttrs = m_avts.size();

		StylesheetExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		theStringedValue = theGuard.get();

		for(AVTVectorType::size_type i = 0; i < nAttrs; ++i)
		{
			const AVT* const	avt = m_avts[i];

			const XalanDOMString&	theName = avt->getName();

			const XalanDOMString&	thePrefix = avt->getPrefix();

			const XalanDOMString&	theSimpleValue = avt->getSimpleValue();

			if (isEmpty(theSimpleValue) == false)
			{
				doAddResultAttribute(executionContext, thePrefix, theName, theSimpleValue);
			}
			else
			{
				avt->evaluate(theStringedValue, executionContext.getCurrentNode(), *this, executionContext);

				doAddResultAttribute(executionContext, thePrefix, theName, theStringedValue);
			}
		}
	}

	executeChildren(executionContext);

	executionContext.endElement(c_wstr(getElementName()));
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

			const XalanDOMString* const		ns = getStylesheet().getNamespaceForPrefixFromStack(prefix);

			if (ns != 0 && equals(*ns, constructionContext.getXSLTNamespaceURI()) == false)
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
			const XalanDOMChar*				attrValue)
{
	if(equals(localName, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
	{
		m_namespacesHandler.processExtensionElementPrefixes(attrValue, stylesheetTree.getNamespaces(), constructionContext);

		return true;
	}
	else if (equals(localName, Constants::ATTRNAME_EXCLUDE_RESULT_PREFIXES))
	{
		m_namespacesHandler.processExcludeResultPrefixes(attrValue, stylesheetTree.getNamespaces(), constructionContext);

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
