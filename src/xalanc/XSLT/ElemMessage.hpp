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
#if !defined(XALAN_ELEMMESSAGE_HEADER_GUARD)
#define XALAN_ELEMMESSAGE_HEADER_GUARD 



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class header file.
#include "ElemTemplateElement.hpp"
#include "XSLTProcessorException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class ElemMessage: public ElemTemplateElement
{
public:

	/**
	 * Construct an object corresponding to an "xsl:message" element
	 * 
	 * @param constructionContext context for construction of object
	 * @param stylesheetTree      stylesheet containing element
	 * @param name                name of element
	 * @param atts                list of attributes for element
	 * @param lineNumber				line number in document
	 * @param columnNumber			column number in document
	 */
	ElemMessage(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber);

	// These methods are inherited from ElemTemplateElement ...

	virtual const XalanDOMString&
	getElementName() const;
	
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
	virtual	const ElemTemplateElement*
	startElement(StylesheetExecutionContext&		executionContext) const;

	virtual	void
	endElement(StylesheetExecutionContext&		executionContext) const;
#else
	virtual	void
	execute(StylesheetExecutionContext&		executionContext) const;
#endif

	class ElemMessageTerminateException : public XSLTProcessorException
	{
	public:

		/**
		 * Construct an ElemMessageTerminateException object
		 * 
		 * @param theMessage message to print when exception thrown
		 */
		ElemMessageTerminateException(
                    MemoryManagerType&          theManager,
                    const XalanDOMString&		theMessage);
		
		/**
		 * Construct an ElemMessageTerminateException object
		 * 
		 * @param theLocator The Locator to use for line number information, etc.
		 * @param theMessage message to print when exception thrown
		 */
		ElemMessageTerminateException(
                MemoryManagerType&      theManager,
				const LocatorType&		theLocator,
				const XalanDOMString&	theMessage);

		virtual
		~ElemMessageTerminateException();

		
		virtual const XalanDOMChar*
			getType() const
		{
			return m_type;
		}
		
	private:
		
		static const XalanDOMChar	m_type[];
	};

private:

	bool	m_terminate;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_ELEMMESSAGE_HEADER_GUARD
