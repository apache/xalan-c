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
#if !defined(XERCESWRAPPERTYPES_HEADER_GUARD_1357924680)
#define XERCESWRAPPERTYPES_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



XALAN_DECLARE_XERCES_CLASS(DOMAttr)
XALAN_DECLARE_XERCES_CLASS(DOMCharacterData)
XALAN_DECLARE_XERCES_CLASS(DOMComment)
XALAN_DECLARE_XERCES_CLASS(DOMCDATASection)
XALAN_DECLARE_XERCES_CLASS(DOMDocument)
XALAN_DECLARE_XERCES_CLASS(DOMDocumentFragment)
XALAN_DECLARE_XERCES_CLASS(DOMImplementation)
XALAN_DECLARE_XERCES_CLASS(DOMElement)
XALAN_DECLARE_XERCES_CLASS(DOMEntity)
XALAN_DECLARE_XERCES_CLASS(DOMEntityReference)
XALAN_DECLARE_XERCES_CLASS(DOMNode)
XALAN_DECLARE_XERCES_CLASS(DOMText)
XALAN_DECLARE_XERCES_CLASS(DOMNamedNodeMap)
XALAN_DECLARE_XERCES_CLASS(DOMNodeList)
XALAN_DECLARE_XERCES_CLASS(DOMNotation)
XALAN_DECLARE_XERCES_CLASS(DOMProcessingInstruction)
XALAN_DECLARE_XERCES_CLASS(DOMText)
XALAN_DECLARE_XERCES_CLASS(DOMDocumentType)
XALAN_DECLARE_XERCES_CLASS(DOMException)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMAttr				DOMAttrType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMCharacterData	DOMCharacterDataType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMComment			DOMCommentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMCDATASection		DOMCDATASectionType;
// These next two have an extra '_' because they collide without it...
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocumentType		DOMDocumentType_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument			DOMDocument_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocumentFragment	DOMDocumentFragmentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation	DOMImplementationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMElement			DOMElementType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMEntity			DOMEntityType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMEntityReference	DOMEntityReferenceType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMNode				DOMNodeType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMText				DOMTextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMNamedNodeMap		DOMNamedNodeMapType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList			DOMNodeListType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMNotation			DOMNotationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMProcessingInstruction	DOMProcessingInstructionType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMText				DOMTextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMException			DOMExceptionType;
typedef XMLSize_t											XMLSizeType;



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESWRAPPERTYPES_HEADER_GUARD_1357924680)
