/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Stylesheet.hpp"



#include <algorithm>



#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>



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



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString			Stylesheet::s_emptyString;

const XalanQNameByReference		Stylesheet::s_emptyQName;



Stylesheet::Stylesheet(
		StylesheetRoot& 				root,
		const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	PrefixResolver(),
	m_stylesheetRoot(root),
	m_baseIdent(baseIdentifier),
	m_keyDeclarations(),
	m_whitespaceElements(),
	m_XSLTNamespaceURI(constructionContext.getXSLTNamespaceURI()),
	m_imports(),
	m_importsSize(0),
	m_namespaces(),
	m_namespaceDecls(),
	m_isWrapperless(false),
	m_extensionNamespaces(),
	m_firstTemplate(0),
	m_includeStack(),
	m_namedTemplates(),
	m_topLevelVariables(),
	m_XSLTVerDeclared(1.0L),
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
		catch(const XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtilsException&)
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
#if !defined(XALAN_NO_STD_NAMESPACE)
	using std::for_each;
#endif

	// Clean up all entries in the imports vector.
	for_each(m_imports.begin(),
			 m_imports.end(),
			 DeleteFunctor<Stylesheet>());

	// Clean up the decimal formats vector
	for_each(m_elemDecimalFormats.begin(),
			 m_elemDecimalFormats.end(),
			 DeleteFunctor<ElemDecimalFormat>());

	// Clean up the extension namespaces vector
	for_each(m_extensionNamespaces.begin(),
			 m_extensionNamespaces.end(),
			 MapValueDeleteFunctor<ExtensionNamespacesMapType>());
}



ElemTemplateElement*
Stylesheet::initWrapperless(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator)
{
	if (m_isWrapperless == true)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::StylesheetHasWrapperlessTemplate),
			0,
			locator);
	}

	assert(m_firstTemplate == 0);

	m_isWrapperless = true;


	AttributeListImpl	templateAttrs;

	templateAttrs.addAttribute(c_wstr(Constants::ATTRNAME_NAME),
							   c_wstr(Constants::ATTRTYPE_CDATA),
							   c_wstr(Constants::ATTRVAL_SIMPLE));

	ElemTemplateElement* const	theNewTemplate =
		constructionContext.createElement(
			StylesheetConstructionContext::ELEMNAME_TEMPLATE,
			*this,
			templateAttrs,
			locator);

	theNewTemplate->addToStylesheet(constructionContext, *this);

	assert(m_firstTemplate == theNewTemplate);

	return theNewTemplate;
}



void
Stylesheet::processKeyElement(
			const PrefixResolver&			nsContext,
			const AttributeListType&		atts,
			const LocatorType*				locator,
			StylesheetConstructionContext&	constructionContext)
{
	const XalanQName*		theQName = 0;
	XPath*					matchAttr = 0;
	XPath*					useAttr = 0;
 
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_NAME))
		{
			theQName = constructionContext.createXalanQName(atts.getValue(i), m_namespaces, locator);

			if (theQName->isValid() == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(
							XalanMessages::AttributeValueNotValidQName_2Param,
							Constants::ATTRNAME_NAME.c_str(),
							atts.getValue(i)),
						0,
						locator);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_MATCH))
		{
			matchAttr =
					constructionContext.createMatchPattern(
						0,
						XalanDOMString(atts.getValue(i)),
						nsContext);
		}
		else if(equals(aname, Constants::ATTRNAME_USE))
		{
			useAttr =
					constructionContext.createXPath(
						0,
						atts.getValue(i),
						nsContext);
		}
		else if (isAttrOK(aname, atts, i, constructionContext) == false)
		{
			constructionContext.error(
				XalanMessageLoader::getMessage(
					XalanMessages::TemplateHasIllegalAttribute_2Param,
					Constants::ELEMNAME_KEY_WITH_PREFIX_STRING.c_str(),
					aname),
				0,
				locator);
		}
	}

	if(0 == theQName)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				Constants::ELEMNAME_KEY_WITH_PREFIX_STRING,
				Constants::ATTRNAME_NAME),
			0,
			locator);
	}

	if(0 == matchAttr)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				Constants::ELEMNAME_KEY_WITH_PREFIX_STRING,
				Constants::ATTRNAME_MATCH),
			0,
			locator);
	}

	if(0 == useAttr)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::ElementRequiresAttribute_2Param,
				Constants::ELEMNAME_KEY_WITH_PREFIX_STRING,
				Constants::ATTRNAME_USE),
			0,
			locator);
	}

	m_keyDeclarations.push_back(
		KeyDeclaration(
			*theQName,
			*matchAttr,
			*useAttr,
			m_baseIdent,
			XalanLocator::getLineNumber(locator),
			XalanLocator::getColumnNumber(locator)));
}



void
Stylesheet::pushNamespaces(const AttributeListType&		atts)
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
			Stylesheet::PatternTableVectorType&		theList,
			const Stylesheet::MatchPattern2*		thePattern)
{
	typedef Stylesheet::size_type	size_type;
	assert(thePattern != 0);

	const double		thePatternPriority = thePattern->getPriorityOrDefault();
	const size_type		thePatternPosition = thePattern->getPositionInStylesheet();

	typedef Stylesheet::PatternTableVectorType	PatternTableListType;
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
		   Stylesheet::PatternTableMapType&				theTable,
		   const Stylesheet::PatternTableVectorType&	theList)
{
	typedef Stylesheet::PatternTableMapType		PatternTableMapType;
	typedef Stylesheet::PatternTableVectorType	PatternTableListType;

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
Stylesheet::addWhitespaceElement(const XalanSpaceNodeTester&	theTester)
{
	typedef WhitespaceElementsVectorType::iterator	iterator;

	const XPath::eMatchScore	theMatchScore = theTester.getMatchScore();

	iterator	i = m_whitespaceElements.begin();

	while(i != m_whitespaceElements.end())
	{
		if (theMatchScore >= (*i).getMatchScore())
		{
			break;
		}
		else
		{
			++i;
		}
	}

	m_whitespaceElements.insert(i, theTester);
}



void
Stylesheet::postConstruction(StylesheetConstructionContext&		constructionContext)
{
	KeyDeclarationVectorType::size_type			theKeyDeclarationsCount = 0;
	WhitespaceElementsVectorType::size_type		theWhitespaceElementsCount = 0;

	{
		m_importsSize = m_imports.size();

		// Call postConstruction() on any imported stylesheets, in reverse order,
		// so namespace aliases are processed properly. Also, get any key
		// declarations and preserve/strip space information.
		const StylesheetVectorType::reverse_iterator	theEnd = m_imports.rend();
		StylesheetVectorType::reverse_iterator			i = m_imports.rbegin();

		while(i != theEnd)
		{
			(*i)->postConstruction(constructionContext);

			m_namespacesHandler.copyNamespaceAliases((*i)->getNamespacesHandler());

			theKeyDeclarationsCount += (*i)->m_keyDeclarations.size();
			theWhitespaceElementsCount += (*i)->m_whitespaceElements.size();

			++i;
		}
	}

	{
		// Call postConstruction() on any imported stylesheets, in import order,
		// and process preserve/strip space information.
		const StylesheetVectorType::iterator	theEnd = m_imports.end();
		StylesheetVectorType::iterator			i = m_imports.begin();

		m_keyDeclarations.reserve(
			m_keyDeclarations.size() + theKeyDeclarationsCount);

		m_whitespaceElements.reserve(
			m_whitespaceElements.size() + theWhitespaceElementsCount);

		while(i != theEnd)
		{
			m_keyDeclarations.insert(
				m_keyDeclarations.end(),
				(*i)->m_keyDeclarations.begin(),
				(*i)->m_keyDeclarations.end());

			KeyDeclarationVectorType().swap((*i)->m_keyDeclarations);

			m_whitespaceElements.insert(
				m_whitespaceElements.end(),
				(*i)->m_whitespaceElements.begin(),
				(*i)->m_whitespaceElements.end());

			WhitespaceElementsVectorType().swap((*i)->m_whitespaceElements);

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

	addToTable(m_elementPatternTable, m_elementAnyPatternList);
	addToTable(m_attributePatternTable, m_attributeAnyPatternList);

	m_patternCount = m_matchPattern2Container.size();
}



bool
Stylesheet::isAttrOK(
			const XalanDOMChar* 			attrName,
			const AttributeListType&		/* atts */,
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

			attrOK = ns != 0 && !ns->empty() && *ns != constructionContext.getXSLTNamespaceURI();
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



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(
			const XalanDOMString&			prefix,
			StylesheetConstructionContext&	constructionContext) const
{
	const XalanDOMString* const		theURI = getNamespaceForPrefix(prefix);

	if (theURI == 0)
	{
		constructionContext.error(XalanMessageLoader::getMessage(XalanMessages::UndeclaredNamespacePrefix_1Param, prefix));
	}

	return theURI;
}



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(
			const XalanDOMChar*				prefix,
			StylesheetConstructionContext&	constructionContext) const
{
	StylesheetConstructionContext::GetAndReleaseCachedString	theGuard(constructionContext);

	XalanDOMString&		theTemp = theGuard.get();

	theTemp.assign(prefix);

	return getNamespaceForPrefix(theTemp, constructionContext);
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
		constructionContext.error(XalanMessageLoader::getMessage(XalanMessages::AttributeMustBe_2Params,Constants::ATTRVAL_YES,Constants::ATTRVAL_NO));
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

	if (m_isWrapperless == true)
	{
		if (m_firstTemplate != 0)
		{
			constructionContext.error(
				XalanMessageLoader::getMessage(XalanMessages::StylesheetHasWrapperlessTemplate),
				0,
				theTemplate);
		}
		else
		{
			m_firstTemplate = theTemplate;
		}
	}
	else if(0 == m_firstTemplate)
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
	const XalanQName&	theName = theTemplate->getNameAttribute();

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
				XalanMessageLoader::getMessage(XalanMessages::StylesheetHasDuplicateNamedTemplate),
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
			StylesheetConstructionContext::GetAndReleaseCachedString	theGuard(constructionContext);

			XalanDOMString&	tempString = theGuard.get();

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
	const ElemTemplateMapType::const_iterator	it = m_namedTemplates.find(qname);

	if(it != m_namedTemplates.end())
	{
		return (*it).second;
	}
	else
	{
		const ElemTemplate*		namedTemplate = 0;

		// Look for the template in the imports
		const StylesheetVectorType::size_type	importsCount = m_imports.size();

		for(StylesheetVectorType::size_type i = 0; i < importsCount; ++i)
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
	XALAN_USING_STD(find)

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
	assert(thePattern != 0 && thePatternArray != 0);

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



inline const Stylesheet::PatternTableVectorType* 
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



inline const Stylesheet::PatternTableVectorType* 
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



inline const Stylesheet::PatternTableVectorType*
Stylesheet::locateMatchPatternList2(
			const XalanNode&		theNode,
			XalanNode::NodeType		targetNodeType) const
{
	assert(theNode.getNodeType() == targetNodeType);

	switch(targetNodeType)
	{
	case XalanNode::ELEMENT_NODE:
		return locateElementMatchPatternList2(DOMServices::getLocalNameOfNode(theNode));
		break;

	case XalanNode::PROCESSING_INSTRUCTION_NODE:
		return &m_piPatternList;
		break;

	case XalanNode::ATTRIBUTE_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
		assert(DOMServices::isNamespaceDeclaration((const XalanAttr&)theNode) == false);
#else
		assert(DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(theNode)) == false);
#endif
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
			XalanNode::NodeType				targetNodeType,
			const XalanQName&				mode) const
{
	assert(targetNode->getNodeType() == targetNodeType);
	assert(m_importsSize == m_imports.size());

	for(StylesheetVectorType::size_type i = 0; i < m_importsSize; i++)
	{
		const Stylesheet* const 	stylesheet =
			m_imports[i];

		const ElemTemplate* const	bestMatchedRule =
			stylesheet->findTemplate(
				executionContext,
				targetNode,
				targetNodeType,
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
			XalanNode::NodeType				targetNodeType,
			const XalanQName&				mode,
			bool							onlyUseImports) const
{
	assert(targetNode != 0);
	assert(targetNode->getNodeType() == targetNodeType);
	assert(m_patternCount == m_matchPattern2Container.size());

	if(m_isWrapperless == true)
	{
		return m_firstTemplate;
	}
	else if (onlyUseImports == true)
	{
		return findTemplateInImports(executionContext, targetNode, targetNodeType, mode);
	}
	else
	{
		const ElemTemplate*		bestMatchedRule = 0;

		if (executionContext.getQuietConflictWarnings() == true)
		{
			// Points to the current list of match patterns.  Note
			// that this may point to more than one table.
			const PatternTableVectorType* 	matchPatternList =
					locateMatchPatternList2(*targetNode, targetNodeType);
			assert(matchPatternList != 0);

			PatternTableVectorType::const_iterator		theCurrentEntry =
					matchPatternList->begin();

			const PatternTableVectorType::const_iterator	theTableEnd =
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
				bestMatchedRule = findTemplateInImports(executionContext, targetNode, targetNodeType, mode);
			}
		}
		else
		{
			const PatternTableVectorType* 	matchPatternList =
					locateMatchPatternList2(*targetNode, targetNodeType);
			assert(matchPatternList != 0);

			PatternTableVectorType::const_iterator		theCurrentEntry =
					matchPatternList->begin();

			const PatternTableVectorType::const_iterator	theTableEnd =
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
					assert(conflicts != 0 && nConflicts <= m_patternCount);

					XalanDOMString	conflictsString(XalanMessageLoader::getMessage(XalanMessages::ConflictsFound));
					
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
					conflictsString += XalanMessageLoader::getMessage(XalanMessages::LastFoundStylesheetWillBeUsed);
					executionContext.warn(conflictsString, targetNode, bestMatchedRule->getLocator());
				}
			}

			if(0 == bestMatchedRule)
			{
				bestMatchedRule = findTemplateInImports(executionContext, targetNode, targetNodeType, mode);
			}
		}

		return bestMatchedRule;
	}
}



void
Stylesheet::processExtensionNamespace(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			uri)
{
	XalanAutoPtr<ExtensionNSHandler>	theGuard(new ExtensionNSHandler(uri));

	m_extensionNamespaces.insert(ExtensionNamespacesMapType::value_type(uri, theGuard.get()));

	theGuard.release();

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

				if(arg.getName().equals(var->getNameAttribute()))
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
			executionContext.pushVariable(var->getNameAttribute(),
										  var,
										  var->getParentNodeElem());
		}
	}
}



void
Stylesheet::processNSAliasElement(
			const XalanDOMChar*				name,
			const AttributeListType&		atts,
			StylesheetConstructionContext&	constructionContext)
{
	const unsigned int		nAttrs = atts.getLength();

	const XalanDOMString*	stylesheetNamespace = 0;
	const XalanDOMString*	resultNamespace = 0;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_STYLESHEET_PREFIX) == true)
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				stylesheetNamespace = getNamespaceForPrefix(DOMServices::s_emptyString, constructionContext);
			}
			else
			{
				stylesheetNamespace = getNamespaceForPrefix(value, constructionContext);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_RESULT_PREFIX))
		{
			const XalanDOMChar* const	value = atts.getValue(i);

			if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
			{
				resultNamespace = getNamespaceForPrefix(DOMServices::s_emptyString, constructionContext);
			}
			else
			{
				resultNamespace = getNamespaceForPrefix(value, constructionContext);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
				XalanMessageLoader::getMessage(
					XalanMessages::TemplateHasIllegalAttribute_2Param,
					name,
					aname));
		}
	}

	// Build a table of aliases, the key is the stylesheet uri and the
	// value is the result uri
	if (stylesheetNamespace == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				name,
				Constants::ATTRNAME_STYLESHEET_PREFIX.c_str()));
	}
	else if (resultNamespace == 0)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				name,
				Constants::ATTRNAME_RESULT_PREFIX.c_str()));
	}
	else
	{
		assert(length(*stylesheetNamespace) != 0  && length(*resultNamespace) != 0);

		m_namespacesHandler.setNamespaceAlias(
				constructionContext,
				*stylesheetNamespace,
				*resultNamespace);
	}
}



void
Stylesheet::processDecimalFormatElement(
			StylesheetConstructionContext&	constructionContext,
			const AttributeListType&		atts,
			const LocatorType*				locator)
{
	const int	lineNumber = XalanLocator::getLineNumber(locator);
	const int	columnNumber = XalanLocator::getColumnNumber(locator);

	m_elemDecimalFormats.reserve(m_elemDecimalFormats.size() + 1);

	m_elemDecimalFormats.push_back(
					new ElemDecimalFormat(
							constructionContext,
							*this,
							atts,
							lineNumber,
							columnNumber));
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



XALAN_CPP_NAMESPACE_END
