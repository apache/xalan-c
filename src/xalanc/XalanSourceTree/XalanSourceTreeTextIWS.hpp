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
#if !defined(XALANSOURCETREETEXTIWS_HEADER_GUARD_1357924680)
#define XALANSOURCETREETEXTIWS_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeText.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeTextIWS : public XalanSourceTreeText
{
public:

	/**
	 * Constructor.
	 *
	 * @param theData The text data of the node
	 * @param theParentNode The parent node, if any.
	 * @param thePreviousSibling The previous sibling, if any.
	 * @param theNextSibling The next sibling, if any.
	 * @param theIndex The document-order index of the node.
	 */
	XalanSourceTreeTextIWS(
			const XalanDOMString&	theData,
			XalanNode*				theParentNode = 0,
			XalanNode*				thePreviousSibling = 0,
			XalanNode*				theNextSibling = 0,
			IndexType				theIndex = 0);

	virtual
	~XalanSourceTreeTextIWS();


	virtual bool
	isIgnorableWhitespace() const;

protected:

	XalanSourceTreeTextIWS(
			const XalanSourceTreeTextIWS&	theSource,
			bool							/* deep */);

private:

	// Not implemented...
	XalanSourceTreeTextIWS&
	operator=(const XalanSourceTreeTextIWS&		theSource);

	bool
	operator==(const XalanSourceTreeTextIWS&	theRHS) const;

	// Data members...
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREETEXTIWS_HEADER_GUARD_1357924680)
