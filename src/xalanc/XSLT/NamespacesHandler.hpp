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

#if !defined(XALAN_NAMESPACESHANDLER_HEADER_GUARD)
#define XALAN_NAMESPACESHANDLER_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/NameSpace.hpp>
#include <xalanc/XPath/XalanQName.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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

	class XALAN_XSLT_EXPORT Namespace
	{
	public:

		Namespace() :
			m_prefix(&s_emptyString),
			m_uri(&s_emptyString)
		{
		}

		Namespace(
					const XalanDOMString&	prefix,
					const XalanDOMString&	uri) :
			m_prefix(&prefix),
			m_uri(&uri)
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

	protected:

		static const XalanDOMString		s_emptyString;

	private:

		const XalanDOMString*	m_prefix;

		const XalanDOMString*	m_uri;
	};

	class XALAN_XSLT_EXPORT NamespaceExtended : public Namespace
	{
	public:

		NamespaceExtended() :
			Namespace(),
			m_resultAttributeName(&s_emptyString)
		{
		}

		NamespaceExtended(
					const XalanDOMString&	prefix,
					const XalanDOMString&	uri) :
			Namespace(prefix, uri),
			m_resultAttributeName(&s_emptyString)
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

		const XalanDOMString*	m_resultAttributeName;
	};

	typedef XalanQName::NamespaceVectorType				NamespaceVectorType;
	typedef XalanQName::NamespacesStackType				NamespacesStackType;

	typedef XalanVector<Namespace>						NamespacesVectorType;
	typedef XalanVector<NamespaceExtended>				NamespaceExtendedVectorType;
	typedef XalanVector<const XalanDOMString*>			XalanDOMStringPointerVectorType;

	typedef XalanMap<const XalanDOMString*,
					const XalanDOMString*>				NamespaceAliasesMapType;


	/**
	 * Create a default, empty instance.
	 */
	explicit
	NamespacesHandler(MemoryManagerType& theManager);

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
			const XalanDOMString&			theElementName = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
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

	NamespaceExtendedVectorType::size_type
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
	 * Copy the contents of the supplied vector
	 *
	 * @param theExcludeResultPrefixes The vector to copy.
	 */
	void
	copyExcludeResultPrefixes(const NamespacesVectorType&	theExcludeResultPrefixes);

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
	NamespacesVectorType				m_excludedResultPrefixes;

	NamespaceExtendedVectorType			m_namespaceDeclarations;

	XalanDOMStringPointerVectorType		m_extensionNamespaceURIs;

	NamespaceAliasesMapType				m_namespaceAliases;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_NAMESPACESHANDLER_HEADER_GUARD
