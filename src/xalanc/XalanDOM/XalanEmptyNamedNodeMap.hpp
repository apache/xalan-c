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
#if !defined(XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680)
#define XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/*
 * <meta name="usage" content="experimental"/>
 *
 * Class to implement a "faux" (always empty) named node map.
 * The map is read-only!
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanEmptyNamedNodeMap : public XalanNamedNodeMap
{
public:

	XalanEmptyNamedNodeMap();

	virtual
	~XalanEmptyNamedNodeMap();

	XalanEmptyNamedNodeMap(const XalanEmptyNamedNodeMap&	theSource);

	XalanEmptyNamedNodeMap&
	operator=(const XalanEmptyNamedNodeMap&		theSource);

	bool
	operator==(const XalanEmptyNamedNodeMap& 	theRHS) const;


	virtual XalanNode*
	setNamedItem(XalanNode* 	arg);

	virtual XalanNode*
	item(unsigned int	index) const;

	virtual XalanNode*
	getNamedItem(const XalanDOMString& 	name) const;

	virtual unsigned int
	getLength() const;

	virtual XalanNode*
	removeNamedItem(const XalanDOMString&	name);

	virtual XalanNode*
	getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanNode*
	setNamedItemNS(XalanNode*	arg);

	virtual XalanNode*
	removeNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName);

private:
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANEMPTYNAMEDNODEMAP_HEADER_GUARD_1357924680)
