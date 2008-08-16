/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#if !defined(EXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define EXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>

XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;



class XalanDOMString;
class XalanNode;
class XalanText;



//
// An abstract class which provides support for execution.
//
class XALAN_PLATFORMSUPPORT_EXPORT ExecutionContext
{
public:

	ExecutionContext(MemoryManager&     m_memoryManager);

	virtual
	~ExecutionContext();

	/**
	 * Report an error and throw an exception.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the error occurred.  May be 0.
	 * @param locator A Locator to determine where the error occurred.  May be 0.
	 */
	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

	/**
	 * Report a warning
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the warning occurred.  May be 0.
	 * @param locator A Locator to determine where the warning occurred.  May be 0.
	 */
	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

	/**
	 * Output a message.
	 * 
	 * @param msg The text of the message.
	 * @param sourceNode The source node where the message occurred.  May be 0.
	 * @param locator A Locator to determine where the message occurred.  May be 0.
	 */
	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const = 0;

    bool
    hasPreserveOrStripSpaceConditions() const
    {
        return m_hasPreserveOrStripConditions;
    }

    /**
     * Determine if a text node should be stripped from the source tree,
     * as if it weren't there.
     *
     * @param textNode text node from the source tree
     * @return true if the text node should be stripped
     */
    virtual bool
    shouldStripSourceNode(const XalanText&  node) = 0;

    MemoryManager&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    MemoryManager&
    getExceptionMemoryManager()
    {
        return *m_memoryManager.getExceptionMemoryManager();
    }

protected:

    MemoryManager&  m_memoryManager;

    bool            m_hasPreserveOrStripConditions;
};



XALAN_CPP_NAMESPACE_END



#endif	// EXECUTIONCONTEXT_HEADER_GUARD_1357924680
