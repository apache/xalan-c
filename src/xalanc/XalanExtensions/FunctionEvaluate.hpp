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
#if !defined(FUNCTIONEVALUATE_HEADER_GUARD_1357924680)
#define FUNCTIONEVALUATE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XalanExtensions/XalanExtensionsDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * XPath implementation of "node-set" function.
 */
class XALAN_XALANEXTENSIONS_EXPORT FunctionEvaluate : public Function
{
public:

	typedef Function	ParentType;

	FunctionEvaluate();

	virtual
	~FunctionEvaluate();

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionEvaluate*
#endif
	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...
	FunctionEvaluate&
	operator=(const FunctionEvaluate&);

	bool
	operator==(const FunctionEvaluate&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// FUNCTIONEVALUATE_HEADER_GUARD_1357924680
