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
#if !defined(EXSLT_COMMONIMPL_HEADER_GUARD_1357924680)
#define EXSLT_COMMONIMPL_HEADER_GUARD_1357924680



#include "XalanEXSLTDefinitions.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/Function.hpp>



#include <xalanc/XalanExtensions/FunctionNodeSet.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionNodeSet : public FunctionNodeSet
{
public:

	XalanEXSLTFunctionNodeSet() :
		FunctionNodeSet(true)
	{
	}

	virtual
	~XalanEXSLTFunctionNodeSet()
	{
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionNodeSet*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionNodeSet(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return XalanMessageLoader::getMessage(XalanMessages::EXSLTFunctionAcceptsOneArgument_1Param,"node-set()");
	}

	virtual const XalanDOMString
	getInvalidArgumentTypeError() const 
	{
		return XalanMessageLoader::getMessage(XalanMessages::InvalidArgumentType_1Param,"node-set()");;
	}

private:

	// Not implemented...
	XalanEXSLTFunctionNodeSet&
	operator=(const XalanEXSLTFunctionNodeSet&);

	bool
	operator==(const XalanEXSLTFunctionNodeSet&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionObjectType : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionObjectType() :
		Function(),
		m_boolean(s_booleanString),
		m_external(s_externalString),
		m_nodeSet(s_nodeSetString),
		m_number(s_numberString),
		m_rtf(s_rtfString),
		m_string(s_stringString)
	{
	}

	// A dummy constructor for use internally.  Do not use this one!!!!
	XalanEXSLTFunctionObjectType(int	/* theDummy */) :
		Function(),
		m_boolean(),
		m_external(),
		m_nodeSet(),
		m_number(),
		m_rtf(),
		m_string()
	{
	}

	virtual
	~XalanEXSLTFunctionObjectType()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionObjectType*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionObjectType;
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionObjectType(const XalanEXSLTFunctionObjectType&);

	XalanEXSLTFunctionObjectType&
	operator=(const XalanEXSLTFunctionObjectType&);

	bool
	operator==(const XalanEXSLTFunctionObjectType&) const;


	// Data members...
	const XalanDOMString	m_boolean;
	const XalanDOMString	m_external;
	const XalanDOMString	m_nodeSet;
	const XalanDOMString	m_number;
	const XalanDOMString	m_rtf;
	const XalanDOMString	m_string;

	static const XalanDOMChar	s_booleanString[];
	static const XalanDOMChar	s_externalString[];
	static const XalanDOMChar	s_nodeSetString[];
	static const XalanDOMChar	s_numberString[];
	static const XalanDOMChar	s_rtfString[];
	static const XalanDOMChar	s_stringString[];
};



XALAN_CPP_NAMESPACE_END



#endif	// EXSLT_COMMONIMPL_HEADER_GUARD_1357924680
