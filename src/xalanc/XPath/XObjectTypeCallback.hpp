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
#if !defined(XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680)
#define XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// $$$ ToDo: This is necessarh while XalanDOMString is a typedef...
#include <xalanc/XalanDOM/XalanDOMString.hpp>




XALAN_CPP_NAMESPACE_BEGIN



class MutableNodeRefList;
class NodeRefListBase;
class XalanDocumentFragment;
class XObject;



/**
 * The purpose of this class is to provide a way to get the "preferred" or
 * closest matching type for XObject-derived classes.  The idea is to pass a
 * reference to an instance of this class to an XObject instance.  It will
 * then call back to the member function that most closely matches its type.
 * This makes it possible to get preferred typing information without doing
 * RTTI or other such type introspections.
 */
class XALAN_XPATH_EXPORT XObjectTypeCallback
{
public:

	XObjectTypeCallback(MemoryManagerType& theManager);

	virtual
	~XObjectTypeCallback();

	/**
	 * Call back the XObject with a numeric value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Number(const XObject&	theXObject,
		   double			theValue) = 0;

	/**
	 * Call back the XObject with a boolean value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Boolean(const XObject&	theXObject,
		    bool			theValue) = 0;

	/**
	 * Call back the XObject with a string value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	String(const XObject&			theXObject,
		   const XalanDOMString&	theValue) = 0;

	/**
	 * Call back the XObject with a result tree fragment value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	ResultTreeFragment(const XObject&				theXObject,
					   const XalanDocumentFragment&	theValue) = 0;

	/**
	 * Call back the XObject with a result tree fragment value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	ResultTreeFragment(const XObject&			theXObject,
					   XalanDocumentFragment&	theValue) = 0;

	/**
	 * Call back the XObject with a node set value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	NodeSet(const XObject&			theXObject,
			const NodeRefListBase&	theValue) = 0;

	/**
	 * Call back the XObject with an unknown value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Unknown(const XObject&			theObject,
			const XalanDOMString&	theName) = 0;

	/**
	 * Call back the XObject with a null value.
	 *
	 * @param theXObject
	 * @param theValue
	 */
	virtual void
	Null(const XObject&		theObject) = 0;

    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }
private:
    MemoryManagerType& m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECT_TYPE_CALLBACK_HEADER_GUARD_1357924680
