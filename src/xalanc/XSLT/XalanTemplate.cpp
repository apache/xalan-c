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


// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#if !defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)

	// No-op function to allow file to compile and link.	
	static void foo(){}

#else

#include <stl/_range_errors.h>

#include <algorithm>
#include <stl/_algo.c>
#include <stl/_algobase.c>
#include <stl/_heap.c>

#include <list>
#include <stl/_list.c>

#include <vector>
#include <stl/_vector.c>

#include <map>
#include <stl/_tree.c>

#include <deque>
#include <stl/_deque.c>



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/AttributeVectorEntry.hpp>
#include <xalanc/PlatformSupport/AttributesImpl.hpp>
#include <xalanc/PlatformSupport/AttributeVectorEntryExtended.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringHashTable.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringCache.hpp>



#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>



#include <xalanc/XPath/NodeRefList.hpp>
#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathExpression.hpp>
#include <xalanc/XPath/XPathFactoryBlock.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>



#include <xalanc/XercesParserLiaison/XercesDocumentBridge.hpp>
#include <xalanc/XercesParserLiaison/XercesDocumentWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesNamedNodeListCache.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesToXalanNodeMap.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeElement.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeContentHandler.hpp>



#include <xalanc/XSLT/AVT.hpp>
#include <xalanc/XSLT/AVTPart.hpp>
#include <xalanc/XSLT/ElemAttributeSet.hpp>
#include <xalanc/XSLT/ElemDecimalFormat.hpp>
#include <xalanc/XSLT/ElemForEach.hpp>
#include <xalanc/XSLT/ElemLiteralResult.hpp>
#include <xalanc/XSLT/ElemSort.hpp>
#include <xalanc/XSLT/ElemTextLiteral.hpp>
#include <xalanc/XSLT/ElemUse.hpp>
#include <xalanc/XSLT/ElemVariable.hpp>
#include <xalanc/XSLT/ExtensionFunctionHandler.hpp>
#include <xalanc/XSLT/ExtensionNSHandler.hpp>
#include <xalanc/XSLT/KeyTable.hpp>
#include <xalanc/XSLT/NamespacesHandler.hpp>
#include <xalanc/XSLT/NodeSorter.hpp>
#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/StylesheetHandler.hpp>
#include <xalanc/XSLT/TraceListener.hpp>
#include <xalanc/XSLT/VariablesStack.hpp>
#include <xalanc/XSLT/XalanNumberingResourceBundle.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>
#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



#if defined(XALAN_USE_ICU)
#include <xalanc/ICUBridge/ICUBridgeCollationCompareFunctorImpl.hpp>
#endif



static void
foo(XPathExecutionContext&	theExecutionContext)
{
	XALAN_USING_STD(for_each)
	XALAN_USING_STD(replace)

	{
		vector<XalanDOMString> theDOMStringVector;
		vector<char> theCharVector;
		vector<wchar_t> theWCharVector;
		vector<unsigned char> theUnsignedCharVector;
		AttributeListImpl::AttributeVectorType theAttributeVectorEntryVector;
		AttributesImpl::AttributesVectorType theAttributesVectorEntryVector;
		allocator<DOMString> theAllocator;
		XPathExpression::TokenQueueType theTokenQueueType;
		set<const XalanNode*,less<const XalanNode*> > theXalanNodeSet;
		XPathExecutionContext::XObjectArgVectorType theVector;
		Stylesheet::PatternTableVectorType thePatternTableVector;
		map<int,int,less<int> > theIntMap;
		vector<NamespacesHandler::NamespacesMapType::iterator> theNamespacesMapTypeIteratorVector;
		VariablesStack::ParamsVectorType	theParamsVector;
		set<XalanNode*, less<XalanNode*> >	theInstanceSetType;
		XalanTransformer::CompiledStylesheetPtrVectorType	theCompiledStylesheetVector;
		XalanTransformer::ParsedSourcePtrVectorType			theParsedSourceVector;
		XalanTransformer::ParamPairVectorType				theParamsPairVector;
		XalanTransformer::TraceListenerVectorType			theTraceListenerVector;
		XalanDOMStringHashTable::BucketCountsType			theBucketCountsVector;
		vector<pair<const char*, const char*> >				theStringPairVector;
	}

	{
		XObjectFactoryDefault::XObjectCollectionType 	theVector;
		XObjectFactoryDefault			theFactory;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 XObjectFactoryDefault::DeleteXObjectFunctor(theFactory, true));
	}
	
	{
		XPathFactoryDefault::CollectionType 	theVector;	
		XPathFactoryDefault						theXPath;		

		for_each(theVector.begin(),
			 theVector.end(),
			 XPathFactoryDefault::DeleteXPathFunctor(theXPath, true));
	}

	{
		XalanDOMStringCache::StringListType theVector;	
				
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<XalanDOMString>());
	}

	{
		AttributeListImpl::AttributeVectorType	theVector;
		
		for_each(theVector.begin(),
				 theVector.end(),
				 DeleteFunctor<AttributeVectorEntry>());
	}	

	{
		AttributesImpl::AttributesVectorType	theVector;
		
		for_each(theVector.begin(),
				 theVector.end(),
				 DeleteFunctor<AttributeVectorEntryExtended>());
	}	

	{
		typedef XPathEnvSupportDefault::NamespaceFunctionTableDeleteFunctor		NamespaceFunctionTableDeleteFunctor;
	
		typedef NamespaceFunctionTableDeleteFunctor::FunctionTableInnerType 			FunctionTableType;
		typedef NamespaceFunctionTableDeleteFunctor::NamespaceFunctionTablesInnerType 	NamespaceFunctionTablesType;
	
		NamespaceFunctionTablesType	theTable;
		
		const NamespaceFunctionTablesType::value_type		theValue;
		
		for_each(theTable.begin(),
			 theTable.end(),
			 NamespaceFunctionTableDeleteFunctor());
	}
	
	{
		XercesDocumentBridge::NodeVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<XalanNode>());
	}

	{
		XercesNamedNodeListCache::NodeListCacheType		theCache;
		
		for_each(theCache.begin(),
			 theCache.end(),
			 MapValueDeleteFunctor<XercesNamedNodeListCache::NodeListCacheType>());
	}

	{
		Stylesheet::AttributeSetVectorType	theVector;
		
		for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<ElemAttributeSet>());
	}

	{
		StylesheetExecutionContextDefault::KeyTablesTableType	theTable;

		for_each(theTable.begin(),
			 theTable.end(),
			 MapValueDeleteFunctor<StylesheetExecutionContextDefault::KeyTablesTableType>());
	}

	{
		Stylesheet::ExtensionNamespacesMapType	theMap;

		for_each(theMap.begin(),
			 theMap.end(),
			 makeMapValueDeleteFunctor(theMap));
	}

	{
		Stylesheet::StylesheetVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<Stylesheet>());
	}

	{
		Stylesheet::ElemDecimalFormatVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemDecimalFormat>());
	}

	{
		Stylesheet::ElemVariableVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemVariable>());
	}

	{
		Stylesheet::ElemVariableVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemVariable>());
	}

	{
		const Stylesheet::PatternTableVectorType	theList;

		for_each(
			theList.begin(),
			theList.end(),
			DeleteFunctor<Stylesheet::MatchPattern2>());
	}

	{
		StylesheetHandler::ElemTemplateStackType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemTemplateElement>());
	}

	{
		const VariablesStack::ParamsVectorType	theVector;
		VariablesStack 							theVariablesStack;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 VariablesStack::PushParamFunctor(theVariablesStack));
	}

	{
		ElemForEach::SortElemsVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemSort>());
	}

	{
		StylesheetConstructionContextDefault::StylesheetVectorType	theVector;
		
		StylesheetRoot*		theStylesheetRoot;

		find(
			theVector.begin(),
			theVector.end(),
			theStylesheetRoot);

		for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<StylesheetRoot>());
	}

	{
		XALAN_USING_STD(sort)

		StylesheetRoot::XalanQNameVectorType	theVector;

		sort(
				theVector.begin(),
				theVector.end(),
				pointer_less<XalanQName>());
	}

	{
		StylesheetExecutionContextDefault::FormatterListenerVectorType	theVector;
		
		for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<FormatterListener>());
	}

	{
		StylesheetExecutionContextDefault::PrintWriterVectorType	theVector;
		
		for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<PrintWriter>());
	}

	{
		StylesheetExecutionContextDefault::OutputStreamVectorType	theVector;

		for_each(
			theVector.begin(),
			theVector.end(),
			DeleteFunctor<XalanOutputStream>());
	}

	{
		typedef StylesheetExecutionContextDefault::XPathCacheMapType		XPathCacheMapType;
		typedef StylesheetExecutionContextDefault::XPathCacheReturnFunctor	XPathCacheReturnFunctor;

		XPathCacheMapType	theMap;

		XSLTEngineImpl*	const	xsltProcessor = 0;

		for_each(theMap.begin(),
			 theMap.end(),
			 XPathCacheReturnFunctor(*xsltProcessor));
	}


	{
		XSLTEngineImpl::TraceListenerVectorType	theVector;
		
		const GenerateEvent*	theEvent;

		for_each(
			theVector.begin(),
			theVector.end(),
			TraceListener::TraceListenerGenerateFunctor(*theEvent));
	}

	{
		XSLTEngineImpl::TraceListenerVectorType	theVector;
		
		const SelectionEvent*	theEvent;

		for_each(
			theVector.begin(),
			theVector.end(),
			TraceListener::TraceListenerSelectFunctor(*theEvent));
	}

	{
		XSLTEngineImpl::TraceListenerVectorType	theVector;
		
		const TracerEvent*	theEvent;

		for_each(
			theVector.begin(),
			theVector.end(),
			TraceListener::TraceListenerTraceFunctor(*theEvent));
	}
	
	{
		XalanSourceTreeParserLiaison::DocumentMapType	theMap;

		for_each(theMap.begin(),
			 theMap.end(),
			 makeMapValueDeleteFunctor(theMap));
	}

	{
		XSLTEngineImpl::TraceListenerVectorType		theVector;

		remove(
			theVector.begin(),
			theVector.end(),
		 	XSLTEngineImpl::TraceListenerVectorType::value_type(0));
	}

	{
		XSLTEngineImpl::XalanDOMStringPointerVectorType		theVector;
		
		XalanDOMString	nodeName;

		find_if(
			theVector.begin(),
			theVector.end(),
			XSLTEngineImpl::FindStringPointerFunctor(nodeName));
	}


	{
		XalanDOMString	theString;

		replace(
			theString.begin(),
			theString.end(),
			XalanDOMChar(XalanUnicode::charReverseSolidus),
			XalanDOMChar(XalanUnicode::charSolidus));
	}
	
	{
		NodeRefList::NodeListVectorType theVector;

		remove(	
			theVector.begin(),
			theVector.end(),
			NodeRefList::NodeListVectorType::value_type(0));
	}

	{
		XALAN_USING_STD(back_inserter)
		XALAN_USING_STD(copy)

		typedef MutableNodeRefList::addNodeInDocOrderFunctor	addNodeInDocOrderFunctor;

		{
			NodeRefList::NodeListVectorType theVector;

			copy(
				theVector.rbegin(),
				theVector.rend(),
				back_inserter(theVector));
		}

		{
			MutableNodeRefList	theList;

			MutableNodeRefList::addNodeInDocOrderFunctor	theFunctor(theList, theExecutionContext);

			const NodeRefList::NodeListVectorType	theConstVector;

			for_each(
				theConstVector.begin(),
				theConstVector.end(),
				theFunctor);

			for_each(
				theConstVector.rbegin(),
				theConstVector.rend(),
				theFunctor);
		}
	}

	{
		NodeSorter::NodeVectorType			theVector;
		NodeSorter::NodeSortKeyCompare*		theComparer;

		stable_sort(	
			theVector.begin(),
			theVector.end(),
			*theComparer);
	}

	{
		XalanTransformer::CompiledStylesheetPtrVectorType	theVector;
		
		for_each(theVector.begin(),
				 theVector.end(),
				 DeleteFunctor<XalanCompiledStylesheet>());
	}	

	{
		XalanTransformer::ParsedSourcePtrVectorType		theVector;

		for_each(theVector.begin(),
				 theVector.end(),
				 DeleteFunctor<XalanParsedSource>());
	}

#if defined(XALAN_USE_ICU)
	{
		ICUBridgeCollationCompareFunctorImpl::CollatorCacheListType  theCache;

		for_each(
			theCache.begin(),
			theCache.end(),
			ICUBridgeCollationCompareFunctorImpl::CollationCacheStruct::CollatorDeleteFunctor());

		find_if(
			theCache.begin(),
			theCache.end(),
			ICUBridgeCollationCompareFunctorImpl::CollationCacheStruct::CollatorFindFunctor(0));
	}
#endif

#if __SGI_STL_PORT >= 452

	{
		VariablesStack::RecursionGuardStackType		theStack;

		const ElemVariable* const	var = 0;

		find(
			theStack.begin(),
			theStack.end(),
			var);
	}

	{
		XalanTransformer::CompiledStylesheetPtrVectorType	theVector;

		const XalanCompiledStylesheet*	theStylesheet = 0;

		find(
			theVector.begin(),
			theVector.end(),
			theStylesheet);
	}

	{
		XalanTransformer::ParsedSourcePtrVectorType		theVector;

		const XalanParsedSource*	theParsedSource = 0;

		find(
			theVector.begin(),
			theVector.end(),
			theParsedSource);
	}

	{
		const XMLCh* const name = 0;

		AttributeListImpl::AttributeVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributeListImpl::NameCompareFunctor(name));
	}

	{
		const XMLCh* const name = 0;

		const AttributeListImpl::AttributeVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributeListImpl::NameCompareFunctor(name));
	}

	{
		const XMLCh* const	uri = 0;
		const XMLCh* const	localName = 0;

		const AttributesImpl::AttributesVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributesImpl::URIAndLocalNameCompareFunctor(uri, localName));
	}

	{
		const XMLCh* const	uri = 0;
		const XMLCh* const	localName = 0;

		const AttributesImpl::AttributesVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributesImpl::URIAndLocalNameCompareFunctor(uri, localName));
	}

	{
		const XMLCh* const name = 0;

		AttributesImpl::AttributesVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributesImpl::NameCompareFunctor(name));
	}

	{
		const XMLCh* const name = 0;

		const AttributesImpl::AttributesVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			AttributesImpl::NameCompareFunctor(name));
	}

	{
		const XalanDOMChar*		theString = 0;

		const XalanDOMStringHashTable::BucketType		theBucket;

		find_if(
			theBucket.begin(),
			theBucket.end(),
			XalanDOMStringHashTable::equalsXalanDOMString(theString, 0));
	}

	{
		const XalanNode*	theXalanNode = 0;

		const XercesToXalanNodeMap::XercesNodeMapType	theMap;

		find_if(
			theMap.begin(),
			theMap.end(),
			XercesToXalanNodeMap::NameMapEqualsFunctor(theXalanNode));
	}

	{
		const XalanNode*	theXalanNode = 0;

		NodeRefList::NodeListVectorType		theVector;

		find(
			theVector.begin(),
			theVector.end(),
			theXalanNode);
	}

	{
		const XalanNode*	theXalanNode = 0;

		const NodeRefList::NodeListVectorType	theVector;

		find(
			theVector.begin(),
			theVector.end(),
			theXalanNode);
	}

	{
		const Stylesheet::MatchPattern2*	thePattern = 0;

		Stylesheet::PatternTableVectorType	theVector;

		find(
			theVector.begin(),
			theVector.end(),
			thePattern);
	}

	{
		XalanNode*	theXalanNode = 0;

		XercesDocumentBridge::NodeVectorType	theVector;

		find(
			theVector.begin(),
			theVector.end(),
			theXalanNode);
	}

	{
		XalanDOMString*		theString = 0;

		XalanDOMStringCache::StringListType		theList;

		find(
			theList.begin(),
			theList.end(),
			theString);
	}

	{
		const XalanQName*	theQName;

		const StylesheetRoot::XalanQNameVectorType	theVector;

		find_if(
			theVector.begin(),
			theVector.end(),
			pointer_equals_predicate<XalanQName>(theQName));
	}

	{
		const StylesheetHandler::BoolStackType	theStack;

		find(
			theStack.rbegin(),
			theStack.rend(),
			true);
	}

	{
		const ElemTemplateElement*	theElement = 0;

		const StylesheetExecutionContextDefault::ElementRecursionStackType	theStack;

		find(
			theStack.begin(),
			theStack.end(),
			theElement);
	}

	{
		XObjectFactoryDefault::XObjectCollectionType	theCollection;	
		XObject*										theXObject;		

		find(
			theCollection.begin(),
			theCollection.end(),
			theXObject);
	}

	{
		XPath::TargetDataVectorType		theVector;
	}
#endif
}



#include <stl/_alloc.h>
#include <stl/_alloc.c>


void
foo2()
{
	__node_alloc<0,0> alloc1;
	__node_alloc<1,0> alloc2;
}



#endif
