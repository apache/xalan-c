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



const Stylesheet::NamespaceVectorType	Stylesheet::s_emptyNamespace;



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
	m_patternTable(),
	m_patternTableEnd(m_patternTable.end()),
	m_textPatternList(),
	m_commentPatternList(),
	m_rootPatternList(),
	m_piPatternList(),
	m_nodePatternList(),
	m_anyPatternList(),
	m_matchPattern2Container(),
	m_patternCount(0),
	m_attributeSets(),
	m_attributeSetsSize(0),
	m_surrogateChildren(*this),
	m_elemDecimalFormats(),
	m_prefixAliases(),
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
		catch(const XMLException&)
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

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);
		const XalanDOMChar* const	value = atts.getValue(i);

		const bool	isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

		if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
		{
			const XalanDOMString	p = isPrefix ? substring(aname, 6) : XalanDOMString();

			namespaces.push_back(NameSpace(p, XalanDOMString(value)));
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
	m_namespacesHandler.postConstruction();


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

	// OK, now we need to add everything template that matches "node()"
	// to the end of the text, comment, and PI template lists.
	PatternTableListType::iterator	theBegin = m_nodePatternList.begin();
	PatternTableListType::iterator	theEnd = m_nodePatternList.end();

	if (theBegin != theEnd)
	{
		m_textPatternList.insert(
			m_textPatternList.end(),
			theBegin,
			theEnd);

		m_commentPatternList.insert(
			m_commentPatternList.end(),
			theBegin,
			theEnd);

		m_piPatternList.insert(
			m_piPatternList.end(),
			theBegin,
			theEnd);
	}

	m_nodePatternList.insert(
			theEnd,
			m_anyPatternList.begin(),
			m_anyPatternList.end());

	theBegin = m_nodePatternList.begin();
	theEnd = m_nodePatternList.end();

	if (theBegin != theEnd)
	{
		PatternTableMapType::iterator	i =
				m_patternTable.begin();

		while(i != m_patternTable.end())
		{
			PatternTableListType&	theTable = (*i).second;

			theTable.insert(
				theTable.end(),
				theBegin,
				theEnd);

			++i;
		}
	}

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
			const XalanDOMString	prefix = substring(attrName, 0, indexOfNSSep);
			const XalanDOMString*	ns = getNamespaceForPrefixFromStack(prefix);

			attrOK = ns != 0 && !::isEmpty(*ns) && !equals(*ns, constructionContext.getXSLTNamespaceURI());
		}
		else
		{
			attrOK = false;
		}
	}

	return attrOK;
}



const XalanDOMString*
Stylesheet::getNamespaceFromStack(const XalanDOMChar* 	nodeName) const
{
	assert(nodeName != 0);

	const XalanDOMString::size_type		indexOfNSSep = indexOf(nodeName, XalanUnicode::charColon);

	const XalanDOMString	prefix =
		indexOfNSSep < length(nodeName) ?
				substring(nodeName, 0, indexOfNSSep) :
				XalanDOMString();

	return getNamespaceForPrefixFromStack(prefix);
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



void
Stylesheet::addTemplate(
			ElemTemplate*					theTemplate,
			StylesheetConstructionContext&	constructionContext)
{
	assert(theTemplate != 0);

	unsigned int	pos = 0;

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

			pos++;

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
		typedef XPath::TargetElementStringsVectorType	TargetElementStringsVectorType;

		TargetElementStringsVectorType		strings;

		xp->getTargetElementStrings(strings);

		TargetElementStringsVectorType::size_type	nTargets =
				strings.size();

		if(nTargets != 0)
		{
			for(TargetElementStringsVectorType::size_type stringIndex = 0;
								stringIndex < nTargets; stringIndex++) 
			{
				const XalanDOMString& target = strings[stringIndex];

				m_matchPattern2Container.push_back(
					MatchPattern2(
						*theTemplate,
						pos,
						target,
						*xp,
						xp->getExpression().getCurrentPattern()));

				const MatchPattern2* const	newMatchPat =
					&m_matchPattern2Container.back();

				// Always put things on the front of the list, so
				// templates later in the stylesheet are always
				// selected first.
				if (equals(target, XPath::PSEUDONAME_TEXT) == true)
				{
					m_textPatternList.insert(
						m_textPatternList.begin(),
						newMatchPat);
				}
				else if (equals(target, XPath::PSEUDONAME_COMMENT) == true)
				{
					m_commentPatternList.insert(
						m_commentPatternList.begin(),
						newMatchPat);
				}
				else if (equals(target, XPath::PSEUDONAME_ROOT) == true)
				{
					m_rootPatternList.insert(
						m_rootPatternList.begin(),
						newMatchPat);
				}
				else if (equals(target, XPath::PSEUDONAME_PI) == true)
				{
					m_piPatternList.insert(
						m_piPatternList.begin(),
						newMatchPat);
				}
				else if (equals(target, XPath::PSEUDONAME_NODE) == true)
				{
					m_nodePatternList.insert(
						m_nodePatternList.begin(),
						newMatchPat);
				}
				else if (equals(target, XPath::PSEUDONAME_ANY) == true)
				{
					m_anyPatternList.insert(
						m_anyPatternList.begin(),
						newMatchPat);
				}
				else
				{
					// Put it in the map.
					PatternTableListType&	theTable =
						m_patternTable[target];

					theTable.insert(
						theTable.begin(),
						newMatchPat);
				}
			}
		}
	}
}



const ElemTemplate*
Stylesheet::findNamedTemplate(const XalanQName&		qname) const
{
	ElemTemplateMapType::const_iterator it = m_namedTemplates.find(qname);

	if(it != m_namedTemplates.end())
	{
		return (*it).second;
	}
	else
	{
		const ElemTemplate*		namedTemplate = 0;

		// Look for the template in the imports
		const StylesheetVectorType::size_type	nImports = m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
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
Stylesheet::locateMatchPatternList2(const XalanDOMString&	theName) const
{
	assert(m_patternTableEnd == m_patternTable.end());

	const PatternTableMapType::const_iterator	i =
		m_patternTable.find(theName);

	if (i != m_patternTableEnd)
	{
		return &(*i).second;
	}
	else
	{
		return &m_nodePatternList;
	}
}



inline const Stylesheet::PatternTableListType*
Stylesheet::locateMatchPatternList2(const XalanNode&	theNode) const
{
	switch(theNode.getNodeType())
	{
	case XalanNode::ELEMENT_NODE:
		return locateMatchPatternList2(DOMServices::getLocalNameOfNode(theNode));
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		return &m_piPatternList;
		break;

	case XalanNode::ATTRIBUTE_NODE:
		return locateMatchPatternList2(DOMServices::getLocalNameOfNode(theNode));
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

	case XalanNode::DOCUMENT_FRAGMENT_NODE:
		return &m_anyPatternList;
		break;

	default:
		return &m_nodePatternList;
		break;
	}

	return locateMatchPatternList2(theNode.getNodeName());
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
	else
	{
		const ElemTemplate*		bestMatchedRule = 0;
		const MatchPattern2*	bestMatchedPattern = 0; // Syncs with bestMatchedRule
		const double			matchScoreNoneValue = 
			XPath::getMatchScoreValue(XPath::eMatchScoreNone);

		double					bestMatchPatPriority = matchScoreNoneValue;
			

		unsigned int			nConflicts = 0;

		// Use a stack-based array when possible...
		const MatchPattern2*	conflictsArray[100];

		XalanArrayAutoPtr<const MatchPattern2*>		conflictsVector;

		const MatchPattern2**	conflicts = 0;

		if(onlyUseImports == false)
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

			if (theCurrentEntry != theTableEnd)
			{
				const XalanDOMString*	prevPat = 0;
				const MatchPattern2*	prevMatchPat = 0;
				double					prevMatchPatPriority = matchScoreNoneValue;

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
							prevMatchPatPriority = matchPatPriority;
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
			}
		} // end if(useImports == false)

		if(0 == bestMatchedRule)
		{
			assert(m_importsSize == m_imports.size());

			for(unsigned int i = 0; i < m_importsSize; i++)
			{
				const Stylesheet* const 	stylesheet =
					m_imports[i];

				bestMatchedRule = stylesheet->findTemplate(executionContext,
														   targetNode,
														   mode, 
														   false);
				if(0 != bestMatchedRule)
					break;
			}
		}

		if(nConflicts > 0)
		{
			assert(conflicts != 0);

			const bool		quietConflictWarnings = executionContext.getQuietConflictWarnings();

			XalanDOMString	conflictsString;
			
			if (quietConflictWarnings == false)
			{
				conflictsString = XALAN_STATIC_UCODE_STRING("Specificity conflicts found: ");
			}

			for(unsigned int i = 0; i < nConflicts; i++)
			{
				const MatchPattern2* const	conflictPat = conflicts[i];

				if(0 != i)
				{
					if(quietConflictWarnings == false)
					{
						conflictsString += XALAN_STATIC_UCODE_STRING(", ");
					}
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

				if(quietConflictWarnings == false)
				{
					conflictsString += XalanDOMString(XALAN_STATIC_UCODE_STRING("\"")) +
										*conflictPat->getPattern() +
										XalanDOMString(XALAN_STATIC_UCODE_STRING("\""));
				}
			}

			bestMatchedRule = bestMatchedPattern->getTemplate();

			if(quietConflictWarnings == false)
			{
				conflictsString += XALAN_STATIC_UCODE_STRING(" ");
				conflictsString += XALAN_STATIC_UCODE_STRING("Last found in stylesheet will be used.");
				executionContext.warn(conflictsString);
			}
		}

		return bestMatchedRule;
	}
}



void
Stylesheet::addExtensionNamespace(
			const XalanDOMString&	uri,
			ExtensionNSHandler*		nsh)
{
	m_extensionNamespaces.insert(ExtensionNamespacesMapType::value_type(uri, nsh));

	m_namespacesHandler.addExtensionNamespaceURI(uri);
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
				Constants::ELEMNAME_PARAMVARIABLE == var->getXSLToken();

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
#if 1
		// $$$ ToDo: Enable other code.  Perhaps an error?
		m_prefixAliases[*stylesheetNamespace] = *resultNamespace;

		m_namespacesHandler.setNamespaceAlias(*stylesheetNamespace, *resultNamespace);
#else
		const PrefixAliasesMapType::iterator	i =
			m_prefixAliases.find(*stylesheetNamespace);

		if (i != m_prefixAliases.end())
		{
			// $$$ ToDo: This could also be an error?
			(*i).second = *resultNamespace;
		}
		else
		{
			m_prefixAliases.insert(PrefixAliasesMapType::value_type(*stylesheetNamespace, *resultNamespace));
		}
#endif
	}
}


#if 0
XalanDOMString
Stylesheet::getAliasNamespaceURI(const XalanDOMChar*	uri) const
{
	assert(uri != 0);

	return getAliasNamespaceURI(XalanDOMString(uri));
}



XalanDOMString
Stylesheet::getAliasNamespaceURI(const XalanDOMString&	uri) const
{
	const StringToStringMapType::const_iterator	i =
		m_prefixAliases.find(uri);

	if (i != m_prefixAliases.end())
	{
		assert(length((*i).second) > 0);

		return (*i).second;
	}
	else
	{
		XalanDOMString	theResult;

		const StylesheetVectorType::size_type	nImports =
			m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
		{
			theResult = m_imports[i]->getAliasNamespaceURI(uri);

			if(length(theResult) != 0)
			{
				break;
			}
		}

		return theResult;
	}
}
#endif


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
			const QNameVectorType&			attributeSetsNames,
			StylesheetExecutionContext& 	executionContext,			
			XalanNode*						sourceNode) const
{
	const QNameVectorType::size_type	nNames = attributeSetsNames.size();

	if(0 != nNames)
	{
		// Process up the import chain...
		const StylesheetVectorType::const_reverse_iterator	theEnd = m_imports.rend();
		StylesheetVectorType::const_reverse_iterator		i = m_imports.rbegin();

		while(i != theEnd)
		{
			(*i)->applyAttrSets(
				attributeSetsNames, 
				executionContext,
				sourceNode);

			++i;
		}

		for(QNameVectorType::size_type j = 0; j < nNames; j++)
		{
			const XalanQName&	qname = attributeSetsNames[j];

			assert(m_attributeSetsSize == m_attributeSets.size());

			for(StylesheetVectorType::size_type k = 0; k < m_attributeSetsSize; k++)
			{
				const ElemAttributeSet* const	attrSet = m_attributeSets[k];
				assert(attrSet != 0);

				if(qname.equals(attrSet->getQName()))
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



unsigned long
Stylesheet::getNumber() const
{
	// This member functionshould not be called
	assert(false);	

	return 0;
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
