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
#if !defined(XERCESWRAPPERTOXALANNODEMAP_HEADER_GUARD_1357924680)
#define XERCESWRAPPERTOXALANNODEMAP_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if defined(XALAN_USE_HASH_MAP)
#include <hash_map>
#else
#include <map>
#endif



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesWrapperToXalanNodeMap
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef map<const DOMNodeType*, XalanNode*, less<const DOMNodeType*> >	XercesNodeMapType;
#else
#if defined(XALAN_USE_HASH_MAP)
	typedef std::hash_map<const DOMNodeType*, XalanNode*>	XercesNodeMapType;
#else
	typedef std::map<const DOMNodeType*, XalanNode*>	XercesNodeMapType;
#endif
#endif


	XercesWrapperToXalanNodeMap();

	~XercesWrapperToXalanNodeMap();

	void
	addAssociation(
			const DOMNodeType*	theXercesNode,
			XalanNode*			theXalanNode);

	void
	clear();

	XalanNode*
	getNode(const DOMNodeType*	theXercesNode) const;

private:

	XercesNodeMapType	m_xercesMap;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESWRAPPERTOXALANNODEMAP_HEADER_GUARD_1357924680)
