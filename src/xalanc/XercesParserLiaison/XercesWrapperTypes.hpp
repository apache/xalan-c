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
#if !defined(XERCESWRAPPERTYPES_HEADER_GUARD_1357924680)
#define XERCESWRAPPERTYPES_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



namespace XERCES_CPP_NAMESPACE { class DOMAttr; }
namespace XERCES_CPP_NAMESPACE { class DOMCharacterData; }
namespace XERCES_CPP_NAMESPACE { class DOMComment; }
namespace XERCES_CPP_NAMESPACE { class DOMCDATASection; }
namespace XERCES_CPP_NAMESPACE { class DOMDocument; }
namespace XERCES_CPP_NAMESPACE { class DOMDocumentFragment; }
namespace XERCES_CPP_NAMESPACE { class DOMImplementation; }
namespace XERCES_CPP_NAMESPACE { class DOMElement; }
namespace XERCES_CPP_NAMESPACE { class DOMEntity; }
namespace XERCES_CPP_NAMESPACE { class DOMEntityReference; }
namespace XERCES_CPP_NAMESPACE { class DOMNode; }
namespace XERCES_CPP_NAMESPACE { class DOMText; }
namespace XERCES_CPP_NAMESPACE { class DOMNamedNodeMap; }
namespace XERCES_CPP_NAMESPACE { class DOMNodeList; }
namespace XERCES_CPP_NAMESPACE { class DOMNotation; }
namespace XERCES_CPP_NAMESPACE { class DOMProcessingInstruction; }
namespace XERCES_CPP_NAMESPACE { class DOMText; }
namespace XERCES_CPP_NAMESPACE { class DOMDocumentType; }
namespace XERCES_CPP_NAMESPACE { class DOMException; }



XALAN_CPP_NAMESPACE_BEGIN



typedef xercesc::DOMAttr              DOMAttrType;
typedef xercesc::DOMCharacterData DOMCharacterDataType;
typedef xercesc::DOMComment           DOMCommentType;
typedef xercesc::DOMCDATASection      DOMCDATASectionType;
// These next two have an extra '_' because they collide without it...
typedef xercesc::DOMDocumentType      DOMDocumentType_Type;
typedef xercesc::DOMDocument          DOMDocument_Type;
typedef xercesc::DOMDocumentFragment  DOMDocumentFragmentType;
typedef xercesc::DOMImplementation    DOMImplementationType;
typedef xercesc::DOMElement           DOMElementType;
typedef xercesc::DOMEntity            DOMEntityType;
typedef xercesc::DOMEntityReference   DOMEntityReferenceType;
typedef xercesc::DOMNode              DOMNodeType;
typedef xercesc::DOMText              DOMTextType;
typedef xercesc::DOMNamedNodeMap      DOMNamedNodeMapType;
typedef xercesc::DOMNodeList          DOMNodeListType;
typedef xercesc::DOMNotation          DOMNotationType;
typedef xercesc::DOMProcessingInstruction DOMProcessingInstructionType;
typedef xercesc::DOMText              DOMTextType;
typedef xercesc::DOMException         DOMExceptionType;
typedef XMLSize_t                                           XMLSizeType;



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESWRAPPERTYPES_HEADER_GUARD_1357924680)
