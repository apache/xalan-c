/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XOBJECT_HEADER_GUARD_1357924680)
#define XOBJECT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <XPath/XPathException.hpp>



class MutableNodeRefList;
class NodeRefListBase;
class ResultTreeFragBase;
class XObjectTypeCallback;
class XPathExecutionContext;



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XObject
{
public:

	/**
	 * Create an XObject.
	 *
	 */
	explicit
	XObject();

	XObject(const XObject&	source);

	/**
	 * Clone the instance
	 *
	 * @param If theAddress is not null, the XObject will clone itself into that address.
	 * @return a clone of the instance.
	 */
	virtual XObject*
	clone(void*		theAddress = 0) const = 0;

	/**
	 * Given a request type, return the equivalent string. 
	 * For diagnostic purposes.
	 *
	 * @return string for type
	 */
	virtual XalanDOMString
	getTypeString() const = 0;

	/**
	 * Cast result object to a number.
	 *
	 * @return numeric value
	 */
	virtual double
	num() const;

	/**
	 * Cast result object to a boolean.
	 *
	 * @return boolean value
	 */
	virtual bool
	boolean() const;

	/**
	 * Cast result object to a string.
	 *
	 * @return string value
	 */
	virtual XalanDOMString
	str() const;

	/**
	 * Cast result object to a result tree fragment.
	 *
	 * @param executionContext the current execution context
	 * @return result tree fragment
	 */
	virtual const ResultTreeFragBase&
	rtree(XPathExecutionContext&	executionContext) const;

	/**
	 * Cast result object to a nodelist.
	 *
	 * @return node list
	 */
	virtual const NodeRefListBase&
	nodeset() const;

	/**
	 * Process a callback request for preferred type information.
	 *
	 * @param theCallbackObject object to call back
	 */
	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) = 0;

	/**
	 * Process a callback request for preferred type information.
	 *
	 * @param theCallbackObject object to call back
	 */
	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const = 0;

	/**
	 * Tell if two objects are functionally equal.
	 *
	 * @param theRHS object to compare
	 * @param executionContext the current execution context
	 * @return true if they are equal
	 */
	bool
	equals(const XObject&	theRHS) const;

	/**
	 * Tell if two objects are functionally not equal.
	 *
	 * @param theRHS object to compare
	 * @param executionContext the current execution context
	 * @return true if they are equal
	 */
	bool
	notEquals(const XObject&	theRHS) const;

	/**
	 * Tell if one object is less than the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	lessThan(const XObject&		theRHS) const;

	/**
	 * Tell if one object is less than or equal
	 * the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	lessThanOrEqual(const XObject&	theRHS) const;

	/**
	 * Tell if one object is greater than the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	greaterThan(const XObject&	theRHS) const;

	/**
	 * Tell if one object is greater than or equal
	 * the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	greaterThanOrEqual(const XObject&	theRHS) const;

	/**
	 * Enumeration of possible object types
	 */
	enum	eObjectType { eTypeNull = 0,
						  eTypeUnknown = 1,
						  eTypeBoolean = 2,
						  eTypeNumber = 3,
						  eTypeString = 4,
						  eTypeNodeSet = 5,
						  eTypeResultTreeFrag = 6,
						  eTypeUserDefined = 7,
						  eUnknown
						};

	/**
	 * Tell what kind of class this is.
	 *
	 * @return numeric type value
	 */
	virtual	eObjectType
	getType() const = 0;

	// All XObject instances are controlled by an instance of an XObjectFactory.
	friend class XObjectFactory;

	// Base class for all XObject exceptions...
	class XObjectException : public XPathException
	{
	public:

		explicit
		XObjectException(
				const XalanDOMString&	message = XalanDOMString(),
				const XalanNode*		styleNode = 0);

		virtual
		~XObjectException();
	};

	class XObjectInvalidCastException : public XObjectException
	{
	public:

		explicit
		XObjectInvalidCastException(
				const XalanDOMString&	fromType,
				const XalanDOMString&	toType);

		virtual
		~XObjectInvalidCastException();

		const XalanDOMString&
		getFromType() const
		{
			return m_fromType;
		}

		const XalanDOMString&
		getToType() const
		{
			return m_toType;
		}

	private:

		static const XalanDOMString
		formatErrorString(
				const XalanDOMString&	fromType,
				const XalanDOMString&	toType);

		const XalanDOMString	m_fromType;

		const XalanDOMString	m_toType;
	};

protected:

	virtual
	~XObject();

private:

	// Not implemented...
	XObject&
	operator=(const XObject&);
};



inline bool
operator==(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.equals(theRHS);
}



inline bool
operator!=(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.notEquals(theRHS);
}



inline bool
operator<(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.lessThan(theRHS);
}



inline bool
operator<=(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.lessThanOrEqual(theRHS);
}



inline bool
operator>(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.greaterThan(theRHS);
}



inline bool
operator>=(
			const XObject&	theLHS,
			const XObject&	theRHS)
{
	return theLHS.greaterThanOrEqual(theRHS);
}



#endif	// XOBJECT_HEADER_GUARD_1357924680
