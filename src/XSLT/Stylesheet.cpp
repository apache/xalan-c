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



#include <sax/AttributeList.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <PlatformSupport/STLHelper.hpp>



#include <XMLSupport/XMLParserLiaison.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPath.hpp>



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



const XalanDOMString	Stylesheet::s_emptyString;



Stylesheet::Stylesheet(
		StylesheetRoot& 				root,
		const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	XalanDocument(),
	PrefixResolver(),
	m_stylesheetRoot(root),
	m_needToBuildKeysTable(false),
	m_baseIdent(baseIdentifier),
	m_keyDeclarations(),
	m_XSLTNamespaceURI(constructionContext.getXSLTNamespaceURI()),
	m_whitespacePreservingElements(),
	m_whitespaceStrippingElements(),	
	m_imports(),
	m_namespaces(),
	m_namespaceDecls(),
	m_tablesAreInvalid(true),
	m_isWrapperless(false),
	m_wrapperlessTemplate(0),
	m_extensionNamespaces(),
	m_firstTemplate(0),
	m_includeStack(),
	m_defaultSpace(true),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_XSLTVerDeclared(1.0L),
	m_isRoot(&root == this ? true: false),
	m_patternTable(),
	m_attributeSets(),
	m_surrogateChildren(*this),
	m_fakeAttributes(),
	m_elemDecimalFormats(),
	m_prefixAliases(),
	m_namespacesHandler()
{
	if (length(m_baseIdent) != 0)
	{
		const XalanDOMString urlString = constructionContext.getURLStringFromString(m_baseIdent);

		if (length(urlString) != 0)
		{
			m_includeStack.push_back(urlString);
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

	// Clean up the atribute sets vector
	for_each(m_topLevelVariables.begin(),
			 m_topLevelVariables.end(),
			 DeleteFunctor<ElemVariable>());

	// Clean up the atribute sets vector
	for_each(m_elemDecimalFormats.begin(),
			 m_elemDecimalFormats.end(),
			 DeleteFunctor<ElemDecimalFormat>());

	// Clean up the extension namespaces vector
	for_each(m_extensionNamespaces.begin(),
			 m_extensionNamespaces.end(),
			 MapValueDeleteFunctor<ExtensionNamespacesMapType>());

	// Clean up the match pattern vector
	for (PatternTableMapType::const_iterator	it = m_patternTable.begin();
			it != m_patternTable.end();
				++it)
	{
		const PatternTableListType&		theList = (*it).second;

		for_each(theList.begin(),
				 theList.end(),
				 DeleteFunctor<MatchPattern2>());
	}

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
						XalanDOMString(atts.getValue(i)),
						*nsContext);
		}
		else if(equals(aname, Constants::ATTRNAME_USE))
		{
			useAttr =
					constructionContext.createXPath(
						atts.getValue(i),
						*nsContext);
		}
		else if (isAttrOK(aname, atts, i, constructionContext) == false)
		{
			constructionContext.error(
				TranscodeFromLocalCodePage("xsl:key, unrecognized keyword '") +
					aname +
					TranscodeFromLocalCodePage("'!"));
		}
	}

	if(0 == nameAttr)
		constructionContext.error(TranscodeFromLocalCodePage("xsl:key	requires a ") + Constants::ATTRNAME_NAME + " attribute!");

	if(0 == matchAttr)
		constructionContext.error(TranscodeFromLocalCodePage("xsl:key	requires a ") + Constants::ATTRNAME_MATCH + " attribute!");

	if(0 == useAttr)
		constructionContext.error(TranscodeFromLocalCodePage("xsl:key	requires a ") + Constants::ATTRNAME_USE + " attribute!");

	m_keyDeclarations.push_back(KeyDeclaration(XalanDOMString(nameAttr), *matchAttr, *useAttr));

	m_needToBuildKeysTable = true;
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



void
Stylesheet::popNamespaces() 
{
	assert(m_namespaces.empty() == false);

	m_namespaces.pop_back(); 
}



void
Stylesheet::postConstruction()
{
	// Call postConstruction() on any imported stylesheets, the get any aliases
	// in reverse order, to preserve import precedence. Also, get any key declarations.
	const StylesheetVectorType::reverse_iterator	theEnd = m_imports.rend();
	StylesheetVectorType::reverse_iterator	i = m_imports.rbegin();

	while(i != theEnd)
	{
		(*i)->postConstruction();

		m_namespacesHandler.copyNamespaceAliases((*i)->getNamespacesHandler());

		// $$ ToDo: Should we clear the imported stylesheet's key
		// declarations after we copy them?
		m_keyDeclarations.insert(
			m_keyDeclarations.end(),
			(*i)->m_keyDeclarations.begin(),
			(*i)->m_keyDeclarations.end());

		++i;
	}

	// We may need to build keys, since we may have inherited them from
	// our imports.
	if (m_needToBuildKeysTable == false && m_keyDeclarations.size() > 0)
	{
		m_needToBuildKeysTable = true;
	}

	// Call postConstruction() on our own namespaces handler...
	m_namespacesHandler.postConstruction();

	ElemTemplateElement* node = m_firstTemplate;

    for (; node != 0; node = node->getNextSiblingElem())
    {
		node->postConstruction(m_namespacesHandler);
	}

    for (node = m_wrapperlessTemplate; node != 0; node = node->getNextSiblingElem())
    {
		node->postConstruction(m_namespacesHandler);
	}
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
		const unsigned int	indexOfNSSep = indexOf(attrName, XalanUnicode::charColon);

		if(indexOfNSSep < length(attrName))
		{
			const XalanDOMString	prefix = substring(attrName, 0, indexOfNSSep);
			const XalanDOMString&	ns = getNamespaceForPrefixFromStack(prefix);

			attrOK = ! ::isEmpty(ns) && !equals(ns,constructionContext.getXSLTNamespaceURI());
		}
		else
		{
			attrOK = false;
		}
	}

	return attrOK;
}



const XalanDOMString&
Stylesheet::getNamespaceFromStack(const XalanDOMString&		nodeName) const
{
	return getNamespaceFromStack(c_wstr(nodeName));
}



const XalanDOMString&
Stylesheet::getNamespaceFromStack(const XalanDOMChar* 	nodeName) const
{
	assert(nodeName != 0);

	const unsigned int		indexOfNSSep = indexOf(nodeName, XalanUnicode::charColon);

	const XalanDOMString	prefix =
		indexOfNSSep < length(nodeName) ?
				substring(nodeName, 0, indexOfNSSep) :
				XalanDOMString();

	return getNamespaceForPrefixFromStack(prefix);
}



const XalanDOMString&
Stylesheet::getNamespaceForPrefix(const XalanDOMString& 	prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaceDecls, prefix);
}



const XalanDOMString&
Stylesheet::getNamespaceForPrefixFromStack(const XalanDOMString&	prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaces, prefix);
}



const XalanDOMString&
Stylesheet::getNamespaceForPrefixFromStack(const XalanDOMChar*	prefix) const
{
	assert(prefix != 0);

	return QName::getNamespaceForPrefix(m_namespaces, XalanDOMString(prefix));
}



bool
Stylesheet::getYesOrNo(
			const XalanDOMChar* 			aname,
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
		constructionContext.error(XalanDOMString(val) + " is unknown value for " + aname);

		return false;
	}
}



void
Stylesheet::addTemplate(
			ElemTemplate*					tmpl,
			StylesheetConstructionContext&	constructionContext)
{
	unsigned int	pos = 0;

	if(0 == m_firstTemplate)
	{
		m_firstTemplate = tmpl;
	}
	else
	{
		ElemTemplateElement*	next = m_firstTemplate;

		// Find the last one, then append the new one.
		while(0 != next)
		{
			if(0 == next->getNextSiblingElem())
			{
				next->setNextSiblingElem(tmpl);
				tmpl->setNextSiblingElem(0); // just to play it safe.
				tmpl->setPreviousSiblingElem(next);
				break;
			}

			pos++;

			next = next->getNextSiblingElem();
		}
	}

	// If it's a named template, then we need to
	// and it to the map of named templates.
	const QName&	theName = tmpl->getName();

	if(theName.isEmpty() == false)
	{
		if (m_namedTemplates.find(theName) == m_namedTemplates.end())
		{
			m_namedTemplates[theName] = tmpl;
		}
		else
		{
			// This is an error...
			XalanDOMString	theMessage(TranscodeFromLocalCodePage("The stylesheet already has a template with the name "));

			const XalanDOMString&	theNamespace = theName.getNamespace();

			if (length(theNamespace) != 0)
			{
				theMessage += theNamespace;
				theMessage += DOMServices::s_XMLNamespaceSeparatorString;
			}

			theMessage += theName.getLocalPart();

			constructionContext.error(theMessage, 0, tmpl);
		}
	}

	// Now, process the match pattern associated with the
	// template.
	const XPath* const	xp = tmpl->getMatchPattern();

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

				MatchPattern2* newMatchPat =
					new MatchPattern2(xp->getExpression().getCurrentPattern(), 
						xp, tmpl, pos, target, this);

				// Put it in the map, on the front of the list for that
				// target string, so that templates later in the stylesheet
				// are always selected first.
				m_patternTable[target].push_front(newMatchPat);
			}
		}
	}
}
	  


const ElemTemplate*
Stylesheet::findNamedTemplate(
			const XalanDOMString&			name,
			StylesheetExecutionContext& 	executionContext) const
{
	return findNamedTemplate(QName(name, m_namespaces), executionContext);
}



const ElemTemplate*
Stylesheet::findNamedTemplate(
			const QName&					qname,
			StylesheetExecutionContext& 	executionContext) const
{
	const ElemTemplate*		namedTemplate = 0;

	ElemTemplateMapType::const_iterator it = m_namedTemplates.find(qname);

	// Look for the template in the imports
	if(it == m_namedTemplates.end())
	{
		const StylesheetVectorType::size_type	nImports = m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
		{
			const Stylesheet* const stylesheet = m_imports[i];

			namedTemplate = stylesheet->findNamedTemplate(qname, executionContext);

			if(0 != namedTemplate)
				break;
		}
	}
	else
	{
		namedTemplate = (*it).second;
	}

	if(0 == namedTemplate)
	{
		executionContext.warn("Could not find xsl:template named: " + qname.getLocalPart());
	}

	return namedTemplate;
}
	


const ElemTemplate*
Stylesheet::findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						targetNode) const
{
	const Stylesheet*	theDummy;

	return findTemplate(executionContext, sourceTree, targetNode, QName(), false, theDummy);
}



const ElemTemplate*
Stylesheet::findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						targetNode, 
			const QName&					mode,
			bool							useImports,
			const Stylesheet*&				foundStylesheet) const
{
	assert(sourceTree != 0);
	assert(targetNode != 0);

	const ElemTemplate*		theResult = 0;

	if(m_isWrapperless)
	{
		theResult = m_wrapperlessTemplate;
	}
	else
	{
		bool					usedWildcard = false;

		const ElemTemplate*		bestMatchedRule = 0;
		const MatchPattern2*	bestMatchedPattern = 0; // Syncs with bestMatchedRule
		double					bestMatchPatPriority = XPath::s_MatchScoreNone;

		PatternTableVectorType	conflicts;

		if(useImports == false)
		{
			// Points to the current list of match patterns.  Note
			// that this may point to more than one table.
			const PatternTableListType* 	matchPatternList = 0;
			const XalanNode::NodeType		targetNodeType = targetNode->getNodeType();

			switch(targetNodeType)
			{
			case XalanNode::ELEMENT_NODE:
				{
					const XalanDOMString&	targetName = DOMServices::getLocalNameOfNode(*targetNode);

					matchPatternList = locateMatchPatternList2(targetName, true);
				}
				break;

			case XalanNode::PROCESSING_INSTRUCTION_NODE:
			case XalanNode::ATTRIBUTE_NODE:
				matchPatternList = locateMatchPatternList2(targetNode->getNodeName(), true);
				break;

			case XalanNode::CDATA_SECTION_NODE:
			case XalanNode::TEXT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_TEXT, true);
				break;

			case XalanNode::COMMENT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_COMMENT, true);
				break;

			case XalanNode::DOCUMENT_NODE:
					matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ROOT, false);
				break;

			case XalanNode::DOCUMENT_FRAGMENT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ANY, false);
				break;

			default:
				{
					matchPatternList = locateMatchPatternList2(targetNode->getNodeName(), true);
				}
			}

			if (matchPatternList != 0)
			{
				const XalanDOMString*	prevPat = 0;
				const MatchPattern2*	prevMatchPat = 0;
				double					prevMatchPatPriority = XPath::s_MatchScoreNone;

				// These are iterators into the current table.
				// Note that we may re-seat these iterators to
				// point into a different table, if we have
				// to match wildcards.
				PatternTableListType::const_iterator	theCurrentEntry =
					matchPatternList->begin();

				PatternTableListType::const_iterator	theTableEnd =
					matchPatternList->end();

				while(theCurrentEntry != theTableEnd)
				{
					const MatchPattern2*	matchPat = *theCurrentEntry;
					double					matchPatPriority = XPath::s_MatchScoreNone;
					assert(matchPat != 0);

					const ElemTemplate*		rule = matchPat->getTemplate();

					// We'll be needing to match rules according to what 
					// mode we're in.
					const QName&			ruleMode =
						rule->getMode();

					// The logic here should be that if we are not in a mode AND
					// the rule does not have a node, then go ahead.
					// OR if we are in a mode, AND the rule has a node, 
					// AND the rules match, then go ahead.

					bool haveMode = !mode.isEmpty();
					bool haveRuleMode = !ruleMode.isEmpty();

					if ( (!haveMode && !haveRuleMode) || (haveMode && haveRuleMode && ruleMode.equals(mode)))
					{
						const XalanDOMString&	patterns = matchPat->getPattern();

						if(!isEmpty(patterns) &&
						   !(prevMatchPat != 0 &&
						     (prevPat != 0 && equals(*prevPat, patterns)) &&
							 prevMatchPat->getTemplate()->getPriority() == matchPat->getTemplate()->getPriority()))
						{
							prevPat = &patterns;
							prevMatchPat = matchPat;
							prevMatchPatPriority = matchPatPriority;
							matchPatPriority = XPath::s_MatchScoreNone;

							const XPath* const	xpath = matchPat->getExpression();

							double score =
								xpath->getMatchScore(targetNode, *this, executionContext);

							if(XPath::s_MatchScoreNone != score)
							{
								const double priorityVal = rule->getPriority();
								const double priorityOfRule 
									  = (XPath::s_MatchScoreNone != priorityVal) 
															  ? priorityVal : score;

								matchPatPriority = priorityOfRule;
								const double priorityOfBestMatched =
										(0 != bestMatchedPattern) ?
												bestMatchPatPriority : 
												XPath::s_MatchScoreNone;

								if(priorityOfRule > priorityOfBestMatched)
								{
									conflicts.clear();
									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
									bestMatchPatPriority = matchPatPriority;
								}
								else if(priorityOfRule == priorityOfBestMatched)
								{
									addObjectIfNotFound(bestMatchedPattern, conflicts);
									conflicts.push_back(matchPat);
									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
									bestMatchPatPriority = matchPatPriority;
								}
							}
						}
					}

					theCurrentEntry++;

					// We also have to consider wildcard matches.
					if(theCurrentEntry == theTableEnd &&
					   equals(matchPat->getTargetString(),
							  Constants::PSEUDONAME_ANY) == false
//						&& (XalanNode::ELEMENT_NODE == targetNodeType || 
//							XalanNode::ATTRIBUTE_NODE == targetNodeType ||
//							XalanNode::PROCESSING_INSTRUCTION_NODE == targetNodeType)
						)
					{
						{
							assert(usedWildcard==false);	// Should only be here once ??
							usedWildcard = true;
							const PatternTableMapType::const_iterator 	theTableIterator =
								m_patternTable.find(Constants::PSEUDONAME_ANY);

							assert(m_patternTable.size());
							if (theTableIterator != m_patternTable.end())
							{
								// Re-seat the iterators...
								theCurrentEntry = (*theTableIterator).second.begin();
								theTableEnd = (*theTableIterator).second.end();
							}
						}
					}
				}	// end while
			} // end if (matchPatternList != 0)
		} // end if(useImports == false)

		// @@ JMD: Here we are using the imports anyway if bestMatchedRule is zero,
		// instead of just doing if (useImports) {...} else.  Is this right ??
		// Does this assume that bestMatchedRule will always be non-zero exit the
		// if clause, and, if so, is it an error if it's not ?
		// else
		if(0 == bestMatchedRule)
		{
			const unsigned int	nImports = m_imports.size();

			for(unsigned int i = 0; i < nImports; i++)
			{
				const Stylesheet* const 	stylesheet =
					m_imports[i];

				bestMatchedRule = stylesheet->findTemplate(executionContext,
														   sourceTree,
														   targetNode,
														   mode, 
														   false,
														   foundStylesheet);
				if(0 != bestMatchedRule)
					break;
			}
		}
		
		const unsigned int	nConflicts = conflicts.size();

		if(nConflicts > 0)
		{
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
										conflictPat->getPattern() +
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

		if((0 != bestMatchedPattern) && (0 != foundStylesheet))
		{
			foundStylesheet = bestMatchedPattern->getStylesheet();
		}

		theResult = bestMatchedRule;
	}

	return theResult;
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



const Stylesheet::PatternTableListType*
Stylesheet::locateMatchPatternList2(XalanNode*	sourceNode) const
{
	assert(sourceNode != 0);

	const PatternTableListType* 	matchPatternList = 0;

	switch(sourceNode->getNodeType())
	{
	case XalanNode::ELEMENT_NODE:
		{
			const XalanDOMString&	targetName =
				DOMServices::getLocalNameOfNode(*sourceNode);

			matchPatternList = locateMatchPatternList2(targetName, true);
		}
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
	case XalanNode::ATTRIBUTE_NODE:
		matchPatternList = locateMatchPatternList2(sourceNode->getNodeName(), true);
		break;

	case XalanNode::CDATA_SECTION_NODE:
	case XalanNode::TEXT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_TEXT, false);
		break;

	case XalanNode::COMMENT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_COMMENT, false);
		break;

	case XalanNode::DOCUMENT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_ROOT, false);
		break;

	default:
		matchPatternList = locateMatchPatternList2(sourceNode->getNodeName(), false);
		break;
	}

	return matchPatternList;
}

/**
 * Given an element type, locate the start of a linked list of 
 * possible tmpl matches.
 */
const Stylesheet::PatternTableListType* 
Stylesheet::locateMatchPatternList2(
			const XalanDOMString&	sourceElementType,
			bool					tryWildCard) const
{
	const PatternTableListType* 	theMatchList = 0;

	PatternTableMapType::const_iterator 	i =
		m_patternTable.find(sourceElementType);

	if (i != m_patternTable.end())
	{
		theMatchList = &(*i).second;
		assert(theMatchList != 0);
	}
	else if(tryWildCard == true)
	{
		i = m_patternTable.find(Constants::PSEUDONAME_ANY);

		if (i != m_patternTable.end())
		{
			theMatchList = &(*i).second;
			assert(theMatchList != 0);
		}
	}

	return theMatchList;
}



Stylesheet::MatchPattern2::MatchPattern2(
			const XalanDOMString&	pat,
			const XPath*			exp,
			const ElemTemplate*		theTemplate,
			int 					posInStylesheet, 
			const XalanDOMString&	targetString,
			const Stylesheet* 		stylesheet) :
	m_stylesheet(stylesheet),
	m_targetString(targetString),
	m_expression(exp),
	m_posInStylesheet(posInStylesheet),
	m_pattern(pat),
	m_template(theTemplate)
{
}



Stylesheet::MatchPattern2::~MatchPattern2()
{
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
	ParamVectorType::size_type			i = 0;
	const ParamVectorType::size_type	nVars = m_topLevelVariables.size();

	for(; i < nVars; i++)
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

					const XObject* const	theXObject = arg.getXObject();

					if (theXObject != 0)
					{
						executionContext.pushVariable(arg.getName(),
													  theXObject,
													  0);
					}
					else
					{
						executionContext.pushVariable(arg.getName(),
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
			XalanNode* const	doc = executionContext.getRootDocument();
			assert(doc != 0);

			var->execute(executionContext,
						 doc,
						 doc);
		}
	}

	// Now, push any imports...
	const ParamVectorType::size_type	nImports = m_imports.size();

	for(i = 0; i < nImports; i++)
	{
		const Stylesheet* const stylesheet = m_imports[i];

		stylesheet->pushTopLevelVariables(executionContext, topLevelParams);
	}
}



const XalanDOMString&
Stylesheet::getCurrentIncludeBaseIdentifier() const
{
	if (m_includeStack.size() == 0)
	{
		return getBaseIdentifier();
	}
	else
	{
		return m_includeStack.back();
	}
}



void
Stylesheet::processNSAliasElement(
			const XalanDOMChar*				name,
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
				stylesheetNamespace = &getNamespaceForPrefix(dummy);
			}
			else
			{
				stylesheetNamespace = &getNamespaceForPrefix(XalanDOMString(value));
			}
		}
		else if(equals(aname, Constants::ATTRNAME_RESULT_PREFIX))
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				resultNamespace = &getNamespaceForPrefix(dummy);
			}
			else
			{
				resultNamespace = &getNamespaceForPrefix(XalanDOMString(value));
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
		}
	}

	// Build a table of aliases, the key is the stylesheet uri and the
	// value is the result uri
	if (length(*stylesheetNamespace) != 0 &&
		length(*resultNamespace) != 0)
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
	else
	{
		constructionContext.error("Missing namespace URI for specified prefix");
	}
}



XalanDOMString
Stylesheet::getAliasNamespaceURI(const XalanDOMChar*	uri) const
{
	assert(uri != 0);

	return getAliasNamespaceURI(XalanDOMString(uri));
}



XalanDOMString
Stylesheet::getAliasNamespaceURI(const XalanDOMString&	uri) const
{
	XalanDOMString	result;

	const StringToStringMapType::const_iterator	i =
		m_prefixAliases.find(uri);

	if (i != m_prefixAliases.end())
	{
		result = (*i).second;

		assert(length(result) > 0);
	}
	else
	{
		const StylesheetVectorType::size_type	nImports =
			m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
		{
			result = m_imports[i]->getAliasNamespaceURI(uri);

			if(length(result) != 0)
			{
				break;
			}
		}
	}

	return result;
}



void
Stylesheet::processExcludeResultPrefixes(
		const XalanDOMChar*				theValue,
		StylesheetConstructionContext&	theConstructionContext)
{
	m_namespacesHandler.processExcludeResultPrefixes(theValue, m_namespaces, theConstructionContext);
}



const Stylesheet*
Stylesheet::getPreviousImport(const Stylesheet*		stylesheet) const
{
	const Stylesheet*	previous = 0;

	const StylesheetVectorType::size_type	nImports =
			m_imports.size();

	for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
	{
		if (m_imports[i] == stylesheet)
		{
			if (i + 1 < nImports)
			{
				previous = m_imports[i + 1];

				break;
			}
		}
	}

	return previous;
}



void
Stylesheet::processDecimalFormatElement(
			ElemDecimalFormat*				elemDecimalFormat,
			const AttributeList&			/* atts */,
			StylesheetConstructionContext&	/* constructionContext */)
{
	assert(elemDecimalFormat != 0);

	m_elemDecimalFormats.push_back(elemDecimalFormat);
}



const XalanDecimalFormatSymbols*
Stylesheet::getDecimalFormatSymbols(const XalanDOMString&	name) const
{
	const XalanDecimalFormatSymbols* 				dfs = 0;

	const ElemDecimalFormatVectorType::size_type	theSize =
		m_elemDecimalFormats.size();

	if(theSize > 0)
	{
		// Start from the top of the stack
		for (int i = theSize - 1; i >= 0; --i)
		{
			assert(m_elemDecimalFormats[i] != 0);

			if (equals(m_elemDecimalFormats[i]->getName(), name) == true)
			{
				dfs = &m_elemDecimalFormats[i]->getDecimalFormatSymbols();

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
		const StylesheetVectorType::size_type	nImports =
			m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; ++i)
		{
			dfs = m_imports[i]->getDecimalFormatSymbols(name);

			if(dfs != 0)
			{
				break;
			}
		}
	}

	return dfs;
}



/**
 * Add an attribute set to the list.
 */
void
Stylesheet::addAttributeSet(
		const QName&		/* qname */, 
		ElemAttributeSet*	attrSet)
{
	m_attributeSets.push_back(attrSet);
}		



/**
 * Add the attributes from the named attribute sets to the attribute list.
 * TODO: Error handling for: "It is an error if there are two attribute sets 
 * with the same expanded-name and with equal import precedence and that both 
 * contain the same attribute unless there is a definition of the attribute 
 * set with higher import precedence that also contains the attribute."
 */
void
Stylesheet::applyAttrSets(
			const QNameVectorType&			attributeSetsNames, 
			StylesheetExecutionContext& 	executionContext, 
			XalanNode*						sourceTree, 
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	const QNameVectorType::size_type	nNames = attributeSetsNames.size();

	if(0 != nNames)
	{
		// Process up the import chain...
		const StylesheetVectorType::size_type	nImports =
			m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < nImports; i++)
		{
			const Stylesheet* const 	stylesheet = m_imports[i];

			stylesheet->applyAttrSets(attributeSetsNames, 
									 executionContext, sourceTree, sourceNode, mode);
		}

		for(QNameVectorType::size_type j = 0; j < nNames; j++)
		{
			const QName&							qname = attributeSetsNames[j];
			const StylesheetVectorType::size_type	nSets = m_attributeSets.size();

			for(StylesheetVectorType::size_type k = 0; k < nSets; k++)
			{
				const ElemAttributeSet* const	attrSet = m_attributeSets[k];
				assert(attrSet != 0);

				if(qname.equals(attrSet->getQName()))
				{
					attrSet->execute(executionContext, sourceTree, sourceNode, mode);
				}
			}
		}
	}
}	



const Stylesheet::NamespaceVectorType&
Stylesheet::getCurrentNamespace() const
{
	if (m_namespaces.size() > 0)
	{
		return m_namespaces.back();
	}
	else
	{
		return s_emptyNamespace;
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
	return &m_fakeAttributes;
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
	// $$$ ToDo: Is this always true?
	return true;
}



void
Stylesheet::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
Stylesheet::normalize()
{
}



bool
Stylesheet::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
Stylesheet::getNamespaceURI() const
{
	// $$ ToDo: Is this the same value as PrefixResolver::getURI()?
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
	//should not be called
	assert(false);	

	return 0;
}



XalanDocumentFragment*
Stylesheet::createDocumentFragment()
{
	//should not be called
	assert(false);	

	return 0;
}



XalanText*
Stylesheet::createTextNode(const XalanDOMString&	/* data */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanComment*
Stylesheet::createComment(const XalanDOMString& 	/* data */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanCDATASection*
Stylesheet::createCDATASection(const XalanDOMString&	/* data */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanProcessingInstruction*
Stylesheet::createProcessingInstruction(
			const XalanDOMString&	/* target */,
			const XalanDOMString&	/* data */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanAttr*
Stylesheet::createAttribute(const XalanDOMString&	/* name */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanEntityReference*
Stylesheet::createEntityReference(const XalanDOMString& 	/* name */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanDocumentType*
Stylesheet::getDoctype() const
{
	//should not be called
	assert(false);	

	return 0;
}



XalanDOMImplementation*
Stylesheet::getImplementation() const
{
	//should not be called
	assert(false);	

	return 0;
}



XalanElement*
Stylesheet::getDocumentElement() const
{
	// $$$ ToDo: Is this correct?

	return m_wrapperlessTemplate != 0 ? m_wrapperlessTemplate : m_firstTemplate;
}



XalanNodeList*
Stylesheet::getElementsByTagName(const XalanDOMString&		/* name */) const
{
	return 0;
}



XalanNodeList*
Stylesheet::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
}



XalanNode*
Stylesheet::importNode(
			XalanNode*	/* importedNode */,
			bool		/* deep */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanElement*
Stylesheet::createElementNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanAttr*
Stylesheet::createAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	//should not be called
	assert(false);	

	return 0;
}



XalanElement*
Stylesheet::getElementById(const XalanDOMString&	/* elementId */) const
{
	//should not be called
	assert(false);	

	return 0;
}



bool
Stylesheet::isIndexed() const
{
	//should not be called
	assert(false);	

	return false;
}



const XalanDOMString&
Stylesheet::getURI() const
{
	return m_baseIdent;
}
