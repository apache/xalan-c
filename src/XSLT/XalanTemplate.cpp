/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */


// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



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



#include <XalanDOM/XalanNode.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/AttributeVectorEntry.hpp>
#include <PlatformSupport/AttributesImpl.hpp>
#include <PlatformSupport/AttributeVectorEntryExtended.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanDOMStringHashTable.hpp>
#include <PlatformSupport/PrintWriter.hpp>
#include <PlatformSupport/XalanOutputStream.hpp>
#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanDOMStringCache.hpp>	



#include <XPath/NodeRefList.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathExpression.hpp>
#include <XPath/XPathFactoryBlock.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XercesParserLiaison/XercesDocumentBridge.hpp>
#include <XercesParserLiaison/XercesDocumentWrapper.hpp>
#include <XercesParserLiaison/XercesNamedNodeListCache.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>
#include <XercesParserLiaison/XercesToXalanNodeMap.hpp>



#include <XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <XalanSourceTree/XalanSourceTreeElement.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include <XalanSourceTree/XalanSourceTreeContentHandler.hpp>



#include <XSLT/AVT.hpp>
#include <XSLT/AVTPart.hpp>
#include <XSLT/ElemAttributeSet.hpp>
#include <XSLT/ElemDecimalFormat.hpp>
#include <XSLT/ElemForEach.hpp>
#include <XSLT/ElemLiteralResult.hpp>
#include <XSLT/ElemSort.hpp>
#include <XSLT/ElemTextLiteral.hpp>
#include <XSLT/ElemUse.hpp>
#include <XSLT/ElemVariable.hpp>
#include <XSLT/ExtensionFunctionHandler.hpp>
#include <XSLT/ExtensionNSHandler.hpp>
#include <XSLT/KeyTable.hpp>
#include <XSLT/NamespacesHandler.hpp>
#include <XSLT/NodeSorter.hpp>
#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/StylesheetHandler.hpp>
#include <XSLT/TraceListener.hpp>
#include <XSLT/VariablesStack.hpp>
#include <XSLT/XalanNumberingResourceBundle.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>



#include <XalanTransformer/XalanTransformer.hpp>
#include <XalanTransformer/XalanCompiledStylesheet.hpp>
#include <XalanTransformer/XalanParsedSource.hpp>



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
		Stylesheet::PatternTableListType	thePatternTableList;
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
		const Stylesheet::PatternTableListType	theList;
		
		for_each(theList.begin(),
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
		XALAN_UISNG_STD(sort)

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
		XALAN_UISNG_STD(back_inserter)
		XALAN_UISNG_STD(copy)

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
