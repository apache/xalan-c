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
#if !defined(XERCESBRIDGETYPES_HEADER_GUARD_1357924680)
#define XERCESBRIDGETYPES_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



XALAN_DECLARE_XERCES_CLASS(DOM_Attr)
XALAN_DECLARE_XERCES_CLASS(DOM_CharacterData)
XALAN_DECLARE_XERCES_CLASS(DOM_Comment)
XALAN_DECLARE_XERCES_CLASS(DOM_CDATASection)
XALAN_DECLARE_XERCES_CLASS(DOM_Document)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentFragment)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentType)
XALAN_DECLARE_XERCES_CLASS(DOM_DOMImplementation)
XALAN_DECLARE_XERCES_CLASS(DOM_Element)
XALAN_DECLARE_XERCES_CLASS(DOM_Entity)
XALAN_DECLARE_XERCES_CLASS(DOM_EntityReference)
XALAN_DECLARE_XERCES_CLASS(DOM_Node)
XALAN_DECLARE_XERCES_CLASS(DOM_Text)
XALAN_DECLARE_XERCES_CLASS(DOM_NamedNodeMap)
XALAN_DECLARE_XERCES_CLASS(DOM_NodeList)
XALAN_DECLARE_XERCES_CLASS(DOM_Notation)
XALAN_DECLARE_XERCES_CLASS(DOM_ProcessingInstruction)
XALAN_DECLARE_XERCES_CLASS(DOM_Text)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentType)
XALAN_DECLARE_XERCES_CLASS(DOM_DOMException)
XALAN_DECLARE_XERCES_CLASS(DOMString)
XALAN_DECLARE_XERCES_CLASS(AttrImpl)
XALAN_DECLARE_XERCES_CLASS(ElementImpl)
XALAN_DECLARE_XERCES_CLASS(NodeImpl)
XALAN_DECLARE_XERCES_CLASS(TextImpl)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr             DOM_AttrType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_CharacterData    DOM_CharacterDataType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Comment          DOM_CommentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_CDATASection     DOM_CDATASectionType;
// These next two have an extra '_' because they collide without it...
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DocumentType     DOM_DocumentType_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Document         DOM_Document_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DocumentFragment DOM_DocumentFragmentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DOMImplementation    DOM_DOMImplementationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element          DOM_ElementType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Entity           DOM_EntityType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_EntityReference  DOM_EntityReferenceType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node             DOM_NodeType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text             DOM_TextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_NamedNodeMap     DOM_NamedNodeMapType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_NodeList         DOM_NodeListType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Notation         DOM_NotationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_ProcessingInstruction    DOM_ProcessingInstructionType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text             DOM_TextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DOMException     DOM_DOMExceptionType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER AttrImpl             AttrImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ElementImpl          ElementImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER NodeImpl             NodeImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER TextImpl             TextImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMString            DOMStringType;





XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESBRIDGETYPES_HEADER_GUARD_1357924680)
