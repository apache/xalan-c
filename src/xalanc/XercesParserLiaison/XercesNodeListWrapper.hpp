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
#if !defined(XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680)
#define XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesNodeListWrapper : public XalanNodeList
{
public:

	XercesNodeListWrapper(
			const DOMNodeListType*			theXercesNodeList,
			const XercesWrapperNavigator&	theNavigator);

	XercesNodeListWrapper(const XercesNodeListWrapper&	theSource);

	virtual
	~XercesNodeListWrapper();

	bool
	operator==(const XercesNodeListWrapper& 	theRHS) const
	{
		return m_xercesNodeList == theRHS.m_xercesNodeList ? true : false;
	}

	/** @name Get functions. */
	//@{
	/**
	 * Returns the <code>index</code>th item in the collection. 
	 *
	 * If <code>index</code> is greater than or equal to the number of nodes in 
	 * the list, this returns <code>null</code>.
	 *
	 * @param index Index into the collection.
	 * @return The node at the <code>index</code>th position in the 
	 *	 <code>NodeList</code>, or <code>null</code> if that is not a valid 
	 *	 index.
	 */
	virtual XalanNode*
	item(unsigned int	index) const;

	/**
	 * Returns the number of nodes in the list. 
	 *
	 * The range of valid child node indices is 0 to <code>length-1</code> inclusive. 
	 */
	virtual unsigned int
	getLength() const;

private:

	// Not implemented...
	XercesNodeListWrapper&
	operator=(const XercesNodeListWrapper&	theRHS);


	// Data members...
	const DOMNodeListType*			m_xercesNodeList;

	const XercesWrapperNavigator&	m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESNODELISTWRAPPER_HEADER_GUARD_1357924680)
