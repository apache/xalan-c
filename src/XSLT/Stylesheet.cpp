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
 *
 * $ Id: $
 *
 */

#include "Stylesheet.hpp"



#include <algorithm>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <sax/AttributeList.hpp>



#include <DOMSupport/DOMServices.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <XMLSupport/XMLParserLiaison.hpp>
#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPath.hpp>



#include "Constants.hpp"
#include "ElemAttributeSet.hpp"
#include "ElemTemplate.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemVariable.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"


const Stylesheet::NamespaceVectorType	Stylesheet::s_emptyNamespace;



Stylesheet::Stylesheet(
		StylesheetRoot&					root,
        const DOMString&				baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	UnimplementedDocument(),
	UnimplementedElement(&root),
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
	m_whitespacePreservingElements(),
	m_whitespaceStrippingElements(),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_XSLTVerDeclared(1.0L),
	m_isRoot(&root == this ? true: false),
	m_patternTable(),
	m_attributeSets()
{
	const XMLURL* const	url = constructionContext.getURLFromString(m_baseIdent);

	if (url != 0)
	{
		m_includeStack.push_back(url);
	}

	 // Register the java namespace as being implemented by the xslt-javaclass
	 // engine. Note that there's no real code per se for this extension as the
	 // functions carry the object on which to call etc. and all the logic of
	 // breaking that up is in the xslt-javaclass engine.
/*	 
	@@ LATER
	 ExtensionFunctionHandler doesn't exist yet
    string uri("http://xml.apache.org");
    ExtensionFunctionHandler fh = 
      new ExtensionFunctionHandler (uri, 0, "xslt-javaclass", 0, 0);
    XMLParserLiaisonDefault xld = 
      reinterpret_cast<XMLParserLiaisonDefault> m_processor.m_parserLiaison;
    xld.addExtensionNamespace (uri, fh);
*/	 
}



Stylesheet::~Stylesheet()
{
	// Clean up all entries in the vector.
	std::for_each(m_imports.begin(),
			 m_imports.end(),
			 DeleteFunctor<Stylesheet>());

	// Clean up the key table vector
	std::for_each(m_key_tables.begin(),
			 m_key_tables.end(),
			 DeleteFunctor<KeyTable>());

	// Clean up the match pattern vector
	PatternTableMapType::iterator it = m_patternTable.begin();

	for ( ; it != m_patternTable.end(); it++)
	{
		PatternTableListType&	theList = (*it).second;

		std::for_each(theList.begin(),
					  theList.end(),
					  DeleteFunctor<MatchPattern2>());
	}
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
 *    the use attribute,
 * c) each with a value that is a nodelist.
 * Thus, for a given key or keyref, look up hashtable by name, 
 * look up the nodelist by the given reference.
 */
void
Stylesheet::processKeyElement(
			ElemTemplateElement*			nsContext,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
// throws XSLProcessorException
{
	const XMLCh* nameAttr = 0;
	XPath *matchAttr = 0;
	XPath *useAttr = 0;
 
	int nAttrs = atts.getLength();
	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname = atts.getName(i);
		if (equals(aname, Constants::ATTRNAME_NAME))
			nameAttr = atts.getValue(i);
		else	if(equals(aname, Constants::ATTRNAME_MATCH))
		{
			matchAttr =
			constructionContext.createMatchPattern(DOMString(atts.getValue(i)),
			*nsContext);
		}
		else	if(equals(aname, Constants::ATTRNAME_USE))
			useAttr = constructionContext.createXPath(atts.getValue(i),
				*nsContext);
		else
			constructionContext.error(
				DOMString("xsl:key, unrecognized keyword '")+Constants::ATTRNAME_NAME+DOMString("'!"));
	}
	if(0 == nameAttr)
		constructionContext.error(DOMString("xsl:key	requires a ")+Constants::ATTRNAME_NAME+" attribute!");

	if(0 == matchAttr)
		constructionContext.error(DOMString("xsl:key	requires a ")+Constants::ATTRNAME_MATCH+DOMString(" attribute!"));

	if(0 == useAttr)
		constructionContext.error(DOMString("xsl:key	requires a ")+Constants::ATTRNAME_USE+DOMString(" attribute!"));

	m_keyDeclarations.push_back(KeyDeclaration(nameAttr, *matchAttr, *useAttr));
	m_needToBuildKeysTable = true;
}
  
/**
 * Push the namespace declarations from the current attribute 
 * list onto the namespace stack.
 */
void Stylesheet::pushNamespaces(const AttributeList& atts)
{
	int nAttrs = atts.getLength();
	NamespaceVectorType namespaces;
	for(int i = 0; i < nAttrs; i++)
	{
		const DOMString aname = atts.getName(i);
		const DOMString value = atts.getValue(i);
		bool isPrefix = startsWith(aname, "xmlns:");
		if (equals(aname, "xmlns") || isPrefix) 
		{
			DOMString p = isPrefix ? substring(aname,6) : DOMString();
			NameSpace ns(p, value);
			namespaces.push_back(ns);
		}
	}
	m_namespaces.push_back(namespaces);
}

void Stylesheet::popNamespaces() 
{ 
	assert(m_namespaces.size());
	m_namespaces.pop_back(); 
}

/**
 * See if this is a xmlns attribute or in a non-XSLT.
 * 
 * @param attrName Qualified name of attribute.
 * @param atts The attribute list where the element comes from (not used at 
 *      this time).
 * @param which The index into the attribute list (not used at this time).
 * @return True if this attribute should not be flagged as an error.
 */
bool Stylesheet::isAttrOK(
			const DOMString&				attrName,
			const AttributeList&			/* atts */,
			int								/* which */,
			StylesheetConstructionContext&	constructionContext) const
{
	bool attrOK = equals(attrName, "xmlns") || startsWith(attrName, "xmlns:");

	if(!attrOK)
	{
		const int	indexOfNSSep = indexOf(attrName, ':');

		if(indexOfNSSep >= 0)
		{
			const DOMString prefix = substring(attrName, 0, indexOfNSSep);
			const DOMString ns = getNamespaceForPrefixFromStack(prefix);

			attrOK = indexOf(ns, constructionContext.getXSLNameSpaceURLPre()) >= 0;
		}
		else
			attrOK = true;
	}

	return attrOK;
}



/**
 * Get the namespace from a qualified name.
 */
DOMString Stylesheet::getNamespaceFromStack(const DOMString& nodeName) const
{
	int indexOfNSSep = indexOf(nodeName, ':');
	DOMString prefix = (indexOfNSSep >= 0) ?
		substring(nodeName, 0, indexOfNSSep) : DOMString();
	return getNamespaceForPrefixFromStack(prefix);
}
  
/**
 * Get the namespace from a prefix.
 */
DOMString Stylesheet::getNamespaceForPrefix(const DOMString& prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaceDecls, prefix);
}
      
/**
 * Get the namespace from a prefix.
 */
DOMString Stylesheet::getNamespaceForPrefixFromStack(const DOMString& prefix) const
{
	return QName::getNamespaceForPrefix(m_namespaces, prefix);
}
  
bool Stylesheet::getYesOrNo(
			const DOMString&				aname,
			const DOMString&				val,
			StylesheetConstructionContext&	constructionContext) const
{
	if(val.equals(Constants::ATTRVAL_YES))
		return true;
	else if(val.equals(Constants::ATTRVAL_NO))
		return false;
	else
		constructionContext.error(val+" is unknown value for "+aname);
	return false;
}

/**
 * Add a template to the template list.
 */
void Stylesheet::addTemplate(ElemTemplate *tmpl)
{
	int pos = 0;
	if(0 == m_firstTemplate)
		m_firstTemplate = tmpl;
	else
	{
		ElemTemplateElement*	next = m_firstTemplate;
		while(0 != next)
		{
			if(0 == next->getNextSibling())
			{
				next->setNextSibling(tmpl);
				tmpl->setNextSibling(0); // just to play it safe.
				break;
			}
			pos++;

			next = const_cast<const ElemTemplateElement*>(next)->getNextSibling();
		}
	}

	if(tmpl->getName().isEmpty() == false)
	{
		m_namedTemplates.insert(std::make_pair(tmpl->getName(), tmpl));
	}

	const XPath* xp = tmpl->getMatchPattern();

	if(0 != xp)
	{
		std::vector<DOMString> strings;
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
			int nTargets = strings.size();
			for(int stringIndex = 0; stringIndex < nTargets; stringIndex++) 
			{
				DOMString& target = strings[stringIndex];
				MatchPattern2* newMatchPat =
					new MatchPattern2(xp->getExpression().getCurrentPattern(), 
						*xp, *tmpl, pos, target, *this);
				
				// See if there's already one there
				PatternTableMapType::iterator it = 
					m_patternTable.find(target);
				// New one
				if(it == m_patternTable.end())
				{
					PatternTableListType patternlist;
					patternlist.push_back(newMatchPat);
					m_patternTable.insert(PatternTableMapType::value_type(target, patternlist));
				}
				else
				{
					// Add it to the end of the list
					PatternTableListType& patternlist = (*it).second;
					patternlist.push_back(newMatchPat);
				}
			}
		}
	}
}
      


// JMD: removed buildStylesheetTables

// JMD: removed initIncludes

// JMD: removed initImports
	

/**
 * Locate a macro via the "name" attribute.
 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
 * the error condition is severe enough to halt processing.
 */
ElemTemplateElement*
Stylesheet::findNamedTemplate(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const
//throws XSLProcessorException
{
	QName qname(name, m_namespaces);

	return findNamedTemplate(qname, executionContext);
}


/**
 * Locate a macro via the "name" attribute.
 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
 * the error condition is severe enough to halt processing.
 */
ElemTemplateElement*
Stylesheet::findNamedTemplate(
			const QName&					qname,
            StylesheetExecutionContext&		executionContext) const
//throws XSLProcessorException
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
		namedTemplate = (*it).second;

	if(0 == namedTemplate)
		executionContext.warn(DOMString("Could not find macro def named: ") +
		qname.getLocalPart());

	return namedTemplate;
}
	


XObject*
Stylesheet::getTopLevelVariable(
			const DOMString&				name,
            StylesheetExecutionContext&		executionContext) const
{
	XObject*	theResult = executionContext.getTopLevelVariable(name);

	if(0 == theResult)
	{
		const int	nImports = m_imports.size();

		for(int i = 0; i < nImports; i++)
		{
			Stylesheet* const	stylesheet = m_imports[i];
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
		executionContext.warn(DOMString("Could not find variable def for: ") + name);
	}

	return theResult;
}


ElemTemplateElement* Stylesheet::findTemplate(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					targetNode) const
{
	const Stylesheet*	theDummy;

	return findTemplate(executionContext, sourceTree, targetNode, QName(), false, theDummy);
}



ElemTemplateElement*
Stylesheet::findTemplate(
			StylesheetExecutionContext&		executionContext,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					targetNode, 
			const QName&					mode,
			bool							useImports,
			const Stylesheet*&				foundStylesheet) const
{
	bool usedWildcard = false;
	if(m_isWrapperless) return m_wrapperlessTemplate;

	const ElemTemplateElement*			bestMatchedRule = 0;
	const MatchPattern2*				bestMatchedPattern = 0; // Syncs with bestMatchedRule

	PatternTableVectorType conflicts;

	if(useImports == false)
	{
		//odd that this variable is only set, never read
		double highScore = XPath::s_MatchScoreNone;

		// Points to the current list of match patterns.  Note
		// that this may point to more than one table.
		const PatternTableListType* 	matchPatternList = 0;
		int targetNodeType = targetNode.getNodeType();

		switch(targetNodeType)
		{
		case DOM_Node::ELEMENT_NODE:
			{
				//java: DOMString targetName = m_processor->getParserLiaison().getLocalNameOfNode(targetNode);
				DOMString targetName = DOMServices::getLocalNameOfNode(targetNode);
				matchPatternList = locateMatchPatternList2(targetName, true);
			}
			break;
			
		case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		case DOM_Node::ATTRIBUTE_NODE:
			matchPatternList = locateMatchPatternList2(targetNode.getNodeName(), true);
			break;

		case DOM_Node::CDATA_SECTION_NODE:
		case DOM_Node::TEXT_NODE:
			matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_TEXT, false);
			break;

		case DOM_Node::COMMENT_NODE:
			matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_COMMENT, false);
			break;

		case DOM_Node::DOCUMENT_NODE:
				matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ROOT, false);
			break;

		case DOM_Node::DOCUMENT_FRAGMENT_NODE:
			matchPatternList = locateMatchPatternList2(XPath::PSEUDONAME_ANY, false);
			break;
			
		default:
			{
				matchPatternList = locateMatchPatternList2(targetNode.getNodeName(), false);
			}
		}

		if (matchPatternList != 0)
		{
			DOMString	prevPat;

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
				const MatchPattern2* matchPat = *theCurrentEntry;

				const ElemTemplate* rule = &matchPat->getTemplate();

				// We'll be needing to match rules according to what 
				// mode we're in.
				const QName& ruleMode = rule->getMode();

				// The logic here should be that if we are not in a mode AND
				// the rule does not have a node, then go ahead.
				// OR if we are in a mode, AND the rule has a node, 
				// AND the rules match, then go ahead.

				bool haveMode = !mode.isEmpty();
				bool haveRuleMode = !ruleMode.isEmpty();

				if ( (!haveMode && !haveRuleMode) || (haveMode && haveRuleMode && ruleMode.equals(mode)))
				{
					const DOMString 	patterns = matchPat->getPattern();

					if((!isEmpty(patterns)) &&
						!(!isEmpty(prevPat) && equals(prevPat, patterns)))
					{
						prevPat = patterns;

						const XPath& xpath = matchPat->getExpression();

						double score =
							xpath.getMatchScore(targetNode, executionContext.getXPathExecutionContext());

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
					} // end if(0 != length(patterns))			 
				} // end if if(targetModeString.equals(mode))

				theCurrentEntry++;

				// We also have to consider wildcard matches.
				if(theCurrentEntry == theTableEnd &&
				   equals(matchPat->getTargetString(), "*") == false
					&& (DOM_Node::ELEMENT_NODE == targetNodeType || 
						DOM_Node::ATTRIBUTE_NODE == targetNodeType ||
						DOM_Node::PROCESSING_INSTRUCTION_NODE == targetNodeType)
					)
				{
					{
						assert(usedWildcard==false);	// Should only be here once ??
						usedWildcard = true;
						PatternTableMapType::const_iterator 	theTableIterator =
							m_patternTable.find("*");

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
		DOMString	conflictsString = (quietConflictWarnings == false) 
								 ? "Specificity conflicts found: " : DOMString();
		for(int i = 0; i < nConflicts; i++)
		{
			const MatchPattern2* const	conflictPat = conflicts[i];
			if(0 != i)
			{
				if(quietConflictWarnings == false)
				{
					conflictsString += ", ";
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
				conflictsString += DOMString("\"") + conflictPat->getPattern() + DOMString("\"");
			}
		}

		bestMatchedRule = &bestMatchedPattern->getTemplate();

		if(quietConflictWarnings == false)
		{
			conflictsString += " ";
			conflictsString += "Last found in stylesheet will be used.";
			executionContext.warn(conflictsString);
		}
	}

	if((0 != bestMatchedPattern) && (0 != foundStylesheet))
	{
		foundStylesheet = &bestMatchedPattern->getStylesheet();
	}

	return const_cast<ElemTemplateElement *>(bestMatchedRule);
}
	
void Stylesheet::addObjectIfNotFound(
			const MatchPattern2*			thePattern,
			PatternTableVectorType&	theVector)
{
	const int	n = theVector.size();
	bool		addIt = true;
	for(int i = 0; i < n; i++)
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
Stylesheet::locateMatchPatternList2(DOM_Node	sourceNode) const
{
	assert(sourceNode != 0);

	const PatternTableListType* 	matchPatternList = 0;

	switch(sourceNode.getNodeType())
	{
	case DOM_Node::ELEMENT_NODE:
		{
			// String targetName = m_parserLiaison.getExpandedElementName((Element)targetNode);
			const DOMString 	targetName =
				DOMServices::getLocalNameOfNode(sourceNode);
			matchPatternList = locateMatchPatternList2(targetName, true);
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
	case DOM_Node::ATTRIBUTE_NODE:
		matchPatternList = locateMatchPatternList2(sourceNode.getNodeName(), true);
		break;

	case DOM_Node::CDATA_SECTION_NODE:
	case DOM_Node::TEXT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_TEXT, false);
		break;

	case DOM_Node::COMMENT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_COMMENT, false);
		break;

	case DOM_Node::DOCUMENT_NODE:
		matchPatternList = locateMatchPatternList2(Constants::PSEUDONAME_ROOT, false);
		break;

	default:
		matchPatternList = locateMatchPatternList2(sourceNode.getNodeName(), false);
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
			const DOMString&	sourceElementType,
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
		i = m_patternTable.find("*");
		if (i != m_patternTable.end())
		{
			theMatchList = &(*i).second;
			assert(theMatchList != 0);
		}
	}
	return theMatchList;
}




/**
 * Given a valid element key, return the corresponding node list.
 */
const NodeRefListBase*
Stylesheet::getNodeSetByKey(
							const DOM_Node&			doc,
							const DOMString&		name,
							const DOMString&		ref,
							const PrefixResolver&	resolver,
							XPathExecutionContext&	executionContext) const
{
	const NodeRefListBase *nl = 0;
	if(0 != m_keyDeclarations.size())
	{
		bool foundDoc = false;
		const int nKeyTables = m_key_tables.size();
		for(int i = 0; i < nKeyTables; i++)
		{			
			const KeyTable* const kt = m_key_tables[i];
			if(doc == kt->getDocKey())
			{
				nl = kt->getNodeSetByKey(name, ref);
				if (nl !=0 && nl->getLength() > 0)
				{
					foundDoc = true;
					break;
				}
			}
		}
		if((0 == nl) && !foundDoc && m_needToBuildKeysTable)
		{
			KeyTable* const	kt =
				new KeyTable(doc,
							 doc,
							 resolver,
							 m_keyDeclarations,
							 executionContext);

			m_key_tables.push_back(kt);

			if(doc == kt->getDocKey())
			{
				foundDoc = true;
				nl = kt->getNodeSetByKey(name, ref);
			}
		}
	}
	
	// If the nodelist is null at this point, it should 
	// mean there wasn't an xsl:key declared with the 
	// given name.  So go up the import hierarchy and 
	// see if one of the imported stylesheets declared it.
	if(0 == nl)
	{
		const int	nImports = m_imports.size();

		for(int i = 0; i < nImports; i++)
		{
			const Stylesheet*	const	stylesheet = m_imports[i];

			nl = stylesheet->getNodeSetByKey(doc, name, ref, resolver, executionContext);

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
			const DOMString&		pat,
			const XPath&			exp,
			const ElemTemplate&		theTemplate,
			int						posInStylesheet, 
			const DOMString&		targetString,
			const Stylesheet&		stylesheet) :
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


void Stylesheet::addExtensionNamespace (const DOMString& uri, ExtensionNSHandler* nsh)
{
	m_extensionNamespaces.insert(std::make_pair(uri, nsh));
}



void Stylesheet::pushTopLevelVariables(
			StylesheetExecutionContext&		executionContext,
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
			ElemVariable* const		var = m_topLevelVariables[i];

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

						const DOMString& expr = a.getExpression();

						if(length(expr) != 0)
						{
							pXO = executionContext.executeXPath(expr,
																executionContext.getRootDocument(),
																DOM_UnimplementedElement(const_cast<Stylesheet*>(this)));

							a.setXObjectPtr(pXO);
							a.setExpression(0);
						}

						executionContext.pushVariable(a.getName(),
													  pXO,
													  DOM_UnimplementedElement(const_cast<Stylesheet*>(this)));
						break;
					}
				}
			}
			else
			{
				const DOM_Document	doc = executionContext.getRootDocument();

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

/**
 * Add an attribute set to the list.
 */
void Stylesheet::addAttributeSet(
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
            StylesheetExecutionContext&		executionContext, 
            const DOM_Node&					sourceTree, 
            const DOM_Node&					sourceNode,
            const QName&					mode) const
{
/*
	java: Difference from Java code is we have map of qnames to attribute sets
	instead of a vector where we look for the matching qname in the vector
	@@ Is this right ??
 */
	const int	nNames = attributeSetsNames.size();

	if(0 != nNames)
	{
		int i;
		// Process up the import chain...
		const int	nImports = m_imports.size();
		for(i = 0; i < nImports; i++)
		{
			const Stylesheet* const		stylesheet = m_imports[i];

			stylesheet->applyAttrSets(attributeSetsNames, 
									 executionContext, sourceTree, sourceNode, mode);
		}

		for(i = 0; i < nNames; i++)
		{
			const QName&	qname = attributeSetsNames[i];
			const int		nSets = m_attributeSets.size();

			for(int k = 0; k < nSets; k++)
			{
				const ElemAttributeSet* const	attrSet = m_attributeSets[k];

				if(qname.equals(attrSet->getQName()))
					attrSet->execute(executionContext, sourceTree, sourceNode, mode);
			}
		}
	}
}	



const Stylesheet::NamespaceVectorType& Stylesheet::getNamespaceDecls() const
{ 
	return m_namespaceDecls;
}

void Stylesheet::setNamespaceDecls(const NamespaceVectorType& ns)
{
	m_namespaceDecls = ns;
}


short
Stylesheet::getNodeType()
{
	return DOM_Node::DOCUMENT_NODE;
}



const Stylesheet::NamespaceVectorType&
Stylesheet::getCurrentNamespace() const
{
	if (m_namespaces.size() > 0)
		return m_namespaces.back();
	else
		return s_emptyNamespace;
}


////////////////////////////////////////////////////////////////////
// JAVA Code not implemented
////////////////////////////////////////////////////////////////////

#ifdef JAVANOTIMPLEMENTED  

  /**
   * Extension to be used when serializing to disk.
   */
  const std::string STYLESHEET_EXT(".lxc");

  /**
   * Read the stylesheet from a serialization stream.
   */
  private void readObject(ObjectInputStream stream)
    throws IOException
  {
    try
    {
      stream.defaultReadObject();
    }
    catch(ClassNotFoundException cnfe)
    {
      throw new XSLProcessorException(cnfe);
    }
    m_includeStack.clear();
  }

  private void writeObject(ObjectOutputStream stream)
    throws IOException
  {
    stream.defaultWriteObject();
  }

#endif // JAVANOTIMPLEMENTED  

/*
 *	$ Log: $
 */
