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
#if !defined(FUNCTIONELEMENTAVAILABLE_HEADER_GUARD_1357924680)
#define FUNCTIONELEMENTAVAILABLE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XObject;
class XPathExecutionContext;



// Implementation of the XSLT function element-available.
//
class XALAN_XSLT_EXPORT FunctionElementAvailable : public Function
{
public:

	typedef Function	ParentType;

	FunctionElementAvailable();

	virtual
	~FunctionElementAvailable();

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg,
			const LocatorType*		locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionElementAvailable*
#endif
	clone(MemoryManagerType& theManager) const;

private:
	
	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;


	// Not implemented...
	FunctionElementAvailable&
	operator=(const FunctionElementAvailable&);

	bool
	operator==(const FunctionElementAvailable&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// FUNCTIONELEMENTAVAILABLE_HEADER_GUARD_1357924680
