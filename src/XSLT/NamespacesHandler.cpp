/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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


// Class header file...
#include "NamespacesHandler.hpp"



#include <PlatformSupport/StringTokenizer.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



const XalanDOMString		NamespacesHandler::Namespace::s_emptyString;



NamespacesHandler::PrefixChecker::PrefixChecker()
{
}



NamespacesHandler::PrefixChecker::~PrefixChecker()
{
}



NamespacesHandler::NamespacesHandler() :
	m_excludedResultPrefixes(),
	m_namespaceDeclarations(),
	m_extensionNamespaceURIs(),
	m_namespaceAliases()
{
}



NamespacesHandler::NamespacesHandler(
			StylesheetConstructionContext&	theConstructionContext,
			const NamespacesHandler&		/* stylesheetNamespacesHandler */,
			const NamespacesStackType&		theCurrentNamespaces,
			const XalanDOMString&			theXSLTNamespaceURI) :
	m_excludedResultPrefixes(),
	m_namespaceDeclarations(),
	m_extensionNamespaceURIs(),
	m_namespaceAliases()
{
	// Go through the namespaces stack in reverse order...
	const NamespacesStackType::const_reverse_iterator	theEnd =
		theCurrentNamespaces.rend();

	NamespacesStackType::const_reverse_iterator		i =
		theCurrentNamespaces.rbegin();

	for(; i != theEnd; ++i)
	{
		// Process each namespace in the current vector...
		const NamespaceVectorType::const_iterator	theVectorEnd =
			(*i).end();

		NamespaceVectorType::const_iterator			j =
			(*i).begin();

		for(; j != theVectorEnd; ++j)
		{
			const NameSpace&		theNamespace = *j;

			const XalanDOMString&	theURI = theNamespace.getURI();

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();

			if(shouldExcludeResultNamespaceNode(
					theXSLTNamespaceURI,
					theURI) == false)
			{
				if (m_namespaceDeclarations.count(&thePrefix) == 0)
				{
					m_namespaceDeclarations.insert(
						NamespacesMapType::value_type(
							&theConstructionContext.getPooledString(thePrefix),
							Namespace(
								theConstructionContext.getPooledString(theNamespace.getPrefix()),
								theConstructionContext.getPooledString(theNamespace.getURI()))));
				}
			}
			else
			{
				m_excludedResultPrefixes.insert(
					ExcludedResultPrefixesMapType::value_type(
						&theConstructionContext.getPooledString(thePrefix),
						&theConstructionContext.getPooledString(theURI)));
			}
		}
	}
}



NamespacesHandler::~NamespacesHandler()
{
}



void
NamespacesHandler::addExtensionNamespaceURI(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			theURI)
{
	m_extensionNamespaceURIs.push_back(&theConstructionContext.getPooledString(theURI));
}



const XalanDOMString*
NamespacesHandler::getNamespace(const XalanDOMString&	thePrefix) const
{
	// Check the excluded result prefixes first...
	const ExcludedResultPrefixesMapType::const_iterator		i =
			m_excludedResultPrefixes.find(&thePrefix);

	if (i != m_excludedResultPrefixes.end())
	{
		return (*i).second;
	}
	else
	{
		// Not found, so check the namespace declarations...
		const NamespacesMapType::const_iterator		i =
				m_namespaceDeclarations.find(&thePrefix);

		if (i != m_namespaceDeclarations.end())
		{
			return &(*i).second.getURI();
		}
		else
		{
			return 0;
		}
	}
}



void
NamespacesHandler::copyNamespaceAliases(const NamespacesHandler&	parentNamespacesHandler)
{
	copyNamespaceAliases(parentNamespacesHandler.m_namespaceAliases);
}



const XalanDOMString*
NamespacesHandler::getNamespaceAlias(const XalanDOMString&		theStylesheetNamespace) const
{
	if (m_namespaceAliases.empty() == true)
	{
		return 0;
	}
	else
	{
		const NamespaceAliasesMapType::const_iterator	i =
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
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			theStylesheetNamespace,
			const XalanDOMString&			theResultNamespace)
{
	m_namespaceAliases[&theConstructionContext.getPooledString(theStylesheetNamespace)] =
		&theConstructionContext.getPooledString(theResultNamespace);
}



void
NamespacesHandler::processExcludeResultPrefixes(
		StylesheetConstructionContext&	theConstructionContext,
		const XalanDOMChar*				theValue,
		const NamespacesStackType&		theCurrentNamespaces)
{
	typedef StylesheetConstructionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	StringTokenizer		tokenizer(
					theValue,
					Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

	const GetAndReleaseCachedString		theGuard(theConstructionContext);

	XalanDOMString&		thePrefix = theGuard.get();

    while(tokenizer.hasMoreTokens() == true)
    {
		 tokenizer.nextToken(thePrefix);

		if(equalsIgnoreCaseASCII(thePrefix, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
		{
			::clear(thePrefix);
		}

		const XalanDOMString* const		theNamespace =
			XalanQName::getNamespaceForPrefix(theCurrentNamespaces, thePrefix);

		if(theNamespace == 0)
		{
			theConstructionContext.error("Undeclared prefix in exclude-result-prefixes");
		}

		m_excludedResultPrefixes[&theConstructionContext.getPooledString(thePrefix)] =
			&theConstructionContext.getPooledString(*theNamespace);
    }
}



void
NamespacesHandler::processExtensionElementPrefixes(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces)
{
	typedef StylesheetConstructionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	StringTokenizer		tokenizer(
					theValue,
					Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

	const GetAndReleaseCachedString		theGuard(theConstructionContext);

	XalanDOMString&		thePrefix = theGuard.get();

	while(tokenizer.hasMoreTokens() == true)
    {
		tokenizer.nextToken(thePrefix);

		if(equalsIgnoreCaseASCII(thePrefix, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
		{
			::clear(thePrefix);
		}

		const XalanDOMString* const		theNamespace =
			XalanQName::getNamespaceForPrefix(theCurrentNamespaces, thePrefix);

		if(theNamespace == 0)
		{
			theConstructionContext.error("Undeclared prefix in extension-element-prefixes");
		}

		assert(theNamespace != 0);

		m_extensionNamespaceURIs.push_back(&theConstructionContext.getPooledString(*theNamespace));
    }
}



void
NamespacesHandler::postConstruction(
			StylesheetConstructionContext&	theConstructionContext,
			bool							fProcessNamespaceAliases,
			const XalanDOMString&			theElementName,
			const NamespacesHandler*		parentNamespacesHandler,
			const PrefixChecker*			prefixChecker)
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
	const XalanDOMString::size_type		indexOfNSSep = indexOf(theElementName, XalanUnicode::charColon);

	typedef StylesheetConstructionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	const GetAndReleaseCachedString		theGuard(theConstructionContext);

	XalanDOMString&		thePrefix = theGuard.get();

	if (indexOfNSSep < length(theElementName))
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
NamespacesHandler::operator=(const NamespacesHandler&	theRHS)
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
			const XalanDOMString&	theXSLTNamespaceURI,
			const XalanDOMString&	theURI) const
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
			StylesheetExecutionContext&		theExecutionContext,
			bool							supressDefault) const
{
	// Write out the namespace declarations...
	if (m_namespaceDeclarations.empty() == false)
	{
		const NamespacesMapType::const_iterator	theEnd =
				m_namespaceDeclarations.end();

		NamespacesMapType::const_iterator	i =
				m_namespaceDeclarations.begin();

		for(; i != theEnd; ++i)
		{
			const Namespace&		theNamespace = (*i).second;

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();

			// If we're not supposed to suppress the default namespace, or
			// there's a prefix (so it's not the default), we can continue
			// to see if we need to add the result namespace.
			if (supressDefault == false ||
				length(thePrefix) != 0)
			{
				const XalanDOMString&		theResultURI = theNamespace.getURI();
				assert(length(theNamespace.getResultAttributeName()) > 0);

				// Get the any namespace declaration currently active for the
				// prefix.
				const XalanDOMString* const		desturi =
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
NamespacesHandler::shouldExcludeResultNamespaceNode(
			const XalanDOMString&		theXSLTNamespaceURI,
			const NamespacesHandler&	stylesheetNamespacesHandler,
			const NameSpace&			theNamespace) const
{
	const XalanDOMString&	theURI = theNamespace.getURI();

	if(shouldExcludeResultNamespaceNode(theXSLTNamespaceURI, theURI) == true)
	{
		return true;
	}
	else
	{
		return stylesheetNamespacesHandler.isExcludedNamespaceURI(theURI);
	}
}



bool
NamespacesHandler::isExcludedNamespaceURI(const XalanDOMString&		theNamespaceURI) const
{
	ExcludedResultPrefixesMapType::const_iterator		i =
			m_excludedResultPrefixes.begin();

	const ExcludedResultPrefixesMapType::const_iterator		theEnd =
			m_excludedResultPrefixes.end();

	while(i != theEnd)
	{
		assert((*i).second != 0);

		if (equals(*(*i).second, theNamespaceURI) == true)
		{
			return true;
		}
		else
		{
			++i;
		}
	}

	return false;
}



bool
NamespacesHandler::findString(
			const XalanDOMString&					theString,
			const XalanDOMStringPointerVectorType&	theVector)
{
	const XalanDOMStringPointerVectorType::const_iterator	theEnd = theVector.end();
	XalanDOMStringPointerVectorType::const_iterator			theCurrent = theVector.begin();

	while(theCurrent != theEnd)
	{
		assert(*theCurrent != 0);

		const XalanDOMString* const		theCurrentString = *theCurrent;

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
NamespacesHandler::swap(NamespacesHandler&	theOther)
{
	m_excludedResultPrefixes.swap(theOther.m_excludedResultPrefixes);

	m_namespaceDeclarations.swap(theOther.m_namespaceDeclarations);

	m_extensionNamespaceURIs.swap(theOther.m_extensionNamespaceURIs);

	m_namespaceAliases.swap(theOther.m_namespaceAliases);
}



void
NamespacesHandler::createResultAttributeNames(StylesheetConstructionContext&	theConstructionContext)
{
	// Go through all of the result namespaces and create the attribute
	// name that will be used when they're written to the result tree.
	// This is more efficient if the stylesheet is used multiple times.
	if (m_namespaceDeclarations.empty() == false)
	{
		const NamespacesMapType::iterator	theEnd =
				m_namespaceDeclarations.end();

		NamespacesMapType::iterator		i =
				m_namespaceDeclarations.begin();

		XalanDOMString	theName;

		for(; i != theEnd; ++i)
		{
			Namespace&				theNamespace = (*i).second;

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();

			if (isEmpty(thePrefix) == false)
			{
				// Create a name of the form xmlns:prefix, where "prefix" is the
				// text of the prefix.
				// Reserve the appropriate amount of space in the string.
				reserve(theName, DOMServices::s_XMLNamespaceWithSeparatorLength + length(thePrefix) + 1);

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
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			theElementPrefix,
			const PrefixChecker*			prefixChecker)
{
	if (m_excludedResultPrefixes.empty() == false)
	{
#if defined(XALAN_NO_NAMESPACES)
	typedef vector<NamespacesMapType::iterator> 		IteratorVectorType;
#else
	typedef std::vector<NamespacesMapType::iterator> 	IteratorVectorType;
#endif

		// This vector will hold all of the iterators that we need to erase...
		IteratorVectorType	theDeadEntries;

		theDeadEntries.reserve(m_excludedResultPrefixes.size());

		const NamespacesMapType::iterator	theEnd =
				m_namespaceDeclarations.end();

		NamespacesMapType::iterator		i =
				m_namespaceDeclarations.begin();

		// Check for any result prefixes we should exclude...
		while(i != theEnd)
		{
			const Namespace&		theNamespace = (*i).second;

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();
			const XalanDOMString&	theURI = theNamespace.getURI();

			// We can never exclude the prefix of our owner element, so
			// check that first...
			if (equals(thePrefix, theElementPrefix) == false &&
				(prefixChecker == 0 || prefixChecker->isActive(thePrefix) == false) &&
				(isExcludedNamespaceURI(theURI) == true ||
				 isExtensionNamespaceURI(theURI) == true))
			{
				// It's excluded, so remove it...
				theDeadEntries.push_back(i);

				// Add it to the excluded prefixes, in case we need it later...
				m_excludedResultPrefixes.insert(
						ExcludedResultPrefixesMapType::value_type(
							&theConstructionContext.getPooledString(thePrefix),
							&theConstructionContext.getPooledString(theURI)));
			}

			++i;
		}

		while(theDeadEntries.empty() == false)
		{
			m_namespaceDeclarations.erase(theDeadEntries.back());

			theDeadEntries.pop_back();
		}
	}
}



void
NamespacesHandler::processNamespaceAliases()
{
	if (m_namespaceDeclarations.empty() == false)
	{
		const NamespacesMapType::iterator	theEnd =
				m_namespaceDeclarations.end();

		NamespacesMapType::iterator		i =
				m_namespaceDeclarations.begin();

		// Look at everyone of my namespaces for an alias, and substitute the
		// alias as appropriate...
		for(; i != theEnd; ++i)
		{
			Namespace&	theNamespace = (*i).second;

			const XalanDOMString&			theURI =
						theNamespace.getURI();

			const XalanDOMString* const		theAlias =
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
NamespacesHandler::copyNamespaceAliases(const NamespaceAliasesMapType&	theNamespaceAliases)
{
	if (theNamespaceAliases.empty() == false)
	{
		if (m_namespaceAliases.empty() == true)
		{
			m_namespaceAliases = theNamespaceAliases;
		}
		else
		{
			const NamespaceAliasesMapType::const_iterator	theEnd =
					theNamespaceAliases.end();

			NamespaceAliasesMapType::const_iterator		i =
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
NamespacesHandler::copyExtensionNamespaceURIs(const XalanDOMStringPointerVectorType&	theExtensionNamespaceURIs)
{
	if (theExtensionNamespaceURIs.empty() == false)
	{
		if (m_extensionNamespaceURIs.empty() == true)
		{
			m_extensionNamespaceURIs = theExtensionNamespaceURIs;
		}
		else
		{
			const XalanDOMStringPointerVectorType::const_iterator	theEnd =
					theExtensionNamespaceURIs.end();

			XalanDOMStringPointerVectorType::const_iterator	i =
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
NamespacesHandler::copyExcludeResultPrefixes(const ExcludedResultPrefixesMapType&	theExcludeResultPrefixes)
{
	if (theExcludeResultPrefixes.empty() == false)
	{
		if (m_excludedResultPrefixes.empty() == true)
		{
			m_excludedResultPrefixes = theExcludeResultPrefixes;
		}
		else
		{
			const ExcludedResultPrefixesMapType::const_iterator	theEnd =
					theExcludeResultPrefixes.end();

			ExcludedResultPrefixesMapType::const_iterator	i =
					theExcludeResultPrefixes.begin();

			// Add them in...
			while(i != theEnd)
			{
				m_excludedResultPrefixes.insert(*i);

				++i;
			}
		}
	}
}
