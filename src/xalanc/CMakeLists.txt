# CMake build for xalan-c
#
# Written by Roger Leigh <rleigh@codelibre.net>
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Include/PlatformDefinitions.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/Include/PlatformDefinitions.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Include/XalanVersion.hpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/Include/XalanVersion.hpp)

set(xalandom_sources
  XalanDOM/XalanAttr.cpp
  XalanDOM/XalanCDataSection.cpp
  XalanDOM/XalanCharacterData.cpp
  XalanDOM/XalanComment.cpp
  XalanDOM/XalanDocument.cpp
  XalanDOM/XalanDocumentFragment.cpp
  XalanDOM/XalanDocumentType.cpp
  XalanDOM/XalanDOMException.cpp
  XalanDOM/XalanDOMImplementation.cpp
  XalanDOM/XalanDOMInit.cpp
  XalanDOM/XalanDOMString.cpp
  XalanDOM/XalanElement.cpp
  XalanDOM/XalanEmptyNamedNodeMap.cpp
  XalanDOM/XalanEntity.cpp
  XalanDOM/XalanEntityReference.cpp
  XalanDOM/XalanNamedNodeMap.cpp
  XalanDOM/XalanNode.cpp
  XalanDOM/XalanNodeList.cpp
  XalanDOM/XalanNodeListDummy.cpp
  XalanDOM/XalanNodeListSurrogate.cpp
  XalanDOM/XalanNotation.cpp
  XalanDOM/XalanProcessingInstruction.cpp
  XalanDOM/XalanText.cpp)

set(xalandom_headers
  XalanDOM/XalanAttr.hpp
  XalanDOM/XalanCDATASection.hpp
  XalanDOM/XalanCharacterData.hpp
  XalanDOM/XalanComment.hpp
  XalanDOM/XalanDocumentFragment.hpp
  XalanDOM/XalanDocument.hpp
  XalanDOM/XalanDocumentType.hpp
  XalanDOM/XalanDOMDefinitions.hpp
  XalanDOM/XalanDOMException.hpp
  XalanDOM/XalanDOMImplementation.hpp
  XalanDOM/XalanDOMInit.hpp
  XalanDOM/XalanDOMString.hpp
  XalanDOM/XalanElement.hpp
  XalanDOM/XalanEmptyNamedNodeMap.hpp
  XalanDOM/XalanEntity.hpp
  XalanDOM/XalanEntityReference.hpp
  XalanDOM/XalanNamedNodeMap.hpp
  XalanDOM/XalanNode.hpp
  XalanDOM/XalanNodeListDummy.hpp
  XalanDOM/XalanNodeList.hpp
  XalanDOM/XalanNodeListSurrogate.hpp
  XalanDOM/XalanNotation.hpp
  XalanDOM/XalanProcessingInstruction.hpp
  XalanDOM/XalanText.hpp
)

set(platformsupport_sources
  PlatformSupport/AttributeListImpl.cpp
  PlatformSupport/AttributesImpl.cpp
  PlatformSupport/DOMStringHelper.cpp
  PlatformSupport/DOMStringPrintWriter.cpp
  PlatformSupport/DoubleSupport.cpp
  PlatformSupport/ExecutionContext.cpp
  PlatformSupport/FormatterListener.cpp
  PlatformSupport/NamedNodeMapAttributeList.cpp
  PlatformSupport/NullPrintWriter.cpp
  PlatformSupport/PlatformSupportInit.cpp
  PlatformSupport/PrefixResolver.cpp
  PlatformSupport/PrintWriter.cpp
  PlatformSupport/ProblemListenerBase.cpp
  PlatformSupport/StdBinInputStream.cpp
  PlatformSupport/StringTokenizer.cpp
  PlatformSupport/URISupport.cpp
  PlatformSupport/Writer.cpp
  PlatformSupport/XalanBitmap.cpp
  PlatformSupport/XalanDecimalFormatSymbols.cpp
  PlatformSupport/XalanDOMStringAllocator.cpp
  PlatformSupport/XalanDOMStringCache.cpp
  PlatformSupport/XalanDOMStringHashTable.cpp
  PlatformSupport/XalanDOMStringPool.cpp
  PlatformSupport/XalanDOMStringReusableAllocator.cpp
  PlatformSupport/XalanEncodingPropertyCache.cpp
  PlatformSupport/XalanFileOutputStream.cpp
  PlatformSupport/XalanFStreamOutputStream.cpp
  PlatformSupport/XalanICUMessageLoader.cpp
  PlatformSupport/XalanInMemoryMessageLoader.cpp
  PlatformSupport/XalanMemoryManagement.cpp
  PlatformSupport/XalanMemoryManagerDefault.cpp
  PlatformSupport/XalanMessageLoader.cpp
  PlatformSupport/XalanNLSMessageLoader.cpp
  PlatformSupport/XalanNullOutputStream.cpp
  PlatformSupport/XalanNumberFormat.cpp
  PlatformSupport/XalanOutputStream.cpp
  PlatformSupport/XalanOutputStreamPrintWriter.cpp
  PlatformSupport/XalanParsedURI.cpp
  PlatformSupport/XalanReferenceCountedObject.cpp
  PlatformSupport/XalanSimplePrefixResolver.cpp
  PlatformSupport/XalanStdOutputStream.cpp
  PlatformSupport/XalanToXercesTranscoderWrapper.cpp
  PlatformSupport/XalanTranscodingServices.cpp
  PlatformSupport/XalanUTF16Transcoder.cpp
  PlatformSupport/XalanXMLChar.cpp
  PlatformSupport/XSLException.cpp)

set(platformsupport_headers
  PlatformSupport/ArenaAllocator.hpp
  PlatformSupport/ArenaBlockBase.hpp
  PlatformSupport/ArenaBlock.hpp
  PlatformSupport/AttributeListImpl.hpp
  PlatformSupport/AttributesImpl.hpp
  PlatformSupport/AttributeVectorEntryExtended.hpp
  PlatformSupport/AttributeVectorEntry.hpp
  PlatformSupport/DirectoryEnumerator.hpp
  PlatformSupport/DOMStringHelper.hpp
  PlatformSupport/DOMStringPrintWriter.hpp
  PlatformSupport/DoubleSupport.hpp
  PlatformSupport/ExecutionContext.hpp
  PlatformSupport/FormatterListener.hpp
  PlatformSupport/NamedNodeMapAttributeList.hpp
  PlatformSupport/NullPrintWriter.hpp
  PlatformSupport/PlatformSupportDefinitions.hpp
  PlatformSupport/PlatformSupportInit.hpp
  PlatformSupport/PrefixResolver.hpp
  PlatformSupport/PrintWriter.hpp
  PlatformSupport/ProblemListenerBase.hpp
  PlatformSupport/ReusableArenaAllocator.hpp
  PlatformSupport/ReusableArenaBlock.hpp
  PlatformSupport/StdBinInputStream.hpp
  PlatformSupport/StringTokenizer.hpp
  PlatformSupport/URISupport.hpp
  PlatformSupport/Writer.hpp
  PlatformSupport/XalanAllocator.hpp
  PlatformSupport/XalanArrayAllocator.hpp
  PlatformSupport/XalanBitmap.hpp
  PlatformSupport/XalanCollationServices.hpp
  PlatformSupport/XalanDecimalFormatSymbols.hpp
  PlatformSupport/XalanDOMStringAllocator.hpp
  PlatformSupport/XalanDOMStringCache.hpp
  PlatformSupport/XalanDOMStringHashTable.hpp
  PlatformSupport/XalanDOMStringPool.hpp
  PlatformSupport/XalanDOMStringReusableAllocator.hpp
  PlatformSupport/XalanEncodingPropertyCache.hpp
  PlatformSupport/XalanFileOutputStream.hpp
  PlatformSupport/XalanFStreamOutputStream.hpp
  PlatformSupport/XalanICUMessageLoader.hpp
  PlatformSupport/XalanInMemoryMessageLoader.hpp
  PlatformSupport/XalanLocator.hpp
  PlatformSupport/XalanMemoryManagerDefault.hpp
  PlatformSupport/XalanMessageLoader.hpp
  PlatformSupport/XalanNamespace.hpp
  PlatformSupport/XalanNLSMessageLoader.hpp
  PlatformSupport/XalanNullOutputStream.hpp
  PlatformSupport/XalanNumberFormat.hpp
  PlatformSupport/XalanOutputStream.hpp
  PlatformSupport/XalanOutputStreamPrintWriter.hpp
  PlatformSupport/XalanParsedURI.hpp
  PlatformSupport/XalanReferenceCountedObject.hpp
  PlatformSupport/XalanSimplePrefixResolver.hpp
  PlatformSupport/XalanStdOutputStream.hpp
  PlatformSupport/XalanToXercesTranscoderWrapper.hpp
  PlatformSupport/XalanTranscodingServices.hpp
  PlatformSupport/XalanUnicode.hpp
  PlatformSupport/XalanUTF16Transcoder.hpp
  PlatformSupport/XalanXMLChar.hpp
  PlatformSupport/XSLException.hpp)

set(domsupport_sources
  DOMSupport/DOMServices.cpp
  DOMSupport/DOMSupport.cpp
  DOMSupport/DOMSupportDefault.cpp
  DOMSupport/DOMSupportException.cpp
  DOMSupport/DOMSupportInit.cpp
  DOMSupport/TreeWalker.cpp
  DOMSupport/XalanDocumentPrefixResolver.cpp
  DOMSupport/XalanNamespacesStack.cpp)

set(domsupport_headers
  DOMSupport/DOMServices.hpp
  DOMSupport/DOMSupportDefault.hpp
  DOMSupport/DOMSupportDefinitions.hpp
  DOMSupport/DOMSupportException.hpp
  DOMSupport/DOMSupport.hpp
  DOMSupport/DOMSupportInit.hpp
  DOMSupport/TreeWalker.hpp
  DOMSupport/XalanDocumentPrefixResolver.hpp
  DOMSupport/XalanNamespacesStack.hpp)

set(xmlsupport_sources
  XMLSupport/FormatterToHTML.cpp
  XMLSupport/FormatterToNull.cpp
  XMLSupport/FormatterToText.cpp
  XMLSupport/FormatterToXML.cpp
  XMLSupport/FormatterTreeWalker.cpp
  XMLSupport/XalanHTMLElementsProperties.cpp
  XMLSupport/XalanUTF16Writer.cpp
  XMLSupport/XalanUTF8Writer.cpp
  XMLSupport/XalanXMLSerializerBase.cpp
  XMLSupport/XalanXMLSerializerFactory.cpp
  XMLSupport/XMLParserLiaison.cpp
  XMLSupport/XMLSupportException.cpp
  XMLSupport/XMLSupportInit.cpp)

set(xmlsupport_headers
  XMLSupport/FormatterToHTML.hpp
  XMLSupport/FormatterToNull.hpp
  XMLSupport/FormatterToText.hpp
  XMLSupport/FormatterToXML.hpp
  XMLSupport/FormatterToXMLUnicode.hpp
  XMLSupport/FormatterTreeWalker.hpp
  XMLSupport/XalanDummyIndentWriter.hpp
  XMLSupport/XalanFormatterWriter.hpp
  XMLSupport/XalanHTMLElementsProperties.hpp
  XMLSupport/XalanIndentWriter.hpp
  XMLSupport/XalanOtherEncodingWriter.hpp
  XMLSupport/XalanUTF16Writer.hpp
  XMLSupport/XalanUTF8Writer.hpp
  XMLSupport/XalanXMLSerializerBase.hpp
  XMLSupport/XalanXMLSerializerFactory.hpp
  XMLSupport/XMLParserLiaison.hpp
  XMLSupport/XMLSupportDefinitions.hpp
  XMLSupport/XMLSupportException.hpp
  XMLSupport/XMLSupportInit.hpp)

set(xpath_sources
  XPath/ElementPrefixResolverProxy.cpp
  XPath/FormatterStringLengthCounter.cpp
  XPath/FunctionConcat.cpp
  XPath/FunctionContains.cpp
  XPath/Function.cpp
  XPath/FunctionID.cpp
  XPath/FunctionLang.cpp
  XPath/FunctionNamespaceURI.cpp
  XPath/FunctionNormalizeSpace.cpp
  XPath/FunctionStartsWith.cpp
  XPath/FunctionString.cpp
  XPath/FunctionSubstringAfter.cpp
  XPath/FunctionSubstringBefore.cpp
  XPath/FunctionSubstring.cpp
  XPath/FunctionTranslate.cpp
  XPath/MutableNodeRefList.cpp
  XPath/NodeRefListBase.cpp
  XPath/NodeRefList.cpp
  XPath/XalanDocumentFragmentNodeRefListBaseProxy.cpp
  XPath/XalanQNameByReference.cpp
  XPath/XalanQNameByValueAllocator.cpp
  XPath/XalanQNameByValue.cpp
  XPath/XalanQName.cpp
  XPath/XalanXPathException.cpp
  XPath/XBoolean.cpp
  XPath/XNodeSetAllocator.cpp
  XPath/XNodeSetBase.cpp
  XPath/XNodeSet.cpp
  XPath/XNodeSetNodeProxyAllocator.cpp
  XPath/XNodeSetNodeProxy.cpp
  XPath/XNodeSetResultTreeFragProxy.cpp
  XPath/XNumberAllocator.cpp
  XPath/XNumberBase.cpp
  XPath/XNumber.cpp
  XPath/XObject.cpp
  XPath/XObjectFactory.cpp
  XPath/XObjectFactoryDefault.cpp
  XPath/XObjectResultTreeFragProxyBase.cpp
  XPath/XObjectResultTreeFragProxy.cpp
  XPath/XObjectResultTreeFragProxyText.cpp
  XPath/XObjectTypeCallback.cpp
  XPath/XPathAllocator.cpp
  XPath/XPathConstructionContext.cpp
  XPath/XPathConstructionContextDefault.cpp
  XPath/XPath.cpp
  XPath/XPathEnvSupport.cpp
  XPath/XPathEnvSupportDefault.cpp
  XPath/XPathEvaluator.cpp
  XPath/XPathExecutionContext.cpp
  XPath/XPathExecutionContextDefault.cpp
  XPath/XPathExpression.cpp
  XPath/XPathFactoryBlock.cpp
  XPath/XPathFactory.cpp
  XPath/XPathFactoryDefault.cpp
  XPath/XPathFunctionTable.cpp
  XPath/XPathInit.cpp
  XPath/XPathParserException.cpp
  XPath/XPathProcessor.cpp
  XPath/XPathProcessorImpl.cpp
  XPath/XStringAdapterAllocator.cpp
  XPath/XStringAdapter.cpp
  XPath/XStringAllocator.cpp
  XPath/XStringBase.cpp
  XPath/XStringCachedAllocator.cpp
  XPath/XStringCached.cpp
  XPath/XString.cpp
  XPath/XStringReferenceAllocator.cpp
  XPath/XStringReference.cpp
  XPath/XToken.cpp
  XPath/XTokenNumberAdapterAllocator.cpp
  XPath/XTokenNumberAdapter.cpp
  XPath/XTokenStringAdapterAllocator.cpp
  XPath/XTokenStringAdapter.cpp
  XPath/XUnknown.cpp)

set(xpath_headers
  XPath/ElementPrefixResolverProxy.hpp
  XPath/FormatterStringLengthCounter.hpp
  XPath/FunctionConcat.hpp
  XPath/FunctionContains.hpp
  XPath/Function.hpp
  XPath/FunctionID.hpp
  XPath/FunctionLang.hpp
  XPath/FunctionNamespaceURI.hpp
  XPath/FunctionNormalizeSpace.hpp
  XPath/FunctionStartsWith.hpp
  XPath/FunctionString.hpp
  XPath/FunctionSubstringAfter.hpp
  XPath/FunctionSubstringBefore.hpp
  XPath/FunctionSubstring.hpp
  XPath/FunctionTranslate.hpp
  XPath/MutableNodeRefList.hpp
  XPath/NameSpace.hpp
  XPath/NodeRefListBase.hpp
  XPath/NodeRefList.hpp
  XPath/XalanDocumentFragmentNodeRefListBaseProxy.hpp
  XPath/XalanQNameByReference.hpp
  XPath/XalanQNameByValueAllocator.hpp
  XPath/XalanQNameByValue.hpp
  XPath/XalanQName.hpp
  XPath/XalanXPathException.hpp
  XPath/XBoolean.hpp
  XPath/XNodeSetAllocator.hpp
  XPath/XNodeSetBase.hpp
  XPath/XNodeSet.hpp
  XPath/XNodeSetNodeProxyAllocator.hpp
  XPath/XNodeSetNodeProxy.hpp
  XPath/XNodeSetResultTreeFragProxy.hpp
  XPath/XNumberAllocator.hpp
  XPath/XNumberBase.hpp
  XPath/XNumber.hpp
  XPath/XObjectFactoryDefault.hpp
  XPath/XObjectFactory.hpp
  XPath/XObject.hpp
  XPath/XObjectResultTreeFragProxyBase.hpp
  XPath/XObjectResultTreeFragProxy.hpp
  XPath/XObjectResultTreeFragProxyText.hpp
  XPath/XObjectTypeCallback.hpp
  XPath/XPathAllocator.hpp
  XPath/XPathConstructionContextDefault.hpp
  XPath/XPathConstructionContext.hpp
  XPath/XPathDefinitions.hpp
  XPath/XPathEnvSupportDefault.hpp
  XPath/XPathEnvSupport.hpp
  XPath/XPathEvaluator.hpp
  XPath/XPathExecutionContextDefault.hpp
  XPath/XPathExecutionContext.hpp
  XPath/XPathExpression.hpp
  XPath/XPathFactoryBlock.hpp
  XPath/XPathFactoryDefault.hpp
  XPath/XPathFactory.hpp
  XPath/XPathFunctionTable.hpp
  XPath/XPath.hpp
  XPath/XPathInit.hpp
  XPath/XPathParserException.hpp
  XPath/XPathProcessor.hpp
  XPath/XPathProcessorImpl.hpp
  XPath/XStringAdapterAllocator.hpp
  XPath/XStringAdapter.hpp
  XPath/XStringAllocator.hpp
  XPath/XStringBase.hpp
  XPath/XStringCachedAllocator.hpp
  XPath/XStringCached.hpp
  XPath/XString.hpp
  XPath/XStringReferenceAllocator.hpp
  XPath/XStringReference.hpp
  XPath/XToken.hpp
  XPath/XTokenNumberAdapterAllocator.hpp
  XPath/XTokenNumberAdapter.hpp
  XPath/XTokenStringAdapterAllocator.hpp
  XPath/XTokenStringAdapter.hpp
  XPath/XUnknown.hpp)

set(xercesparserliaison_sources
  XercesParserLiaison/FormatterToXercesDOM.cpp
  XercesParserLiaison/XercesAttrWrapperAllocator.cpp
  XercesParserLiaison/XercesAttrWrapper.cpp
  XercesParserLiaison/XercesCDATASectionWrapper.cpp
  XercesParserLiaison/XercesCommentWrapper.cpp
  XercesParserLiaison/XercesDocumentTypeWrapper.cpp
  XercesParserLiaison/XercesDocumentWrapper.cpp
  XercesParserLiaison/XercesDOMException.cpp
  XercesParserLiaison/XercesDOMFormatterWalker.cpp
  XercesParserLiaison/XercesDOMImplementationWrapper.cpp
  XercesParserLiaison/XercesDOMSupport.cpp
  XercesParserLiaison/XercesDOMWalker.cpp
  XercesParserLiaison/XercesDOMWrapperException.cpp
  XercesParserLiaison/XercesElementWrapperAllocator.cpp
  XercesParserLiaison/XercesElementWrapper.cpp
  XercesParserLiaison/XercesEntityReferenceWrapper.cpp
  XercesParserLiaison/XercesEntityWrapper.cpp
  XercesParserLiaison/XercesLiaisonXalanDOMStringPool.cpp
  XercesParserLiaison/XercesNamedNodeMapAttributeList.cpp
  XercesParserLiaison/XercesNamedNodeMapWrapper.cpp
  XercesParserLiaison/XercesNodeListWrapper.cpp
  XercesParserLiaison/XercesNotationWrapper.cpp
  XercesParserLiaison/XercesParserLiaison.cpp
  XercesParserLiaison/XercesProcessingInstructionWrapper.cpp
  XercesParserLiaison/XercesTextWrapperAllocator.cpp
  XercesParserLiaison/XercesTextWrapper.cpp
  XercesParserLiaison/XercesWrapperHelper.cpp
  XercesParserLiaison/XercesWrapperNavigatorAllocator.cpp
  XercesParserLiaison/XercesWrapperNavigator.cpp
  XercesParserLiaison/XercesWrapperToXalanNodeMap.cpp)

set(xercesparserliaison_headers
  XercesParserLiaison/FormatterToXercesDOM.hpp
  XercesParserLiaison/XercesAttrWrapperAllocator.hpp
  XercesParserLiaison/XercesAttrWrapper.hpp
  XercesParserLiaison/XercesCDATASectionWrapper.hpp
  XercesParserLiaison/XercesCommentWrapper.hpp
  XercesParserLiaison/XercesDocumentTypeWrapper.hpp
  XercesParserLiaison/XercesDocumentWrapper.hpp
  XercesParserLiaison/XercesDOMException.hpp
  XercesParserLiaison/XercesDOMFormatterWalker.hpp
  XercesParserLiaison/XercesDOMImplementationWrapper.hpp
  XercesParserLiaison/XercesDOMSupport.hpp
  XercesParserLiaison/XercesDOMWalker.hpp
  XercesParserLiaison/XercesDOMWrapperException.hpp
  XercesParserLiaison/XercesElementWrapperAllocator.hpp
  XercesParserLiaison/XercesElementWrapper.hpp
  XercesParserLiaison/XercesEntityReferenceWrapper.hpp
  XercesParserLiaison/XercesEntityWrapper.hpp
  XercesParserLiaison/XercesLiaisonXalanDOMStringPool.hpp
  XercesParserLiaison/XercesNamedNodeMapAttributeList.hpp
  XercesParserLiaison/XercesNamedNodeMapWrapper.hpp
  XercesParserLiaison/XercesNodeListWrapper.hpp
  XercesParserLiaison/XercesNotationWrapper.hpp
  XercesParserLiaison/XercesParserLiaisonDefinitions.hpp
  XercesParserLiaison/XercesParserLiaison.hpp
  XercesParserLiaison/XercesProcessingInstructionWrapper.hpp
  XercesParserLiaison/XercesTextWrapperAllocator.hpp
  XercesParserLiaison/XercesTextWrapper.hpp
  XercesParserLiaison/XercesWrapperHelper.hpp
  XercesParserLiaison/XercesWrapperNavigatorAllocator.hpp
  XercesParserLiaison/XercesWrapperNavigator.hpp
  XercesParserLiaison/XercesWrapperToXalanNodeMap.hpp
  XercesParserLiaison/XercesWrapperTypes.hpp)

set(xercesparserliaison_deprecated_sources
  XercesParserLiaison/Deprecated/FormatterToDeprecatedXercesDOM.cpp
  XercesParserLiaison/Deprecated/XercesAttrBridge.cpp
  XercesParserLiaison/Deprecated/XercesAttributeBridgeAllocator.cpp
  XercesParserLiaison/Deprecated/XercesBridgeHelper.cpp
  XercesParserLiaison/Deprecated/XercesBridgeNavigator.cpp
  XercesParserLiaison/Deprecated/XercesCDATASectionBridge.cpp
  XercesParserLiaison/Deprecated/XercesCommentBridge.cpp
  XercesParserLiaison/Deprecated/XercesDocumentBridge.cpp
  XercesParserLiaison/Deprecated/XercesDocumentFragmentBridge.cpp
  XercesParserLiaison/Deprecated/XercesDocumentTypeBridge.cpp
  XercesParserLiaison/Deprecated/XercesDOMImplementationBridge.cpp
  XercesParserLiaison/Deprecated/XercesDOM_NodeHack.cpp
  XercesParserLiaison/Deprecated/XercesElementBridgeAllocator.cpp
  XercesParserLiaison/Deprecated/XercesElementBridge.cpp
  XercesParserLiaison/Deprecated/XercesEntityBridge.cpp
  XercesParserLiaison/Deprecated/XercesEntityReferenceBridge.cpp
  XercesParserLiaison/Deprecated/XercesNamedNodeMapBridge.cpp
  XercesParserLiaison/Deprecated/XercesNodeListBridge.cpp
  XercesParserLiaison/Deprecated/XercesNotationBridge.cpp
  XercesParserLiaison/Deprecated/XercesProcessingInstructionBridge.cpp
  XercesParserLiaison/Deprecated/XercesTextBridgeAllocator.cpp
  XercesParserLiaison/Deprecated/XercesTextBridge.cpp
  XercesParserLiaison/Deprecated/XercesToXalanNodeMap.cpp
  XercesParserLiaison/Deprecated/XercesTreeWalker.cpp)

set(xercesparserliaison_deprecated_headers
  XercesParserLiaison/Deprecated/FormatterToDeprecatedXercesDOM.hpp
  XercesParserLiaison/Deprecated/XercesAttrBridge.hpp
  XercesParserLiaison/Deprecated/XercesAttributeBridgeAllocator.hpp
  XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp
  XercesParserLiaison/Deprecated/XercesBridgeNavigator.hpp
  XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp
  XercesParserLiaison/Deprecated/XercesCDATASectionBridge.hpp
  XercesParserLiaison/Deprecated/XercesCommentBridge.hpp
  XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp
  XercesParserLiaison/Deprecated/XercesDocumentFragmentBridge.hpp
  XercesParserLiaison/Deprecated/XercesDocumentTypeBridge.hpp
  XercesParserLiaison/Deprecated/XercesDOMImplementationBridge.hpp
  XercesParserLiaison/Deprecated/XercesDOM_NodeHack.hpp
  XercesParserLiaison/Deprecated/XercesElementBridgeAllocator.hpp
  XercesParserLiaison/Deprecated/XercesElementBridge.hpp
  XercesParserLiaison/Deprecated/XercesEntityBridge.hpp
  XercesParserLiaison/Deprecated/XercesEntityReferenceBridge.hpp
  XercesParserLiaison/Deprecated/XercesNamedNodeMapBridge.hpp
  XercesParserLiaison/Deprecated/XercesNodeListBridge.hpp
  XercesParserLiaison/Deprecated/XercesNotationBridge.hpp
  XercesParserLiaison/Deprecated/XercesProcessingInstructionBridge.hpp
  XercesParserLiaison/Deprecated/XercesTextBridgeAllocator.hpp
  XercesParserLiaison/Deprecated/XercesTextBridge.hpp
  XercesParserLiaison/Deprecated/XercesToXalanNodeMap.hpp
  XercesParserLiaison/Deprecated/XercesTreeWalker.hpp)

set(xalansourcetree_sources
  XalanSourceTree/FormatterToSourceTree.cpp
  XalanSourceTree/XalanSourceTreeAttr.cpp
  XalanSourceTree/XalanSourceTreeAttributeAllocator.cpp
  XalanSourceTree/XalanSourceTreeAttributeNSAllocator.cpp
  XalanSourceTree/XalanSourceTreeAttrNS.cpp
  XalanSourceTree/XalanSourceTreeCommentAllocator.cpp
  XalanSourceTree/XalanSourceTreeComment.cpp
  XalanSourceTree/XalanSourceTreeContentHandler.cpp
  XalanSourceTree/XalanSourceTreeDocument.cpp
  XalanSourceTree/XalanSourceTreeDocumentFragment.cpp
  XalanSourceTree/XalanSourceTreeDOMSupport.cpp
  XalanSourceTree/XalanSourceTreeElementAAllocator.cpp
  XalanSourceTree/XalanSourceTreeElementA.cpp
  XalanSourceTree/XalanSourceTreeElementANSAllocator.cpp
  XalanSourceTree/XalanSourceTreeElementANS.cpp
  XalanSourceTree/XalanSourceTreeElement.cpp
  XalanSourceTree/XalanSourceTreeElementNAAllocator.cpp
  XalanSourceTree/XalanSourceTreeElementNA.cpp
  XalanSourceTree/XalanSourceTreeElementNANSAllocator.cpp
  XalanSourceTree/XalanSourceTreeElementNANS.cpp
  XalanSourceTree/XalanSourceTreeHelper.cpp
  XalanSourceTree/XalanSourceTreeInit.cpp
  XalanSourceTree/XalanSourceTreeParserLiaison.cpp
  XalanSourceTree/XalanSourceTreeProcessingInstructionAllocator.cpp
  XalanSourceTree/XalanSourceTreeProcessingInstruction.cpp
  XalanSourceTree/XalanSourceTreeTextAllocator.cpp
  XalanSourceTree/XalanSourceTreeText.cpp
  XalanSourceTree/XalanSourceTreeTextIWSAllocator.cpp
  XalanSourceTree/XalanSourceTreeTextIWS.cpp)

set(xalansourcetree_headers
  XalanSourceTree/FormatterToSourceTree.hpp
  XalanSourceTree/XalanSourceTreeAttr.hpp
  XalanSourceTree/XalanSourceTreeAttributeAllocator.hpp
  XalanSourceTree/XalanSourceTreeAttributeNSAllocator.hpp
  XalanSourceTree/XalanSourceTreeAttrNS.hpp
  XalanSourceTree/XalanSourceTreeCommentAllocator.hpp
  XalanSourceTree/XalanSourceTreeComment.hpp
  XalanSourceTree/XalanSourceTreeContentHandler.hpp
  XalanSourceTree/XalanSourceTreeDefinitions.hpp
  XalanSourceTree/XalanSourceTreeDocumentFragment.hpp
  XalanSourceTree/XalanSourceTreeDocument.hpp
  XalanSourceTree/XalanSourceTreeDOMSupport.hpp
  XalanSourceTree/XalanSourceTreeElementAAllocator.hpp
  XalanSourceTree/XalanSourceTreeElementA.hpp
  XalanSourceTree/XalanSourceTreeElementANSAllocator.hpp
  XalanSourceTree/XalanSourceTreeElementANS.hpp
  XalanSourceTree/XalanSourceTreeElement.hpp
  XalanSourceTree/XalanSourceTreeElementNAAllocator.hpp
  XalanSourceTree/XalanSourceTreeElementNA.hpp
  XalanSourceTree/XalanSourceTreeElementNANSAllocator.hpp
  XalanSourceTree/XalanSourceTreeElementNANS.hpp
  XalanSourceTree/XalanSourceTreeHelper.hpp
  XalanSourceTree/XalanSourceTreeInit.hpp
  XalanSourceTree/XalanSourceTreeParserLiaison.hpp
  XalanSourceTree/XalanSourceTreeProcessingInstructionAllocator.hpp
  XalanSourceTree/XalanSourceTreeProcessingInstruction.hpp
  XalanSourceTree/XalanSourceTreeTextAllocator.hpp
  XalanSourceTree/XalanSourceTreeText.hpp
  XalanSourceTree/XalanSourceTreeTextIWSAllocator.hpp
  XalanSourceTree/XalanSourceTreeTextIWS.hpp)

set(xpathcapi_sources
  XPathCAPI/XPathCAPI.cpp)

set(xpathcapi_headers
  XPathCAPI/XPathCAPI.h)

set(xslt_sources
  XSLT/AVT.cpp
  XSLT/AVTPart.cpp
  XSLT/AVTPartSimple.cpp
  XSLT/AVTPartXPath.cpp
  XSLT/Constants.cpp
  XSLT/CountersTable.cpp
  XSLT/ElemApplyImport.cpp
  XSLT/ElemApplyTemplates.cpp
  XSLT/ElemAttribute.cpp
  XSLT/ElemAttributeSet.cpp
  XSLT/ElemCallTemplate.cpp
  XSLT/ElemChoose.cpp
  XSLT/ElemComment.cpp
  XSLT/ElemCopy.cpp
  XSLT/ElemCopyOf.cpp
  XSLT/ElemDecimalFormat.cpp
  XSLT/ElemElement.cpp
  XSLT/ElemEmpty.cpp
  XSLT/ElemExtensionCall.cpp
  XSLT/ElemFallback.cpp
  XSLT/ElemForEach.cpp
  XSLT/ElemForwardCompatible.cpp
  XSLT/ElemIf.cpp
  XSLT/ElemLiteralResult.cpp
  XSLT/ElemMessage.cpp
  XSLT/ElemNumber.cpp
  XSLT/ElemOtherwise.cpp
  XSLT/ElemParam.cpp
  XSLT/ElemPI.cpp
  XSLT/ElemSort.cpp
  XSLT/ElemTemplate.cpp
  XSLT/ElemTemplateElement.cpp
  XSLT/ElemText.cpp
  XSLT/ElemTextLiteral.cpp
  XSLT/ElemUse.cpp
  XSLT/ElemValueOf.cpp
  XSLT/ElemVariable.cpp
  XSLT/ElemWhen.cpp
  XSLT/ElemWithParam.cpp
  XSLT/ExtensionFunctionHandler.cpp
  XSLT/ExtensionNSHandler.cpp
  XSLT/FunctionCurrent.cpp
  XSLT/FunctionDocument.cpp
  XSLT/FunctionElementAvailable.cpp
  XSLT/FunctionFormatNumber.cpp
  XSLT/FunctionFunctionAvailable.cpp
  XSLT/FunctionGenerateID.cpp
  XSLT/FunctionKey.cpp
  XSLT/FunctionSystemProperty.cpp
  XSLT/FunctionUnparsedEntityURI.cpp
  XSLT/GenerateEvent.cpp
  XSLT/KeyTable.cpp
  XSLT/NamespacesHandler.cpp
  XSLT/NodeSorter.cpp
  XSLT/NodeSortKey.cpp
  XSLT/OutputContextStack.cpp
  XSLT/ProblemListener.cpp
  XSLT/ProblemListenerDefault.cpp
  XSLT/ResultNamespacesStack.cpp
  XSLT/SelectionEvent.cpp
  XSLT/StylesheetConstructionContext.cpp
  XSLT/StylesheetConstructionContextDefault.cpp
  XSLT/Stylesheet.cpp
  XSLT/StylesheetExecutionContext.cpp
  XSLT/StylesheetExecutionContextDefault.cpp
  XSLT/StylesheetHandler.cpp
  XSLT/StylesheetRoot.cpp
  XSLT/TopLevelArg.cpp
  XSLT/TraceListener.cpp
  XSLT/TraceListenerDefault.cpp
  XSLT/TracerEvent.cpp
  XSLT/VariablesStack.cpp
  XSLT/XalanAVTAllocator.cpp
  XSLT/XalanAVTPartSimpleAllocator.cpp
  XSLT/XalanAVTPartXPathAllocator.cpp
  XSLT/XalanElemApplyTemplatesAllocator.cpp
  XSLT/XalanElemAttributeAllocator.cpp
  XSLT/XalanElemAttributeSetAllocator.cpp
  XSLT/XalanElemCallTemplateAllocator.cpp
  XSLT/XalanElemElementAllocator.cpp
  XSLT/XalanElemEmptyAllocator.cpp
  XSLT/XalanElemLiteralResultAllocator.cpp
  XSLT/XalanElemTemplateAllocator.cpp
  XSLT/XalanElemTextAllocator.cpp
  XSLT/XalanElemTextLiteralAllocator.cpp
  XSLT/XalanElemValueOfAllocator.cpp
  XSLT/XalanElemVariableAllocator.cpp
  XSLT/XalanMatchPatternDataAllocator.cpp
  XSLT/XalanMatchPatternData.cpp
  XSLT/XalanNumberingResourceBundle.cpp
  XSLT/XalanSourceTreeDocumentAllocator.cpp
  XSLT/XalanSourceTreeDocumentFragmentAllocator.cpp
  XSLT/XalanSpaceNodeTester.cpp
  XSLT/XResultTreeFragAllocator.cpp
  XSLT/XResultTreeFrag.cpp
  XSLT/XSLTEngineImpl.cpp
  XSLT/XSLTInit.cpp
  XSLT/XSLTInputSource.cpp
  XSLT/XSLTProcessor.cpp
  XSLT/XSLTProcessorEnvSupport.cpp
  XSLT/XSLTProcessorEnvSupportDefault.cpp
  XSLT/XSLTProcessorException.cpp
  XSLT/XSLTResultTarget.cpp
)

set(xslt_headers
  XSLT/AVT.hpp
  XSLT/AVTPart.hpp
  XSLT/AVTPartSimple.hpp
  XSLT/AVTPartXPath.hpp
  XSLT/Constants.hpp
  XSLT/CountersTable.hpp
  XSLT/DecimalToRoman.hpp
  XSLT/ElemApplyImport.hpp
  XSLT/ElemApplyTemplates.hpp
  XSLT/ElemAttribute.hpp
  XSLT/ElemAttributeSet.hpp
  XSLT/ElemCallTemplate.hpp
  XSLT/ElemChoose.hpp
  XSLT/ElemComment.hpp
  XSLT/ElemCopy.hpp
  XSLT/ElemCopyOf.hpp
  XSLT/ElemDecimalFormat.hpp
  XSLT/ElemElement.hpp
  XSLT/ElemEmpty.hpp
  XSLT/ElemExtensionCall.hpp
  XSLT/ElemFallback.hpp
  XSLT/ElemForEach.hpp
  XSLT/ElemForwardCompatible.hpp
  XSLT/ElemIf.hpp
  XSLT/ElemLiteralResult.hpp
  XSLT/ElemMessage.hpp
  XSLT/ElemNumber.hpp
  XSLT/ElemOtherwise.hpp
  XSLT/ElemParam.hpp
  XSLT/ElemPI.hpp
  XSLT/ElemSort.hpp
  XSLT/ElemTemplateElement.hpp
  XSLT/ElemTemplate.hpp
  XSLT/ElemText.hpp
  XSLT/ElemTextLiteral.hpp
  XSLT/ElemUse.hpp
  XSLT/ElemValueOf.hpp
  XSLT/ElemVariable.hpp
  XSLT/ElemWhen.hpp
  XSLT/ElemWithParam.hpp
  XSLT/ExtensionFunctionHandler.hpp
  XSLT/ExtensionNSHandler.hpp
  XSLT/FunctionCurrent.hpp
  XSLT/FunctionDocument.hpp
  XSLT/FunctionElementAvailable.hpp
  XSLT/FunctionFormatNumber.hpp
  XSLT/FunctionFunctionAvailable.hpp
  XSLT/FunctionGenerateID.hpp
  XSLT/FunctionKey.hpp
  XSLT/FunctionSystemProperty.hpp
  XSLT/FunctionUnparsedEntityURI.hpp
  XSLT/GenerateEvent.hpp
  XSLT/KeyDeclaration.hpp
  XSLT/KeyTable.hpp
  XSLT/NamespacesHandler.hpp
  XSLT/NodeSorter.hpp
  XSLT/NodeSortKey.hpp
  XSLT/OutputContextStack.hpp
  XSLT/ProblemListenerDefault.hpp
  XSLT/ProblemListener.hpp
  XSLT/ResultNamespacesStack.hpp
  XSLT/SelectionEvent.hpp
  XSLT/StylesheetConstructionContextDefault.hpp
  XSLT/StylesheetConstructionContext.hpp
  XSLT/StylesheetExecutionContextDefault.hpp
  XSLT/StylesheetExecutionContext.hpp
  XSLT/StylesheetHandler.hpp
  XSLT/Stylesheet.hpp
  XSLT/StylesheetRoot.hpp
  XSLT/TopLevelArg.hpp
  XSLT/TraceListenerDefault.hpp
  XSLT/TraceListener.hpp
  XSLT/TracerEvent.hpp
  XSLT/VariablesStack.hpp
  XSLT/XalanAVTAllocator.hpp
  XSLT/XalanAVTPartSimpleAllocator.hpp
  XSLT/XalanAVTPartXPathAllocator.hpp
  XSLT/XalanElemApplyTemplatesAllocator.hpp
  XSLT/XalanElemAttributeAllocator.hpp
  XSLT/XalanElemAttributeSetAllocator.hpp
  XSLT/XalanElemCallTemplateAllocator.hpp
  XSLT/XalanElemElementAllocator.hpp
  XSLT/XalanElemEmptyAllocator.hpp
  XSLT/XalanElemLiteralResultAllocator.hpp
  XSLT/XalanElemTemplateAllocator.hpp
  XSLT/XalanElemTextAllocator.hpp
  XSLT/XalanElemTextLiteralAllocator.hpp
  XSLT/XalanElemValueOfAllocator.hpp
  XSLT/XalanElemVariableAllocator.hpp
  XSLT/XalanMatchPatternDataAllocator.hpp
  XSLT/XalanMatchPatternData.hpp
  XSLT/XalanNumberingResourceBundle.hpp
  XSLT/XalanParamHolder.hpp
  XSLT/XalanSourceTreeDocumentAllocator.hpp
  XSLT/XalanSourceTreeDocumentFragmentAllocator.hpp
  XSLT/XalanSpaceNodeTester.hpp
  XSLT/XResultTreeFragAllocator.hpp
  XSLT/XResultTreeFrag.hpp
  XSLT/XSLTDefinitions.hpp
  XSLT/XSLTEngineImpl.hpp
  XSLT/XSLTInit.hpp
  XSLT/XSLTInputSource.hpp
  XSLT/XSLTProcessorEnvSupportDefault.hpp
  XSLT/XSLTProcessorEnvSupport.hpp
  XSLT/XSLTProcessorException.hpp
  XSLT/XSLTProcessor.hpp
  XSLT/XSLTResultTarget.hpp)

set(xalanextensions_sources
  XalanExtensions/FunctionDifference.cpp
  XalanExtensions/FunctionDistinct.cpp
  XalanExtensions/FunctionEvaluate.cpp
  XalanExtensions/FunctionHasSameNodes.cpp
  XalanExtensions/FunctionIntersection.cpp
  XalanExtensions/FunctionNodeSet.cpp
  XalanExtensions/XalanExtensions.cpp)

set(xalanextensions_headers
  XalanExtensions/FunctionDifference.hpp
  XalanExtensions/FunctionDistinct.hpp
  XalanExtensions/FunctionEvaluate.hpp
  XalanExtensions/FunctionHasSameNodes.hpp
  XalanExtensions/FunctionIntersection.hpp
  XalanExtensions/FunctionNodeSet.hpp
  XalanExtensions/XalanExtensionsDefinitions.hpp
  XalanExtensions/XalanExtensions.hpp)

set(xalantransformer_sources
  XalanTransformer/XalanCAPI.cpp
  XalanTransformer/XalanCompiledStylesheetDefault.cpp
  XalanTransformer/XalanDefaultDocumentBuilder.cpp
  XalanTransformer/XalanDefaultParsedSource.cpp
  XalanTransformer/XalanParsedSource.cpp
  XalanTransformer/XalanSourceTreeWrapperParsedSource.cpp
  XalanTransformer/XalanTransformer.cpp
  XalanTransformer/XalanTransformerOutputStream.cpp
  XalanTransformer/XalanTransformerProblemListener.cpp
  XalanTransformer/XercesDOMParsedSource.cpp
  XalanTransformer/XercesDOMWrapperParsedSource.cpp)

set(xalantransformer_headers
  XalanTransformer/XalanCompiledStylesheetDefault.hpp
  XalanTransformer/XalanCompiledStylesheet.hpp
  XalanTransformer/XalanDefaultDocumentBuilder.hpp
  XalanTransformer/XalanDefaultParsedSource.hpp
  XalanTransformer/XalanDocumentBuilder.hpp
  XalanTransformer/XalanParsedSource.hpp
  XalanTransformer/XalanSourceTreeWrapperParsedSource.hpp
  XalanTransformer/XalanTransformerDefinitions.hpp
  XalanTransformer/XalanTransformer.hpp
  XalanTransformer/XalanTransformerOutputStream.hpp
  XalanTransformer/XalanTransformerProblemListener.hpp
  XalanTransformer/XercesDOMParsedSource.hpp
  XalanTransformer/XercesDOMWrapperParsedSource.hpp)

set(xalanexslt_sources
  XalanEXSLT/XalanEXSLTCommon.cpp
  XalanEXSLT/XalanEXSLTDateTime.cpp
  XalanEXSLT/XalanEXSLTDynamic.cpp
  XalanEXSLT/XalanEXSLTMath.cpp
  XalanEXSLT/XalanEXSLTSet.cpp
  XalanEXSLT/XalanEXSLTString.cpp)

set(xalanexslt_headers
  XalanEXSLT/XalanEXSLTCommon.hpp
  XalanEXSLT/XalanEXSLTCommonImpl.hpp
  XalanEXSLT/XalanEXSLTDateTime.hpp
  XalanEXSLT/XalanEXSLTDateTimeImpl.hpp
  XalanEXSLT/XalanEXSLTDefinitions.hpp
  XalanEXSLT/XalanEXSLTDynamic.hpp
  XalanEXSLT/XalanEXSLTDynamicImpl.hpp
  XalanEXSLT/XalanEXSLTMath.hpp
  XalanEXSLT/XalanEXSLTMathImpl.hpp
  XalanEXSLT/XalanEXSLTSet.hpp
  XalanEXSLT/XalanEXSLTSetImpl.hpp
  XalanEXSLT/XalanEXSLTString.hpp
  XalanEXSLT/XalanEXSLTStringImpl.hpp)

set(include_sources
  Include/XalanVersion.cpp)

set(harness_sources
  Harness/XalanDiagnosticMemoryManager.cpp
  Harness/XalanFileUtility.cpp
  Harness/XalanXMLFileReporter.cpp)

set(harness_headers
  Harness/XalanDiagnosticMemoryManager.hpp
  Harness/XalanFileUtility.hpp
  Harness/XalanHarnessDefinitions.hpp
  Harness/XalanXMLFileReporter.hpp)

set(include_headers
  Include/STLHelper.hpp
  Include/XalanAutoPtr.hpp
  Include/XalanDeque.hpp
  Include/XalanList.hpp
  Include/XalanMap.hpp
  Include/XalanMemMgrAutoPtr.hpp
  Include/XalanMemoryManagement.hpp
  Include/XalanObjectCache.hpp
  Include/XalanObjectStackCache.hpp
  Include/XalanSet.hpp
  Include/XalanVector.hpp)

set(generated_include_headers
  ${CMAKE_CURRENT_BINARY_DIR}/Include/PlatformDefinitions.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/Include/XalanVersion.hpp)

set(icubridge_sources
  ICUBridge/ICUBridgeCleanup.cpp
  ICUBridge/ICUBridgeCollationCompareFunctor.cpp
  ICUBridge/ICUBridgeCollationCompareFunctorImpl.cpp
  ICUBridge/ICUBridge.cpp
  ICUBridge/ICUFormatNumberFunctor.cpp
  ICUBridge/ICUXalanNumberFormatFactory.cpp
  ICUBridge/ICUXalanNumberFormatProxy.cpp)

set(icubridge_headers
  ICUBridge/ICUBridgeCleanup.hpp
  ICUBridge/ICUBridgeCollationCompareFunctor.hpp
  ICUBridge/ICUBridgeCollationCompareFunctorImpl.hpp
  ICUBridge/ICUBridgeDefinitions.hpp
  ICUBridge/ICUBridge.hpp
  ICUBridge/ICUFormatNumberFunctor.hpp
  ICUBridge/ICUXalanNumberFormatFactory.hpp
  ICUBridge/ICUXalanNumberFormatProxy.hpp)

set(libxalan_c_SOURCES
  ${xalandom_sources}
  ${platformsupport_sources}
  ${domsupport_sources}
  ${xmlsupport_sources}
  ${xpath_sources}
  ${xercesparserliaison_sources}
  ${xercesparserliaison_deprecated_sources}
  ${xalansourcetree_sources}
  ${xpathcapi_sources}
  ${xslt_sources}
  ${xalanextensions_sources}
  ${xalantransformer_sources}
  ${xalanexslt_sources}
  ${harness_sources}
  ${include_sources}
)

set(libxalan_c_HEADERS
  ${xalandom_headers}
  ${platformsupport_headers}
  ${domsupport_headers}
  ${xmlsupport_headers}
  ${xpath_headers}
  ${xercesparserliaison_headers}
  ${xercesparserliaison_deprecated_headers}
  ${xalansourcetree_headers}
  ${xpathcapi_headers}
  ${xslt_headers}
  ${xalanextensions_headers}
  ${xalantransformer_headers}
  ${xalanexslt_headers}
  ${harness_headers}
  ${include_headers}
)

set(libxalan_c_GENERATED_HEADERS
  ${generated_include_headers})

set(libxalan_c_RESOURCES
  XalanC.rc)

if(transcoder STREQUAL "icu")
  list(APPEND libxalan_c_SOURCES ${icubridge_sources})
  list(APPEND libxalan_c_HEADERS ${icubridge_headers})
endif()

# Search the project binary dir for config.h
include_directories(
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Search the project binary dir for config.h
include_directories(
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(xalan-c-headers SOURCES ${libxalan_c_HEADERS} ${libxalan_c_GENERATED_HEADERS})
set_target_properties(xalan-c-headers PROPERTIES FOLDER "Library")

add_library(xalan-c
  ${libxalan_c_SOURCES}
  ${libxalan_c_HEADERS}
  ${libxalan_c_GENERATED_HEADERS}
  ${libxalan_c_RESOURCES})
target_compile_definitions(xalan-c PRIVATE "${MSGLOADER_DEF}=1")
if(BUILD_SHARED_LIBS)
  target_compile_definitions(xalan-c PRIVATE XALAN_BUILD_DLL=1)
endif()
target_include_directories(xalan-c PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/xalanc/PlatformSupport>
  $<INSTALL_INTERFACE:include/xalanc>)
if(transcoder STREQUAL "icu")
  target_compile_definitions(xalan-c PRIVATE XALAN_USE_ICU=1)
  target_link_libraries(xalan-c ICU::uc ICU::i18n ICU::data)
endif()
target_link_libraries(xalan-c XercesC::XercesC Threads::Threads)
if(msgloader STREQUAL "inmemory")
target_link_libraries(xalan-c XalanC::XalanMsg)
endif()

add_library(XalanC::XalanC ALIAS xalan-c)

if(MSVC)
  # Add configuration-specific library name to resource file.
  target_compile_definitions(xalan-c PRIVATE "XALAN_DLL_NAME=\"$<TARGET_FILE_NAME:xalan-c>\\0\"")
  # Different naming for Windows than for Unix builds for backward
  # compatibility with the existing project files.
  set_target_properties(xalan-c PROPERTIES OUTPUT_NAME "Xalan-C_${XALAN_MS_LIB_MAJOR_VER}")
  set_target_properties(xalan-c PROPERTIES RUNTIME_OUTPUT_NAME "Xalan-C_${XALAN_MS_LIB_MAJOR_VER}_${XALAN_MS_LIB_MINOR_VER}")
  set_target_properties(xalan-c PROPERTIES DEBUG_POSTFIX "D")
else()
  # Not used for the common cases, though this would be the default if
  # not for libtool compatibility.
  set_target_properties(xalan-c PROPERTIES SOVERSION ${XALAN_LIB_MAJOR_VER})
  if(NOT CYGWIN)
    # This property causes shared libraries on Linux to have the full
    # version encoded into their final filename.  We disable this on
    # Cygwin because it causes xalan-c-${XALAN_C_FULL_VERSION}.dll
    # to be created when xalan-c.dll seems to be the default.
    set_target_properties(xalan-c PROPERTIES VERSION ${XALAN_LIB_MAJOR_VER}.${XALAN_LIB_MINOR_VER})
  endif()
endif()
set_target_properties(xalan-c PROPERTIES FOLDER "Library")

if(WIN32)
  set(xalan_config_dir "cmake")
else()
  set(xalan_config_dir "${CMAKE_INSTALL_LIBDIR}/cmake/XalanC")
endif()

install(TARGETS xalan-c
  EXPORT XalanCConfigInternal
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT "runtime"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(EXPORT XalanCConfigInternal
        DESTINATION "${xalan_config_dir}"
        NAMESPACE "xalan_"
        COMPONENT "development")

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/XalanCConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/XalanCConfig.cmake"
  INSTALL_DESTINATION "${xalan_config_dir}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/XalanCConfigVersion.cmake
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/XalanCConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/XalanCConfigVersion.cmake
              DESTINATION "${xalan_config_dir}")

foreach(hdr IN LISTS libxalan_c_HEADERS)
  get_filename_component(hdrdir "${hdr}" DIRECTORY)
  install(
    FILES "${hdr}"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xalanc/${hdrdir}"
    COMPONENT "development")
endforeach()

foreach(hdr IN LISTS libxalan_c_GENERATED_HEADERS)
    get_filename_component(hdrdir "${hdr}" DIRECTORY)
    file(RELATIVE_PATH relhdrdir "${CMAKE_CURRENT_BINARY_DIR}" "${hdrdir}")
    install(
            FILES "${hdr}"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xalanc/${relhdrdir}"
            COMPONENT "development")
endforeach()

# Source file grouping (for IDE project layout)
set(source_files ${libxalan_c_SOURCES} ${libxalan_c_HEADERS})
list(SORT source_files)
unset(group_dir)
unset(group_files)
foreach(file IN LISTS source_files)
  get_filename_component(dir "${file}" PATH)
  if(group_dir AND NOT dir STREQUAL group_dir)
    if(group_files)
      source_group("${group_dir}" FILES ${group_files})
    endif()
    unset(group_files)
  endif()
  list(APPEND group_files "${file}")
  set(group_dir "${dir}")
endforeach()
if(group_files)
  source_group("${group_dir}" FILES ${group_files})
endif()
unset(group_dir)
unset(group_files)

add_executable(Xalan XalanExe/XalanExe.cpp)
target_link_libraries(Xalan XalanC::XalanC)
set_target_properties(Xalan PROPERTIES FOLDER "Programs")

install(TARGETS Xalan
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT "runtime")
