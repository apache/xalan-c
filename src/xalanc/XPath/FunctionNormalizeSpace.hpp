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
#if !defined(FUNCTIONNORMALIZE_HEADER_GUARD_1357924680)
#define FUNCTIONNORMALIZE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/Function.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * XPath implementation of "normalize-space" function.
 */
class XALAN_XPATH_EXPORT FunctionNormalizeSpace : public Function
{
public:
	
	typedef Function	ParentType;

	FunctionNormalizeSpace();

	virtual
	~FunctionNormalizeSpace();

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator) const;

	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionNormalizeSpace*
#endif
	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	bool
	needsNormalization(const XalanDOMString&	theString) const;

	XObjectPtr
	normalize(
		XPathExecutionContext&	executionContext,
		const XalanDOMString&	theString) const;

	XObjectPtr
	normalize(
		XPathExecutionContext&	executionContext,
		const XObjectPtr&		theArg) const;

	// Not implemented...
	FunctionNormalizeSpace&
	operator=(const FunctionNormalizeSpace&);

	bool
	operator==(const FunctionNormalizeSpace&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// FUNCTIONNORMALIZE_HEADER_GUARD_1357924680
