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

#if !defined(XALAN_TOPLEVELARG_HEADER_GUARD)
#define XALAN_TOPLEVELARG_HEADER_GUARD




// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XObjectPtr;



/**
 * This class holds an instance of an argument on the stack.
 */
class XALAN_XSLT_EXPORT TopLevelArg
{
public:

	/**
	 * Construct an argument object from a string expression
	 * 
	 * @param name	name of argument
	 * @param expr	expression argument represents
	 */
	TopLevelArg(
        MemoryManagerType&      theManager,
		const XalanQName&		name,
		const XalanDOMString&	expr);

	static TopLevelArg*
    create(
        MemoryManagerType&      theManager,
		const XalanQName&		name,
		const XalanDOMString&	expr);
	/**
	 * Construct an argument object from an XObject instance.
	 * 
	 * @param name	name of argument
	 * @param variable	the XObject instance.
	 */
	TopLevelArg(
        MemoryManagerType&  theManager,
		const XalanQName&	name,
		const XObjectPtr	variable = XObjectPtr());

	static TopLevelArg*
    create(
        MemoryManagerType&      theManager,
		const XalanQName&		name,
		const XObjectPtr	variable = XObjectPtr());
	/**
	 * Copy constructor
	 * 
	 * @param theSource	the TopLevelArg to copy.
	 */
	TopLevelArg(
                const TopLevelArg&	theSource,
                MemoryManagerType&  theManager);


	/**
	 * Destructor
	 */
	~TopLevelArg();

	/**
	 * Retrieve object name
	 * 
	 * @return qualified name of object
	 */
	const XalanQName&
	getName() const
	{
		return m_qname;
	}

	/**
	 * Retrieve object's expression
	 * 
	 * @return string representation of expression
	 */
	const XalanDOMString&
	getExpression() const
	{
		return m_expression;
	};

	/**
	 * Retrieve object's XObject variable.
	 * 
	 * @return pointer to the XObject instance
	 */
	const XObjectPtr
	getXObject() const
	{
		return m_xobject;
	}

	/**
	 * Assignment operator
	 */
	TopLevelArg&
	operator=(const TopLevelArg&	theRHS)
	{
		if (&theRHS != this)
		{
			m_qname = theRHS.m_qname;

			m_expression = theRHS.m_expression;
		}

		return *this;
	}

private:
    // not implemented
    TopLevelArg(const TopLevelArg&	theSource);

	XalanQNameByValue	m_qname;

	XalanDOMString		m_expression;

	const XObjectPtr	m_xobject;
};

XALAN_USES_MEMORY_MANAGER(TopLevelArg)

XALAN_CPP_NAMESPACE_END



#endif	// XALAN_TOPLEVELARG_HEADER_GUARD
