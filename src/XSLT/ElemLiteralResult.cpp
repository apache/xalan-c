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



#include <sax/AttributeList.hpp>


#include <PlatformSupport/StringTokenizer.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemLiteralResult::ElemLiteralResult(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const DOMString&				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			name,
			lineNumber,
			columnNumber),
	m_QName(name)
{
	const int nAttrs = atts.getLength();

	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname(atts.getName(i));

		bool		needToProcess = true;
		const int	indexOfNSSep = indexOf(aname,':');

		DOMString prefix;

		if(indexOfNSSep > 0)
		{
			prefix = substring(aname,0,indexOfNSSep);

			if(!equals(prefix, "xmlns"))
			{
				DOMString ns = getNamespaceForPrefix(prefix);

				if(startsWith(ns, constructionContext.getXSLNameSpaceURLPre()))
				{
					const DOMString localName = substring(aname,indexOfNSSep + 1);

					if(equals(localName, Constants::ATTRNAME_EXTENSIONELEMENTPREFIXES))
					{
						needToProcess = false;

						const DOMString qnames = atts.getValue(i);

						StringTokenizer tokenizer(qnames, " \t\n\r", false);

						m_extensionElementPrefixes.reserve(tokenizer.countTokens());

						while(tokenizer.hasMoreTokens())
						{
							m_extensionElementPrefixes.push_back(tokenizer.nextToken());
						}
					}
				}
			}
		}

		if(needToProcess == true)
		{
			processSpaceAttr(aname, atts, i);
			// Add xmlns attribute(except xmlns:xsl), xml:space, etc... 
			// Ignore anything with xsl:xxx 
			if(! processUseAttributeSets(constructionContext, aname, atts, i) &&
					isAttrOK(aname, atts, i, constructionContext) )
			{
				m_avts.push_back(new AVT(aname, atts.getType(i), atts.getValue(i), 	
							*this, constructionContext));
			}
		}
	}
}


ElemLiteralResult::~ElemLiteralResult()
{
}



int ElemLiteralResult::getXSLToken() const 
{		
	return Constants::ELEMNAME_LITERALRESULT;		
}
	

void ElemLiteralResult::execute(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode) const
{
	executionContext.startElement(toCharArray(m_QName));
	
	ElemUse::execute(executionContext, sourceTree, sourceNode, mode);

	if(0 != m_avts.size())
	{
		const int	nAttrs = m_avts.size();

		for(int i = 0; i < nAttrs; i++)
		{
			const AVT* const	avt = m_avts[i];

			DOMString stringedValue;

			avt->evaluate(stringedValue, sourceNode, *this, 
				executionContext.getXPathExecutionContext());

			if(!isEmpty(stringedValue))
			{
				executionContext.replacePendingAttribute(toCharArray(avt->getName()), 
					avt->getType(), toCharArray(stringedValue));
			}
		}
	}

	const ElemTemplateElement*	elem = this;

	const NamespaceVectorType*	nsVector = &elem->getNameSpace();

	bool more = true;

	while(more == true)
	{
		NameSpace ns;

		for (int i = 0; i < nsVector->size(); i++)
		{
			ns = (*nsVector)[i];

			if(!isEmpty(ns.getURI()) && ns.getResultCandidate())
			{
				bool		hasPrefix = !isEmpty(ns.getPrefix());

				DOMString	prefix = hasPrefix ? ns.getPrefix() : DOMString();
				DOMString	desturi = executionContext.getResultNamespaceForPrefix(prefix);
				DOMString	attrName = hasPrefix ? 
					(DOMString("xmlns:") + prefix) : DOMString("xmlns");

				DOMString srcURI = ns.getURI();

				bool isXSLNS = equals(srcURI, executionContext.getXSLNameSpaceURL())
					|| 0 != getStylesheet().lookupExtensionNSHandler(srcURI)
					|| equalsIgnoreCase(srcURI,executionContext.getXalanXSLNameSpaceURL());

				if(!isXSLNS)
				{
					if(startsWith(srcURI,DOMString("quote:")))
					{
						srcURI = substring(srcURI, 6);
					}

					if(!equalsIgnoreCase(srcURI,desturi)) // TODO: Check for extension namespaces
					{
						executionContext.addResultAttribute(attrName, srcURI);
					}
				}
				else
				{
					ns.setResultCandidate(false);
				}
			}
		}

		// We didn't find a namespace, start looking at the parents
		if (0 != elem)
		{
			elem = elem->getParentNode();

			while(0 != elem)
			{
				nsVector = &elem->getNameSpace();

				if(0 == nsVector->size())
					elem = elem->getParentNode();
				else
					break;
			}

			// Last chance, try the stylesheet namespace
			if (0 == nsVector || 0 == nsVector->size())
				nsVector = &getStylesheet().getNamespaceDecls();
			if (0 == nsVector || 0 == nsVector->size())
				more = false;
		}
		else
			more = false;
	}

	executeChildren(executionContext, sourceTree, sourceNode, mode);

	executionContext.endElement(toCharArray(m_QName));
}
