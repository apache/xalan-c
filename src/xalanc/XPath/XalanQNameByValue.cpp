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
// Class header file...
#include "XalanQNameByValue.hpp"



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>
#include <xalanc/DOMSupport/DOMSupport.hpp>



#include "ElementPrefixResolverProxy.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanQNameByValue::XalanQNameByValue() :
	XalanQName(),
	m_namespace(),
	m_localpart()
{
}



XalanQNameByValue::XalanQNameByValue(const XalanQNameByValue&	theSource) :
	XalanQName(),
	m_namespace(theSource.m_namespace),
	m_localpart(theSource.m_localpart)
{
}



XalanQNameByValue::XalanQNameByValue(const XalanQName&	theSource) :
	XalanQName(),
	m_namespace(theSource.getNamespace()),
	m_localpart(theSource.getLocalPart())
{
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart) :
	XalanQName(),
	m_namespace(theNamespace),
	m_localpart(theLocalPart)
{
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault) :
	m_namespace(),
	m_localpart()
{
	initialize(
		c_wstr(qname),
		length(qname),
		namespaces,
		locator,
		fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault) :
	m_namespace(),
	m_localpart()
{
	assert(qname != 0);

	initialize(
		qname,
		length(qname),
		namespaces,
		locator,
		fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const DOMSupport& 		domSupport,
			const LocatorType*		locator) :
	m_namespace(),
	m_localpart()
{
	ElementPrefixResolverProxy	theProxy(namespaceContext, envSupport, domSupport);

	resolvePrefix(
		c_wstr(qname),
		length(qname),
		&theProxy,
		locator);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver,
			const LocatorType*		locator) :
	m_namespace(),
	m_localpart()
{
	resolvePrefix(
		c_wstr(qname),
		length(qname),
		theResolver,
		locator);
}



XalanQNameByValue::~XalanQNameByValue()
{
}



const XalanDOMString&
XalanQNameByValue::getLocalPart() const
{
	return m_localpart;
}



const XalanDOMString&
XalanQNameByValue::getNamespace() const
{
	return m_namespace;
}



void
XalanQNameByValue::set(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault)
{
	initialize(
		c_wstr(qname),
		length(qname),
		namespaces,
		locator,
		fUseDefault);
}



void
XalanQNameByValue::set(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault)
{
	assert(qname != 0);

	initialize(
		qname,
		length(qname),
		namespaces,
		locator,
		fUseDefault);
}



void
XalanQNameByValue::set(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver,
			const LocatorType*		locator)
{
	resolvePrefix(
		c_wstr(qname),
		length(qname),
		theResolver,
		locator);
}



void
XalanQNameByValue::set(
			const XalanDOMChar*		qname,
			const PrefixResolver*	theResolver,
			const LocatorType*		locator)
{
	assert(qname != 0);

	resolvePrefix(
		qname,
		length(qname),
		theResolver,
		locator);
}



void
throwException(
			const XalanDOMString&					theMessage,
			const XalanQNameByValue::LocatorType*	theLocator)
{
	if (theLocator == 0)
	{
		throw XSLException(theMessage);
	}
	else
	{
		throw XSLException(*theLocator, theMessage);
	}
}




void
XalanQNameByValue::initialize(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	len,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault)
{
	const XalanDOMString::size_type		indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

	if (indexOfNSSep == 0)
	{
		throwException(XalanMessageLoader::getMessage(XalanMessages::PrefixOfLengthZeroDetected), locator);
	}
	else if(indexOfNSSep < len)
	{
		// Reserve some space for the local part right now, and
		// use it as a temporary for the prefix.
		m_localpart.reserve(len);

		m_localpart.assign(qname, indexOfNSSep);

		if(m_localpart == DOMServices::s_XMLString)
		{
			m_namespace = DOMServices::s_XMLNamespaceURI;
		}
		else if(m_localpart == DOMServices::s_XMLNamespace)
		{
			// The xmlns prefix is not normally bound to a namespace URI, but we're really trying
			// to form a QName, so we'll do what the DOM does...
			m_namespace = DOMServices::s_XMLNamespacePrefixURI;
		}
		else
		{
			const XalanDOMString* const		theNamespace = 
						getNamespaceForPrefix(namespaces, m_localpart);

			if(theNamespace == 0 || 0 == length(*theNamespace))
			{
				throwException(
					XalanMessageLoader::getMessage(XalanMessages::PrefixMustResolveToNamespace_1Param, m_localpart),
					locator);
			}
			else
			{
				m_namespace = *theNamespace;
			}
		}

		m_localpart.assign(qname + indexOfNSSep + 1, len - (indexOfNSSep + 1));
	}
	else
	{
		if (fUseDefault == false)
		{
			m_namespace.clear();
		}
		else
		{
			const XalanDOMString* const		theNamespace = 
						getNamespaceForPrefix(namespaces, s_emptyString);

			if(theNamespace != 0 && 0 != length(*theNamespace))
			{
				m_namespace = *theNamespace;
			}
		}

		m_localpart = qname;
	}

	validate(qname, len, locator);
}



void
XalanQNameByValue::resolvePrefix(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	theLength,
			const PrefixResolver*		theResolver,
			const LocatorType*			locator)
{
	const XalanDOMString::size_type		indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

	if (indexOfNSSep == 0)
	{
		throwException(XalanMessageLoader::getMessage(XalanMessages::PrefixOfLengthZeroDetected), locator);
	}
	else if(indexOfNSSep >= theLength)
	{
		m_localpart.assign(qname, theLength);

		m_namespace.clear();
	}
	else
	{
		// Reserve some space for the local part right now, and
		// use it as a temporary for the prefix.
		m_localpart.reserve(theLength);

		m_localpart.assign(qname, indexOfNSSep);

		if(m_localpart == DOMServices::s_XMLString)
		{
			m_namespace = DOMServices::s_XMLNamespaceURI;
		}
		else if(m_localpart == DOMServices::s_XMLNamespace)
		{
			// The xmlns prefix is not normally bound to a namespace URI, but we're really trying
			// to form a QName, so we'll do what the DOM does...
			m_namespace = DOMServices::s_XMLNamespacePrefixURI;
		}
		else if (theResolver == 0)
		{
			throwException(
				XalanMessageLoader::getMessage(
					XalanMessages::PrefixMustResolveToNamespace_1Param,
					m_localpart),
				locator);
		}
		else
		{
			const XalanDOMString* const		theNamespace =
				theResolver->getNamespaceForPrefix(m_localpart);

			if (theNamespace != 0)
			{
				m_namespace = *theNamespace;
			}
		}  

		if(0 == length(m_namespace))
		{
			throwException(
				XalanMessageLoader::getMessage(
					XalanMessages::PrefixMustResolveToNamespace_1Param,
					m_localpart)
				,locator);
		}

		m_localpart.assign(qname + indexOfNSSep + 1, theLength - (indexOfNSSep + 1));
	}

	validate(qname, theLength, locator);
}



void
XalanQNameByValue::validate(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	theLength,
			const LocatorType*			locator)
{
	if (isValid() == false)
	{
		if (locator != 0)
		{
			throw InvalidQNameException(*locator, qname, theLength);
		}
		else
		{
			throw InvalidQNameException(qname, theLength);
		}
	}
}


	
XALAN_CPP_NAMESPACE_END
