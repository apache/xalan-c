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
#include <PlatformSupport/StringTokenizer.hpp>



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



Stylesheet::Stylesheet(
		StylesheetRoot& 				root,
		const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	XalanDocument(),
	PrefixResolver(),
	m_stylesheetRoot(root),
	m_baseIdent(baseIdentifier),
	m_document(),
	m_key_tables(),
	m_keyDeclarations(),
	m_needToBuildKeysTable(false),
	m_imports(),
	m_defaultATXpath(0),
	m_namespaces(),
	m_namespaceDecls(),
	m_tablesAreInvalid(true),
	m_isWrapperless(false),
	m_wrapperlessTemplate(0),
	m_extensionNamespaces(),
	m_firstTemplate(0),
	m_includeStack(),
	m_defaultSpace(true),
	m_XSLTNamespaceURI(constructionContext.getXSLTNamespaceURI()),
	m_whitespacePreservingElements(),
	m_whitespaceStrippingElements(),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_XSLTVerDeclared(1.0L),
	m_isRoot(&root == this ? true: false),
	m_patternTable(),
	m_attributeSets(),
	m_surrogateChildren(*this),
	m_elemDecimalFormats()
{
	if (length(m_baseIdent) != 0)
	{
		typedef StylesheetConstructionContext::URLAutoPtrType	URLAutoPtrType;

		URLAutoPtrType	url = constructionContext.getURLFromString(m_baseIdent);

		if (url.get() != 0)
		{
			m_includeStack.push_back(url.get());

			url.release();
		}
	}
}



Stylesheet::~Stylesheet()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the vector.
	for_each(m_imports.begin(),
			 m_imports.end(),
			 DeleteFunctor<Stylesheet>());

	// Clean up the key table vector
	for_each(m_key_tables.begin(),
			 m_key_tables.end(),
			 DeleteFunctor<KeyTable>());

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
		const PatternTableListType&		theList = it->second;

		for_each(theList.begin(),
				 theList.end(),
				 DeleteFunctor<MatchPattern2>());
	}

	delete m_wrapperlessTemplate;

	delete m_firstTemplate;
}



/**
 * Process the xsl:key element.
 * 
 * (Notes to myself)
 * What we need to do is:
 * 1) As this function is called, build a table of KeyDeclarations.
 * 2) During either XML processing, or upon request, walk the XML 
 * document tree, and build a hash table:
 * a) keyed by name,
 * b) each with a value of a hashtable, keyed by the value returned by 
 *	  the use attribute,
 * c) each with a value that is a nodelist.
 * Thus, for a given key or keyref, look up hashtable by name, 
 * look up the nodelist by the given reference.
 */
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
			constructionContext.createMatchPattern(XalanDOMString(atts.getValue(i)),
			*nsContext);
		}
		else if(equals(aname, Constants::ATTRNAME_USE))
			useAttr = constructionContext.createXPath(atts.getValue(i),
				*nsContext);
		else
		{
			constructionContext.error(
				XalanDOMString("xsl:key, unrecognized keyword '") +
					Constants::ATTRNAME_NAME + 
					XalanDOMString("'!"));
		}
	}

	if(0 == nameAttr)
		constructionContext.error(XalanDOMString("xsl:key	requires a ") + Constants::ATTRNAME_NAME+" attribute!");

	if(0 == matchAttr)
		constructionContext.error(XalanDOMString("xsl:key	requires a ") + Constants::ATTRNAME_MATCH+XalanDOMString(" attribute!"));

	if(0 == useAttr)
		constructionContext.error(XalanDOMString("xsl:key	requires a ") + Constants::ATTRNAME_USE+XalanDOMString(" attribute!"));

	m_keyDeclarations.push_back(KeyDeclaration(nameAttr, *matchAttr, *useAttr));

	m_needToBuildKeysTable = true;
}



void
Stylesheet::pushNamespaces(const AttributeList& atts)
{
	const unsigned int		nAttrs = atts.getLength();

	NamespaceVectorType 	namespaces;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);
		const XalanDOMChar* const	value = atts.getValue(i);

		bool isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

		if (equals(aname, DOMServices::s_XMLNamespace) || isPrefix) 
		{
			XalanDOMString p = isPrefix ? substring(aname, 6) : XalanDOMString();

			namespaces.push_back(NameSpace(p, value));
		}
	}

	m_namespaces.push_back(namespaces);
}



void
Stylesheet::popNamespaces() 
{ 
	assert(m_namespaces.size());
	m_namespaces.pop_back(); 
}



bool
Stylesheet::isAttrOK(
			const XalanDOMChar* 			attrName,
			const AttributeList&			/* atts */,
			int 							/* which */,
			StylesheetConstructionContext&	constructionContext) const
{
	bool attrOK = equals(attrName, DOMServices::s_XMLNamespace) ||
						 startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator);

	if(!attrOK)
	{
		const unsigned int	indexOfNSSep = indexOf(attrName, ':');

		if(indexOfNSSep < length(attrName))
		{
			const XalanDOMString	prefix = substring(attrName, 0, indexOfNSSep);
			const XalanDOMString	ns = getNamespaceForPrefixFromStack(prefix);

			attrOK = indexOf(ns, constructionContext.getXSLTNamespaceURI()) < length(ns);
		}
		else
			attrOK = true;
	}

	return attrOK;
}



XalanDOMString
Stylesheet::getNamespaceFromStack(const XalanDOMString& nodeName) const
{
	const unsigned int		indexOfNSSep = indexOf(nodeName, ':');

	const XalanDOMString	prefix =
		indexOfNSSep < length(nodeName) ?
				substring(nodeName, 0, indexOfNSSep) :
				XalanDOMString();

	return getNamespaceForPrefixFromStack(prefix);
}


  
XalanDOMString
Stylesheet::getNamespaceForPrefix(const XalanDOMString& 	prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaceDecls, prefix);
}



XalanDOMString
Stylesheet::getNamespaceForPrefixFromStack(const XalanDOMString&	prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaces, prefix);
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
		constructionContext.error(XalanDOMString(val) +
									XalanDOMString(" is unknown value for ") +
									aname);

		return false;
	}
}



/**
 * Add a template to the template list.
 */
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

	const QName&	theName = tmpl->getName();

	if(theName.isEmpty() == false)
	{
		if (m_namedTemplates.find(theName) != m_namedTemplates.end())
		{
			XalanDOMString	theMessage("The stylesheet already has a template with the name ");

			const XalanDOMString&	theNamespace = theName.getNamespace();

			if (length(theNamespace) != 0)
			{
				theMessage += theNamespace;
				theMessage += ":";
			}

			theMessage += theName.getLocalPart();

			constructionContext.error(theMessage, 0, tmpl);
		}
		else
		{
			m_namedTemplates[theName] = tmpl;
		}
	}

	const XPath* const	xp = tmpl->getMatchPattern();

	if(0 != xp)
	{
		typedef XPath::TargetElementStringsVectorType	TargetElementStringsVectorType;

		TargetElementStringsVectorType		strings;

		xp->getTargetElementStrings(strings);

		/* Each string has a list of pattern tables associated with it; if the
		 * string is not in the map, then create a list of pattern tables with one
		 * entry for the string, otherwise add to the existing pattern table list
		 * for that string
		 * NOTE: C++ version uses a map keyed on string to a vector of match patterns
		 * while the java version uses a map to a linked list
		 */
		if(0 != strings.size())
		{
			TargetElementStringsVectorType::size_type	nTargets =
				strings.size();

			for(TargetElementStringsVectorType::size_type stringIndex = 0;
								stringIndex < nTargets; stringIndex++) 
			{
				const XalanDOMString& target = strings[stringIndex];

				MatchPattern2* newMatchPat =
					new MatchPattern2(xp->getExpression().getCurrentPattern(), 
						xp, tmpl, pos, target, this);

				// Put it in the map...
				m_patternTable[target].push_front(newMatchPat);
			}
		}
	}
}
	  


/**
 * Locate a macro via the "name" attribute.
 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
 * the error condition is severe enough to halt processing.
 */
ElemTemplateElement*
Stylesheet::findNamedTemplate(
			const XalanDOMString&			name,
			StylesheetExecutionContext& 	executionContext) const
//throws XSLProcessorException
{
	return findNamedTemplate(QName(name, m_namespaces), executionContext);
}



/**
 * Locate a macro via the "name" attribute.
 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
 * the error condition is severe enough to halt processing.
 */
ElemTemplateElement*
Stylesheet::findNamedTemplate(
			const QName&					qname,
			StylesheetExecutionContext& 	executionContext) const
{
	ElemTemplateElement 	*namedTemplate = 0;

	ElemTemplateElementMapType::const_iterator it = m_namedTemplates.find(qname);

	// Look for the template in the imports
	if(it == m_namedTemplates.end())
	{
		const int	nImports = m_imports.size();

		for(int i = 0; i < nImports; i++)
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
		executionContext.warn(XalanDOMString("Could not find macro def named: ") +
											  qname.getLocalPart());
	}

	return namedTemplate;
}
	


XObject*
Stylesheet::getTopLevelVariable(
			const XalanDOMString&			name,
			StylesheetExecutionContext& 	executionContext) const
{
	XObject*	theResult = executionContext.getTopLevelVariable(name);

	if(0 == theResult)
	{
		const unsigned int	nImports = m_imports.size();

		for(unsigned int i = 0; i < nImports; i++)
		{
			const Stylesheet* const		stylesheet = m_imports[i];
			assert(stylesheet != 0);

			theResult = stylesheet->getTopLevelVariable(name, executionContext);

			if(0 != theResult)
			{
				break;
			}
		}
	}

	if(0 == theResult)
	{
		executionContext.warn(XalanDOMString("Could not find variable def for: ") + name);
	}

	return theResult;
}


ElemTemplate*
Stylesheet::findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						targetNode) const
{
	const Stylesheet*	theDummy;

	return findTemplate(executionContext, sourceTree, targetNode, QName(), false, theDummy);
}



ElemTemplate*
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

	ElemTemplate*	theResult = 0;

	if(m_isWrapperless)
	{
		theResult = m_wrapperlessTemplate;
	}
	else
	{
		bool					usedWildcard = false;

		ElemTemplate*			bestMatchedRule = 0;
		const MatchPattern2*	bestMatchedPattern = 0; // Syncs with bestMatchedRule

		PatternTableVectorType	conflicts;

		if(useImports == false)
		{
			//odd that this variable is only set, never read
			double highScore = XPath::s_MatchScoreNone;

			// Points to the current list of match patterns.  Note
			// that this may point to more than one table.
			const PatternTableListType* 	matchPatternList = 0;
			const XalanNode::NodeType		targetNodeType = targetNode->getNodeType();

			switch(targetNodeType)
			{
			case XalanNode::ELEMENT_NODE:
				{
					const XalanDOMString	targetName = DOMServices::getLocalNameOfNode(*targetNode);
					matchPatternList = locateMatchPatternList2(targetName, true);
				}
				break;
				
			case XalanNode::PROCESSING_INSTRUCTION_NODE:
			case XalanNode::ATTRIBUTE_NODE:
				matchPatternList = locateMatchPatternList2(targetNode->getNodeName(), true);
				break;

			case XalanNode::CDATA_SECTION_NODE:
			case XalanNode::TEXT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_TEXT, false);
				break;

			case XalanNode::COMMENT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_COMMENT, false);
				break;

			case XalanNode::DOCUMENT_NODE:
					matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ROOT, false);
				break;

			case XalanNode::DOCUMENT_FRAGMENT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ANY, false);
				break;
				
			default:
				{
					matchPatternList = locateMatchPatternList2(targetNode->getNodeName(), false);
				}
			}

			if (matchPatternList != 0)
			{
				XalanDOMString			prevPat;
				const MatchPattern2*	prevMatchPat = 0;

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
					assert(matchPat != 0);

					ElemTemplate*			rule = matchPat->getTemplate();

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
						const XalanDOMString	patterns = matchPat->getPattern();

						if(!isEmpty(patterns) &&
						   !(prevMatchPat != 0 &&
						     equals(prevPat, patterns) &&
							 prevMatchPat->getTemplate()->getPriority() == matchPat->getTemplate()->getPriority()))
						{
							prevPat = patterns;
							prevMatchPat = matchPat;

							const XPath* const	xpath = matchPat->getExpression();

							double score =
								xpath->getMatchScore(targetNode, *this, executionContext);

							if(XPath::s_MatchScoreNone != score)
							{
								const double priorityVal = rule->getPriority();
								const double priorityOfRule 
									  = (XPath::s_MatchScoreNone != priorityVal) 
															  ? priorityVal : score;

								matchPat->setPriority(priorityOfRule);
								const double priorityOfBestMatched =
										(0 != bestMatchedPattern) ?
												bestMatchedPattern->getPriority() : 
												XPath::s_MatchScoreNone;

								if(priorityOfRule > priorityOfBestMatched)
								{
									conflicts.clear();
									highScore = score;
									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
								}
								else if(priorityOfRule == priorityOfBestMatched)
								{
									addObjectIfNotFound(bestMatchedPattern, conflicts);
									conflicts.push_back(matchPat);
									highScore = score;
									bestMatchedRule = rule;
									bestMatchedPattern = matchPat;
								}
							}
						}
					}

					theCurrentEntry++;

					// We also have to consider wildcard matches.
					if(theCurrentEntry == theTableEnd &&
					   equals(matchPat->getTargetString(),
							  XALAN_STATIC_UCODE_STRING("*")) == false
						&& (XalanNode::ELEMENT_NODE == targetNodeType || 
							XalanNode::ATTRIBUTE_NODE == targetNodeType ||
							XalanNode::PROCESSING_INSTRUCTION_NODE == targetNodeType)
						)
					{
						{
							assert(usedWildcard==false);	// Should only be here once ??
							usedWildcard = true;
							PatternTableMapType::const_iterator 	theTableIterator =
								m_patternTable.find(XALAN_STATIC_UCODE_STRING("*"));

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
			const int	nImports = m_imports.size();

			for(int i = 0; i < nImports; i++)
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
		
		const int	nConflicts = conflicts.size();
		if(nConflicts > 0)
		{
			const bool	quietConflictWarnings = executionContext.getQuietConflictWarnings();
			XalanDOMString	conflictsString = (quietConflictWarnings == false) 
									 ? XALAN_STATIC_UCODE_STRING("Specificity conflicts found: ") : XalanDOMString();
			for(int i = 0; i < nConflicts; i++)
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
	const PatternTableVectorType::size_type 	n =
		theVector.size();

	bool		addIt = true;

	for(PatternTableVectorType::size_type i = 0; i < n; i++)
	{
		if(theVector[i] == thePattern)
		{
			addIt = false;
			break;
		}
	}

	if(addIt == true)
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
			// String targetName = m_parserLiaison.getExpandedElementName((Element)targetNode);
			const XalanDOMString	targetName =
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
			bool				tryWildCard) const
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
		i = m_patternTable.find(XALAN_STATIC_UCODE_STRING("*"));
		if (i != m_patternTable.end())
		{
			theMatchList = &(*i).second;
			assert(theMatchList != 0);
		}
	}
	return theMatchList;
}



const NodeRefListBase*
Stylesheet::getNodeSetByKey(
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext,
			KeyTablesTableType& 	theKeysTable) const
{
	const NodeRefListBase *nl = 0;

	if(0 != m_keyDeclarations.size())
	{
		bool	foundDoc = false;

		const KeyTablesTableType::const_iterator	i =
			theKeysTable.find(doc);

		if (i != theKeysTable.end())
		{
			nl = i->second->getNodeSetByKey(name, ref);

			if (nl->getLength() > 0)
			{
				foundDoc = true;
			}
		}

		if((0 == nl || nl->getLength() == 0) && !foundDoc && m_needToBuildKeysTable)
		{
			KeyTable* const kt =
				new KeyTable(doc,
							 doc,
							 resolver,
							 name,
							 m_keyDeclarations,
							 executionContext);
			assert(doc == kt->getDocKey());

			theKeysTable[doc] = kt;

			foundDoc = true;

			nl = kt->getNodeSetByKey(name, ref);
		}
	}
	
	// If the nodelist is null at this point, it should 
	// mean there wasn't an xsl:key declared with the 
	// given name.	So go up the import hierarchy and 
	// see if one of the imported stylesheets declared it.
	if(0 == nl)
	{
		const int	nImports = m_imports.size();

		for(int i = 0; i < nImports; i++)
		{
			const Stylesheet*	const	stylesheet = m_imports[i];

			nl = stylesheet->getNodeSetByKey(doc, name, ref, resolver, executionContext, theKeysTable);

			if(0 != nl)
				break;
		}
	}

	return nl;
}



/**
 * Construct a match pattern from a pattern and template.
 * @param pat For now a Nodelist that contains old-style element patterns.
 * @param template The node that contains the template for this pattern.
 * @param isMatchPatternsOnly tells if pat param contains only match 
 * patterns (for compatibility with old syntax).
 */
Stylesheet::MatchPattern2::MatchPattern2(
			const XalanDOMString&	pat,
			const XPath*			exp,
			ElemTemplate*			theTemplate,
			int 					posInStylesheet, 
			const XalanDOMString&	targetString,
			Stylesheet* 	stylesheet) :
	m_stylesheet(stylesheet),
	m_targetString(targetString),
	m_expression(exp),
	m_posInStylesheet(posInStylesheet),
	m_priority(XPath::s_MatchScoreNone),
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
}



void Stylesheet::pushTopLevelVariables(
			StylesheetExecutionContext& 	executionContext,
			ParamVectorType&				topLevelParams) const
{
//	try
	{
		int i, nImports = m_imports.size();
		for(i = 0; i < nImports; i++)
		{
			const Stylesheet* const stylesheet = m_imports[i];

			stylesheet->pushTopLevelVariables(executionContext, topLevelParams);
		}

		const int	nVars = m_topLevelVariables.size();

		for(i = 0; i < nVars; i++)
		{
			ElemVariable* const 	var = m_topLevelVariables[i];

			bool					isParam =
				Constants::ELEMNAME_PARAMVARIABLE == var->getXSLToken();

			if(isParam == true)
			{
				isParam = false;

				const int	n = topLevelParams.size();

				for(int k = 0; k < n; k++)
				{
					Arg& a = topLevelParams[k];

					if(a.getName().equals(var->getName()))
					{
						isParam = true;

						XObject *pXO = 0;

						const XalanDOMString& expr = a.getExpression();

						if(length(expr) != 0)
						{
							pXO = executionContext.executeXPath(expr,
																executionContext.getRootDocument(),
																*this);

							a.setXObjectPtr(pXO);
							a.setExpression(0);
						}

						executionContext.pushVariable(a.getName(),
													  pXO,
													  this);
						break;
					}
				}
			}

			if (isParam == false)
			{
				XalanDocument* const	doc = executionContext.getRootDocument();

				var->execute(executionContext,
							 doc,
							 doc,
							 QName());
			}
		}
	}
/*
	catch(Exception e)
	{
		// Turn it into a runtime exception.
		throw new XSLProcessorException(e);
	}
 */
}



const XalanDOMString
Stylesheet::getCurrentIncludeBaseIdentifier() const
{
	if (m_includeStack.size() == 0)
	{
		return getBaseIdentifier();
	}
	else
	{
		return m_includeStack.back()->getURLText();
	}
}



void
Stylesheet::processNSAliasElement(
			const XalanDOMString&			name,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
{
	const unsigned int	nAttrs = atts.getLength();

	XalanDOMString		stylesheetPrefix;
	XalanDOMString		resultPrefix;

	for(unsigned int i = 0; i < nAttrs; i++)
	{

		const XalanDOMChar* const	aname = atts.getName(i);

		XalanDOMString				prefix;

		if(equals(aname, Constants::ATTRNAME_STYLESHEET_PREFIX) == true)
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				stylesheetPrefix = getNamespaceForPrefix(value);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_RESULT_PREFIX))
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				resultPrefix = getNamespaceForPrefix(value);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(name + " has an illegal attribute: " + aname);
		}
	}

	// Build a table of aliases, the key is the stylesheet uri and the
	// value is the result uri
	if (length(stylesheetPrefix) != 0 && length(resultPrefix) != 0 )
	{
		// $$$ ToDo: Fix this!!!
	}
	else
	{
		constructionContext.error("Missing namespace URI for specified prefix");
	}
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
		const QName&				/*qname */, 
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
void Stylesheet::applyAttrSets(
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
					attrSet->execute(executionContext, sourceTree, sourceNode, mode);
			}
		}
	}
}	



const
Stylesheet::NamespaceVectorType& Stylesheet::getNamespaceDecls() const
{ 
	return m_namespaceDecls;
}



void
Stylesheet::setNamespaceDecls(const NamespaceVectorType& ns)
{
	m_namespaceDecls = ns;
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



XalanDOMString
Stylesheet::getNodeName() const
{
	return XalanDOMString();
}



XalanDOMString
Stylesheet::getNodeValue() const
{
	return XalanDOMString();
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
	return 0;
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



XalanDOMString
Stylesheet::getNamespaceURI() const
{
	// $$ ToDo: Is this the same value as PrefixResolver::getURI()?
	return XalanDOMString();
}



XalanDOMString
Stylesheet::getPrefix() const
{
	return XalanDOMString();
}



XalanDOMString
Stylesheet::getLocalName() const
{
	return XalanDOMString();
}



void
Stylesheet::setPrefix(const XalanDOMString& /* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
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



XalanDOMString
Stylesheet::getURI() const
{
	return m_baseIdent;
}
