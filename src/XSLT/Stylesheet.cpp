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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
 * $ Id: $
 *
 */

#include "Stylesheet.hpp"



#include <algorithm>



#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <Include/STLHelper.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XMLSupport/XMLParserLiaison.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XalanQNameByReference.hpp>



#include "Constants.hpp"
#include "ElemAttributeSet.hpp"
#include "ElemDecimalFormat.hpp"
#include "ElemTemplate.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemVariable.hpp"
#include "ExtensionNSHandler.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



const XalanDOMString			Stylesheet::s_emptyString;

const XalanQNameByReference		Stylesheet::s_emptyQName;

const XalanEmptyNamedNodeMap	Stylesheet::s_fakeAttributes;



Stylesheet::Stylesheet(
		StylesheetRoot& 				root,
		const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	XalanDocument(),
	PrefixResolver(),
	m_stylesheetRoot(root),
	m_baseIdent(baseIdentifier),
	m_keyDeclarations(),
	m_XSLTNamespaceURI(constructionContext.getXSLTNamespaceURI()),
	m_whitespacePreservingElements(),
	m_whitespaceStrippingElements(),	
	m_imports(),
	m_importsSize(0),
	m_namespaces(),
	m_namespaceDecls(),
	m_isWrapperless(false),
	m_wrapperlessTemplate(0),
	m_extensionNamespaces(),
	m_firstTemplate(0),
	m_includeStack(),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_XSLTVerDeclared(1.0L),
	m_isRoot(&root == this ? true: false),
	m_elementPatternTable(),
	m_elementPatternTableEnd(m_elementPatternTable.end()),
	m_elementAnyPatternList(),
	m_attributePatternTable(),
	m_attributePatternTableEnd(m_attributePatternTable.end()),
	m_attributeAnyPatternList(),
	m_textPatternList(),
	m_commentPatternList(),
	m_rootPatternList(),
	m_piPatternList(),
	m_nodePatternList(),
	m_matchPattern2Container(),
	m_patternCount(0),
	m_attributeSets(),
	m_attributeSetsSize(0),
	m_surrogateChildren(*this),
	m_elemDecimalFormats(),
	m_namespacesHandler()
{
	if (length(m_baseIdent) == 0)
	{
		m_includeStack.push_back(m_baseIdent);
	}
	else
	{
		try
		{
			const XalanDOMString urlString = constructionContext.getURLStringFromString(m_baseIdent);

			if (length(urlString) != 0)
			{
				m_includeStack.push_back(urlString);

				m_baseIdent = urlString;
			}
		}
		catch(const XMLPlatformUtilsException&)
		{
			// Assume that any exception here relates to get the urlString from
			// m_baseIdent.  We'll assume that it's just a fake base identifier
			// since the parser will throw the real error if the base identifier
			// can't be resolved.
			m_includeStack.push_back(baseIdentifier);
		}
	}
}



Stylesheet::~Stylesheet()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the imports vector.
	for_each(m_imports.begin(),
			 m_imports.end(),
			 DeleteFunctor<Stylesheet>());

	// Clean up the atribute sets vector
	for_each(m_attributeSets.begin(),
			 m_attributeSets.end(),
			 DeleteFunctor<ElemAttributeSet>());

	// Clean up the top-level variables vector
	for_each(m_topLevelVariables.begin(),
			 m_topLevelVariables.end(),
			 DeleteFunctor<ElemVariable>());

	// Clean up the decimal formats vector
	for_each(m_elemDecimalFormats.begin(),
			 m_elemDecimalFormats.end(),
			 DeleteFunctor<ElemDecimalFormat>());

	// Clean up the extension namespaces vector
	for_each(m_extensionNamespaces.begin(),
			 m_extensionNamespaces.end(),
			 MapValueDeleteFunctor<ExtensionNamespacesMapType>());

	delete m_wrapperlessTemplate;

	delete m_firstTemplate;
}



void
Stylesheet::processKeyElement(
			ElemTemplateElement*			nsContext,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
{
	const XalanDOMChar* 	nameAttr = 0;
	XPath*					matchAttr = 0;
	XPath*					useAttr = 0;
 
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_NAME))
		{
			nameAttr = atts.getValue(i);
		}
		else if(equals(aname, Constants::ATTRNAME_MATCH))
		{
			matchAttr =
					constructionContext.createMatchPattern(
						0,
						XalanDOMString(atts.getValue(i)),
						*nsContext);
		}
		else if(equals(aname, Constants::ATTRNAME_USE))
		{
			useAttr =
					constructionContext.createXPath(
						0,
						atts.getValue(i),
						*nsContext);
		}
		else if (isAttrOK(aname, atts, i, constructionContext) == false)
		{
			constructionContext.error("xsl:key has an illegal attribute");
		}
	}

	if(0 == nameAttr)
	{
		constructionContext.error("xsl:key requires a 'name' attribute");
	}

	if(0 == matchAttr)
	{
		constructionContext.error("xsl:key requires a 'match' attribute");
	}

	if(0 == useAttr)
	{
		constructionContext.error("xsl:key requires a 'use' attribute");
	}

	const XalanQNameByValue		theQName(nameAttr, m_namespaces, constructionContext.getLocatorFromStack());

	if (theQName.isValid() == false)
	{
		constructionContext.error("xsl:key has an invalid 'name' attribute");
	}

	m_keyDeclarations.push_back(KeyDeclaration(theQName, *matchAttr, *useAttr));
}



void
Stylesheet::pushNamespaces(const AttributeList&		atts)
{
	const unsigned int		nAttrs = atts.getLength();

	NamespaceVectorType 	namespaces;

	XalanDOMString			prefix;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);
		const XalanDOMChar* const	value = atts.getValue(i);

		const bool	isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

		if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
		{
			if (isPrefix == false)
			{
				prefix.clear();
			}
			else
			{
				substring(aname, prefix, DOMServices::s_XMLNamespaceWithSeparatorLength);
			}

			namespaces.push_back(NameSpace(prefix, XalanDOMString(value)));
		}
	}

	m_namespaces.push_back(namespaces);
}



class attrSetCompare
{
public:

	attrSetCompare(const ElemAttributeSet&	theAttrSet) :
		m_attrSet(theAttrSet)
	{
	}

	bool
	operator()(const ElemAttributeSet*	theRHS) const
	{
		assert(theRHS != 0);

		return m_attrSet == *theRHS;
	}

private:

	const ElemAttributeSet&		m_attrSet;
};



static void
addToList(
			Stylesheet::PatternTableListType&	theList,
			const Stylesheet::MatchPattern2*	thePattern)
{
	typedef Stylesheet::size_type	size_type;
	assert(thePattern != 0);

	const double		thePatternPriority = thePattern->getPriorityOrDefault();
	const size_type		thePatternPosition = thePattern->getPositionInStylesheet();

	typedef Stylesheet::PatternTableListType	PatternTableListType;
	typedef PatternTableListType::iterator		iterator;

	iterator		theCurrent = theList.begin();

	const iterator	theEnd = theList.end();

	while(theCurrent != theEnd)
	{
		const double	theCurrentPriority = (*theCurrent)->getPriorityOrDefault();

		if (thePatternPriority > theCurrentPriority)
		{
			break;
		}
		else if (thePatternPriority == theCurrentPriority &&
				 thePatternPosition > (*theCurrent)->getPositionInStylesheet())
		{
			break;
		}

		++theCurrent;
	}

	theList.insert(theCurrent, thePattern);
}



static void
addToTable(
		   Stylesheet::PatternTableMapType&			theTable,
		   const Stylesheet::PatternTableListType&	theList)
{
	typedef Stylesheet::PatternTableMapType		PatternTableMapType;
	typedef Stylesheet::PatternTableListType	PatternTableListType;

	PatternTableMapType::iterator				theCurrentTable = theTable.begin();
	const PatternTableMapType::iterator			theTableEnd = theTable.end();
	const PatternTableListType::const_iterator	theListEnd = theList.end();

	while(theCurrentTable != theTableEnd)
	{
		PatternTableListType::const_iterator	theCurrent = theList.begin();

		while(theCurrent != theListEnd)
		{
			addToList((*theCurrentTable).second, *theCurrent);

			++theCurrent;
		}

		++theCurrentTable;
	}
}



void
Stylesheet::postConstruction(StylesheetConstructionContext&		constructionContext)
{
	{
		m_importsSize = m_imports.size();

		// Call postConstruction() on any imported stylesheets, the get any aliases
		// in reverse order, to preserve import precedence. Also, get any key declarations.
		const StylesheetVectorType::reverse_iterator	theEnd = m_imports.rend();
		StylesheetVectorType::reverse_iterator	i = m_imports.rbegin();

		while(i != theEnd)
		{
			(*i)->postConstruction(constructionContext);

			m_namespacesHandler.copyNamespaceAliases((*i)->getNamespacesHandler());

			// $$ ToDo: Should we clear the imported stylesheet's key
			// declarations after we copy them?
			m_keyDeclarations.insert(
				m_keyDeclarations.end(),
				(*i)->m_keyDeclarations.begin(),
				(*i)->m_keyDeclarations.end());

			++i;
		}
	}

	// Call postConstruction() on our own namespaces handler...
	m_namespacesHandler.postConstruction(constructionContext);


	{
		for (ElemTemplateElement* node = m_firstTemplate;
			 node != 0;
			 node = node->getNextSiblingElem())
		{
			node->postConstruction(constructionContext, m_namespacesHandler);
		}
	}

	{
		for (ElemVariableVectorType::iterator it = m_topLevelVariables.begin();
			 it != m_topLevelVariables.end();
			 ++it)
		{
			(*it)->postConstruction(constructionContext, m_namespacesHandler);
		}
	}

	{
		for (ElemTemplateElement* node = m_wrapperlessTemplate;
			 node != 0;
			 node = node->getNextSiblingElem())
		{
			node->postConstruction(constructionContext, m_namespacesHandler);
		}
	}

	// Cache the size...
	m_attributeSetsSize = m_attributeSets.size();

	addToTable(m_elementPatternTable, m_elementAnyPatternList);
	addToTable(m_attributePatternTable, m_attributeAnyPatternList);

	m_patternCount = m_matchPattern2Container.size();
}



bool
Stylesheet::isAttrOK(
			const XalanDOMChar* 			attrName,
			const AttributeList&			/* atts */,
			int 							/* which */,
			StylesheetConstructionContext&	constructionContext) const
{
	// Namespace declarations are OK by definition
	bool attrOK = equals(attrName, DOMServices::s_XMLNamespace) ||
						 startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator);

	if(!attrOK)
	{
		// Others are OK if their prefix has been
		// bound to a non-null Namespace URI other than XSLT's
		const XalanDOMString::size_type		indexOfNSSep = indexOf(attrName, XalanUnicode::charColon);

		if(indexOfNSSep < length(attrName))
		{
			const XalanDOMString	prefix(attrName, indexOfNSSep);
			const XalanDOMString*	ns = getNamespaceForPrefixFromStack(prefix);

			attrOK = ns != 0 && !::isEmpty(*ns) && !equals(*ns, constructionContext.getXSLTNamespaceURI());
		}
		else if (m_XSLTVerDeclared > constructionContext.getXSLTVersionSupported())
		{
			attrOK = true;
		}
	}

	return attrOK;
}



const XalanDOMString*
Stylesheet::getNamespaceFromStack(const XalanDOMChar* 	nodeName) const
{
	assert(nodeName != 0);

	const XalanDOMString::size_type		indexOfNSSep = indexOf(nodeName, XalanUnicode::charColon);

	if (indexOfNSSep == length(nodeName))
	{
		return getNamespaceForPrefixFromStack(s_emptyString);
	}
	else
	{
		return getNamespaceForPrefixFromStack(XalanDOMString(nodeName, indexOfNSSep));
	}
}



bool
Stylesheet::getYesOrNo(
			const XalanDOMChar* 			/* aname */,
			const XalanDOMChar* 			val,
			StylesheetConstructionContext&	constructionContext) const
{
	if(equals(val, Constants::ATTRVAL_YES))
	{
		return true;
	}
	else if(equals(val, Constants::ATTRVAL_NO))
	{
		return false;
	}
	else
	{
		constructionContext.error("The attribute value must be 'yes' or 'no'");

		return false;
	}
}



double
Stylesheet::MatchPattern2::getPriorityOrDefault() const
{
	const double	templatePriority =
		m_template->getPriority();

	if (DoubleSupport::isNegativeInfinity(templatePriority) == true)
	{
		return XPath::getMatchScoreValue(m_priority);
	}
	else
	{
		return templatePriority;
	}
}



void
Stylesheet::addTemplate(
			ElemTemplate*					theTemplate,
			StylesheetConstructionContext&	constructionContext)
{
	assert(theTemplate != 0);

	if(0 == m_firstTemplate)
	{
		m_firstTemplate = theTemplate;
	}
	else
	{
		ElemTemplateElement*	next = m_firstTemplate;

		// Find the last one, then append the new one.
		while(0 != next)
		{
			if(0 == next->getNextSiblingElem())
			{
				next->setNextSiblingElem(theTemplate);
				theTemplate->setNextSiblingElem(0); // just to play it safe.
				theTemplate->setPreviousSiblingElem(next);
				break;
			}

			next = next->getNextSiblingElem();
		}
	}

	// If it's a named template, then we need to
	// and it to the map of named templates.
	const XalanQName&	theName = theTemplate->getName();

	if(theName.isEmpty() == false)
	{
		if (m_namedTemplates.find(theName) == m_namedTemplates.end())
		{
			m_namedTemplates[theName] = theTemplate;
		}
		else
		{
			// This is an error...
			constructionContext.error(
				"The stylesheet already has a template with this name",
				0,
				theTemplate);
		}
	}

	// Now, process the match pattern associated with the
	// template.
	const XPath* const	xp = theTemplate->getMatchPattern();

	if(0 != xp)
	{
		/* Each string has a list of pattern tables associated with it; if the
		 * string is not in the map, then create a list of pattern tables with one
		 * entry for the string, otherwise add to the existing pattern table list
		 * for that string
		 */
		typedef XPath::TargetDataVectorType		TargetDataVectorType;

		TargetDataVectorType	data;

		xp->getTargetData(data);

		TargetDataVectorType::size_type	nTargets =
				data.size();

		if(nTargets != 0)
		{
			XalanDOMString	tempString;

			for(TargetDataVectorType::size_type i = 0;
								i < nTargets; ++i) 
			{
				assert(data[i].getString() != 0);
				
				tempString = data[i].getString();

				m_matchPattern2Container.push_back(
					MatchPattern2(
						*theTemplate,
						m_matchPattern2Container.size(),
						tempString,
						*xp,
						xp->getExpression().getCurrentPattern(),
						data[i].getDefaultPriority()));

				const MatchPattern2* const	newMatchPat =
					&m_matchPattern2Container.back();

				// Always put things on the front of the list, so
				// templates later in the stylesheet are always
				// selected first.
				if (equals(tempString, XPath::PSEUDONAME_TEXT) == true)
				{
					addToList(m_textPatternList, newMatchPat);
				}
				else if (equals(tempString, XPath::PSEUDONAME_COMMENT) == true)
				{
					addToList(m_commentPatternList, newMatchPat);
				}
				else if (equals(tempString, XPath::PSEUDONAME_ROOT) == true)
				{
					addToList(m_rootPatternList, newMatchPat);
				}
				else if (equals(tempString, XPath::PSEUDONAME_PI) == true)
				{
					addToList(m_piPatternList, newMatchPat);
				}
				else if (equals(tempString, XPath::PSEUDONAME_NODE) == true)
				{
					addToList(m_nodePatternList, newMatchPat);

					addToList(m_elementAnyPatternList, newMatchPat);
					addToList(m_attributeAnyPatternList, newMatchPat);
					addToList(m_commentPatternList, newMatchPat);
					addToList(m_textPatternList, newMatchPat);
					addToList(m_piPatternList, newMatchPat);
				}
				else if (equals(tempString, XPath::PSEUDONAME_ANY) == true)
				{
					if (data[i].getTargetType() == XPath::TargetData::eElement)
					{
						addToList(m_elementAnyPatternList, newMatchPat);
					}
					else if (data[i].getTargetType() == XPath::TargetData::eAttribute)
					{
						addToList(m_attributeAnyPatternList, newMatchPat);
					}
					else if (data[i].getTargetType() == XPath::TargetData::eAny)
					{
						addToList(m_elementAnyPatternList, newMatchPat);
						addToList(m_attributeAnyPatternList, newMatchPat);
					}
				}
				else
				{
					if (data[i].getTargetType() == XPath::TargetData::eElement)
					{
						addToList(m_elementPatternTable[tempString], newMatchPat);
					}
					else if (data[i].getTargetType() == XPath::TargetData::eAttribute)
					{
						addToList(m_attributePatternTable[tempString], newMatchPat);
					}
				}
			}
		}
	}
}



const ElemTemplate*
Stylesheet::findNamedTemplate(const XalanQName&		qname) const
{
	assert(m_importsSize == m_imports.size());

	const ElemTemplateMapType::const_iterator	it = m_namedTemplates.find(qname);

	if(it != m_namedTemplates.end())
	{
		return (*it).second;
	}
	else
	{
		const ElemTemplate*		namedTemplate = 0;

		// Look for the template in the imports
		for(StylesheetVectorType::size_type i = 0; i < m_importsSize; ++i)
		{
			const Stylesheet* const stylesheet = m_imports[i];

			namedTemplate = stylesheet->findNamedTemplate(qname);

			if(0 != namedTemplate)
				break;
		}

		return namedTemplate;
	}
}
	


void
Stylesheet::addObjectIfNotFound(
			const MatchPattern2*		thePattern,
			PatternTableVectorType& 	theVector)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	const PatternTableVectorType::const_iterator 	theResult =
		find(
				theVector.begin(),
				theVector.end(),
				thePattern);

	// Did we find it?
	if(theResult == theVector.end())
	{
		theVector.push_back(thePattern);
	}
}



inline void
Stylesheet::addObjectIfNotFound(
			const MatchPattern2*	thePattern,
			const MatchPattern2* 	thePatternArray[],
			unsigned int&			thePatternArraySize)
{
	if (thePatternArraySize == 0)
	{
		thePatternArray[0] = thePattern;

		++thePatternArraySize;
	}
	else
	{
		unsigned int i = 0;

		while(i < thePatternArraySize)
		{
			if (thePatternArray[i] != thePattern)
			{
				++i;
			}
			else
			{
				break;
			}
		}

		if (i == thePatternArraySize)
		{
			thePatternArray[thePatternArraySize++] = thePattern;
		}
	}
}



inline const Stylesheet::PatternTableListType* 
Stylesheet::locateElementMatchPatternList2(const XalanDOMString&	theName) const
{
	assert(m_elementPatternTableEnd == m_elementPatternTable.end());

	const PatternTableMapType::const_iterator	i =
		m_elementPatternTable.find(theName);

	if (i != m_elementPatternTableEnd)
	{
		return &(*i).second;
	}
	else
	{
		return &m_elementAnyPatternList;
	}
}



inline const Stylesheet::PatternTableListType* 
Stylesheet::locateAttributeMatchPatternList2(const XalanDOMString&	theName) const
{
	assert(m_attributePatternTableEnd == m_attributePatternTable.end());

	const PatternTableMapType::const_iterator	i =
		m_attributePatternTable.find(theName);

	if (i != m_attributePatternTableEnd)
	{
		return &(*i).second;
	}
	else
	{
		return &m_attributeAnyPatternList;
	}
}



inline const Stylesheet::PatternTableListType*
Stylesheet::locateMatchPatternList2(const XalanNode&	theNode) const
{
	switch(theNode.getNodeType())
	{
	case XalanNode::ELEMENT_NODE:
		return locateElementMatchPatternList2(DOMServices::getLocalNameOfNode(theNode));
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		return &m_piPatternList;
		break;

	case XalanNode::ATTRIBUTE_NODE:
		return locateAttributeMatchPatternList2(DOMServices::getLocalNameOfNode(theNode));
		break;

	case XalanNode::CDATA_SECTION_NODE:
	case XalanNode::TEXT_NODE:
		return &m_textPatternList;
		break;

	case XalanNode::COMMENT_NODE:
		return &m_commentPatternList;
		break;

	case XalanNode::DOCUMENT_NODE:
		return &m_rootPatternList;
		break;

	default:
		break;
	}

	return &m_nodePatternList;
}



inline const ElemTemplate*
Stylesheet::findTemplateInImports(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode, 
			const XalanQName&				mode) const
{
	assert(m_importsSize == m_imports.size());

	for(StylesheetVectorType::size_type i = 0; i < m_importsSize; i++)
	{
		const Stylesheet* const 	stylesheet =
			m_imports[i];

		const ElemTemplate* const	bestMatchedRule =
			stylesheet->findTemplate(
				executionContext,
				targetNode,
				mode, 
				false);

		if(bestMatchedRule != 0)
		{
			return bestMatchedRule;
		}
	}

	return 0;
}



const ElemTemplate*
Stylesheet::findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						targetNode, 
			const XalanQName&				mode,
			bool							onlyUseImports) const
{
	assert(targetNode != 0);
	assert(m_patternCount == m_matchPattern2Container.size());

	if(m_isWrapperless == true)
	{
		return m_wrapperlessTemplate;
	}
	else if (onlyUseImports == true)
	{
		return findTemplateInImports(executionContext, targetNode, mode);
	}
	else
	{
		const ElemTemplate*		bestMatchedRule = 0;

		if (executionContext.getQuietConflictWarnings() == true)
		{
			// Points to the current list of match patterns.  Note
			// that this may point to more than one table.
			const PatternTableListType* 	matchPatternList =
					locateMatchPatternList2(*targetNode);
			assert(matchPatternList != 0);

			PatternTableListType::const_iterator		theCurrentEntry =
					matchPatternList->begin();

			const PatternTableListType::const_iterator	theTableEnd =
					matchPatternList->end();

			while(theCurrentEntry != theTableEnd)
			{
				const MatchPattern2*	matchPat = *theCurrentEntry;
				assert(matchPat != 0);

				const ElemTemplate*	const	rule = matchPat->getTemplate();
				assert(rule != 0);

				// We'll be needing to match rules according to what 
				// mode we're in.
				const XalanQName&	ruleMode = rule->getMode();

				// The logic here should be that if we are not in a mode AND
				// the rule does not have a node, then go ahead.
				// OR if we are in a mode, AND the rule has a node, 
				// AND the rules match, then go ahead.
				const bool	haveMode = !mode.isEmpty();
				const bool	haveRuleMode = !ruleMode.isEmpty();

				if ((!haveMode && !haveRuleMode) ||
					(haveMode && haveRuleMode && ruleMode.equals(mode)))
				{
					const XPath* const	xpath = matchPat->getExpression();

					XPath::eMatchScore	score =
								xpath->getMatchScore(targetNode, *this, executionContext);

					if(XPath::eMatchScoreNone != score)
					{
						bestMatchedRule = rule;

						break;
					}
				}

				++theCurrentEntry;
			}

			if(0 == bestMatchedRule)
			{
				bestMatchedRule = findTemplateInImports(executionContext, targetNode, mode);
			}
		}
		else
		{
			const PatternTableListType* 	matchPatternList =
					locateMatchPatternList2(*targetNode);
			assert(matchPatternList != 0);

			PatternTableListType::const_iterator		theCurrentEntry =
					matchPatternList->begin();

			const PatternTableListType::const_iterator	theTableEnd =
					matchPatternList->end();

			if (theCurrentEntry != theTableEnd)
			{
				const MatchPattern2*	bestMatchedPattern = 0; // Syncs with bestMatchedRule
				const double			matchScoreNoneValue = 
					XPath::getMatchScoreValue(XPath::eMatchScoreNone);

				double					bestMatchPatPriority = matchScoreNoneValue;

				unsigned int			nConflicts = 0;

				// Use a stack-based array when possible...
				const MatchPattern2*	conflictsArray[100];

				XalanArrayAutoPtr<const MatchPattern2*>		conflictsVector;

				const MatchPattern2**	conflicts = 0;

				const XalanDOMString*	prevPat = 0;
				const MatchPattern2*	prevMatchPat = 0;

				do
				{
					const MatchPattern2*	matchPat = *theCurrentEntry;
					double					matchPatPriority = matchScoreNoneValue;
					assert(matchPat != 0);

					const ElemTemplate*	const	rule = matchPat->getTemplate();
					assert(rule != 0);

					// We'll be needing to match rules according to what 
					// mode we're in.
					const XalanQName&	ruleMode = rule->getMode();

					// The logic here should be that if we are not in a mode AND
					// the rule does not have a node, then go ahead.
					// OR if we are in a mode, AND the rule has a node, 
					// AND the rules match, then go ahead.
					const bool	haveMode = !mode.isEmpty();
					const bool	haveRuleMode = !ruleMode.isEmpty();

					if ((!haveMode && !haveRuleMode) ||
						(haveMode && haveRuleMode && ruleMode.equals(mode)))
					{
						const XalanDOMString*	patterns = matchPat->getPattern();
						assert(patterns != 0);

						if(!isEmpty(*patterns) &&
						   !(prevMatchPat != 0 &&
							 (prevPat != 0 && equals(*prevPat, *patterns)) &&
							 prevMatchPat->getTemplate()->getPriority() == matchPat->getTemplate()->getPriority()))
						{
							prevPat = patterns;
							prevMatchPat = matchPat;
							matchPatPriority = matchScoreNoneValue;

							const XPath* const	xpath = matchPat->getExpression();

							XPath::eMatchScore	score =
										xpath->getMatchScore(targetNode, *this, executionContext);

							if(XPath::eMatchScoreNone != score)
							{
								const double priorityVal = rule->getPriority();
								const double priorityOfRule 
											  = (matchScoreNoneValue != priorityVal) 
											  ? priorityVal : XPath::getMatchScoreValue(score);

								matchPatPriority = priorityOfRule;
								const double priorityOfBestMatched =
												(0 != bestMatchedPattern) ?
														bestMatchPatPriority : 
														matchScoreNoneValue;

								if(priorityOfRule > priorityOfBestMatched)
								{
									nConflicts = 0;

									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
									bestMatchPatPriority = matchPatPriority;
								}
								else if(priorityOfRule == priorityOfBestMatched)
								{
									if (conflicts == 0)
									{
										if (m_patternCount > sizeof(conflictsArray) / sizeof(conflictsArray[0]))
										{
											conflictsVector.reset(new const MatchPattern2*[m_patternCount]);

											conflicts = conflictsVector.get();
										}
										else
										{
											conflicts = conflictsArray;
										}
									}

									assert(conflicts != 0);

									// Add the best matched pattern so far.
									addObjectIfNotFound(bestMatchedPattern, conflicts, nConflicts);

									// Add the pattern that caused the conflict...
									conflicts[nConflicts++] = matchPat;

									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
									bestMatchPatPriority = matchPatPriority;
								}
							}
						}
					}

					++theCurrentEntry;

				} while(theCurrentEntry != theTableEnd);

				if(nConflicts > 0)
				{
					assert(conflicts != 0);

					XalanDOMString	conflictsString(XALAN_STATIC_UCODE_STRING("Specificity conflicts found: "));
					
					for(unsigned int i = 0; i < nConflicts; i++)
					{
						const MatchPattern2* const	conflictPat = conflicts[i];

						if(0 != i)
						{
							conflictsString += XALAN_STATIC_UCODE_STRING(", ");

							// Find the furthest one towards the bottom of the document.
							if(conflictPat->getPositionInStylesheet() >
								bestMatchedPattern->getPositionInStylesheet())
							{
								bestMatchedPattern = conflictPat;
							}
						}
						else
						{
							bestMatchedPattern = conflictPat;
						}

						conflictsString += XalanDOMString(XALAN_STATIC_UCODE_STRING("\"")) +
												*conflictPat->getPattern() +
												XalanDOMString(XALAN_STATIC_UCODE_STRING("\""));
					}

					bestMatchedRule = bestMatchedPattern->getTemplate();

					conflictsString += XALAN_STATIC_UCODE_STRING(" ");
					conflictsString += XALAN_STATIC_UCODE_STRING("Last found in stylesheet will be used.");
					executionContext.warn(conflictsString, targetNode, bestMatchedRule->getLocator());
				}
			}

			if(0 == bestMatchedRule)
			{
				bestMatchedRule = findTemplateInImports(executionContext, targetNode, mode);
			}
		}

		return bestMatchedRule;
	}
}



void
Stylesheet::addExtensionNamespace(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			uri,
			ExtensionNSHandler*				nsh)
{
	m_extensionNamespaces.insert(ExtensionNamespacesMapType::value_type(uri, nsh));

	m_namespacesHandler.addExtensionNamespaceURI(theConstructionContext, uri);
}



void
Stylesheet::pushTopLevelVariables(
			StylesheetExecutionContext& 	executionContext,
			const ParamVectorType&			topLevelParams) const
{
	{
		// First, push any imports...
		const StylesheetVectorType::const_reverse_iterator	rend = m_imports.rend();

		for(StylesheetVectorType::const_reverse_iterator i = m_imports.rbegin(); i != rend; ++i)
		{
			const Stylesheet* const stylesheet = *i;
			assert(stylesheet != 0);

			stylesheet->pushTopLevelVariables(executionContext, topLevelParams);
		}
	}

	const ParamVectorType::size_type	nVars = m_topLevelVariables.size();

	for(ParamVectorType::size_type i = 0; i < nVars; ++i)
	{
		ElemVariable* const 	var = m_topLevelVariables[i];

		bool					isParam =
				StylesheetConstructionContext::ELEMNAME_PARAM == var->getXSLToken();

		if(isParam == true)
		{
			isParam = false;

			const ParamVectorType::size_type	n = topLevelParams.size();

			for(ParamVectorType::size_type k = 0; k < n; k++)
			{
				const ParamVectorType::value_type&	arg = topLevelParams[k];

				if(arg.getName().equals(var->getName()))
				{
					isParam = true;

					if (arg.getXObject().null() == false)
					{
						executionContext.pushVariable(
							arg.getName(),
							arg.getXObject(),
							0);
					}
					else
					{
						executionContext.pushVariable(
							arg.getName(),
							0,
							arg.getExpression(),
							executionContext.getRootDocument(),
							*this);
					}

					break;
				}
			}
		}

		if (isParam == false)
		{
			executionContext.pushVariable(var->getName(),
										  var,
										  var->getParentNodeElem());
		}
	}
}



void
Stylesheet::processNSAliasElement(
			const XalanDOMChar*				/* name */,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
{
	const unsigned int		nAttrs = atts.getLength();

	const XalanDOMString*	stylesheetNamespace = &DOMServices::s_emptyString;
	const XalanDOMString*	resultNamespace = &DOMServices::s_emptyString;
	const XalanDOMString	dummy;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_STYLESHEET_PREFIX) == true)
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				stylesheetNamespace = getNamespaceForPrefix(dummy);
			}
			else
			{
				stylesheetNamespace = getNamespaceForPrefix(XalanDOMString(value));
			}
		}
		else if(equals(aname, Constants::ATTRNAME_RESULT_PREFIX))
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				resultNamespace = getNamespaceForPrefix(dummy);
			}
			else
			{
				resultNamespace = getNamespaceForPrefix(XalanDOMString(value));
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error("xsl:namespace-alias has an illegal attribute");
		}
	}

	// Build a table of aliases, the key is the stylesheet uri and the
	// value is the result uri
	if (length(*stylesheetNamespace) == 0 ||
		length(*resultNamespace) == 0)
	{
		constructionContext.error("Missing namespace URI for specified prefix");
	}
	else
	{
		m_namespacesHandler.setNamespaceAlias(
				constructionContext,
				*stylesheetNamespace,
				*resultNamespace);
	}
}



const XalanDecimalFormatSymbols*
Stylesheet::getDecimalFormatSymbols(const XalanQName&	theQName) const
{
	const XalanDecimalFormatSymbols* 				dfs = 0;

	const ElemDecimalFormatVectorType::size_type	theSize =
		m_elemDecimalFormats.size();

	if(theSize > 0)
	{
		// Start from the top of the stack
		for (ElemDecimalFormatVectorType::size_type i = theSize; i > 0; --i)
		{
			assert(m_elemDecimalFormats[i - 1] != 0);

			const ElemDecimalFormat* const	theCurrent =
				m_elemDecimalFormats[i - 1];
			assert(theCurrent != 0);

			if (theCurrent->getQName().equals(theQName) == true)
			{
				dfs = &theCurrent->getDecimalFormatSymbols();

				break;
			}
		}
	}

	// If dfs is null at this point, it should
	// mean there wasn't an xsl:decimal-format declared
	// with the given name.	So go up the import hierarchy
	// and see if one of the imported stylesheets declared
	// it.
	if(dfs == 0)
	{
		for(StylesheetVectorType::size_type i = 0; i < m_importsSize; ++i)
		{
			dfs = m_imports[i]->getDecimalFormatSymbols(theQName);

			if(dfs != 0)
			{
				break;
			}
		}
	}

	return dfs;
}



void
Stylesheet::applyAttrSets(
			const XalanQName**				attributeSetsNames,
			size_type						attributeSetsNamesCount,
			StylesheetExecutionContext& 	executionContext,			
			XalanNode*						sourceNode) const
{
	if(0 != attributeSetsNamesCount)
	{
		// Process up the import chain...
		const StylesheetVectorType::const_reverse_iterator	theEnd = m_imports.rend();
		StylesheetVectorType::const_reverse_iterator		i = m_imports.rbegin();

		while(i != theEnd)
		{
			(*i)->applyAttrSets(
				attributeSetsNames,
				attributeSetsNamesCount,
				executionContext,
				sourceNode);

			++i;
		}

		for(size_type j = 0; j < attributeSetsNamesCount; j++)
		{
			const XalanQName* const		qname = attributeSetsNames[j];
			assert(qname != 0);

			assert(m_attributeSetsSize == m_attributeSets.size());

			for(StylesheetVectorType::size_type k = 0; k < m_attributeSetsSize; k++)
			{
				const ElemAttributeSet* const	attrSet = m_attributeSets[k];
				assert(attrSet != 0);

				if(qname->equals(attrSet->getQName()))
				{
					attrSet->execute(executionContext);
				}
			}
		}
	}
}



const XalanDOMString&
Stylesheet::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
Stylesheet::getNodeValue() const
{
	return s_emptyString;
}



Stylesheet::NodeType
Stylesheet::getNodeType() const
{
	return XalanNode::DOCUMENT_NODE;
}



XalanNode*
Stylesheet::getParentNode() const
{
	return 0;
}



const XalanNodeList*
Stylesheet::getChildNodes() const
{
	return &m_surrogateChildren;
}



XalanNode*
Stylesheet::getFirstChild() const
{
	return 0;
}



XalanNode*
Stylesheet::getLastChild() const
{
	return 0;
}



XalanNode*
Stylesheet::getPreviousSibling() const 
{
	return 0;
}



XalanNode*
Stylesheet::getNextSibling() const 
{
	return 0;
}



const XalanNamedNodeMap*
Stylesheet::getAttributes() const
{
	return &s_fakeAttributes;
}



XalanDocument*
Stylesheet::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
Stylesheet*
#endif
Stylesheet::cloneNode(bool		/* deep */) const
{
	//should not be called
	assert(false);	

	return 0;
}



XalanNode*
Stylesheet::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanNode*
Stylesheet::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanNode*
Stylesheet::removeChild(XalanNode*		/* oldChild */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanNode*
Stylesheet::appendChild(XalanNode*		/* oldChild */)
{
	//should not be called
	assert(false);	

	return 0;
}



bool
Stylesheet::hasChildNodes() const
{
	return true;
}



void
Stylesheet::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
Stylesheet::normalize()
{
}



bool
Stylesheet::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
Stylesheet::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
Stylesheet::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
Stylesheet::getLocalName() const
{
	return s_emptyString;
}



void
Stylesheet::setPrefix(const XalanDOMString& /* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



unsigned long
Stylesheet::getIndex() const
{
	return 0;
}



XalanElement*
Stylesheet::createElement(const XalanDOMString& 	/* tagName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanDocumentFragment*
Stylesheet::createDocumentFragment()
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanText*
Stylesheet::createTextNode(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanComment*
Stylesheet::createComment(const XalanDOMString& 	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanCDATASection*
Stylesheet::createCDATASection(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanProcessingInstruction*
Stylesheet::createProcessingInstruction(
			const XalanDOMString&	/* target */,
			const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanAttr*
Stylesheet::createAttribute(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanEntityReference*
Stylesheet::createEntityReference(const XalanDOMString& 	/* name */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanDocumentType*
Stylesheet::getDoctype() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanDOMImplementation*
Stylesheet::getImplementation() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanElement*
Stylesheet::getDocumentElement() const
{
	return m_wrapperlessTemplate != 0 ? m_wrapperlessTemplate : m_firstTemplate;
}



XalanNodeList*
Stylesheet::getElementsByTagName(const XalanDOMString&		/* name */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNodeList*
Stylesheet::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
Stylesheet::importNode(
			XalanNode*	/* importedNode */,
			bool		/* deep */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanElement*
Stylesheet::createElementNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanAttr*
Stylesheet::createAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanElement*
Stylesheet::getElementById(const XalanDOMString&	/* elementId */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



bool
Stylesheet::isIndexed() const
{
	return false;
}



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(const XalanDOMString& 	prefix) const
{
	return XalanQName::getNamespaceForPrefix(m_namespaceDecls, prefix);
}



const XalanDOMString&
Stylesheet::getURI() const
{
	return m_baseIdent;
}
