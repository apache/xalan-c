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
#if !defined(XSTRINGBASE_HEADER_GUARD_1357924680)
#define XSTRINGBASE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XObjectResultTreeFragProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XPathEnvSupport;



class XALAN_XPATH_EXPORT XStringBase : public XObject
{
public:

	typedef XObject		ParentType;

	/**
	 * Construct an XStringBase object from a string.
	 * 
	 */
	XStringBase(MemoryManagerType& theManager);

	XStringBase(const XStringBase&	source,
                MemoryManagerType&  theManager);

    MemoryManagerType&
    getMemoryManager()const
    {
        return m_resultTreeFrag.getMemoryManager();
    }

	virtual
	~XStringBase();


	// These methods are inherited from XObject ...

	virtual const XalanDOMString&
	getTypeString() const;
  
	virtual double
	num() const;

	virtual bool
	boolean() const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::str;
#endif

	virtual const XalanDOMString&
	str() const = 0;
  
	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const = 0;

	virtual double
	stringLength() const = 0;

	virtual const XalanDocumentFragment&
	rtree() const;

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

private:
    // Not implemented
    XStringBase(const XStringBase&	source);
    XStringBase();

	mutable double						m_cachedNumberValue;

	mutable XObjectResultTreeFragProxy	m_resultTreeFrag;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSTRINGBASE_HEADER_GUARD_1357924680
