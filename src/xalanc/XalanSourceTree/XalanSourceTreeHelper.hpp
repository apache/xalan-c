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
#if !defined(XALANSOURCETREEHELPER_HEADER_GUARD_1357924680)
#define XALANSOURCETREEHELPER_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;
class XalanSourceTreeComment;
class XalanSourceTreeDocument;
class XalanSourceTreeDocumentFragment;
class XalanSourceTreeElement;
class XalanSourceTreeProcessingInstruction;
class XalanSourceTreeText;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeHelper
{
public:

	static XalanNode*
	getLastSibling(XalanNode*	theNode);

	static void
	appendSibling(
			XalanSourceTreeDocument*	theDocument,
			XalanNode*&					theNextSiblingSlot,
			XalanNode*					theNewSibling);

	static void
	appendSibling(
			XalanSourceTreeComment*		theNode,
			XalanNode*&					theNextSiblingSlot,
			XalanNode*					theNewSibling);

	static void
	appendSibling(
			XalanSourceTreeElement*		theNode,
			XalanNode*&					theNextSiblingSlot,
			XalanNode*					theNewSibling);

	static void
	appendSibling(
			XalanSourceTreeProcessingInstruction*	theNode,
			XalanNode*&								theNextSiblingSlot,
			XalanNode*								theNewSibling);

	static void
	appendSibling(
			XalanSourceTreeText*	theNode,
			XalanNode*&				theNextSiblingSlot,
			XalanNode*				theNewSibling);

	// New appendSibling interfaces...
	static void
	appendSibling(
			XalanNode*					theLastSibling,
			XalanSourceTreeComment*		theNewLastSibling);

	static void
	appendSibling(
			XalanNode*					theLastSibling,
			XalanSourceTreeElement*		theNewLastSibling);

	static void
	appendSibling(
			XalanNode*								theLastSibling,
			XalanSourceTreeProcessingInstruction*	theNewLastSibling);

	static void
	appendSibling(
			XalanNode*				theLastSibling,
			XalanSourceTreeText*	theNewLastSibling);


	static void
	appendSiblingToChild(
			XalanSourceTreeElement*		theOwnerElement,
			XalanNode*&					theFirstChildSlot,
			XalanSourceTreeComment*		theNewSibling);


	static void
	appendSiblingToChild(
			XalanSourceTreeElement*		theOwnerElement,
			XalanNode*&					theFirstChildSlot,
			XalanSourceTreeElement*		theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeElement*					theOwnerElement,
			XalanNode*&								theFirstChildSlot,
			XalanSourceTreeProcessingInstruction*	theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeElement*		theOwnerElement,
			XalanNode*&					theFirstChildSlot,
			XalanSourceTreeText*		theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeDocumentFragment*	theOwnerDocumentFragment,
			XalanNode*&							theFirstChildSlot,
			XalanSourceTreeComment*				theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeDocumentFragment*	theOwnerDocumentFragment,
			XalanNode*&							theFirstChildSlot,
			XalanSourceTreeElement*				theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeDocumentFragment*		theOwnerDocumentFragment,
			XalanNode*&								theFirstChildSlot,
			XalanSourceTreeProcessingInstruction*	theNewSibling);

	static void
	appendSiblingToChild(
			XalanSourceTreeDocumentFragment*	theOwnerDocumentFragment,
			XalanNode*&							theFirstChildSlot,
			XalanSourceTreeText*				theNewSibling);

private:

	// Not implemented...
	XalanSourceTreeHelper();

	XalanSourceTreeHelper(const XalanSourceTreeHelper&);
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEHELPER_HEADER_GUARD_1357924680)
