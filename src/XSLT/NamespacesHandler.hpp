/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#if !defined(XALAN_NAMESPACESHANDLER_HEADER_GUARD)
#define XALAN_NAMESPACESHANDLER_HEADER_GUARD



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <map>
#include <set>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <XPath/NameSpace.hpp>
#include <XPath/QName.hpp>



class StylesheetConstructionContext;
class StylesheetExecutionContext;



class XALAN_XSLT_EXPORT NamespacesHandler
{
public:

	/**
	 * Perform static initialization.  See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XSLTInit.
	 */
	static void
	terminate();


	/* Some static strings for various namespace URIs...
	 *
	 */
	static const XalanDOMString&	s_ApacheXSLTNamespaceURI;

	static const XalanDOMString&	s_LotusXSLTNamespaceURI;

	static const XalanDOMString&	s_LotusXSLTNamespaceURIWithSeparator;


	class NameSpaceExtended : public NameSpace
	{
	public:

		NameSpaceExtended(
					const XalanDOMString&	prefix = XalanDOMString(),
					const XalanDOMString&	uri = XalanDOMString()) :
			NameSpace(prefix, uri),
			m_resultAttributeName()
		{
		}

		NameSpaceExtended(const NameSpace&	theNamespace) :
			NameSpace(theNamespace),
			m_resultAttributeName()
		{
		}

		/**
		 * Retrieve the name of the result attribute.
		 * 
		 * @return name string
		 */
		const XalanDOMString&
		getResultAttributeName() const
		{
			return m_resultAttributeName;
		}

		/**
		 * Set the name of the result attribute.
		 * 
		 * @param name The new name value
		 */
		void
		setResultAttributeName(const XalanDOMString&	name)
		{
			m_resultAttributeName = name;
		}

	private:

		XalanDOMString	m_resultAttributeName;
	};

	typedef QName::NamespaceVectorType					NamespaceVectorType;
	typedef QName::NamespacesStackType					NamespacesStackType;

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >					ExcludedResultPrefixesMapType;

	typedef map<XalanDOMString,
				NameSpaceExtended,
				less<XalanDOMString> >					NamespacesMapType;

	typedef map<XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >					NamespaceAliasesMapType;

	typedef set<XalanDOMString,
				less<XalanDOMString> >					ExtensionNamespaceURISetType;

	typedef set<XalanDOMString,
				less<XalanDOMString> >					ActivePrefixesSetType;
#else
	typedef std::map<XalanDOMString, XalanDOMString>	ExcludedResultPrefixesMapType;

	typedef std::map<XalanDOMString,
					 NameSpaceExtended>					NamespacesMapType;

	typedef std::map<XalanDOMString, XalanDOMString>	NamespaceAliasesMapType;

	typedef std::set<XalanDOMString>					ExtensionNamespaceURISetType;

	typedef std::set<XalanDOMString>					ActivePrefixesSetType;
#endif

	/**
	 * Create a default, empty instance.
	 */
	explicit
	NamespacesHandler();

	/**
	 * Create an instance namespace handler using the
	 * current namespaces in effect.
	 *
	 * @param stylesheetNamespacesHandler The stylesheet's handler.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 * @param theXSLTNamespaceURI The namespace URI for XSLT.
	 */
	NamespacesHandler(
			const NamespacesHandler&	stylesheetNamespacesHandler,
			const NamespacesStackType&	theCurrentNamespaces,
			const XalanDOMString&		theXSLTNamespaceURI);

	NamespacesHandler(const NamespacesHandler&	theSource);

	~NamespacesHandler();

	/**
	 * Process an exclude-result-prefixes attribute.
	 *
	 * @param theValue The attribute's value.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 * @param theConstructionContext The current construction context.
	 */
	void
	processExcludeResultPrefixes(
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces,
			StylesheetConstructionContext&	theConstructionContext);

	/**
	 * Process an extension-element-prefixes attribute.
	 *
	 * @param theValue The attribute's value.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 * @param theConstructionContext The current construction context.
	 */
	void
	processExtensionElementPrefixes(
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces,
			StylesheetConstructionContext&	theConstructionContext);

	/**
	 * Notify the instance that the stylesheet is fully constructed.
	 *
	 * @param theElementName The name of the owning element.
	 * @param parentNamespacesHandler The parent handler, if any.
	 */
	void
	postConstruction(
			const XalanDOMString&		theElementName = XalanDOMString(),
			const NamespacesHandler*	parentNamespacesHandler = 0);

	NamespacesHandler&
	operator=(const NamespacesHandler&	theRHS);

	/**
	 * Determine of a given namespace should be excluded.
	 *
	 * @param theXSLTNamespaceURI The namespace URI for XSLT.
	 * @param thePrefix The namespace prefix.
	 * @param theURI The namespace URI.
	 * @return true of the namespace should be excluded, false if not.
	 */
	bool
	shouldExcludeResultNamespaceNode(
			const XalanDOMString&	theXSLTNamespaceURI,
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theURI) const;

	/**
	 * Determine if a given prefix is in the list of excluded
	 * prefixes.
	 *
	 * @param thePrefix The namespace prefix.
	 * @return true of the prefix should be excluded, false if not.
	 */
	bool
	hasExcludedPrefix(const XalanDOMString&		thePrefix) const
	{
		return m_excludedResultPrefixes.count(thePrefix) != 0 ? true : false;
	}

	/**
	 * Add a URI as an extension namespace prefixes.
	 *
	 * @param theURI The namespace URI.
	 */
	void
	addExtensionNamespaceURI(const XalanDOMString&	theURI)
	{
		m_extensionNamespaceURIs.insert(theURI);
	}

	/**
	 * Get the namespace URI for the given prefix.
	 *
	 * @param thePrefix The namespace prefix.
	 * @return The namespace URI
	 */
	const XalanDOMString*
	getNamespace(const XalanDOMString&	thePrefix) const;

	/**
	 * Get the namespace alias URI for the given namespace.
	 *
	 * @param theStylesheetNamespace The namespace as declared in the stylesheet.
	 * @return The namespace alias URI
	 */
	const XalanDOMString*
	getNamespaceAlias(const XalanDOMString&		theStylesheetNamespace) const;

	/**
	 * Set the namespace alias URI for the given namespace.
	 *
	 * @param theStylesheetNamespace The namespace as declared in the stylesheet.
	 * @param theResultNamespace The namespace as it should appear in the result tree.
	 */
	void
	setNamespaceAlias(
			const XalanDOMString&	theStylesheetNamespace,
			const XalanDOMString&	theResultNamespace)
	{
		m_namespaceAliases[theStylesheetNamespace] = theResultNamespace;
	}

	/**
	 * Add a prefix to the list of active prefixes.
	 *
	 * @param thePrefix The prefix that is active and requires a namespace declaration.
	 */
	void
	addActivePrefix(const XalanDOMString&	thePrefix)
	{
		m_activePrefixes.insert(thePrefix);
	}

	/**
	 * Copy the aliases from the given NamespacesHandler.
	 *
	 * @param parentNamespacesHandler The parent handler.
	 */
	void
	copyNamespaceAliases(const NamespacesHandler&	parentNamespacesHandler);

	/**
	 * Output the result tree namespace declarations.
	 *
	 * @param theExecutionContext The current execution context.
	 */
	void
	outputResultNamespaces(
			StylesheetExecutionContext&		theExecutionContext,
			const XalanDOMString*			theNamespaceToExclude = 0) const;

	/**
	 * Clear out the handler.
	 */
	void
	clear();

	/**
	 * Swap the contents of this instance with another.
	 *
	 * @param theOther The other instance.
	 */
	void
	swap(NamespacesHandler&		theOther);

	bool
	getProcessNamespaceAliaises() const
	{
		return m_processAliases;
	}

	void
	setProcessNamespaceAliaises(bool	fValue)
	{
		m_processAliases = fValue;
	}

private:

	/**
	 * Create all of the result attribute names.
	 */
	void
	createResultAttributeNames();

	/**
	 * Process the exclude result prefix data.
	 *
	 * @param theElementPrefix The prefix of the owning element.
	 */
	void
	processExcludeResultPrefixes(const XalanDOMString&	theElementPrefix);

	/**
	 * Process the namespace aliases data.
	 */
	void
	processNamespaceAliases();

	/**
	 * Copy the contents of the supplied map
	 *
	 * @param theNamespaceAliases The map to copy.
	 */
	void
	copyNamespaceAliases(const NamespaceAliasesMapType&		theNamespaceAliases);

	/**
	 * Copy the contents of the supplied set
	 *
	 * @param theExtensionNamespaceURIs The set to copy.
	 */
	void
	copyExtensionNamespaceURIs(const ExtensionNamespaceURISetType&	theExtensionNamespaceURIs);

	/**
	 * Copy the contents of the supplied map
	 *
	 * @param theExcludeResultPrefixes The map to copy.
	 */
	void
	copyExcludeResultPrefixes(const ExcludedResultPrefixesMapType&	theExcludeResultPrefixes);

	/**
	 * Determine of a given namespace should be excluded.  For use during
	 * stylesheet construction only.
	 *
	 * @param theXSLTNamespaceURI The namespace URI for XSLT.
	 * @param parentNamespacesHandler The parent handler instance.
	 * @param theNamespace The namespace.
	 * @return true of the namespace should be excluded, false if not.
	 */
	bool
	shouldExcludeResultNamespaceNode(
			const XalanDOMString&		theXSLTNamespaceURI,
			const NamespacesHandler&	parentNamespacesHandler,
			const NameSpace&			theNamespace) const;

	// Not implemented...
	bool
	operator==(const NamespacesHandler&) const;

	// Data members...
	ExcludedResultPrefixesMapType	m_excludedResultPrefixes;

	NamespacesMapType				m_namespaceDeclarations;

	ExtensionNamespaceURISetType	m_extensionNamespaceURIs;

	NamespaceAliasesMapType			m_namespaceAliases;

	ActivePrefixesSetType			m_activePrefixes;

	// If true namespace aliases will be processed.  If false, they will not.
	bool							m_processAliases;
};



#endif	// XALAN_NAMESPACESHANDLER_HEADER_GUARD
