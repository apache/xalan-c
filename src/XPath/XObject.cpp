/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
// Class header file.
#include "XObject.hpp"



#include <XalanDOM/XalanNode.hpp>



#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "NodeRefList.hpp"
#include "XObjectFactory.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	XObject::s_nullString;



XObject::XObject(eObjectType	theObjectType) :
	m_objectType(theObjectType),
	m_factory(0)
{
}



XObject::XObject(const XObject&		source) :
	m_objectType(source.m_objectType),
	m_factory(0)
{
}



XObject::~XObject()
{
}



XObject::eObjectType
XObject::getRealType() const
{
	return m_objectType;
}



void
XObject::referenced()
{
}



void 
XObject::dereferenced()
{
	if (m_factory != 0)
	{
		m_factory->returnObject(this);
	}
}



double
XObject::num() const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("number"));

	// This is just a dummy value to satisfy the compiler.
	return 0.0;
}



bool
XObject::boolean() const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("boolean"));

	// This is just a dummy value to satisfy the compiler.
	return false;
}



const XalanDOMString&
XObject::str() const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("string"));

	// This is just a dummy value to satisfy the compiler.
	return s_nullString;
}



void
XObject::str(
			FormatterListener&	/* formatterListener */,
			MemberFunctionPtr	/* function */) const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("string"));
}



void
XObject::str(XalanDOMString&	theBuffer) const
{
	append(theBuffer, str());
}



static int foo = 0;

const ResultTreeFragBase&
XObject::rtree() const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("result tree fragment"));

	// This is just a dummy value to satisfy the compiler.
#if defined(XALAN_OLD_STYLE_CASTS)
	return (const ResultTreeFragBase&)foo;
#else
	return reinterpret_cast<const ResultTreeFragBase&>(foo);
#endif
}



static const NodeRefList	s_dummyList;



const NodeRefListBase&
XObject::nodeset() const
{
	throw XObjectInvalidConversionException(getTypeString(), TranscodeFromLocalCodePage("node set"));

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return s_dummyList;
}



void
getStringFromNode(
			const XalanNode&	theNode,
			XalanDOMString&		theString)
{
	DOMServices::getNodeData(theNode, theString);
}



struct
getStringFromNodeFunction
{
	getStringFromNodeFunction(XPathExecutionContext&	executionContext) :
		m_executionContext(executionContext)
	{
	}

	const XalanDOMString
	operator()(const XalanNode&		theNode) const
	{
		XPathExecutionContext::GetAndReleaseCachedString	theGuard(m_executionContext);

		XalanDOMString&		theString = theGuard.get();

		getStringFromNode(theNode, theString);

		return theString;
	}

	void
	operator()(
			const XalanNode&	theNode,
			XalanDOMString&		theString) const
	{
		getStringFromNode(theNode, theString);
	}

private:

	XPathExecutionContext&	m_executionContext;
};



struct
getNumberFromNodeFunction
{
	getNumberFromNodeFunction(XPathExecutionContext&	executionContext) :
		m_executionContext(executionContext)
	{
	}

	double
	operator()(const XalanNode&		theNode) const
	{
		return getNumberFromNode(theNode);
	}

	void
	operator()(
			const XalanNode&	theNode,
			double&				theNumber) const
	{
		getNumberFromNode(theNode, theNumber);
	}

private:

	double
	getNumberFromNode(const XalanNode&	theNode) const
	{
		XPathExecutionContext::GetAndReleaseCachedString	theString(m_executionContext);

		getStringFromNode(theNode, theString.get());

		return DoubleSupport::toDouble(theString.get());
	}

	void
	getNumberFromNode(
			const XalanNode&	theNode,
			double&				theNumber) const
	{
		theNumber = getNumberFromNode(theNode);
	}

	XPathExecutionContext&	m_executionContext;
};



struct
equalsDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DOMStringEqualsFunction()(theLHS, theRHS);
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DOMStringEqualsFunction()(theLHS.str(), theRHS);
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DOMStringEqualsFunction()(theLHS, theRHS.str());
	}
};



struct
notEqualsDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DOMStringNotEqualsFunction()(theLHS, theRHS);
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DOMStringNotEqualsFunction()(theLHS.str(), theRHS);
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DOMStringNotEqualsFunction()(theLHS, theRHS.str());
	}
};



struct
lessThanDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::lessThan(
				DOMStringToDouble(theLHS),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::lessThan(
				theLHS.num(),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DoubleSupport::lessThan(
				DOMStringToDouble(theLHS),
				theRHS.num());
	}
};



struct
lessThanOrEqualDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::lessThanOrEqual(
				DOMStringToDouble(theLHS),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::lessThanOrEqual(
				theLHS.num(),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DoubleSupport::lessThanOrEqual(
				DOMStringToDouble(theLHS),
				theRHS.num());
	}
};



struct
greaterThanDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::greaterThan(
				DOMStringToDouble(theLHS),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::greaterThan(
				theLHS.num(),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DoubleSupport::greaterThan(
				DOMStringToDouble(theLHS),
				theRHS.num());
	}
};



struct
greaterThanOrEqualDOMString
{
	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::greaterThanOrEqual(
				DOMStringToDouble(theLHS),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XObject&			theLHS,
			const XalanDOMString&	theRHS) const
	{
		return DoubleSupport::greaterThanOrEqual(
				theLHS.num(),
				DOMStringToDouble(theRHS));
	}

	bool
	operator()(
			const XalanDOMString&	theLHS,
			const XObject&			theRHS) const
	{
		return DoubleSupport::greaterThanOrEqual(
				DOMStringToDouble(theLHS),
				theRHS.num());
	}
};



template<class CompareFunction, class TypeFunction>
bool
doCompareNodeSets(
			const NodeRefListBase&	theLHSNodeSet,
			const NodeRefListBase&	theRHSNodeSet,
			const TypeFunction&		theTypeFunction,
			const CompareFunction&	theCompareFunction,
			XPathExecutionContext&	executionContext)
{
	// From http://www.w3.org/TR/xpath: 
	// If both objects to be compared are node-sets, then the comparison 
	// will be true if and only if there is a node in the first node-set 
	// and a node in the second node-set such that the result of performing 
	// the comparison on the string-values of the two nodes is true.

	// Note this little gem from the draft:
	// NOTE: If $x is bound to a node-set, then $x="foo" 
	// does not mean the same as not($x!="foo"): the former 
	// is true if and only if some node in $x has the string-value 
	// foo; the latter is true if and only if all nodes in $x have 
	// the string-value foo.
	bool	theResult = false;

	const NodeRefListBase::size_type	len1 = theLHSNodeSet.getLength();

	if (len1 > 0)
	{
		const NodeRefListBase::size_type	len2 = theRHSNodeSet.getLength();

		if (len2 > 0)
		{
			XPathExecutionContext::GetAndReleaseCachedString	s1(executionContext);

			XPathExecutionContext::GetAndReleaseCachedString	s2(executionContext);

			for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
			{
				const XalanNode* const	theLHSNode = theLHSNodeSet.item(i);
				assert(theLHSNode != 0);

				theTypeFunction(*theLHSNode, s1.get());

				for(NodeRefListBase::size_type k = 0; k < len2 && theResult == false; k++)
				{
					const XalanNode* const	theRHSNode = theRHSNodeSet.item(k);
					assert(theRHSNode != 0);

					theTypeFunction(*theRHSNode, s2.get());

					if(theCompareFunction(s1.get(), s2.get()) == true)
					{
						theResult = true;
					}

					clear(s2.get());
				}

				clear(s1.get());
			}
		}
	}

	return theResult;
}



template<class CompareFunction, class StringFunction>
bool
doCompareString(
			const NodeRefListBase&	theLHSNodeSet,
			const StringFunction&	theStringFunction,
			const XObject&			theRHS,
			const CompareFunction&	theCompareFunction,
			XPathExecutionContext&	executionContext)
{
	bool				theResult = false;

	const NodeRefListBase::size_type	len1 = theLHSNodeSet.getLength();

	XPathExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

	XalanDOMString&		theLHS = theGuard.get();

	for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
	{
		const XalanNode* const	theLHSNode = theLHSNodeSet.item(i);
		assert(theLHSNode != 0);

		theStringFunction(*theLHSNode, theLHS);

		if (theCompareFunction(theLHS, theRHS) == true)
		{
			theResult = true;
		}

		clear(theLHS);
	}

	return theResult;
}



template<class CompareFunction, class NumberFunction>
bool
doCompareNumber(
			const NodeRefListBase&	theLHSNodeSet,
			const NumberFunction&	theNumberFunction,
			const double			theRHS,
			const CompareFunction&	theCompareFunction)
{
	bool				theResult = false;

	const NodeRefListBase::size_type	len1 = theLHSNodeSet.getLength();

	for(NodeRefListBase::size_type i = 0; i < len1 && theResult == false; i++)
	{
		const XalanNode* const	theLHSNode = theLHSNodeSet.item(i);
		assert(theLHSNode != 0);

		const double	theLHS = theNumberFunction(*theLHSNode);

		if (theCompareFunction(theLHS, theRHS) == true)
		{
			theResult = true;
		}
	}

	return theResult;
}



template<class StringCompareFunction, class NumberCompareFunction>
bool
compareNodeSets(
			const XObject&					theLHS,
			const XObject&					theRHS,
			XObject::eObjectType			theRHSType,
			const StringCompareFunction&	theStringCompareFunction,
			const NumberCompareFunction&	theNumberCompareFunction,
			XPathExecutionContext&			executionContext)
{
	bool	theResult = false;

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(executionContext),
				theStringCompareFunction,
				executionContext);

	}
	else if(theRHSType == XObject::eTypeBoolean)
	{
	  // From http://www.w3.org/TR/xpath: 
	  // If one object to be compared is a node-set and the other is a boolean, 
	  // then the comparison will be true if and only if the result of 
	  // performing the comparison on the boolean and on the result of 
	  // converting the node-set to a boolean using the boolean function 
	  // is true.
		const double	num1 = theLHS.boolean() == true ? 1.0 : 0.0;

		theResult = theNumberCompareFunction(num1, theRHS.num());
	}
	else if(theRHSType == XObject::eTypeNumber)
	{
		// From http://www.w3.org/TR/xpath: 
		// If one object to be compared is a node-set and the other is a number, 
		// then the comparison will be true if and only if there is a 
		// node in the node-set such that the result of performing the 
		// comparison on the number to be compared and on the result of 
		// converting the string-value of that node to a number using 
		// the number function is true. 

		theResult = doCompareNumber(
				theLHS.nodeset(),
				getNumberFromNodeFunction(executionContext),
				theRHS.num(),
				theNumberCompareFunction);
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompareNumber(
					theLHS.nodeset(),
					getNumberFromNodeFunction(executionContext),
					theRHS.num(),
					theNumberCompareFunction);
		}
		else
		{
			// Compare as string...
			theResult = doCompareString(
					theLHS.nodeset(),
					getStringFromNodeFunction(executionContext),
					theRHS,
					theStringCompareFunction,
					executionContext);
		}
	}
	else if(theRHSType == XObject::eTypeString)
	{
		// From http://www.w3.org/TR/xpath: 
		// If one object to be compared is a node-set and the other is a 
		// string, then the comparison will be true if and only if there 
		// is a node in the node-set such that the result of performing 
		// the comparison on the string-value of the node and the other 
		// string is true. 
		theResult = doCompareString(
				theLHS.nodeset(),
				getStringFromNodeFunction(executionContext),
				theRHS,
				theStringCompareFunction,
				executionContext);
	}
	else if (theRHSType != XObject::eTypeUnknown)
	{
		assert(false);
	}

	return theResult;
}



inline bool
equalNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				equalsDOMString(),
				DoubleSupport::equalFunction(),
				executionContext);
}



inline bool
notEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				notEqualsDOMString(),
				DoubleSupport::notEqualFunction(),
				executionContext);
}



inline bool
lessThanNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				lessThanDOMString(),
				DoubleSupport::lessThanFunction(),
				executionContext);
}



inline bool
lessThanOrEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				lessThanOrEqualDOMString(),
				DoubleSupport::lessThanOrEqualFunction(),
				executionContext);
}



inline bool
greaterThanNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				greaterThanDOMString(),
				DoubleSupport::greaterThanFunction(),
				executionContext);
}



inline bool
greaterThanOrEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathExecutionContext&	executionContext)
{
	return compareNodeSets(
				theLHS,
				theRHS,
				theRHSType,
				greaterThanOrEqualDOMString(),
				DoubleSupport::greaterThanOrEqualFunction(),
				executionContext);
}



bool
XObject::equals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return true;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeUnknown)
		{
			return this == &theRHS ? true : false;
		}
		else if (theLHSType == eTypeNull)
		{
			return theRHS.getType() == eTypeNull ? true : false;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return equalNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else
		{
			const eObjectType	theRHSType = theRHS.getType();

			if (theRHSType == eTypeNodeSet)
			{
				return equalNodeSet(theRHS, *this, theLHSType, executionContext);
			}
			else
			{
				if (theLHSType == eTypeBoolean || theRHSType == eTypeBoolean)
				{
					return boolean() == theRHS.boolean();
				}
				else if (theLHSType == eTypeNumber || theRHSType == eTypeNumber)
				{
					return DoubleSupport::equal(num(), theRHS.num());
				}
				else
				{
					return str() == theRHS.str();
				}
			}
		}
	}
}



bool
XObject::notEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return false;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeUnknown)
		{
			return this == &theRHS ? false : true;
		}
		else if (theLHSType == eTypeNull)
		{
			return theRHS.getType() == eTypeNull ? false : true;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return notEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else
		{
			const eObjectType	theRHSType = theRHS.getType();

			if (theRHSType == eTypeNodeSet)
			{
				return notEqualNodeSet(theRHS, *this, theLHSType, executionContext);
			}
			else
			{
				if (theLHSType == eTypeBoolean || theRHSType == eTypeBoolean)
				{
					return boolean() != theRHS.boolean();
				}
				else if (theLHSType == eTypeNumber || theRHSType == eTypeNumber)
				{
					return DoubleSupport::notEqual(num(), theRHS.num());
				}
				else
				{
					return str() != theRHS.str();
				}
			}
		}
	}
}



bool
XObject::lessThan(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return false;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
		{
			return false;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return lessThanNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return greaterThanNodeSet(theRHS, *this, theLHSType, executionContext);
		}
		else
		{
			return DoubleSupport::lessThan(num(), theRHS.num());
		}
	}
}



bool
XObject::lessThanOrEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return false;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
		{
			return false;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return lessThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return greaterThanOrEqualNodeSet(theRHS, *this, theLHSType, executionContext);
		}
		else
		{
			return DoubleSupport::lessThanOrEqual(num(), theRHS.num());
		}
	}
}



bool
XObject::greaterThan(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return false;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
		{
			return false;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return greaterThanNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return lessThanNodeSet(theRHS, *this, theLHSType, executionContext);
		}
		else
		{
			return DoubleSupport::greaterThan(num(), theRHS.num());
		}
	}
}



bool
XObject::greaterThanOrEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const
{
	if (this == &theRHS)
	{
		return false;
	}
	else
	{
		const eObjectType	theLHSType = getType();

		if (theLHSType == eTypeNull || theLHSType == eTypeUnknown)
		{
			return false;
		}
		else if (theLHSType == eTypeNodeSet)
		{
			return greaterThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), executionContext);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return lessThanOrEqualNodeSet(theRHS, *this, theLHSType, executionContext);
		}
		else
		{
			return DoubleSupport::greaterThanOrEqual(num(), theRHS.num());
		}
	}
}



XObject::XObjectException::XObjectException(
				const XalanDOMString&	message,
				const XalanNode*		styleNode,
				const XalanDOMString&	theType) :
	XalanXPathException(message, styleNode, theType)
{
}



XObject::XObjectException::~XObjectException()
{
}



XObject::XObjectInvalidConversionException::XObjectInvalidConversionException(
				const XalanDOMString&	fromType,
				const XalanDOMString&	toType) :
	XObjectException(
			formatErrorString(fromType, toType),
			0,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("XObjectInvalidConversionException"))),
	m_fromType(fromType),
	m_toType(toType)
{
}



XObject::XObjectInvalidConversionException::~XObjectInvalidConversionException()
{
}



const XalanDOMString
XObject::XObjectInvalidConversionException::formatErrorString(
				const XalanDOMString&	fromType,
				const XalanDOMString&	toType)
{
	return XalanDOMString(XALAN_STATIC_UCODE_STRING("Cannot convert a ")) +
		   fromType +
		   XalanDOMString(XALAN_STATIC_UCODE_STRING(" to a ")) +
		   toType +
		   XalanDOMString(XALAN_STATIC_UCODE_STRING("."));
}



XALAN_CPP_NAMESPACE_END
