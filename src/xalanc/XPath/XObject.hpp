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
#if !defined(XOBJECT_HEADER_GUARD_1357924680)
#define XOBJECT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/FormatterListener.hpp>
#include <xalanc/PlatformSupport/XalanReferenceCountedObject.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanXPathException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class MutableNodeRefList;
class NodeRefListBase;
class XalanDocumentFragment;
class XObjectFactory;
class XObjectTypeCallback;
class XPathExecutionContext;



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XObject : protected XalanReferenceCountedObject
{
public:

	friend class XObjectPtr;

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
						  // These next types are implementation-specific, and
						  // are never returned by getType().
						  eTypeStringReference = 8,
						  eTypeStringAdapter = 9,
						  eTypeStringCached = 10,
						  eTypeXTokenNumberAdapter = 11,
						  eTypeXTokenStringAdapter = 12,
						  eTypeNodeSetNodeProxy = 13,
						  eUnknown
						};

	/**
	 * Perform static initialization.  See class XPathInit.
	 */
	static void
	initialize(MemoryManagerType& theManager);

	/**
	 * Perform static shut down.  See class XPathInit.
	 */
	static void
	terminate();

	/**
	 * Create an XObject.
	 *
	 * @param theObjectType The enum for the type of the object.
	 */
	XObject(eObjectType		theObjectType);

	XObject(const XObject&	source);


    /**
     * Given a request type, return the equivalent string. 
     * For diagnostic purposes.
     *
     * @return string for type
     */
    virtual const XalanDOMString&
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
	virtual const XalanDOMString&
	str() const;

	typedef void (FormatterListener::*MemberFunctionPtr)(const XMLCh* const, const unsigned int);

	/**
	 * Send the string value to a FormatterListener instance.
	 *
	 * @param formatterListener The FormatterListener instance
	 * @param function A pointer to the member function of FormatterListener to call
	 * 
	 */
	virtual void
	str(
  			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const = 0;

	/**
	 * Get the length of the string value of the instance..
	 *
	 * @return The length of the string value
	 */
	virtual double
	stringLength() const = 0;

	/**
	 * Append the string value directly a string
	 *
	 * @param theBuffer The buffer for the data
	 */
	virtual void
	str(XalanDOMString&	theBuffer) const;

	/**
	 * Cast result object to a result tree fragment.
	 *
	 * @return result tree fragment
	 */
	virtual const XalanDocumentFragment&
	rtree() const;

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
	equals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two objects are functionally not equal.
	 *
	 * @param theRHS object to compare
	 * @param executionContext the current execution context
	 * @return true if they are equal
	 */
	bool
	notEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one object is less than the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	lessThan(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one object is less than or equal
	 * the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	lessThanOrEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one object is greater than the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	greaterThan(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one object is greater than or equal
	 * the other.
	 *
	 * @param theRHS object to compare
	 * @return true if they are equal
	 */
	bool
	greaterThanOrEquals(
			const XObject&			theRHS,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell what kind of class this is.
	 *
	 * @return numeric type value
	 */
	eObjectType
	getType() const
	{
		return m_objectType;
	}

	/**
	 * Static conversion function.
	 *
	 * @return bool value
	 */
	static bool
	boolean(double	theNumber)
	{
		return !DoubleSupport::isNaN(theNumber) && !DoubleSupport::equal(theNumber, 0.0);
	}

	/**
	 * Static conversion function.
	 *
	 * @return bool value
	 */
	static bool
	boolean(const XalanDOMString&	theString)
	{
		return theString.length() == 0 ? false : true;
	}

	/**
	 * Static conversion function.
	 *
	 * @return bool value
	 */
	static bool
	boolean(const NodeRefListBase&	theNodeList)
	{
		return theNodeList.getLength() == 0 ? false : true;
	}

	/**
	 * Static conversion function.
	 *
	 * @return The string value of the number
	 */
	static const XalanDOMString&
	string(bool		theBool)
	{
		return theBool == true ? s_trueString : s_falseString;
	}

	static void
	string(
			bool				theBool,
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function)
	{
		if(theBool == true)
		{
			(formatterListener.*function)(s_trueString.c_str(), s_trueString.length());
		}
		else
		{
			(formatterListener.*function)(s_falseString.c_str(), s_falseString.length());
		}
	}

	/**
	 * Static conversion function.
	 *
	 * @return The string value of the number
	 */
	static void
	string(
			double				theNumber,
			XalanDOMString&		theString)
	{
		DoubleToDOMString(theNumber, theString);
	}

	static void
	string(
			double				theNumber,
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function)
	{
		DOMStringHelper::DoubleToCharacters(theNumber, formatterListener, function);
	}

	/**
	 * Static conversion function.
	 *
	 * @return The string value of the node
	 */
	static void
	string(
			const XalanNode&	theNode,
			XalanDOMString&		theString)
	{
		DOMServices::getNodeData(theNode, theString);
	}

	static void
	string(
			const XalanNode&	theNode,
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function)
	{
		DOMServices::getNodeData(theNode, formatterListener, function);
	}

	/**
	 * Static conversion function.
	 *
	 * @return The string value of the node list
	 */
	static void
	string(
			const NodeRefListBase&	theNodeList,
			XalanDOMString&			theString)
	{
		if (theNodeList.getLength() > 0)
		{
			assert(theNodeList.item(0) != 0);

			string(*theNodeList.item(0), theString);
		}
	}

	static void
	string(
			const NodeRefListBase&	theNodeList,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function)
	{
		if (theNodeList.getLength() > 0)
		{
			assert(theNodeList.item(0) != 0);

			DOMServices::getNodeData(*theNodeList.item(0), formatterListener, function);
		}
	}

	/**
	 * Static conversion function.
	 *
	 * @return bool value
	 */
	static double
	number(bool		theBoolean)
	{
		return theBoolean == true ? 1.0 : 0.0;
	}

	static double
	number(const XalanDOMString&	theString,
            MemoryManagerType&      theManager)
	{
		return DoubleSupport::toDouble(theString, theManager);
	}

	/**
	 * Static conversion function.
	 *
	 * @return The number value of the node list
	 */
	static double
	number(
			XPathExecutionContext&	executionContext,
			const NodeRefListBase&	theNodeList);

	/**
	 * Static conversion function.
	 *
	 * @return The number value of the node
	 */
	static double
	number(
			XPathExecutionContext&	executionContext,
			const XalanNode&		theNode);


	// All XObject instances are controlled by an instance of an XObjectFactory.
	friend class XObjectFactory;

	void
	setFactory(XObjectFactory*	theFactory)
	{
		m_factory = theFactory;
	}

	// Base class for all XObject exceptions...
	class XObjectException : public XalanXPathException
	{
	public:

		explicit
		XObjectException(
				const XalanDOMString&	message = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
                MemoryManagerType& theManager = XalanMemMgrs::getDummyMemMgr());

        XObjectException( const XObjectException& other) ;

		virtual
		~XObjectException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}
	private:

		static const XalanDOMChar	m_type[];
	};

	class XObjectInvalidConversionException : public XObjectException
	{
	public:

		XObjectInvalidConversionException(
				                eObjectType	fromType,
				                eObjectType	toType);


        XObjectInvalidConversionException( const XObjectInvalidConversionException& other);

		virtual
		~XObjectInvalidConversionException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}


	private:

		static const XalanDOMChar	m_type[];

		static const XalanDOMString&
		formatErrorString(
				const XalanDOMString&	fromType,
				const XalanDOMString&	toType,
                XalanDOMString&         theResult);


        const eObjectType       m_from;

        const eObjectType       m_to;
	};

protected:

	/**
	 * Tell what kind of class this is.
	 *
	 * @return numeric type value
	 */
	virtual eObjectType
	getRealType() const;

	virtual void 
	referenced();

	virtual void 
	dereferenced();

	virtual
	~XObject();

    static const XalanDOMString     s_emptyString;

    static const XalanDOMString&    s_booleanString;

    static const XalanDOMString&    s_falseString;

    static const XalanDOMString&    s_nodesetString;

    static const XalanDOMString&    s_numberString;

    static const XalanDOMString&    s_resultTreeFragmentString;

    static const XalanDOMString&    s_stringString;

    static const XalanDOMString&    s_trueString;

private:

	// Not implemented...
	XObject&
	operator=(const XObject&);

	const eObjectType	m_objectType;

	XObjectFactory*		m_factory;
};



/**
 * Class to hold XObjectPtr return types.
 */
class XALAN_XPATH_EXPORT XObjectPtr
{
public:

	friend bool operator==(const XObjectPtr&, const XObjectPtr&);
	friend bool operator<(const XObjectPtr&, const XObjectPtr&);

	/**
	 * Create an XObjectPtr.
	 */
	explicit
	XObjectPtr(XObject*		theXObject = 0) :
		m_xobjectPtr(theXObject)
	{
		XalanReferenceCountedObject::addReference(theXObject);
	}
	
	XObjectPtr(const XObjectPtr&	theSource) :
		m_xobjectPtr(theSource.m_xobjectPtr)
	{ 
		XalanReferenceCountedObject::addReference(m_xobjectPtr);
	};

	XObjectPtr&
	operator=(const XObjectPtr&		theRHS)
	{ 
		if (m_xobjectPtr != theRHS.m_xobjectPtr)
		{
			XalanReferenceCountedObject::removeReference(m_xobjectPtr);

			m_xobjectPtr = theRHS.m_xobjectPtr;

			XalanReferenceCountedObject::addReference(m_xobjectPtr);
		}

		return *this;
	}

	~XObjectPtr()
	{
		XalanReferenceCountedObject::removeReference(m_xobjectPtr);
	};	

	void
	release()
	{
		XObject* const	xobjectPtr = m_xobjectPtr;

		m_xobjectPtr = 0;

		XalanReferenceCountedObject::removeReference(xobjectPtr);
	}

	bool
	null() const
	{
		return m_xobjectPtr == 0 ? true : false;
	}

	const XObject&
	operator*() const
	{
		return *m_xobjectPtr;
	};

	XObject&
	operator*() 
	{
		return *m_xobjectPtr;
	};

	const XObject*
	operator->() const
	{
		return m_xobjectPtr;
	};

	XObject*
	operator->()
	{
		return m_xobjectPtr;
	};

	const XObject*
	get() const
	{
		return m_xobjectPtr;
	};

	XObject*
	get()
	{
		return m_xobjectPtr;
	};

private:

	XObject*	m_xobjectPtr;
};



inline bool
operator==(
			const XObjectPtr&	theLHS,
			const XObjectPtr&	theRHS)
{
	return theLHS.m_xobjectPtr == theRHS.m_xobjectPtr;
}



inline bool
operator!=(
			const XObjectPtr&	theLHS,
			const XObjectPtr&	theRHS)
{
	return !(theLHS == theRHS);
}



inline bool
operator<(
			const XObjectPtr&	theLHS,
			const XObjectPtr&	theRHS)
{
	return theLHS.m_xobjectPtr < theRHS.m_xobjectPtr;
}



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECT_HEADER_GUARD_1357924680
