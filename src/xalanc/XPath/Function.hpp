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
#if !defined(FUNCTION_HEADER_GUARD_1357924680)
#define FUNCTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <vector>



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>


XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



// Base class for all XPath function classes.
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
class XALAN_XPATH_EXPORT Function
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

	explicit
	Function();

	virtual
	~Function();

	typedef XPathExecutionContext::XObjectArgVectorType	XObjectArgVectorType;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object.  Extension functions should override this version of execute(),
	 * rather than one of the other calls designed for a specific number of
	 * arguments, since the extension function mechanism will call this
	 * version first.  Extension functions that accept more than three
	 * arguments _must_ override this version.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param args             vector of pointers to XObject arguments
	 * @param locator		   LocatorType instance for the XPath expression that contains the function call
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has no parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node	 
	 * @param locator		   LocatorType instance for the XPath expression that contains the function call
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator) const;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has one parameter.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg              pointer to XObject argument
	 * @param locator		   LocatorType instance for the XPath expression that contains the function call
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg,
			const LocatorType*		locator) const;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has two parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg1             pointer to XObject argument
	 * @param arg2             pointer to XObject argument
	 * @param locator		   LocatorType instance for the XPath expression that contains the function call
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const LocatorType*		locator) const;

	/**
	 * Execute an XPath function object.  The function must return a valid
	 * object. Called if function has three parameters.
	 *
	 * @param executionContext executing context
	 * @param context          current context node
	 * @param arg1             pointer to XObject arguments
	 * @param arg2             pointer to XObject argument
	 * @param arg3             pointer to XObject argument
	 * @param locator		   LocatorType instance for the XPath expression that contains the function call
	 * @return                 pointer to the result XObject
	 */
	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const LocatorType*		locator) const;

	/**
	 * Create a copy of the function object.
	 *
	 * @return pointer to the new object
	 */
	virtual Function*
	clone(MemoryManagerType&  theManager) const = 0;

protected:

	/**
	 * Get the error message to report when
	 * the function is called with the wrong
	 * number of arguments.
	 *
	 * @return function error message
	 */
	virtual const XalanDOMString&
	getError(XalanDOMString& theBuffer) const = 0;

private:

	// Not implemented...
	Function&
	operator=(const Function&);

	bool
	operator==(const Function&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// FUNCTION_HEADER_GUARD_1357924680
