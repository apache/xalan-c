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
#if !defined(XNULL_HEADER_GUARD_1357924680)
#define XNULL_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XNull : public XObject
{
public:

	typedef XObject		ParentType;

	/**
	 * Create an XNull.
	 *
	 */
	explicit
	XNull();

	/**
	 * Create an XNull from another.
	 *
	 * @param source    object to copy
	 * @param deepClone true to copy all children on nodeset nodes
	 */
	XNull(const XNull&	source);

	virtual
	~XNull();

	// These methods are inherited from XObject ...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XNull*
#endif
	clone(void*		theAddress = 0) const;

	virtual XalanDOMString
	getTypeString() const;

	virtual double
	num() const;

	virtual bool
	boolean() const;

	virtual const XalanDOMString&
	str() const;

	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::str;
#endif

	virtual double
	stringLength() const;

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

private:

	// not implemented
	XNull& operator=(const XNull &);
};



XALAN_CPP_NAMESPACE_END



#endif	// XNULL_HEADER_GUARD_1357924680
