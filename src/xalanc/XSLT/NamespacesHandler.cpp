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


// Class header file...
#include "NamespacesHandler.hpp"



#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



template<class VectorType, class MemberFunctionType>
const typename VectorType::value_type*
find(
            const VectorType&       theVector,
            const XalanDOMString&   theString,
            MemberFunctionType      theMemberFunction)
{
    const typename VectorType::const_iterator   theEnd(theVector.end());
    typename VectorType::const_iterator         theCurrent(theVector.begin());

    while(theCurrent != theEnd)
    {
        if (((*theCurrent).*theMemberFunction)() == theString)
        {
            return &*theCurrent;
        }
        else
        {
            ++theCurrent;
        }
    }

    return 0;
}



template<class VectorType, class MemberFunctionType>
typename VectorType::value_type*
findNonConst(
            VectorType&             theVector,
            const XalanDOMString&   theString,
            MemberFunctionType      theMemberFunction)
{
    const typename VectorType::iterator     theEnd(theVector.end());
    typename VectorType::iterator           theCurrent(theVector.begin());

    while(theCurrent != theEnd)
    {
        if (((*theCurrent).*theMemberFunction)() == theString)
        {
            return &*theCurrent;
        }
        else
        {
            ++theCurrent;
        }
    }

    return 0;
}



template<class VectorType>
const typename VectorType::value_type*
findByPrefix(
            const VectorType&       theVector,
            const XalanDOMString&   thePrefix)
{
    typedef typename VectorType::value_type     value_type;

    return find(theVector, thePrefix, &value_type::getPrefix);
}



template<class VectorType>
typename VectorType::value_type*
findByPrefixNonConst(
            VectorType&             theVector,
            const XalanDOMString&   thePrefix)
{
    typedef typename VectorType::value_type     value_type;

    return findNonConst(theVector, thePrefix, &value_type::getPrefix);
}



template<class VectorType>
const typename VectorType::value_type*
findByURI(
            const VectorType&       theVector,
            const XalanDOMString&   theNamespaceURI)
{
    typedef typename VectorType::value_type     value_type;

    return find(theVector, theNamespaceURI, &value_type::getURI);
}



template<class VectorType>
bool
addByPrefix(
            StylesheetConstructionContext&  theConstructionContext,
            VectorType&                     theVector,
            const XalanDOMString&           thePrefix,
            const XalanDOMString&           theURI)
{
    typedef typename VectorType::value_type     value_type;

    const value_type* const     theEntry =
        findByPrefix(theVector, thePrefix);

    if (theEntry != 0)
    {
        return false;
    }
    else
    {
        theVector.push_back(
            value_type(
                theConstructionContext.getPooledString(thePrefix),
                theConstructionContext.getPooledString(theURI)));

        return true;
    }
}



template<class VectorType>
bool
addOrUpdateByPrefix(
            StylesheetConstructionContext&  theConstructionContext,
            VectorType&                     theVector,
            const XalanDOMString&           thePrefix,
            const XalanDOMString&           theURI)
{
    typedef typename VectorType::value_type     value_type;

    value_type* const   theEntry =
        findByPrefixNonConst(theVector, thePrefix);

    if (theEntry == 0)
    {
        theVector.push_back(
            value_type(
                theConstructionContext.getPooledString(thePrefix),
                theConstructionContext.getPooledString(theURI)));

        return true;
    }
    else
    {
        if (theEntry->getURI() == theURI)
        {
            return false;
        }
        else
        {
            theEntry->setURI(theConstructionContext.getPooledString(theURI));

            return true;
        }
    }
}



template<class VectorType>
const XalanDOMString*
findNamespace(
            const VectorType&       theVector,
            const XalanDOMString&   thePrefix)
{
    typedef typename VectorType::value_type     value_type;

    const value_type* const     theEntry =
        findByPrefix(theVector, thePrefix);

    if (theEntry == 0)
    {
        return 0;
    }
    else
    {
        return &theEntry->getURI();
    }
}



const XalanDOMString        NamespacesHandler::Namespace::s_emptyString(XalanMemMgrs::getDummyMemMgr());



NamespacesHandler::PrefixChecker::PrefixChecker()
{
}



NamespacesHandler::PrefixChecker::~PrefixChecker()
{
}



NamespacesHandler::NamespacesHandler(MemoryManager& theManager) :
    m_excludedResultPrefixes(theManager),
    m_namespaceDeclarations(theManager),
    m_extensionNamespaceURIs(theManager),
    m_namespaceAliases(theManager)
{
}



NamespacesHandler::NamespacesHandler(
            StylesheetConstructionContext&  theConstructionContext,
            const NamespacesHandler&        /* stylesheetNamespacesHandler */,
            const NamespacesStackType&      theCurrentNamespaces,
            const XalanDOMString&           theXSLTNamespaceURI) :
    m_excludedResultPrefixes(theConstructionContext.getMemoryManager()),
    m_namespaceDeclarations(theConstructionContext.getMemoryManager()),
    m_extensionNamespaceURIs(theConstructionContext.getMemoryManager()),
    m_namespaceAliases(theConstructionContext.getMemoryManager())
{
    // Go through the namespaces stack in reverse order...
    const NamespacesStackType::const_reverse_iterator   theEnd =
        theCurrentNamespaces.rend();

    NamespacesStackType::const_reverse_iterator     i =
        theCurrentNamespaces.rbegin();

    for(; i != theEnd; ++i)
    {
        // Process each namespace in the current vector...
        const NamespaceVectorType::const_iterator   theVectorEnd =
            (*i).end();

        NamespaceVectorType::const_iterator         j =
            (*i).begin();

        for(; j != theVectorEnd; ++j)
        {
            const NameSpace&        theNamespace = *j;

            const XalanDOMString&   theURI = theNamespace.getURI();

            const XalanDOMString&   thePrefix = theNamespace.getPrefix();

            if(shouldExcludeResultNamespaceNode(
                    theXSLTNamespaceURI,
                    theURI) == false)
            {
                addByPrefix(
                    theConstructionContext,
                    m_namespaceDeclarations,
                    thePrefix,
                    theURI);
            }
            else
            {
                addByPrefix(
                    theConstructionContext,
                    m_excludedResultPrefixes,
                    thePrefix,
                    theURI);

                assert(findByPrefix(m_excludedResultPrefixes, thePrefix) != 0 &&
                       findByPrefix(m_excludedResultPrefixes, thePrefix)->getURI() == theURI);
            }
        }
    }
}



NamespacesHandler::~NamespacesHandler()
{
}



void
NamespacesHandler::addExtensionNamespaceURI(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMString&           theURI)
{
    m_extensionNamespaceURIs.push_back(&theConstructionContext.getPooledString(theURI));
}



const XalanDOMString*
NamespacesHandler::getNamespace(const XalanDOMString&   thePrefix) const
{
    const NamespacesVectorType::value_type*     theNamespace =
        findByPrefix(m_excludedResultPrefixes, thePrefix);

    if (theNamespace != 0)
    {
        return &theNamespace->getURI();
    }
    else
    {
        return findNamespace(m_namespaceDeclarations, thePrefix);
    }
}



void
NamespacesHandler::copyNamespaceAliases(const NamespacesHandler&    parentNamespacesHandler)
{
    copyNamespaceAliases(parentNamespacesHandler.m_namespaceAliases);
}



const XalanDOMString*
NamespacesHandler::getNamespaceAlias(const XalanDOMString&      theStylesheetNamespace) const
{
    if (m_namespaceAliases.empty() == true)
    {
        return 0;
    }
    else
    {
        const NamespaceAliasesMapType::const_iterator   i =
                        m_namespaceAliases.find(&theStylesheetNamespace);

        if (i != m_namespaceAliases.end())
        {
            return (*i).second;
        }
        else
        {
            return 0;
        }
    }
}



void
NamespacesHandler::setNamespaceAlias(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMString&           theStylesheetNamespace,
            const XalanDOMString&           theResultNamespace)
{
    m_namespaceAliases[&theConstructionContext.getPooledString(theStylesheetNamespace)] =
        &theConstructionContext.getPooledString(theResultNamespace);
}



typedef StylesheetConstructionContext::GetCachedString  GetCachedString;

void
NamespacesHandler::processExcludeResultPrefixes(
        StylesheetConstructionContext&  theConstructionContext,
        const XalanDOMChar*             theValue,
        const NamespacesStackType&      theCurrentNamespaces)
{
    StringTokenizer     tokenizer(
                    theValue,
                    Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

    const GetCachedString   theGuard(theConstructionContext);

    XalanDOMString&     thePrefix = theGuard.get();

    while(tokenizer.hasMoreTokens() == true)
    {
         tokenizer.nextToken(thePrefix);

        if(equalsIgnoreCaseASCII(thePrefix, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
        {
            thePrefix.clear();
        }

        const XalanDOMString* const     theNamespaceURI =
            XalanQName::getNamespaceForPrefix(theCurrentNamespaces, thePrefix);

        if(theNamespaceURI == 0)
        {
            const GetCachedString   theGuard(theConstructionContext);

            theConstructionContext.problem(
                StylesheetConstructionContext::eXSLTProcessor,
                StylesheetConstructionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::PrefixIsNotDeclared_1Param,
                    thePrefix),
                theConstructionContext.getLocatorFromStack(),
                0);
        }

        addOrUpdateByPrefix(
            theConstructionContext,
            m_excludedResultPrefixes,
            thePrefix,
            *theNamespaceURI);

        assert(findByPrefix(m_excludedResultPrefixes, thePrefix) != 0 &&
               findByPrefix(m_excludedResultPrefixes, thePrefix)->getURI() == *theNamespaceURI);
    }
}



void
NamespacesHandler::processExtensionElementPrefixes(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMChar*             theValue,
            const NamespacesStackType&      theCurrentNamespaces)
{
    StringTokenizer     tokenizer(
                    theValue,
                    Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

    const GetCachedString   theGuard(theConstructionContext);

    XalanDOMString&     thePrefix = theGuard.get();

    while(tokenizer.hasMoreTokens() == true)
    {
        tokenizer.nextToken(thePrefix);

        if(equalsIgnoreCaseASCII(thePrefix, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
        {
            thePrefix.clear();
        }

        const XalanDOMString* const     theNamespace =
            XalanQName::getNamespaceForPrefix(theCurrentNamespaces, thePrefix);

        if(theNamespace == 0)
        {
            const GetCachedString   theGuard(theConstructionContext);

            theConstructionContext.problem(
                StylesheetConstructionContext::eXSLTProcessor,
                StylesheetConstructionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::PrefixIsNotDeclared_1Param,
                    thePrefix),
                theConstructionContext.getLocatorFromStack(),
                0);
        }

        assert(theNamespace != 0);

        m_extensionNamespaceURIs.push_back(&theConstructionContext.getPooledString(*theNamespace));
    }
}


void
NamespacesHandler::postConstruction(
            StylesheetConstructionContext&  theConstructionContext,
            bool                            fProcessNamespaceAliases,
            const XalanDOMString&           theElementName,
            const NamespacesHandler*        parentNamespacesHandler,
            const PrefixChecker*            prefixChecker)
{
    // Copy everything from the parent handler, if there is one...
    if (parentNamespacesHandler != 0)
    {
        copyNamespaceAliases(parentNamespacesHandler->m_namespaceAliases);

        copyExtensionNamespaceURIs(parentNamespacesHandler->m_extensionNamespaceURIs);

        copyExcludeResultPrefixes(parentNamespacesHandler->m_excludedResultPrefixes);
    }

    // Figure out the prefix of the owning element, to make sure we
    // don't exclude its prefix.
    const XalanDOMString::size_type     indexOfNSSep = indexOf(theElementName, XalanUnicode::charColon);

    const GetCachedString   theGuard(theConstructionContext);

    XalanDOMString&     thePrefix = theGuard.get();

    if (indexOfNSSep < theElementName.length())
    {
        substring(theElementName, thePrefix, 0, indexOfNSSep);
    }

    processExcludeResultPrefixes(theConstructionContext, thePrefix, prefixChecker);

    if (fProcessNamespaceAliases == true)
    {
        // $$ ToDo: Does this happen before or after exclude-result-prefixes?
        processNamespaceAliases();
    }

    createResultAttributeNames(theConstructionContext);
}



NamespacesHandler&
NamespacesHandler::operator=(const NamespacesHandler&   theRHS)
{
    if (&theRHS != this)
    {
        m_excludedResultPrefixes = theRHS.m_excludedResultPrefixes;

        m_namespaceDeclarations = theRHS.m_namespaceDeclarations;

        m_extensionNamespaceURIs = theRHS.m_extensionNamespaceURIs;

        m_namespaceAliases = theRHS.m_namespaceAliases;
    }

    return *this;
}



bool
NamespacesHandler::shouldExcludeResultNamespaceNode(
            const XalanDOMString&   theXSLTNamespaceURI,
            const XalanDOMString&   theURI) const
{
    // These are commone namespaces that are always excluded...
    if(equals(theURI, theXSLTNamespaceURI)
            || isExtensionNamespaceURI(theURI) == true
            || equals(theURI, DOMServices::s_XMLNamespaceURI))
    {
        return true;
    }
    else if (isExcludedNamespaceURI(theURI) == true)
    {
        // It was found in the excluded result prefixes URIs...
        return true;
    }
    else
    {
        return false;
    }
}



void
NamespacesHandler::outputResultNamespaces(
            StylesheetExecutionContext&     theExecutionContext,
            bool                            supressDefault) const
{
    // Write out the namespace declarations...
    if (m_namespaceDeclarations.empty() == false)
    {
        const NamespaceExtendedVectorType::const_iterator   theEnd =
                m_namespaceDeclarations.end();

        NamespaceExtendedVectorType::const_iterator i =
                m_namespaceDeclarations.begin();

        for(; i != theEnd; ++i)
        {
            const NamespaceExtended&    theNamespace = *i;

            const XalanDOMString&       thePrefix = theNamespace.getPrefix();

            // If we're not supposed to suppress the default namespace, or
            // there's a prefix (so it's not the default), we can continue
            // to see if we need to add the result namespace.
            if (supressDefault == false ||
                thePrefix.empty() == false)
            {
                const XalanDOMString&       theResultURI = theNamespace.getURI();
                assert(theNamespace.getResultAttributeName().empty() == false);

                // Get the any namespace declaration currently active for the
                // prefix.
                const XalanDOMString* const     desturi =
                    theExecutionContext.getResultNamespaceForPrefix(thePrefix);

                // Is there already an active namespace declaration?
                if(desturi == 0 || !equals(theResultURI, *desturi))
                {
                    // No, so add one...
                    theExecutionContext.addResultAttribute(theNamespace.getResultAttributeName(), theResultURI);
                }
            }
        }
    }
}



bool
NamespacesHandler::isExcludedNamespaceURI(const XalanDOMString&     theNamespaceURI) const
{
    return findByURI(m_excludedResultPrefixes, theNamespaceURI) != 0 ? true : false; 
}



bool
NamespacesHandler::findString(
            const XalanDOMString&                   theString,
            const XalanDOMStringPointerVectorType&  theVector)
{
    const XalanDOMStringPointerVectorType::const_iterator   theEnd = theVector.end();
    XalanDOMStringPointerVectorType::const_iterator         theCurrent = theVector.begin();

    while(theCurrent != theEnd)
    {
        assert(*theCurrent != 0);

        const XalanDOMString* const     theCurrentString = *theCurrent;

        if (theString == *theCurrentString)
        {
            return true;
        }

        ++theCurrent;
    }

    return false;
}



void
NamespacesHandler::clear()
{
    m_excludedResultPrefixes.clear();

    m_namespaceDeclarations.clear();

    m_extensionNamespaceURIs.clear();

    m_namespaceAliases.clear();
}



void
NamespacesHandler::swap(NamespacesHandler&  theOther)
{
    m_excludedResultPrefixes.swap(theOther.m_excludedResultPrefixes);

    m_namespaceDeclarations.swap(theOther.m_namespaceDeclarations);

    m_extensionNamespaceURIs.swap(theOther.m_extensionNamespaceURIs);

    m_namespaceAliases.swap(theOther.m_namespaceAliases);
}



void
NamespacesHandler::createResultAttributeNames(StylesheetConstructionContext&    theConstructionContext)
{
    // Go through all of the result namespaces and create the attribute
    // name that will be used when they're written to the result tree.
    // This is more efficient if the stylesheet is used multiple times.
    if (m_namespaceDeclarations.empty() == false)
    {
        const NamespaceExtendedVectorType::iterator theEnd =
                m_namespaceDeclarations.end();

        NamespaceExtendedVectorType::iterator       i =
                m_namespaceDeclarations.begin();

        const GetCachedString   theGuard(theConstructionContext);
        XalanDOMString&     theName = theGuard.get();

        for(; i != theEnd; ++i)
        {
            NamespaceExtended&      theNamespace = *i;

            const XalanDOMString&   thePrefix = theNamespace.getPrefix();

            if (thePrefix.empty() == false)
            {
                // Create a name of the form xmlns:prefix, where "prefix" is the
                // text of the prefix.
                // Reserve the appropriate amount of space in the string.
                theName.reserve(DOMServices::s_XMLNamespaceWithSeparatorLength + thePrefix.length() + 1);

                theName = DOMServices::s_XMLNamespaceWithSeparator;
                theName += thePrefix;

                theNamespace.setResultAttributeName(theConstructionContext.getPooledString(theName));
            }
            else
            {
                // It's the default namespace...
                theNamespace.setResultAttributeName(DOMServices::s_XMLNamespace);
            }
        }
    }
}



void
NamespacesHandler::processExcludeResultPrefixes(
            StylesheetConstructionContext&  theConstructionContext,
            const XalanDOMString&           theElementPrefix,
            const PrefixChecker*            prefixChecker)
{
    if (m_excludedResultPrefixes.empty() == false)
    {
        NamespaceExtendedVectorType::iterator       i =
                m_namespaceDeclarations.begin();

        // Check for any result prefixes we should exclude...
        while(i != m_namespaceDeclarations.end())
        {
            const Namespace&        theNamespace = *i;

            const XalanDOMString&   thePrefix = theNamespace.getPrefix();
            const XalanDOMString&   theURI = theNamespace.getURI();

            // We can never exclude the prefix of our owner element, so
            // check that first...
            if (equals(thePrefix, theElementPrefix) == false &&
                (prefixChecker == 0 || prefixChecker->isActive(thePrefix) == false) &&
                (isExcludedNamespaceURI(theURI) == true ||
                 isExtensionNamespaceURI(theURI) == true))
            {
                // Add it to the excluded prefixes, in case we need it later...
                m_excludedResultPrefixes.push_back(
                        NamespacesVectorType::value_type(
                            theConstructionContext.getPooledString(thePrefix),
                            theConstructionContext.getPooledString(theURI)));

                // It's excluded, so remove it...
                i = m_namespaceDeclarations.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }
}



void
NamespacesHandler::processNamespaceAliases()
{
    if (m_namespaceDeclarations.empty() == false)
    {
        const NamespaceExtendedVectorType::iterator     theEnd =
                m_namespaceDeclarations.end();

        NamespaceExtendedVectorType::iterator   i =
                m_namespaceDeclarations.begin();

        // Look at everyone of my namespaces for an alias, and substitute the
        // alias as appropriate...
        for(; i != theEnd; ++i)
        {
            Namespace&  theNamespace = *i;

            const XalanDOMString&           theURI =
                        theNamespace.getURI();

            const XalanDOMString* const     theAlias =
                        getNamespaceAlias(theURI);

            // Is there a local alias?
            if (theAlias != 0)
            {
                // Yup, so use it...
                theNamespace.setURI(*theAlias);
            }
        }
    }
}



void
NamespacesHandler::copyNamespaceAliases(const NamespaceAliasesMapType&  theNamespaceAliases)
{
    if (theNamespaceAliases.empty() == false)
    {
        if (m_namespaceAliases.empty() == true)
        {
            m_namespaceAliases = theNamespaceAliases;
        }
        else
        {
            const NamespaceAliasesMapType::const_iterator   theEnd =
                    theNamespaceAliases.end();

            NamespaceAliasesMapType::const_iterator     i =
                    theNamespaceAliases.begin();

            // Add them in...
            while(i != theEnd)
            {
                m_namespaceAliases.insert(*i);

                ++i;
            }
        }
    }
}



void
NamespacesHandler::copyExtensionNamespaceURIs(const XalanDOMStringPointerVectorType&    theExtensionNamespaceURIs)
{
    if (theExtensionNamespaceURIs.empty() == false)
    {
        if (m_extensionNamespaceURIs.empty() == true)
        {
            m_extensionNamespaceURIs = theExtensionNamespaceURIs;
        }
        else
        {
            const XalanDOMStringPointerVectorType::const_iterator   theEnd =
                    theExtensionNamespaceURIs.end();

            XalanDOMStringPointerVectorType::const_iterator i =
                    theExtensionNamespaceURIs.begin();

            // Add them in...
            while(i != theEnd)
            {
                assert(*i != 0);

                if (isExtensionNamespaceURI(**i) == false)
                {
                    m_extensionNamespaceURIs.push_back(*i);
                }

                ++i;
            }
        }
    }
}



void
NamespacesHandler::copyExcludeResultPrefixes(const NamespacesVectorType&    theExcludeResultPrefixes)
{
    if (theExcludeResultPrefixes.empty() == false)
    {
        if (m_excludedResultPrefixes.empty() == true)
        {
            m_excludedResultPrefixes = theExcludeResultPrefixes;
        }
        else
        {
            const NamespacesVectorType::const_iterator  theEnd =
                    theExcludeResultPrefixes.end();

            NamespacesVectorType::const_iterator    i =
                    theExcludeResultPrefixes.begin();

            // Add them in...
            while(i != theEnd)
            {
                if (findByPrefix(m_excludedResultPrefixes, (*i).getPrefix()) == 0)
                {
                    m_excludedResultPrefixes.push_back(*i);
                }

                ++i;
            }
        }
    }
}



XALAN_CPP_NAMESPACE_END
