/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#include "XalanMatchPatternData.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString            Stylesheet::s_emptyString(XalanMemMgrs::getDummyMemMgr());

const Stylesheet::PatternTableVectorType    Stylesheet::s_emptyTemplateList(XalanMemMgrs::getDummyMemMgr());

const XalanQNameByReference     Stylesheet::s_emptyQName;



Stylesheet::Stylesheet(
        StylesheetRoot&                 root,
        const XalanDOMString&           baseIdentifier,
        StylesheetConstructionContext&  constructionContext) :
    PrefixResolver(),
    m_stylesheetRoot(root),
    m_baseIdent(baseIdentifier,constructionContext.getMemoryManager()),
    m_keyDeclarations(constructionContext.getMemoryManager()),
    m_whitespaceElements(constructionContext.getMemoryManager()),
    m_XSLTNamespaceURI(constructionContext.getXSLTNamespaceURI(),constructionContext.getMemoryManager()),
    m_imports(constructionContext.getMemoryManager()),
    m_importsSize(0),
    m_namespaces(constructionContext.getMemoryManager()),
    m_namespaceDecls(constructionContext.getMemoryManager()),
    m_isWrapperless(false),
    m_extensionNamespaces(constructionContext.getMemoryManager()),
    m_firstTemplate(0),
    m_includeStack(constructionContext.getMemoryManager()),
    m_namedTemplates(constructionContext.getMemoryManager()),
    m_topLevelVariables(constructionContext.getMemoryManager()),
    m_XSLTVerDeclared(1.0L),
    m_elementPatternTable(constructionContext.getMemoryManager()),
    m_elementPatternTableEnd(m_elementPatternTable.end()),
    m_elementAnyPatternList(constructionContext.getMemoryManager()),
    m_attributePatternTable(constructionContext.getMemoryManager()),
    m_attributePatternTableEnd(m_attributePatternTable.end()),
    m_attributeAnyPatternList(constructionContext.getMemoryManager()),
    m_textPatternList(constructionContext.getMemoryManager()),
    m_commentPatternList(constructionContext.getMemoryManager()),
    m_rootPatternList(constructionContext.getMemoryManager()),
    m_piPatternList(constructionContext.getMemoryManager()),
    m_nodePatternList(constructionContext.getMemoryManager()),
    m_patternCount(0),
    m_elemDecimalFormats(constructionContext.getMemoryManager()),
    m_namespacesHandler(constructionContext.getMemoryManager())
{
    if (m_baseIdent.empty() == true)
    {
        m_includeStack.push_back(m_baseIdent);
    }
    else
    {
        try
        {
            const GetCachedString   theGuard(constructionContext);

            XalanDOMString&     urlString = theGuard.get();

            constructionContext.getURLStringFromString(m_baseIdent, urlString);

            if (urlString.empty() == false)
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

    
    
Stylesheet*
Stylesheet::create(
            MemoryManager&                  theManager,
            StylesheetRoot&                 root,
            const XalanDOMString&           baseIdentifier,
            StylesheetConstructionContext&  constructionContext)
{
    Stylesheet* theInstance;

    return XalanConstruct(
            theManager,
            theInstance,
            root,
            baseIdentifier,
            constructionContext);
}



Stylesheet::~Stylesheet()
{
#if !defined(XALAN_NO_STD_NAMESPACE)
    using std::for_each;
#endif

    // Clean up all entries in the imports vector.
    for_each(
        m_imports.begin(),
        m_imports.end(),
        DeleteFunctor<Stylesheet>(m_imports.getMemoryManager()));

    // Clean up the decimal formats vector
    for_each(
        m_elemDecimalFormats.begin(),
        m_elemDecimalFormats.end(),
        DeleteFunctor<ElemDecimalFormat>(m_elemDecimalFormats.getMemoryManager()));


    for_each(
        m_extensionNamespaces.begin(),
        m_extensionNamespaces.end(),
        makeMapValueDeleteFunctor(m_extensionNamespaces));

}



void
Stylesheet::error(
            StylesheetConstructionContext&  theContext,
            XalanMessages::Codes            theErrorCode,
            const Locator*                  theLocator,
            const XalanDOMChar*             theParam1,
            const XalanDOMChar*             theParam2,
            const XalanDOMChar*             theParam3) const
{
    const GetCachedString   theGuard(theContext);

    theContext.problem(
        StylesheetConstructionContext::eXSLTProcessor,
        StylesheetConstructionContext::eError,
        XalanMessageLoader::getMessage(
            theGuard.get(),
            theErrorCode,
            theParam1,
            theParam2,
            theParam3),
        theLocator,
        0);
}



ElemTemplateElement*
Stylesheet::initWrapperless(
            StylesheetConstructionContext&  constructionContext,
            const Locator*                  locator)
{
    if (m_isWrapperless == true)
    {
        error(
            constructionContext,
            XalanMessages::StylesheetHasWrapperlessTemplate,
            locator);
    }

    assert(m_firstTemplate == 0);

    m_isWrapperless = true;


    AttributeListImpl   templateAttrs(constructionContext.getMemoryManager());

    templateAttrs.addAttribute(Constants::ATTRNAME_NAME.c_str(),
                               Constants::ATTRTYPE_CDATA.c_str(),
                               Constants::ATTRVAL_SIMPLE.c_str());

    ElemTemplateElement* const  theNewTemplate =
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
            const PrefixResolver&           nsContext,
            const AttributeListType&        atts,
            const Locator*                  locator,
            StylesheetConstructionContext&  constructionContext)
{
    const XalanQName*       theQName = 0;
    XPath*                  matchAttr = 0;
    XPath*                  useAttr = 0;
 
    const XalanSize_t   nAttrs = atts.getLength();

    for(XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_NAME))
        {
            theQName = constructionContext.createXalanQName(atts.getValue(i), m_namespaces, locator);

            // $$$TODO: It's not clear of this code will ever be executed, since
            // constructing an invalid QName is not possibly right now.
            assert(theQName->isValid() == true);
            if (theQName->isValid() == false)
            {
                error(
                    constructionContext,
                    XalanMessages::AttributeValueNotValidQName_2Param,
                    locator,
                    Constants::ATTRNAME_NAME.c_str(),
                    atts.getValue(i));
            }
        }
        else if(equals(aname, Constants::ATTRNAME_MATCH))
        {
            const GetCachedString   theGuard(constructionContext);

            XalanDOMString&     theBuffer = theGuard.get();

            theBuffer.assign(atts.getValue(i));

            matchAttr =
                    constructionContext.createMatchPattern(
                        0,
                        theBuffer,
                        nsContext,
                        false,
                        false);
        }
        else if(equals(aname, Constants::ATTRNAME_USE))
        {
            useAttr =
                    constructionContext.createXPath(
                        0,
                        atts.getValue(i),
                        nsContext,
                        false,
                        false);
        }
        else if (isAttrOK(aname, atts, i, constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                locator,
                Constants::ELEMNAME_KEY_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (0 == theQName)
    {
        error(
            constructionContext,
            XalanMessages::ElementRequiresAttribute_2Param,
            locator,
            Constants::ELEMNAME_KEY_WITH_PREFIX_STRING.c_str(),
            Constants::ATTRNAME_NAME.c_str());
    }

    if (0 == matchAttr)
    {
        error(
            constructionContext,
            XalanMessages::ElementRequiresAttribute_2Param,
            locator,
            Constants::ELEMNAME_KEY_WITH_PREFIX_STRING.c_str(),
            Constants::ATTRNAME_MATCH.c_str());
    }

    if (0 == useAttr)
    {
        error(
            constructionContext,
            XalanMessages::ElementRequiresAttribute_2Param,
            locator,
            Constants::ELEMNAME_KEY_WITH_PREFIX_STRING.c_str(),
            Constants::ATTRNAME_USE.c_str());
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
Stylesheet::pushNamespaces(const AttributeListType&     atts)
{
    const XalanSize_t       nAttrs = atts.getLength();

    NamespaceVectorType     namespaces(getMemoryManager());

    XalanDOMString          prefix(getMemoryManager());

    for(XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);
        const XalanDOMChar* const   value = atts.getValue(i);

        const bool  isPrefix = startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator);

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

            const NameSpace     newNS(prefix, XalanDOMString(value, getMemoryManager()), getMemoryManager());

            namespaces.push_back(newNS);
        }
    }

    m_namespaces.push_back(namespaces);
}



class attrSetCompare
{
public:

    attrSetCompare(const ElemAttributeSet&  theAttrSet) :
        m_attrSet(theAttrSet)
    {
    }

    bool
    operator()(const ElemAttributeSet*  theRHS) const
    {
        assert(theRHS != 0);

        return m_attrSet == *theRHS;
    }

private:

    const ElemAttributeSet&     m_attrSet;
};



static void
addToList(
            Stylesheet::PatternTableVectorType&     theList,
            const XalanMatchPatternData*            thePattern)
{
    assert(thePattern != 0);

    const double        thePatternPriority = thePattern->getPriorityOrDefault();
    const XalanSize_t   thePatternPosition = thePattern->getPosition();

    typedef Stylesheet::PatternTableVectorType  PatternTableListType;
    typedef PatternTableListType::iterator      iterator;

    iterator        theCurrent = theList.begin();

    const iterator  theEnd = theList.end();

    while (theCurrent != theEnd)
    {
        const double    theCurrentPriority = (*theCurrent)->getPriorityOrDefault();

        if (thePatternPriority > theCurrentPriority)
        {
            break;
        }
        else if (thePatternPriority == theCurrentPriority &&
                 thePatternPosition > (*theCurrent)->getPosition())
        {
            break;
        }

        ++theCurrent;
    }

    theList.insert(theCurrent, thePattern);
}



static void
addToTable(
           Stylesheet::PatternTableMapType&             theTable,
           const Stylesheet::PatternTableVectorType&    theList)
{
    typedef Stylesheet::PatternTableMapType     PatternTableMapType;
    typedef Stylesheet::PatternTableVectorType  PatternTableListType;

    PatternTableMapType::iterator               theCurrentTable = theTable.begin();
    const PatternTableMapType::iterator         theTableEnd = theTable.end();
    const PatternTableListType::const_iterator  theListEnd = theList.end();

    while(theCurrentTable != theTableEnd)
    {
        PatternTableListType::const_iterator    theCurrent = theList.begin();

        while(theCurrent != theListEnd)
        {
            addToList((*theCurrentTable).second, *theCurrent);

            ++theCurrent;
        }

        ++theCurrentTable;
    }
}



void
Stylesheet::addWhitespaceElement(const XalanSpaceNodeTester&    theTester)
{
    typedef WhitespaceElementsVectorType::iterator  iterator;

    const XPath::eMatchScore    theMatchScore = theTester.getMatchScore();

    iterator    i = m_whitespaceElements.begin();

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
Stylesheet::postConstruction(StylesheetConstructionContext&     constructionContext)
{
    KeyDeclarationVectorType::size_type         theKeyDeclarationsCount = 0;
    WhitespaceElementsVectorType::size_type     theWhitespaceElementsCount = 0;

    {
        m_importsSize = m_imports.size();

        // Call postConstruction() on any imported stylesheets, in reverse order,
        // so namespace aliases are processed properly. Also, get any key
        // declarations and preserve/strip space information.
        const StylesheetVectorType::reverse_iterator    theEnd = m_imports.rend();
        StylesheetVectorType::reverse_iterator          i = m_imports.rbegin();

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
        const StylesheetVectorType::iterator    theEnd = m_imports.end();
        StylesheetVectorType::iterator          i = m_imports.begin();

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

            {
                KeyDeclarationVectorType temp(getMemoryManager());
                temp.swap((*i)->m_keyDeclarations);
            }

            m_whitespaceElements.insert(
                m_whitespaceElements.end(),
                (*i)->m_whitespaceElements.begin(),
                (*i)->m_whitespaceElements.end());

            {
                WhitespaceElementsVectorType temp(getMemoryManager());
                temp.swap((*i)->m_whitespaceElements);
            }

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
}



bool
Stylesheet::isAttrOK(
            const XalanDOMChar*             attrName,
            const AttributeListType&        /* atts */,
            XalanSize_t                     /* which */,
            StylesheetConstructionContext&  constructionContext) const
{
    // Namespace declarations are OK by definition
    bool attrOK =
        equals(
            attrName,
            DOMServices::s_XMLNamespace) ||
        startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator);

    if(!attrOK)
    {
        // Others are OK if their prefix has been
        // bound to a non-null Namespace URI other than XSLT's
        const XalanDOMString::size_type     indexOfNSSep = indexOf(attrName, XalanUnicode::charColon);

        if(indexOfNSSep < length(attrName))
        {
            const GetCachedString   theGuard(constructionContext);

            XalanDOMString&     prefix = theGuard.get();

            prefix.assign(attrName, indexOfNSSep);

            const XalanDOMString*   ns = getNamespaceForPrefixFromStack(prefix);

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
Stylesheet::getNamespaceFromStack(const XalanDOMChar*   nodeName,
                                  XalanDOMString&       theBuffer) const
{
    assert(nodeName != 0);

    const XalanDOMString::size_type     indexOfNSSep = indexOf(nodeName, XalanUnicode::charColon);

    if (indexOfNSSep == length(nodeName))
    {
        return getNamespaceForPrefixFromStack(s_emptyString);
    }
    else
    {
        theBuffer.assign(nodeName, indexOfNSSep);

        return getNamespaceForPrefixFromStack(theBuffer);
    }
}



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(
            const XalanDOMString&           prefix,
            StylesheetConstructionContext&  constructionContext) const
{
    const XalanDOMString* const     theURI = getNamespaceForPrefix(prefix);

    if (theURI == 0)
    {
        error(
            constructionContext,
            XalanMessages::PrefixIsNotDeclared_1Param,
            constructionContext.getLocatorFromStack(),
            prefix.c_str());
    }

    return theURI;
}



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(
            const XalanDOMChar*             prefix,
            StylesheetConstructionContext&  constructionContext) const
{
    const GetCachedString   theGuard(constructionContext);

    XalanDOMString&     theTemp = theGuard.get();

    theTemp.assign(prefix);

    return getNamespaceForPrefix(theTemp, constructionContext);
}



bool
Stylesheet::getYesOrNo(
            const XalanDOMChar*             aname,
            const XalanDOMChar*             val,
            StylesheetConstructionContext&  constructionContext) const
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
        error(
            constructionContext,
            XalanMessages::AttributeMustBe_3Params,
            constructionContext.getLocatorFromStack(),
            aname,
            Constants::ATTRVAL_YES.c_str(),
            Constants::ATTRVAL_NO.c_str());

        return false;
    }
}



void
Stylesheet::addTemplate(
            ElemTemplate*                   theTemplate,
            StylesheetConstructionContext&  constructionContext)
{
    assert(theTemplate != 0);

    if (m_isWrapperless == true)
    {
        if (m_firstTemplate != 0)
        {
            error(
                constructionContext,
                XalanMessages::StylesheetHasWrapperlessTemplate,
                theTemplate->getLocator());
        }
        else
        {
            m_firstTemplate = theTemplate;
        }
    }
    else if (0 == m_firstTemplate)
    {
        m_firstTemplate = theTemplate;
    }
    else
    {
        ElemTemplateElement*    next = m_firstTemplate;

        // Find the last one, then append the new one.
        while (0 != next)
        {
            if (0 == next->getNextSiblingElem())
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
    const XalanQName&   theName = theTemplate->getNameAttribute();

    if (theName.isEmpty() == false)
    {
        if (m_namedTemplates.find(theName) == m_namedTemplates.end())
        {
            m_namedTemplates[theName] = theTemplate;
        }
        else
        {
            const GetCachedString   theGuard(constructionContext);

            error(
                constructionContext,
                XalanMessages::StylesheetHasDuplicateNamedTemplate_1Param,
                theTemplate->getLocator(),
                theName.format(theGuard.get()).c_str());
        }
    }

    // Now, process the match pattern associated with the
    // template.
    const XPath* const  xp = theTemplate->getMatchPattern();

    if (0 != xp)
    {
        /* Each string has a list of pattern tables associated with it; if the
         * string is not in the map, then create a list of pattern tables with one
         * entry for the string, otherwise add to the existing pattern table list
         * for that string
         */
        typedef XPath::TargetDataVectorType     TargetDataVectorType;

        TargetDataVectorType    data(constructionContext.getMemoryManager());

        xp->getTargetData(data);

        TargetDataVectorType::size_type nTargets =
                data.size();

        if (nTargets != 0)
        {
            const GetCachedString   theGuard(constructionContext);

            XalanDOMString& tempString = theGuard.get();

            for (TargetDataVectorType::size_type i = 0; i < nTargets; ++i) 
            {
                assert(data[i].getString() != 0);
                
                tempString = data[i].getString();

                const XalanMatchPatternData* const  newMatchPat =
                    constructionContext.createXalanMatchPatternData(
                        *theTemplate,
                        m_patternCount,
                        tempString,
                        *xp,
                        xp->getExpression().getCurrentPattern(),
                        data[i].getDefaultPriority());

                ++m_patternCount;

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
Stylesheet::findNamedTemplate(const XalanQName&     qname) const
{
    const ElemTemplateMapType::const_iterator   it = m_namedTemplates.find(qname);

    if(it != m_namedTemplates.end())
    {
        return (*it).second;
    }
    else
    {
        const ElemTemplate*     namedTemplate = 0;

        // Look for the template in the imports
        const StylesheetVectorType::size_type   importsCount = m_imports.size();

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
            const XalanMatchPatternData*    thePattern,
            PatternTableVectorType&         theVector)
{
    XALAN_USING_STD(find)

    const PatternTableVectorType::const_iterator    theResult =
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
            const XalanMatchPatternData*    thePattern,
            const XalanMatchPatternData*    thePatternArray[],
            XalanSize_t&                    thePatternArraySize)
{
    assert(thePattern != 0 && thePatternArray != 0);

    if (thePatternArraySize == 0)
    {
        thePatternArray[0] = thePattern;

        ++thePatternArraySize;
    }
    else
    {
        XalanSize_t     i = 0;

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
Stylesheet::locateElementMatchPatternDataList(const XalanDOMString&     theName) const
{
    assert(m_elementPatternTableEnd == m_elementPatternTable.end());

    const PatternTableMapType::const_iterator   i =
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
Stylesheet::locateAttributeMatchPatternDataList(const XalanDOMString&   theName) const
{
    assert(m_attributePatternTableEnd == m_attributePatternTable.end());

    const PatternTableMapType::const_iterator   i =
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
Stylesheet::locateMatchPatternDataList(
            const XalanNode&        theNode,
            XalanNode::NodeType     targetNodeType) const
{
    assert(theNode.getNodeType() == targetNodeType);

    switch(targetNodeType)
    {
    case XalanNode::ELEMENT_NODE:
        return locateElementMatchPatternDataList(DOMServices::getLocalNameOfNode(theNode));
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        return &m_piPatternList;
        break;

    case XalanNode::ATTRIBUTE_NODE:
        if ((DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(theNode)) == true))
        {
            return &s_emptyTemplateList;
        } 
        else
        {
            return locateAttributeMatchPatternDataList(DOMServices::getLocalNameOfNode(theNode));
        }
        break;

    case XalanNode::CDATA_SECTION_NODE:
    case XalanNode::TEXT_NODE:
        return &m_textPatternList;
        break;

    case XalanNode::COMMENT_NODE:
        return &m_commentPatternList;
        break;

    case XalanNode::DOCUMENT_NODE:
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        return &m_rootPatternList;
        break;

    default:
        break;
    }

    return &m_nodePatternList;
}



inline const ElemTemplate*
Stylesheet::findTemplateInImports(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      targetNode,
            XalanNode::NodeType             targetNodeType,
            const XalanQName&               mode) const
{
    assert(targetNode->getNodeType() == targetNodeType);
    assert(m_importsSize == m_imports.size());

    for(StylesheetVectorType::size_type i = 0; i < m_importsSize; i++)
    {
        const Stylesheet* const     stylesheet =
            m_imports[i];

        const ElemTemplate* const   bestMatchedRule =
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
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      targetNode,
            XalanNode::NodeType             targetNodeType,
            const XalanQName&               mode,
            bool                            onlyUseImports) const
{
    assert(targetNode != 0);
    assert(targetNode->getNodeType() == targetNodeType);

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
        const ElemTemplate*     bestMatchedRule = 0;

        if (executionContext.getQuietConflictWarnings() == true)
        {
            // Points to the current list of match patterns.  Note
            // that this may point to more than one table.
            const PatternTableVectorType*   matchPatternList =
                    locateMatchPatternDataList(*targetNode, targetNodeType);
            assert(matchPatternList != 0);

            PatternTableVectorType::const_iterator      theCurrentEntry =
                    matchPatternList->begin();

            const PatternTableVectorType::const_iterator    theTableEnd =
                    matchPatternList->end();

            while (theCurrentEntry != theTableEnd)
            {
                const XalanMatchPatternData*    matchPat = *theCurrentEntry;
                assert(matchPat != 0);

                const ElemTemplate* const   rule = matchPat->getTemplate();
                assert(rule != 0);

                // We'll be needing to match rules according to what 
                // mode we're in.
                const XalanQName&   ruleMode = rule->getMode();

                // The logic here should be that if we are not in a mode AND
                // the rule does not have a node, then go ahead.
                // OR if we are in a mode, AND the rule has a node, 
                // AND the rules match, then go ahead.
                const bool  haveMode = !mode.isEmpty();
                const bool  haveRuleMode = !ruleMode.isEmpty();

                if ((!haveMode && !haveRuleMode) ||
                    (haveMode && haveRuleMode && ruleMode.equals(mode)))
                {
                    const XPath* const  xpath = matchPat->getExpression();

                    XPath::eMatchScore  score =
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
            const PatternTableVectorType*   matchPatternList =
                    locateMatchPatternDataList(*targetNode, targetNodeType);
            assert(matchPatternList != 0);

            PatternTableVectorType::const_iterator      theCurrentEntry =
                    matchPatternList->begin();

            const PatternTableVectorType::const_iterator    theTableEnd =
                    matchPatternList->end();

            if (theCurrentEntry != theTableEnd)
            {
                const XalanMatchPatternData*    bestMatchedPattern = 0; // Syncs with bestMatchedRule
                const double            matchScoreNoneValue = 
                    XPath::getMatchScoreValue(XPath::eMatchScoreNone);

                double                  bestMatchPatPriority = matchScoreNoneValue;

                XalanSize_t             nConflicts = 0;

                // Use a stack-based array when possible...
                const XalanMatchPatternData*    conflictsArray[100];

                XalanVector<const XalanMatchPatternData*>   conflictsVector(executionContext.getMemoryManager());

                const XalanMatchPatternData**   conflicts = 0;

                const XalanDOMString*           prevPat = 0;
                const XalanMatchPatternData*    prevMatchPat = 0;

                do
                {
                    const XalanMatchPatternData*    matchPat = *theCurrentEntry;
                    double                  matchPatPriority = matchScoreNoneValue;
                    assert(matchPat != 0);

                    const ElemTemplate* const   rule = matchPat->getTemplate();
                    assert(rule != 0);

                    // We'll be needing to match rules according to what 
                    // mode we're in.
                    const XalanQName&   ruleMode = rule->getMode();

                    // The logic here should be that if we are not in a mode AND
                    // the rule does not have a node, then go ahead.
                    // OR if we are in a mode, AND the rule has a node, 
                    // AND the rules match, then go ahead.
                    const bool  haveMode = !mode.isEmpty();
                    const bool  haveRuleMode = !ruleMode.isEmpty();

                    if ((!haveMode && !haveRuleMode) ||
                        (haveMode && haveRuleMode && ruleMode.equals(mode)))
                    {
                        const XalanDOMString*   patterns = matchPat->getPattern();
                        assert(patterns != 0);

                        if(!patterns->empty() &&
                           !(prevMatchPat != 0 &&
                             (prevPat != 0 && equals(*prevPat, *patterns)) &&
                             prevMatchPat->getTemplate()->getPriority() == matchPat->getTemplate()->getPriority()))
                        {
                            prevPat = patterns;
                            prevMatchPat = matchPat;
                            matchPatPriority = matchScoreNoneValue;

                            const XPath* const  xpath = matchPat->getExpression();

                            XPath::eMatchScore  score =
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
                                            conflictsVector.resize(m_patternCount);

                                            conflicts = conflictsVector.begin();
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

                    // Since the templates are arranged by their
                    // priority and reverse order in the stylesheet,
                    // the template with the highest priority that
                    // was last in the stylesheet will be the
                    // first one in the array.
                    bestMatchedPattern = conflicts[0];

                    bestMatchedRule = bestMatchedPattern->getTemplate();

                    const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

                    executionContext.problem(
                        StylesheetExecutionContext::eXSLTProcessor,
                        StylesheetExecutionContext::eWarning,
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::ConflictsFound),
                        bestMatchedRule->getLocator(),
                        targetNode);
                }
            }

            if (0 == bestMatchedRule)
            {
                bestMatchedRule = findTemplateInImports(executionContext, targetNode, targetNodeType, mode);
            }
        }

        return bestMatchedRule;
    }
}



void
Stylesheet::processExtensionNamespace(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMString&           uri)
{
    XalanMemMgrAutoPtr<ExtensionNSHandler>  theGuard(
                                                theConstructionContext.getMemoryManager(),
                                                ExtensionNSHandler::create(
                                                    uri,
                                                    theConstructionContext.getMemoryManager()));

    m_extensionNamespaces.insert(uri, theGuard.get());

    theGuard.release();

    m_namespacesHandler.addExtensionNamespaceURI(theConstructionContext, uri);
}



void
Stylesheet::pushTopLevelVariables(
            StylesheetExecutionContext&     executionContext,
            const ParamVectorType&          topLevelParams) const
{
    {
        // First, push any imports...
        const StylesheetVectorType::const_reverse_iterator  rend = m_imports.rend();

        for (StylesheetVectorType::const_reverse_iterator i = m_imports.rbegin(); i != rend; ++i)
        {
            const Stylesheet* const stylesheet = *i;
            assert(stylesheet != 0);

            stylesheet->pushTopLevelVariables(executionContext, topLevelParams);
        }
    }

    const ParamVectorType::size_type    nVars = m_topLevelVariables.size();

    for (ParamVectorType::size_type i = 0; i < nVars; ++i)
    {
        ElemVariable* const     var = m_topLevelVariables[i];

        bool                    isParam =
                StylesheetConstructionContext::ELEMNAME_PARAM == var->getXSLToken();

        if(isParam == true)
        {
            isParam = false;

            const ParamVectorType::size_type    n = topLevelParams.size();

            for(ParamVectorType::size_type k = 0; k < n; k++)
            {
                const ParamVectorType::value_type&  arg = topLevelParams[k];

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
            const XalanDOMChar*             name,
            const AttributeListType&        atts,
            StylesheetConstructionContext&  constructionContext)
{
    const XalanSize_t       nAttrs = atts.getLength();

    const XalanDOMString*   stylesheetNamespace = 0;
    const XalanDOMString*   resultNamespace = 0;

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (aname == Constants::ATTRNAME_STYLESHEET_PREFIX)
        {
            const XalanDOMChar* const   value = atts.getValue(i);

            if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
            {
                stylesheetNamespace =
                    getNamespaceForPrefix(
                        DOMServices::s_emptyString,
                        constructionContext);
            }
            else
            {
                stylesheetNamespace =
                    getNamespaceForPrefix(
                        value,
                        constructionContext);
            }
        }
        else if (aname == Constants::ATTRNAME_RESULT_PREFIX)
        {
            const XalanDOMChar* const   value = atts.getValue(i);

            if (equals(value, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
            {
                resultNamespace =
                    getNamespaceForPrefix(
                        DOMServices::s_emptyString,
                        constructionContext);
            }
            else
            {
                resultNamespace =
                    getNamespaceForPrefix(
                        value,
                        constructionContext);
            }
        }
        else if (!isAttrOK(aname, atts, i, constructionContext))
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                constructionContext.getLocatorFromStack(),
                name,
                aname);
        }
    }

    // Build a table of aliases, the key is the stylesheet uri and the
    // value is the result uri
    if (stylesheetNamespace == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            constructionContext.getLocatorFromStack(),
            name,
            Constants::ATTRNAME_STYLESHEET_PREFIX.c_str());
    }
    else if (resultNamespace == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            constructionContext.getLocatorFromStack(),
            name,
            Constants::ATTRNAME_RESULT_PREFIX.c_str());
    }
    else
    {
        assert(stylesheetNamespace->empty() == false && resultNamespace->empty() == false);

        m_namespacesHandler.setNamespaceAlias(
                constructionContext,
                *stylesheetNamespace,
                *resultNamespace);
    }
}



void
Stylesheet::processDecimalFormatElement(
            StylesheetConstructionContext&  constructionContext,
            const AttributeListType&        atts,
            const Locator*                  locator)
{
    const XalanFileLoc  lineNumber =
        XalanLocator::getLineNumber(locator);

    const XalanFileLoc  columnNumber =
        XalanLocator::getColumnNumber(locator);

    m_elemDecimalFormats.reserve(m_elemDecimalFormats.size() + 1);

    ElemDecimalFormat*  theInstance;

    XalanConstruct(
        constructionContext.getMemoryManager(),
        theInstance,
        constructionContext,
        *this,
        atts,
        lineNumber,
        columnNumber);

    m_elemDecimalFormats.push_back(theInstance);
}



const XalanDecimalFormatSymbols*
Stylesheet::getDecimalFormatSymbols(const XalanQName&   theQName) const
{
    const XalanDecimalFormatSymbols*                dfs = 0;

    const ElemDecimalFormatVectorType::size_type    theSize =
        m_elemDecimalFormats.size();

    if (theSize > 0)
    {
        // Start from the top of the stack
        for (ElemDecimalFormatVectorType::size_type i = theSize; i > 0; --i)
        {
            assert(m_elemDecimalFormats[i - 1] != 0);

            const ElemDecimalFormat* const  theCurrent =
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
    // with the given name. So go up the import hierarchy
    // and see if one of the imported stylesheets declared
    // it.
    if (dfs == 0)
    {
        for (StylesheetVectorType::size_type i = 0; i < m_importsSize; ++i)
        {
            dfs = m_imports[i]->getDecimalFormatSymbols(theQName);

            if (dfs != 0)
            {
                break;
            }
        }
    }

    return dfs;
}



const XalanDOMString*
Stylesheet::getNamespaceForPrefix(const XalanDOMString&     prefix) const
{
    return XalanQName::getNamespaceForPrefix(m_namespaceDecls, prefix);
}



const XalanDOMString&
Stylesheet::getURI() const
{
    return m_baseIdent;
}



XALAN_CPP_NAMESPACE_END
