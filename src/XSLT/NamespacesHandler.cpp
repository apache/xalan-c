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



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



NamespacesHandler::NamespacesHandler() :
	m_excludedResultPrefixes(),
	m_namespaceDeclarations(),
	m_extensionNamespaceURIs(),
	m_namespaceAliases(),
	m_activePrefixes(),
	m_processAliases(true)
{
}



NamespacesHandler::NamespacesHandler(
			const NamespacesHandler&	/* stylesheetNamespacesHandler */,
			const NamespacesStackType&	theCurrentNamespaces,
			const XalanDOMString&		theXSLTNamespaceURI) :
	m_excludedResultPrefixes(),
	m_namespaceDeclarations(),
	m_extensionNamespaceURIs(),
	m_namespaceAliases(),
	m_activePrefixes(),
	m_processAliases(true)
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
				if (m_namespaceDeclarations.count(thePrefix) == 0)
				{
					m_namespaceDeclarations.insert(NamespacesMapType::value_type(thePrefix, theNamespace));
				}
			}
			else
			{
				m_excludedResultPrefixes.insert(
						ExcludedResultPrefixesMapType::value_type(thePrefix, theURI));
			}
		}
	}
}



NamespacesHandler::NamespacesHandler(const NamespacesHandler&	theSource) :
	m_excludedResultPrefixes(theSource.m_excludedResultPrefixes),
	m_namespaceDeclarations(theSource.m_namespaceDeclarations),
	m_extensionNamespaceURIs(theSource.m_extensionNamespaceURIs),
	m_namespaceAliases(theSource.m_namespaceAliases),
	m_activePrefixes(theSource.m_activePrefixes),
	m_processAliases(theSource.m_processAliases)
{
}



NamespacesHandler::~NamespacesHandler()
{
}



const XalanDOMString*
NamespacesHandler::getNamespace(const XalanDOMString&	thePrefix) const
{
	// Check the excluded result prefixes first...
	const ExcludedResultPrefixesMapType::const_iterator		i =
			m_excludedResultPrefixes.find(thePrefix);

	if (i != m_excludedResultPrefixes.end())
	{
		return &(*i).second;
	}
	else
	{
		// Not found, so check the namespace declarations...
		const NamespacesMapType::const_iterator		i =
				m_namespaceDeclarations.find(thePrefix);

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
						m_namespaceAliases.find(theStylesheetNamespace);

		if (i != m_namespaceAliases.end())
		{
			return &(*i).second;
		}
		else
		{
			return 0;
		}
	}
}



void
NamespacesHandler::processExcludeResultPrefixes(
		const XalanDOMChar*				theValue,
		const NamespacesStackType&		theCurrentNamespaces,
		StylesheetConstructionContext&	theConstructionContext)
{
	StringTokenizer		tokenizer(
					theValue,
					Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

	XalanDOMString	thePrefix;

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
			XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid prefix in exclude-result-prefixes: "));

			theMessage += thePrefix;

			theConstructionContext.error(theMessage);
		}

		m_excludedResultPrefixes[thePrefix] = *theNamespace;
    }
}



void
NamespacesHandler::processExtensionElementPrefixes(
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces,
			StylesheetConstructionContext&	theConstructionContext)
{
	StringTokenizer		tokenizer(
					theValue,
					Constants::DEFAULT_WHITESPACE_SEPARATOR_STRING);

    while(tokenizer.hasMoreTokens() == true)
    {
		XalanDOMString	thePrefix = tokenizer.nextToken();

		if(equalsIgnoreCaseASCII(thePrefix, Constants::ATTRVAL_DEFAULT_PREFIX) == true)
		{
			::clear(thePrefix);
		}

		const XalanDOMString* const		theNamespace =
			XalanQName::getNamespaceForPrefix(theCurrentNamespaces, thePrefix);

		if(theNamespace == 0)
		{
			XalanDOMString	theMessage(TranscodeFromLocalCodePage("Invalid prefix in extension-element-prefixes: "));

			theMessage += thePrefix;

			theConstructionContext.error(theMessage);
		}

		m_extensionNamespaceURIs.insert(*theNamespace);
    }
}



void
NamespacesHandler::postConstruction(
			const XalanDOMString&		theElementName,
			const NamespacesHandler*	parentNamespacesHandler)
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

	XalanDOMString	thePrefix;
	
	if (indexOfNSSep < length(theElementName))
	{
		substring(theElementName, thePrefix, 0, indexOfNSSep);
	}

	processExcludeResultPrefixes(thePrefix);

	// $$ ToDo: Does this happen before or after exclude-result-prefixes?
	processNamespaceAliases();

	createResultAttributeNames();

	// We don't need these any more...
	m_activePrefixes.clear();
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
			|| m_extensionNamespaceURIs.find(theURI) != m_extensionNamespaceURIs.end()
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
			const NameSpaceExtended&	theNamespace = (*i).second;

			const XalanDOMString&		thePrefix = theNamespace.getPrefix();

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
		if (equals((*i).second, theNamespaceURI) == true)
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



void
NamespacesHandler::clear()
{
	m_excludedResultPrefixes.clear();

	m_namespaceDeclarations.clear();

	m_extensionNamespaceURIs.clear();

	m_namespaceAliases.clear();

	m_activePrefixes.clear();
}



void
NamespacesHandler::swap(NamespacesHandler&	theOther)
{
	m_excludedResultPrefixes.swap(theOther.m_excludedResultPrefixes);

	m_namespaceDeclarations.swap(theOther.m_namespaceDeclarations);

	m_extensionNamespaceURIs.swap(theOther.m_extensionNamespaceURIs);

	m_namespaceAliases.swap(theOther.m_namespaceAliases);

	m_activePrefixes.swap(theOther.m_activePrefixes);
}



void
NamespacesHandler::createResultAttributeNames()
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

		for(; i != theEnd; ++i)
		{
			NameSpaceExtended&		theNamespace = (*i).second;

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();

			if (isEmpty(thePrefix) == false)
			{
				// Create a name of the form xmlns:prefix, where "prefix" is the
				// text of the prefix.
				XalanDOMString	theName;

				// Reserve the appropriate amount of space in the string.
				reserve(theName, DOMServices::s_XMLNamespaceWithSeparatorLength + length(thePrefix) + 1);

				theName += DOMServices::s_XMLNamespaceWithSeparator;
				theName += thePrefix;

				theNamespace.setResultAttributeName(theName);
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
NamespacesHandler::processExcludeResultPrefixes(const XalanDOMString&	theElementPrefix)
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
			const NameSpace&		theNamespace = (*i).second;

			const XalanDOMString&	thePrefix = theNamespace.getPrefix();
			const XalanDOMString&	theURI = theNamespace.getURI();

			// We can never exclude the prefix of our owner element, so
			// check that first...
			if (equals(thePrefix, theElementPrefix) == false &&
				m_activePrefixes.find(thePrefix) == m_activePrefixes.end() &&
				(isExcludedNamespaceURI(theURI) == true ||
				 m_extensionNamespaceURIs.find(theURI) != m_extensionNamespaceURIs.end()))
			{
				// It's excluded, so remove it...
				theDeadEntries.push_back(i);

				// Add it to the excluded prefixes, in case we need it later...
				m_excludedResultPrefixes.insert(
						ExcludedResultPrefixesMapType::value_type(thePrefix, theURI));
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
	if (m_processAliases == true && m_namespaceDeclarations.empty() == false)
	{
		const NamespacesMapType::iterator	theEnd =
				m_namespaceDeclarations.end();

		NamespacesMapType::iterator		i =
				m_namespaceDeclarations.begin();

		// Look at everyone of my namespaces for an alias, and substitute the
		// alias as appropriate...
		for(; i != theEnd; ++i)
		{
			NameSpace&	theNamespace = (*i).second;

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
NamespacesHandler::copyExtensionNamespaceURIs(const ExtensionNamespaceURISetType&	theExtensionNamespaceURIs)
{
	if (theExtensionNamespaceURIs.empty() == false)
	{
		if (m_extensionNamespaceURIs.empty() == true)
		{
			m_extensionNamespaceURIs = theExtensionNamespaceURIs;
		}
		else
		{
			const ExtensionNamespaceURISetType::const_iterator	theEnd =
					theExtensionNamespaceURIs.end();

			ExtensionNamespaceURISetType::const_iterator	i =
					theExtensionNamespaceURIs.begin();

			// Add them in...
			while(i != theEnd)
			{
				m_extensionNamespaceURIs.insert(*i);

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
