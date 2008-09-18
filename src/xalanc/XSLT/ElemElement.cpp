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
#include "ElemElement.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemElement::ElemElement(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber) :
    ElemUse(constructionContext,
            stylesheetTree,
            lineNumber,
            columnNumber,
            StylesheetConstructionContext::ELEMNAME_ELEMENT),
    m_nameAVT(0),
    m_namespaceAVT(0)
{
    // Namespace aliases are not used for xsl:element, so
    // turn them off...
//  m_namespacesHandler.setProcessNamespaceAliaises(false);

    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if(equals(aname, Constants::ATTRNAME_NAME))
        {
            m_nameAVT =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if(equals(aname, Constants::ATTRNAME_NAMESPACE))
        {
            m_namespaceAVT =
                constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this); 
        }
        else if(processUseAttributeSets(
                    constructionContext,
                    aname,
                    atts,
                    i) == false &&
                processSpaceAttr(
                    Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING.c_str(),
                    aname,
                    atts,
                    i,
                    constructionContext)  == false &&
                isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if(0 == m_nameAVT)
    {
            error(
                constructionContext,
                XalanMessages::ElementMustHaveAttribute_2Param,
                Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING,
                Constants::ATTRNAME_NAME);
    }
}



ElemElement::~ElemElement()
{
}



const XalanDOMString&
ElemElement::getElementName() const
{
    return Constants::ELEMNAME_ELEMENT_WITH_PREFIX_STRING;
}



typedef const StylesheetExecutionContext::GetCachedString   GetCachedString;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemElement::startElement(StylesheetExecutionContext&       executionContext) const
{
    XalanDOMString&     elemName = executionContext.getAndPushCachedString();

    m_nameAVT->evaluate(elemName, *this, executionContext);

    bool    isIllegalElement = !XalanQName::isValidQName(elemName);

    if (isIllegalElement == true)
    {
        warn(
            executionContext,
            XalanMessages::IllegalElementName_1Param,
            elemName);

        executionContext.pushSkipElementAttributes(true);
    }
    else
    {
        const GetCachedString   elemNameSpaceGuard(executionContext);

        XalanDOMString&     elemNameSpace = elemNameSpaceGuard.get();

        if (m_namespaceAVT != 0)
        {
            m_namespaceAVT->evaluate(elemNameSpace, *this, executionContext);
        }

        XalanDOMString::size_type   namespaceLen = elemNameSpace.length();

        XalanDOMString::size_type           len = elemName.length();

        const XalanDOMString::size_type     indexOfNSSep = indexOf(elemName, XalanUnicode::charColon);

        const bool  havePrefix = indexOfNSSep == len ? false : true;

        const GetCachedString   prefixGuard(executionContext);

        XalanDOMString&     prefix = prefixGuard.get();

        if (havePrefix == true)
        {
            substring(elemName, prefix, 0, indexOfNSSep);

            const XalanDOMString* const     theNamespace =
                getNamespacesHandler().getNamespace(prefix);

            if(theNamespace == 0 && namespaceLen == 0)
            {
                warn(
                    executionContext,
                    XalanMessages::PrefixIsNotDeclared_1Param,
                    prefix);

                if (m_namespaceAVT != 0)
                {
                    elemName.erase(0, indexOfNSSep + 1);
                }
                else
                {
                    isIllegalElement = true;

                    warn(
                        executionContext,
                        XalanMessages::IllegalElementName_1Param,
                        elemName);
                }
            }
            else if (theNamespace != 0 &&
                namespaceLen == 0 &&
                equals(prefix, DOMServices::s_XMLNamespace) == false)
            {
                elemNameSpace = *theNamespace;
            }
        }

        if (isIllegalElement == false)
        {
            executionContext.startElement(elemName.c_str());   

            if(0 == m_namespaceAVT &&
               havePrefix == false )
            {
                if (havePrefix == false)
                {
                    fixupDefaultNamespace(executionContext);
                }
            }
            else
            {
                if(havePrefix == false)
                {
                    if (namespaceLen > 0)
                    {
                        const XalanDOMString* const     theDefaultNamespace =
                                executionContext.getResultNamespaceForPrefix(s_emptyString);

                        if (theDefaultNamespace == 0 ||
                            equals(*theDefaultNamespace, elemNameSpace) == false)
                        {
                            executionContext.addResultAttribute(
                                    DOMServices::s_XMLNamespace,
                                    elemNameSpace);
                        }
                    }
                    else
                    {
                        // OK, the namespace we're generating is the default namespace,
                        // so let's make sure that we really need it.  If we don't,
                        // we end up with another xmlns="" on the element we're
                        // generating.  Although this isn't really an error, it's
                        // a bit unsightly, so let's suppress it...
                        const XalanDOMString&   theParentDefaultNamespace =
                                getParentDefaultNamespace();

                        if (theParentDefaultNamespace.empty() == false ||
                            executionContext.getResultNamespaceForPrefix(s_emptyString) != 0)
                        {
                            executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
                        }
                    }
                }
                else
                {
                    const XalanDOMString* const     theNamespace =
                            executionContext.getResultNamespaceForPrefix(prefix);

                    if (theNamespace == 0 ||
                        equals(*theNamespace, elemNameSpace) == false)
                    {
                        prefix.insert(0, DOMServices::s_XMLNamespaceWithSeparator);

                        executionContext.addResultAttribute(prefix, elemNameSpace);
                    }
                }
            }
        }

        if (isIllegalElement == true)
        {
            executionContext.pushSkipElementAttributes(true);
        }
        else
        {
            ElemUse::startElement(executionContext);

            executionContext.pushSkipElementAttributes(false);
        }
    }

    return beginExecuteChildren(executionContext);
}



void
ElemElement::endElement(StylesheetExecutionContext&     executionContext) const
{
    endExecuteChildren(executionContext);

    bool ignoreAttributeElements = executionContext.popSkipElementAttributes();
    
    const XalanDOMString& elemName = executionContext.getAndPopCachedString();

    if (!ignoreAttributeElements)
    {
        executionContext.endElement(elemName.c_str());
        ElemUse::endElement(executionContext);
    }
}



bool
ElemElement::executeChildElement(
            StylesheetExecutionContext& executionContext,
            const ElemTemplateElement*  element) const
{
    return !(element->getXSLToken() == StylesheetConstructionContext::ELEMNAME_ATTRIBUTE
             && executionContext.getSkipElementAttributes() == true);
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemElement::execute(StylesheetExecutionContext&        executionContext) const
{
    const GetCachedString   elemNameGuard(executionContext);

    XalanDOMString&     elemName = elemNameGuard.get();

    m_nameAVT->evaluate(elemName, *this, executionContext);

    bool    isIllegalElement = !XalanQName::isValidQName(elemName);

    if (isIllegalElement == true)
    {
        warn(
            executionContext,
            XalanMessages::IllegalElementName_1Param,
            elemName);

        ElemUse::doExecute(executionContext, false);

        doExecuteChildren(executionContext, true);
    }
    else
    {
        const GetCachedString   elemNameSpaceGuard(executionContext);

        XalanDOMString&     elemNameSpace = elemNameSpaceGuard.get();

        if (m_namespaceAVT != 0)
        {
            m_namespaceAVT->evaluate(elemNameSpace, *this, executionContext);
        }

        XalanDOMString::size_type   namespaceLen = length(elemNameSpace);

        bool    foundResultNamespaceForPrefix = false;

        XalanDOMString::size_type           len = length(elemName);

        const XalanDOMString::size_type     indexOfNSSep = indexOf(elemName, XalanUnicode::charColon);

        const bool  havePrefix = indexOfNSSep == len ? false : true;

        const GetCachedString   prefixGuard(executionContext);

        XalanDOMString&     prefix = prefixGuard.get();

        if (havePrefix == true)
        {
            substring(elemName, prefix, 0, indexOfNSSep);

            const XalanDOMString* const     theNamespace =
                executionContext.getResultNamespaceForPrefix(prefix);

            if (theNamespace != 0)
            {
                foundResultNamespaceForPrefix = true;
            }
            else
            {
                const XalanDOMString* const     theNamespace =
                    getNamespacesHandler().getNamespace(prefix);

                if(theNamespace == 0 && namespaceLen == 0)
                {
                    warn(
                        executionContext,
                        XalanMessages::PrefixIsNotDeclared_1Param,
                        prefix);

                    if (m_namespaceAVT != 0)
                    {
                        elemName.erase(0, indexOfNSSep + 1);
                    }
                    else
                    {
                        isIllegalElement = true;

                        warn(
                            executionContext,
                            XalanMessages::IllegalElementName_1Param,
                            elemName);
                    }
                }
                else if (theNamespace != 0 &&
                         namespaceLen == 0 &&
                         equals(prefix, DOMServices::s_XMLNamespace) == false)
                {
                    elemNameSpace = *theNamespace;
                }
            }
        }

        if (isIllegalElement == false)
        {
            executionContext.startElement(elemName.c_str());   

            if(0 == m_namespaceAVT &&
               (havePrefix == false || foundResultNamespaceForPrefix == true))
            {
                if (havePrefix == false)
                {
                    fixupDefaultNamespace(executionContext);
                }
            }
            else
            {
                if(havePrefix == false)
                {
                    if (namespaceLen > 0)
                    {
                        const XalanDOMString* const     theDefaultNamespace =
                                executionContext.getResultNamespaceForPrefix(s_emptyString);

                        if (theDefaultNamespace == 0 ||
                            equals(*theDefaultNamespace, elemNameSpace) == false)
                        {
                            executionContext.addResultAttribute(
                                    DOMServices::s_XMLNamespace,
                                    elemNameSpace);
                        }
                    }
                    else
                    {
                        // OK, the namespace we're generating is the default namespace,
                        // so let's make sure that we really need it.  If we don't,
                        // we end up with another xmlns="" on the element we're
                        // generating.  Although this isn't really an error, it's
                        // a bit unsightly, so let's suppress it...
                        const XalanDOMString&   theParentDefaultNamespace =
                                getParentDefaultNamespace();

                        if (length(theParentDefaultNamespace) == 0)
                        {
                            if (executionContext.getResultNamespaceForPrefix(s_emptyString) != 0)
                            {
                                executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
                            }
                        }
                        else
                        {
                            executionContext.addResultAttribute(DOMServices::s_XMLNamespace, elemNameSpace);
                        }
                    }
                }
                else
                {
                    const XalanDOMString* const     theNamespace =
                            executionContext.getResultNamespaceForPrefix(prefix);

                    if (theNamespace == 0 ||
                        equals(*theNamespace, elemNameSpace) == false)
                    {
                        insert(prefix, 0, DOMServices::s_XMLNamespaceWithSeparator);

                        executionContext.addResultAttribute(prefix, elemNameSpace);
                    }
                }
            }
        }

        if (isIllegalElement == true)
        {
            ElemUse::doExecute(executionContext, false);

            doExecuteChildren(executionContext, true);
        }
        else
        {
            ElemUse::doExecute(executionContext, true);

            doExecuteChildren(executionContext, false);

            executionContext.endElement(elemName.c_str());
        }
    }
}



void
ElemElement::doExecuteChildren(
            StylesheetExecutionContext&     executionContext,           
            bool                            skipAttributeChildren) const
{
    if (skipAttributeChildren == false)
    {
        // If we should execute all children, then just call
        // executeChildren()...
        executeChildren(executionContext);
    }
    else
    {
        StylesheetExecutionContext::PushAndPopElementFrame  thePushAndPop(executionContext, this);

        for (ElemTemplateElement* node = getFirstChildElem(); node != 0; node = node->getNextSiblingElem()) 
        {
            if (node->getXSLToken() != StylesheetConstructionContext::ELEMNAME_ATTRIBUTE)
            {
                node->execute(executionContext);
            }
        }
    }
}
#endif



void
ElemElement::namespacesPostConstruction(
            StylesheetConstructionContext&  constructionContext,
            const NamespacesHandler&        theParentHandler,
            NamespacesHandler&              theHandler)
{
    theHandler.postConstruction(
            constructionContext,
            false,
            getElementName(),
            &theParentHandler);
}



void
ElemElement::fixupDefaultNamespace(StylesheetExecutionContext&  executionContext) const
{
    // OK, now let's check to make sure we don't have to change the default namespace...
    const XalanDOMString* const     theCurrentDefaultNamespace =
                executionContext.getResultNamespaceForPrefix(s_emptyString);

    const XalanDOMString* const     theElementDefaultNamespace =
                getNamespacesHandler().getNamespace(s_emptyString);

    if (theCurrentDefaultNamespace != 0)
    {
        if (theElementDefaultNamespace == 0)
        {
            // There was no default namespace, so we have to turn the
            // current one off.
            executionContext.addResultAttribute(DOMServices::s_XMLNamespace, s_emptyString);
        }
        else if (equals(*theCurrentDefaultNamespace, *theElementDefaultNamespace) == false)
        {
            // There is a default namespace, but it's different from the current one,
            // so we have to change it.
            executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
        }
    }
    else if (theElementDefaultNamespace != 0)
    {
        executionContext.addResultAttribute(DOMServices::s_XMLNamespace, *theElementDefaultNamespace);
    }
}



const XalanDOMString&
ElemElement::getParentDefaultNamespace() const
{
    const ElemTemplateElement* const    theParent =
        getParentNodeElem();

    if (theParent == 0)
    {
        return s_emptyString;
    }
    else
    {
        const XalanDOMString* const     theParentDefaultNamespace =
                        theParent->getNamespacesHandler().getNamespace(s_emptyString);

        if (theParentDefaultNamespace == 0)
        {
            return s_emptyString;
        }
        else
        {
            return *theParentDefaultNamespace;
        }
    }
}



XALAN_CPP_NAMESPACE_END
