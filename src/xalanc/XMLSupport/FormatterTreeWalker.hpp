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
#if !defined(FORMATTERTREEWALKER_HEADER_GUARD_1357924680)
#define FORMATTERTREEWALKER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <xalanc/DOMSupport/TreeWalker.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;



class XALAN_XMLSUPPORT_EXPORT FormatterTreeWalker : public TreeWalker
{
public:

	/**
	 * Construct a FormatterTreeWalker instance.
	 *
	 * @param formatterListener implemention of the FormatterListener operation
	 *                          (toXMLString, digest, ...)
	 */
	FormatterTreeWalker(FormatterListener&	formatterListener,
                        MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~FormatterTreeWalker();

protected:

	virtual bool
	startNode(const XalanNode*	node);

	virtual bool
	startNode(XalanNode*	node);

	virtual bool
	endNode(const XalanNode*	node);

	virtual bool
	endNode(XalanNode*	node);

private:

	FormatterListener&	m_formatterListener;

    MemoryManagerType&  m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTREEWALKER_HEADER_GUARD_1357924680
