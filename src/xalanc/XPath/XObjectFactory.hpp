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
#if !defined(XOBJECTFACTORY_HEADER_GUARD_1357924680)
#define XOBJECTFACTORY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <algorithm>
#include <cassert>


#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;
class MutableNodeRefList;
class NodeRefListBase;
class XObject;
class XObjectPtr;
class XToken;



/**
 * This class handles the creation of XObjects and manages their lifetime.
 */
class XALAN_XPATH_EXPORT XObjectFactory
{
public:

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;
	typedef XPathExecutionContext::GetAndReleaseCachedString		GetAndReleaseCachedString;


	XObjectFactory(MemoryManagerType& theManager);

	virtual
	~XObjectFactory();


    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }

	/**
	 * Return an XObject to the factory.
	 * 
	 * @param theXObject The XObject to be returned
	 * @return true if the object belongs to the factory, false if not.
	 */
	bool
	returnObject(XObject* 	theXObject)
	{
		return doReturnObject(theXObject);
	}

	/**
	 * Reset the instance.	This invalidates all existing instances created
	 * with this XObjectFactory.
	 */
	virtual void
	reset() = 0;

	/**
	 * Create a boolean XObject from a boolean value.
	 * 
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createBoolean(bool	theValue) = 0;

	/**
	 * Create a node set XObject from a node list.
	 * 
	 * @param theValue	value used to create object.  theValue will be owned by the new XObject.
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createNodeSet(BorrowReturnMutableNodeRefList& 	theValue) = 0;

	/**
	 * Create a node set XObject from a node.
	 * 
	 * @param theNOde value used to create object.
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createNodeSet(XalanNode* 	theValue) = 0;

	/**
	 * Create a null XObject.
	 * 
	 * @param theValue	value used to create object 
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createNull() = 0;

	/**
	 * Create a numeric XObject from a number.
	 * 
	 * @param theValue	value used to create object 
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createNumber(double 	theValue) = 0;

	/**
	 * Create a numeric XObject from an XToken.  The XToken
	 * instance must be inexistence for the lifetime of the
	 * object.
	 *
	 * @param theValue	value used to create object 
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createNumber(const XToken&	theValue) = 0;

	/**
	 * Create a string XObject from a string.
	 * 
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createString(const XalanDOMString&	theValue) = 0;

	/**
	 * Create a string XObject from a null-terminated array of characters.
	 * 
	 * @param theValue	a pointer to the array
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createString(const XalanDOMChar*	theValue) = 0;

	/**
	 * Create a string XObject from an array of characters.
	 * 
	 * @param theValue	a pointer to the array
	 * @paran theLength the length of the array
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createString(
			const XalanDOMChar* 	theValue,
			unsigned int			theLength) = 0;

	/**
	 * Create a string XObject from an XToken.  The XToken
	 * instance must be inexistence for the lifetime of the
	 * object.
	 *
	 * @param theValue	value used to create object 
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createString(const XToken&	theValue) = 0;

	/**
	 * Create a string XObject from a string.  The XObject
	 * will hold a reference to the supplied string, so
	 * the string must be in scope for the lifetime of
	 * the instance
	 *
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createStringReference(const XalanDOMString&		theValue) = 0;

	/**
	 * Create a string XObject that will adapt another XObject to
	 * behave like a string.  The XObject holds a reference to the
	 * other XObject.
	 *
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createStringAdapter(const XObjectPtr&	theValue) = 0;

	/**
	 * Create a string XObject from a cached XalanDOMString,
	 *
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createString(GetAndReleaseCachedString&		theValue) = 0;

	/**
	 * Create an "unknown" XObject from a string.
	 * 
	 * @param theValue	value used to create object  
	 * @return pointer to new object
	 */
	virtual const XObjectPtr
	createUnknown(const XalanDOMString& 	theValue) = 0;

	/**
	 *
	 * A public functor for use with stl algorithms.
	 *
	 */
#if defined(XALAN_NO_STD_NAMESPACE)
	struct DeleteXObjectFunctor : public unary_function<XObject*, void>
#else
	struct DeleteXObjectFunctor : public std::unary_function<XObject*, void>
#endif
	{
	public:

		DeleteXObjectFunctor(
			XObjectFactory& 	theFactoryInstance,
			bool				fInReset = false) :
			m_factoryInstance(theFactoryInstance),
			m_fInReset(fInReset)
		{
		}

		result_type
		operator()(argument_type	theXObject) const
		{
			if (m_fInReset == true)
			{
				m_factoryInstance.doReturnObject(
					theXObject,
					true);
			}
			else
			{
				m_factoryInstance.returnObject(theXObject);
			}
		}

	private:

		XObjectFactory& 	m_factoryInstance;

		const bool			m_fInReset;
	};

	friend struct DeleteXObjectFunctor;

protected:

	/**
	 * Return the actual implementation type of an XObject.
	 *
	 * @param theXObject the XObject
	 */
	XObject::eObjectType
	getRealType(const XObject&	theXObject) const
	{
		return theXObject.getRealType();
	}

	/**
	 * Delete a FactoryObject instance.
	 *
	 * @param theXObject the XObject instance to delete.
	 */
	void
	deleteObject(const XObject* 	theXObject) const
	{
        if( theXObject!= 0)
        {
            XObject* 	theTmpXObject = const_cast<XObject*>(theXObject);
            MemoryManagerType& theManager = const_cast<MemoryManagerType&>(m_memoryManager);

            theTmpXObject->~XObject();

            theManager.deallocate((void*)theTmpXObject);
        }
    
	}

	/**
	 * Return an XObject to the factory.
	 * 
	 * @param theXObject XObject to be returned
	 * @param fInReset true when called during reset().
	 */

	virtual bool
	doReturnObject(
			XObject*	theXObject,
			bool		fInReset = false) = 0;

private:

	// Not implemented...
	XObjectFactory(const XObjectFactory&);

	XObjectFactory&
	operator=(const XObjectFactory&);

	bool
	operator==(const XObjectFactory&) const;

    MemoryManagerType& m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECTFACTORY_HEADER_GUARD_1357924680
