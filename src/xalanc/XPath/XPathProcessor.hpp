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
#if !defined(XPATHPROCESSOR_HEADER_GUARD_1357924680)
#define XPATHPROCESSOR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// $$$ ToDo: This is necessary while XalanDOMString is still a typedef...
#include <xalanc/XalanDOM/XalanDOMString.hpp>




XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class Function;
class PrefixResolver;
class XPath;
class XPathConstructionContext;



class XALAN_XPATH_EXPORT XPathProcessor
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

	explicit
	XPathProcessor();

	virtual
	~XPathProcessor();

	/**
	 * Given a string, make an XPath object, in order that a parse doesn't 
	 * have to be done each time the expression is executed.
	 *
	 * @param pathObj        XPath object to be initialized
	 * @param constructionContext The construction context
	 * @param expression     expression that will be evaluated
	 * @param resolver       prefix resolver to use
	 * @param locator		 the LocatorType to use for error report. May be null
     * @param allowVariableReferences If true, variable references are allowed.
     * @param allowKeyFunction If true, calls to the key() function are allowed.
	 */
	virtual void
	initXPath(
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator = 0,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true) = 0;

	/**
	 * Given a string, create an XSLT Match Pattern object.
	 *
	 * @param pathObj        XPath object to be initialized
	 * @param constructionContext The construction context
	 * @param expression     expression that will be evaluated
	 * @param resolver       prefix resolver to use
	 * @param locator		 the LocatorType to use for error report. May be null
     * @param allowVariableReferences If true, variable references are allowed.
     * @param allowKeyFunction If true, calls to the key() function are allowed.
	 */
	virtual void
	initMatchPattern(
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator = 0,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true) = 0;

	/**
	 * Given a string, and a reference to a function object, install the
	 * function with the given name.
	 *
	 * @param theFunctionName name of function
	 * @param theFunction     function object corresponding to name
	 */
	static void
	installFunction(
			const XalanDOMString&	theFunctionName,
			const Function&			theFunction);
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHPROCESSOR_HEADER_GUARD_1357924680
