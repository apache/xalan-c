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
// Class header file.
#include "XObject.hpp"



#include <stdexcept>



#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "NodeRefListBase.hpp"
#include "XObjectCompare.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathException.hpp"
#include "XPathSupport.hpp"



XObject::XObject(
			XPathEnvSupport*	envSupport,
			XPathSupport*		support) :
	FactoryObject(),
	m_envSupport(envSupport),
	m_support(support)
{
}



XObject::XObject(const XObject&		source) :
	FactoryObject(),
	m_envSupport(source.m_envSupport),
	m_support(source.m_support)
{
}



XObject::~XObject()
{
}



void
XObject::error(const XalanDOMString&	msg) const
{
	const bool	shouldThrow =
		m_envSupport->problem(XPathEnvSupport::eXPATHProcessor,
							  XPathEnvSupport::eError,
#if defined(XALAN_OLD_STYLE_CASTS)
							  (XalanNode*)0,
#else
							  static_cast<XalanNode*>(0),
#endif
							  0,
							  msg,
							  0,
							  0);

	if(shouldThrow == true)
	{
		throw XPathException(msg);
	}
}



XPathSupport*
getSupport(const XObject&			theXObject,
		   const NodeRefListBase&	theNodeList)
{
	XPathSupport*	theSupport = theXObject.getSupport();

	if (theSupport == 0)
	{
		theNodeList.getSupport();
	}

	return theSupport;
}




const XalanDOMString
getStringFromNode(
			const XalanNode&	theNode,
			XPathSupport*		theXPathSupport)
{
	if (theXPathSupport != 0)
	{
		return theXPathSupport->getNodeData(theNode);
	}
	else
	{
		return DOMServices::getNodeData(theNode);
	}
}



struct
getStringFromNodeFunction
{
	getStringFromNodeFunction(XPathSupport*	theSupport = 0) :
		m_support(theSupport)
	{
	}

	const XalanDOMString
	operator()(const XalanNode&		theNode) const
	{
		return getStringFromNode(theNode, m_support);
	}

private:

	XPathSupport* const		m_support;
};



double
getNumberFromNode(
			const XalanNode&	theNode,
			XPathSupport*		theXPathSupport)
{
	return DOMStringToDouble(getStringFromNode(theNode, theXPathSupport));
}



struct
getNumberFromNodeFunction
{
	getNumberFromNodeFunction(XPathSupport*	theSupport = 0) :
		m_support(theSupport)
	{
	}

	double
	operator()(const XalanNode&		theNode) const
	{
		return getNumberFromNode(theNode, m_support);
	}

private:

	XPathSupport* const		m_support;
};




template<class CompareFunction, class TypeFunction>
bool
doCompareNodeSets(
			const NodeRefListBase&	theLHSNodeSet,
			const NodeRefListBase&	theRHSNodeSet,
			const TypeFunction&		theTypeFunction,
			const CompareFunction&	theCompareFunction)
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
	bool				theResult = false;

	const unsigned int	len1 = theLHSNodeSet.getLength();
	const unsigned int	len2 = theRHSNodeSet.getLength();

	for(unsigned int i = 0; i < len1 && theResult == false; i++)
	{
		const XalanNode* const	theLHSNode = theLHSNodeSet.item(i);
		assert(theLHSNode != 0);

		const XalanDOMString	s1 =
				theTypeFunction(*theLHSNode);

		for(unsigned int k = 0; k < len2 && theResult == false; k++)
		{
			const XalanNode* const	theRHSNode = theLHSNodeSet.item(i);
			assert(theRHSNode != 0);

			const XalanDOMString	s2 =
					theTypeFunction(*theLHSNode);

			if(theCompareFunction(s1, s2) == true)
			{
				theResult = true;
			}
		}
	}

	return theResult;
}



template<class CompareFunction, class TypeFunction, class Type>
bool
doCompare(
			const NodeRefListBase&	theLHSNodeSet,
			const TypeFunction&		theTypeFunction,
			const Type&				theRHS,
			const CompareFunction&	theCompareFunction)
{
	bool				theResult = false;

	const unsigned int	len1 = theLHSNodeSet.getLength();

	for(unsigned int i = 0; i < len1 && theResult == false; i++)
	{
		const XalanNode* const	theLHSNode = theLHSNodeSet.item(i);
		assert(theLHSNode != 0);

		const Type	theLHS = theTypeFunction(*theLHSNode);

		if (theCompareFunction(theLHS, theRHS) == true)
		{
			theResult = true;
		}
	}

	return theResult;
}



bool
equalNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringEqualsFunction());

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

		theResult = DoubleSupport::equal(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::equalFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::equalFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringEqualsFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringEqualsFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
notEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringNotEqualsFunction());

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

		theResult = DoubleSupport::notEqual(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::notEqualFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::notEqualFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringNotEqualsFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringNotEqualsFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
lessThanNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringLessThanFunction());

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

		theResult = DoubleSupport::lessThan(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::lessThanFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::lessThanFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringLessThanFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringLessThanFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
lessThanOrEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringLessThanOrEqualFunction());

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

		theResult = DoubleSupport::lessThanOrEqual(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::lessThanOrEqualFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::lessThanOrEqualFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringLessThanOrEqualFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringLessThanOrEqualFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
greaterThanNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringGreaterThanFunction());

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

		theResult = DoubleSupport::greaterThan(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::greaterThanFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::greaterThanFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringGreaterThanFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringGreaterThanFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
greaterThanOrEqualNodeSet(
			const XObject&			theLHS,
			const XObject&			theRHS,
			XObject::eObjectType	theRHSType,
			XPathSupport*			theXPathSupport)
{
	bool	theResult = false;

	XPathSupport* const		theSupport = theXPathSupport != 0 ?
								theXPathSupport :
								theRHS.getSupport();

	if(theRHSType == XObject::eTypeNodeSet)
	{
		// Compare as node sets...
		theResult = doCompareNodeSets(
				theLHS.nodeset(),
				theRHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				DOMStringGreaterThanOrEqualFunction());

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

		theResult = DoubleSupport::greaterThanOrEqual(num1, theRHS.num());
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

		theResult = doCompare(
				theLHS.nodeset(),
				getNumberFromNodeFunction(theSupport),
				theRHS.num(),
				DoubleSupport::greaterThanOrEqualFunction());
	}
	else if(theRHSType == XObject::eTypeResultTreeFrag)
	{
		// hmmm... 
		const double	theRHSNumber = theRHS.num();

		if(DoubleSupport::isNaN(theRHSNumber) == false)
		{
			// Compare as number...
			theResult = doCompare(
					theLHS.nodeset(),
					getNumberFromNodeFunction(theSupport),
					theRHS.num(),
					DoubleSupport::greaterThanOrEqualFunction());
		}
		else
		{
			// Compare as string...
			theResult = doCompare(
					theLHS.nodeset(),
					getStringFromNodeFunction(theSupport),
					theRHS.str(),
					DOMStringGreaterThanOrEqualFunction());
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
		theResult = doCompare(
				theLHS.nodeset(),
				getStringFromNodeFunction(theSupport),
				theRHS.str(),
				DOMStringGreaterThanOrEqualFunction());
	}
	else
	{
		assert(false);
	}

	return theResult;
}



bool
XObject::equals(const XObject&	theRHS) const
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
			return equalNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return equalNodeSet(theRHS, *this, theLHSType, m_support);
		}
		else
		{
			const eObjectType	theRHSType = theRHS.getType();

			if (theLHSType == eTypeBoolean || theLHSType == eTypeBoolean)
			{
				return boolean() == theRHS.boolean();
			}
			else if (theLHSType == eTypeNumber || theLHSType == eTypeNumber)
			{
				return DoubleSupport::equal(num(), theRHS.num());
			}
			else
			{
				return ::equals(str(), theRHS.str());
			}
		}
	}
}



bool
XObject::notEquals(const XObject&	theRHS) const
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
			return notEqualNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return notEqualNodeSet(theRHS, *this, theLHSType, m_support);
		}
		else
		{
			const eObjectType	theRHSType = theRHS.getType();

			if (theLHSType == eTypeBoolean || theLHSType == eTypeBoolean)
			{
				return boolean() != theRHS.boolean();
			}
			else if (theLHSType == eTypeNumber || theLHSType == eTypeNumber)
			{
				return DoubleSupport::notEqual(num(), theRHS.num());
			}
			else
			{
				return !::equals(str(), theRHS.str());
			}
		}
	}
}



bool
XObject::lessThan(const XObject&	theRHS) const
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
			return lessThanNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return greaterThanOrEqualNodeSet(theRHS, *this, theLHSType, theRHS.getSupport());
		}
		else
		{
			return DoubleSupport::lessThan(num(), theRHS.num());
		}
	}
}



bool
XObject::lessThanOrEqual(const XObject&	theRHS) const
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
			return lessThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return greaterThanNodeSet(theRHS, *this, theLHSType, theRHS.getSupport());
		}
		else
		{
			return DoubleSupport::lessThanOrEqual(num(), theRHS.num());
		}
	}
}



bool
XObject::greaterThan(const XObject&	theRHS) const
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
			return greaterThanNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return lessThanOrEqualNodeSet(theRHS, *this, theLHSType, theRHS.getSupport());
		}
		else
		{
			return DoubleSupport::greaterThan(num(), theRHS.num());
		}
	}
}



bool
XObject::greaterThanOrEqual(const XObject&	theRHS) const
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
			return greaterThanOrEqualNodeSet(*this, theRHS, theRHS.getType(), m_support);
		}
		else if (theRHS.getType() == eTypeNodeSet)
		{
			return lessThanNodeSet(theRHS, *this, theLHSType, theRHS.getSupport());
		}
		else
		{
			return DoubleSupport::greaterThanOrEqual(num(), theRHS.num());
		}
	}
}
