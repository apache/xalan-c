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
#if !defined(FORMATTERSTRINGLENGTHCOUNTER_HEADER_GUARD_1357924680)
#define FORMATTERSTRINGLENGTHCOUNTER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * This class takes SAX events (in addition to some extra events 
 * that SAX doesn't handle yet) and produces counts the characters
 * sent to the characters event.
 */
class XALAN_XPATH_EXPORT FormatterStringLengthCounter : public FormatterListener
{
public:

	typedef unsigned long	size_type;

	/**
	 * FormatterStringLengthCounter instance constructor.
	 */
	explicit
	FormatterStringLengthCounter();

	virtual
	~FormatterStringLengthCounter();


	size_type
	getCount() const
	{
		return m_count;
	}

	// These methods are inherited from FormatterListener ...

	virtual void
	setDocumentLocator(const LocatorType* const		locator);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs);

    virtual void
	endElement(const XMLCh* const	name);

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);

    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void
	resetDocument();

	virtual void
	comment(const XMLCh* const	data);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

private:

	// These are not implemented.
	FormatterStringLengthCounter(const FormatterStringLengthCounter&);

	FormatterStringLengthCounter&
	operator=(const FormatterStringLengthCounter&);

	bool
	operator==(const FormatterStringLengthCounter&) const;

	// Data members...
	size_type	m_count;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERSTRINGLENGTHCOUNTER_HEADER_GUARD_1357924680
