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



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/NameSpace.hpp>
#include <XPath/XalanQName.hpp>



class StylesheetConstructionContext;
class StylesheetExecutionContext;



class XALAN_XSLT_EXPORT NamespacesHandler
{
public:

	class PrefixChecker
	{
	public:

		PrefixChecker();

		virtual
		~PrefixChecker();

		virtual bool
		isActive(const XalanDOMString&	thePrefix) const = 0;
	};

	class Namespace
	{
	public:

		Namespace() :
			m_prefix(&s_emptyString),
			m_uri(&s_emptyString),
			m_resultAttributeName(&s_emptyString)
		{
		}

		Namespace(
					const XalanDOMString&	prefix,
					const XalanDOMString&	uri) :
			m_prefix(&prefix),
			m_uri(&uri),
			m_resultAttributeName(&s_emptyString)
		{
		}

		/**
		 * Retrieve the prefix for namespace
		 * 
		 * @return prefix string
		 */
		const XalanDOMString&
		getPrefix() const
		{
			assert(m_prefix != 0);

			return *m_prefix;
		}

		/**
		 * Set the prefix for namespace
		 * 
		 * @param prefix The new prefix value
		 */
		void
		setPrefix(const XalanDOMString&		prefix)
		{
			m_prefix = &prefix;
		}

		/**
		 * Retrieve the URI for namespace
		 * 
		 * @return URI string
		 */
		const XalanDOMString&
		getURI() const
		{
			assert(m_uri != 0);

			return *m_uri;
		}

		/**
		 * Set the URI for namespace
		 * 
		 * @param uri The new uri value
		 */
		void
		setURI(const XalanDOMString&	uri)
		{
			m_uri = &uri;
		}

		/**
		 * Retrieve the name of the result attribute.
		 * 
		 * @return name string
		 */
		const XalanDOMString&
		getResultAttributeName() const
		{
			assert(m_resultAttributeName != 0);

			return *m_resultAttributeName;
		}

		/**
		 * Set the name of the result attribute.
		 * 
		 * @param name The new name value
		 */
		void
		setResultAttributeName(const XalanDOMString&	name)
		{
			m_resultAttributeName = &name;
		}

	private:

		const XalanDOMString*	m_prefix;

		const XalanDOMString*	m_uri;

		const XalanDOMString*	m_resultAttributeName;

		static const XalanDOMString		s_emptyString;
	};

	typedef XalanQName::NamespaceVectorType				NamespaceVectorType;
	typedef XalanQName::NamespacesStackType				NamespacesStackType;

#if defined(XALAN_NO_NAMESPACES)
	typedef map<const XalanDOMString*,
				const XalanDOMString*,
				DOMStringPointerLessThanFunction>		ExcludedResultPrefixesMapType;

	typedef map<const XalanDOMString*,
				Namespace,
				DOMStringPointerLessThanFunction>		NamespacesMapType;

	typedef map<const XalanDOMString*,
				const XalanDOMString*,
				DOMStringPointerLessThanFunction>		NamespaceAliasesMapType;

	typedef vector<const XalanDOMString*>				XalanDOMStringPointerVectorType;
#else
	typedef std::map<const XalanDOMString*,
					 const XalanDOMString*,
					 DOMStringPointerLessThanFunction>	ExcludedResultPrefixesMapType;

	typedef std::map<const XalanDOMString*,
					 Namespace,
					 DOMStringPointerLessThanFunction>	NamespacesMapType;

	typedef std::map<const XalanDOMString*,
					 const XalanDOMString*,
					 DOMStringPointerLessThanFunction>	NamespaceAliasesMapType;

	typedef std::vector<const XalanDOMString*>			XalanDOMStringPointerVectorType;
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
	 * @param theConstructionContext The current construction context.
	 * @param stylesheetNamespacesHandler The stylesheet's handler.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 * @param theXSLTNamespaceURI The namespace URI for XSLT.
	 */
	NamespacesHandler(
			StylesheetConstructionContext&	theConstructionContext,
			const NamespacesHandler&		stylesheetNamespacesHandler,
			const NamespacesStackType&		theCurrentNamespaces,
			const XalanDOMString&			theXSLTNamespaceURI);

	~NamespacesHandler();

	/**
	 * Process an exclude-result-prefixes attribute.
	 *
	 * @param theConstructionContext The current construction context.
	 * @param theValue The attribute's value.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 */
	void
	processExcludeResultPrefixes(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces);

	/**
	 * Process an extension-element-prefixes attribute.
	 *
	 * @param theConstructionContext The current construction context.
	 * @param theValue The attribute's value.
	 * @param theCurrentNamespaces The stack of active namespace declarations.
	 */
	void
	processExtensionElementPrefixes(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMChar*				theValue,
			const NamespacesStackType&		theCurrentNamespaces);

	/**
	 * Notify the instance that the stylesheet is fully constructed.
	 *
	 * @param theConstructionContext The current construction context.
	 * @param fProcessNamespaceAliases If true, process any namespace aliases
	 * @param theElementName The name of the owning element.
	 * @param parentNamespacesHandler The parent handler, if any.
	 * @param prefixChecker A pointer to a PrefixChecker instance to use, if any.
	 */
	void
	postConstruction(
			StylesheetConstructionContext&	theConstructionContext,
			bool							fProcessNamespaceAliases = true,
			const XalanDOMString&			theElementName = XalanDOMString(),
			const NamespacesHandler*		parentNamespacesHandler = 0,
			const PrefixChecker*			prefixChecker = 0);

	NamespacesHandler&
	operator=(const NamespacesHandler&	theRHS);

	/**
	 * Determine of a given namespace should be excluded.
	 *
	 * @param theXSLTNamespaceURI The namespace URI for XSLT.
	 * @param theURI The namespace URI.
	 * @return true of the namespace should be excluded, false if not.
	 */
	bool
	shouldExcludeResultNamespaceNode(
			const XalanDOMString&	theXSLTNamespaceURI,
			const XalanDOMString&	theURI) const;

	/**
	 * Add a URI as an extension namespace prefixes.
	 *
	 * @param theConstructionContext The current construction context.
	 * @param theURI The namespace URI.
	 */
	void
	addExtensionNamespaceURI(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&	theURI);

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
	 * @param theConstructionContext The current construction context.
	 * @param theStylesheetNamespace The namespace as declared in the stylesheet.
	 * @param theResultNamespace The namespace as it should appear in the result tree.
	 */
	void
	setNamespaceAlias(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			theStylesheetNamespace,
			const XalanDOMString&			theResultNamespace);

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
	 * @param supressDefault If true, any default namespace declaration will not be output.
	 */
	void
	outputResultNamespaces(
			StylesheetExecutionContext&		theExecutionContext,
			bool							supressDefault = false) const;

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

	NamespacesMapType::size_type
	getNamespaceDeclarationsCount() const
	{
		return m_namespaceDeclarations.size();
	}

private:

	/**
	 * Create all of the result attribute names.
	 *
	 * @param theConstructionContext The current construction context.
	 */
	void
	createResultAttributeNames(StylesheetConstructionContext&	theConstructionContext);

	/**
	 * Process the exclude result prefix data.
	 *
	 * @param theConstructionContext The current construction context.
	 * @param theElementPrefix The prefix of the owning element.
	 * @param prefixChecker A pointer to a PrefixChecker instance to use, if any.
	 */
	void
	processExcludeResultPrefixes(
			StylesheetConstructionContext&	theConstructionContext,
			const XalanDOMString&			theElementPrefix,
			const PrefixChecker*			prefixChecker);

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
	 * Copy the contents of the supplied vector
	 *
	 * @param theExtensionNamespaceURIs The set to copy.
	 */
	void
	copyExtensionNamespaceURIs(const XalanDOMStringPointerVectorType&	theExtensionNamespaceURIs);

	/**
	 * Copy the contents of the supplied map
	 *
	 * @param theExcludeResultPrefixes The map to copy.
	 */
	void
	copyExcludeResultPrefixes(const ExcludedResultPrefixesMapType&	theExcludeResultPrefixes);

	/**
	 * Determine if a given namespace should be excluded.  For use during
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

	/**
	 * Determine if a given namespace should be excluded as a result of
	 * an exclude-result-prefixes declaration.
	 *
	 * @param theNamespaceURI The namespace URI to check.
	 * @return true if the namespace should be excluded, false if not.
	 */
	bool
	isExcludedNamespaceURI(const XalanDOMString&	theNamespaceURI) const;

	/**
	 * Determine if a given URI is an extension namespace URI
	 *
	 * @param theNamespaceURI The namespace URI to check.
	 * @return true if the namespace uri is an extension namespace URI, false if not.
	 */
	bool
	isExtensionNamespaceURI(const XalanDOMString&	theNamespaceURI) const
	{
		return findString(theNamespaceURI, m_extensionNamespaceURIs);
	}

	/**
	 * Determine if a given string is present in the vector
	 *
	 * @param theString The string to find.
	 * @return true if the string is present, false if not.
	 */
	static bool
	findString(
			const XalanDOMString&					theString,
			const XalanDOMStringPointerVectorType&	theVector);


	// Not implemented...
	bool
	operator==(const NamespacesHandler&) const;

	// Data members...
	ExcludedResultPrefixesMapType		m_excludedResultPrefixes;

	NamespacesMapType					m_namespaceDeclarations;

	XalanDOMStringPointerVectorType		m_extensionNamespaceURIs;

	NamespaceAliasesMapType				m_namespaceAliases;
};



#endif	// XALAN_NAMESPACESHANDLER_HEADER_GUARD
