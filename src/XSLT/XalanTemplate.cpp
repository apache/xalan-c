/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

#include <string>
#include <stl/_string.c>
#include <stl/_string_fwd.c>



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
#include <PlatformSupport/XalanTranscodingServices.hpp>
#include <PlatformSupport/XalanDOMStringCache.hpp>	



#include <DOMSupport/NamespaceResolver.hpp>
#include <DOMSupport/NSInfo.hpp>



#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToText.hpp>



#include <XPath/FunctionID.hpp>
#include <XPath/NodeRefList.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathExpression.hpp>
#include <XPath/XPathFactoryBlock.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathFunctionTable.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include <XercesParserLiaison/XercesDocumentBridge.hpp>
#include <XercesParserLiaison/XercesNamedNodeListCache.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



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
#include <XSLT/ElemNumber.hpp>
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



static string theString;
static vector<XalanDOMString> theDOMStringVector;
static vector<char> theCharVector;
static vector<wchar_t> theWCharVector;
static vector<unsigned char> theUnsignedCharVector;
static AttributeListImpl::AttributeVectorType theAttributeVectorEntryVector;
static AttributesImpl::AttributesVectorType theAttributesVectorEntryVector;
static allocator<DOMString> theAllocator;
static vector<pair<const XalanNode*,NSInfo> > theXalanNodeVector;
static FormatterToHTML::ElemDesc theElemDesc;
static FormatterToHTML::ElementFlagsMapType theElementFlagsMapType;
static XPathExpression::TokenQueueType theTokenQueueType;
static set<const XalanNode*,less<const XalanNode*> > theXalanNodeSet;
static XPathExecutionContext::XObjectArgVectorType theVector;
static XPathProcessorImpl::NodeTypesMapType theNodeTypesMapType;
static Stylesheet::PatternTableListType	thePatternTableList;
static Stylesheet::PatternTableVectorType thePatternTableVector;
static map<int,int,less<int> > theIntMap;
static ElemNumber::DecimalToRomanVectorType theDecimalToRomanVector;
static vector<NamespacesHandler::NamespacesMapType::iterator> theNamespacesMapTypeIteratorVector;
static VariablesStack::ParamsVectorType	theParamsVector;
static ElemNumber::NumberingResourceBundleMapType theNumberingResourceBundleMapType;
static XalanTranscodingServices::MaximumCharacterValueMapType theMaximumCharacterValueMapType;
static set<XalanNode*, less<XalanNode*> >	theInstanceSetType;
static XalanTransformer::CompiledStylesheetPtrVectorType	theCompiledStylesheetVector;
static XalanTransformer::ParsedSourcePtrVectorType			theParsedSourceVector;
static XalanTransformer::ParamPairVectorType				theParamsPairVector;
static XalanDOMStringHashTable::BucketCountsType			theBucketCountsVector;
static vector<pair<const char*, const char*> >				theStringPairVector;



static void
foo(XPathExecutionContext&	theExecutionContext)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
	using std::replace;
#endif

	{		
		XObjectFactoryDefault::XObjectCollectionType 	theVector;
		XObjectFactoryDefault			theFactory;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 XObjectFactoryDefault::DeleteXObjectFunctor(theFactory, true));
	}
	
	{	
		XPathFactoryDefault::CollectionType 	theVector;	
		XPathFactoryDefault			theXPath;		

		for_each(theVector.begin(),
			 theVector.end(),
			 XPathFactoryDefault::DeleteXPathFunctor(theXPath, true));
	}
	
	{		
		XPathFunctionTable::CollectionType theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 XPathFunctionTable::DeleteFunctorType());
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

		for_each(theValue.second.begin(),
			 theValue.second.end(),
			 MapValueDeleteFunctor<FunctionTableType>());
	}
	
	{
		XPathEnvSupportDefault::FunctionTableType	theTable;

		for_each(theTable.begin(),
			 theTable.end(),
			 makeMapValueDeleteFunctor(theTable));
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
		XercesParserLiaison::DocumentMapType	theMap;
		
		for_each(theMap.begin(),
			 theMap.end(),
			 MapValueDeleteFunctor<XercesParserLiaison::DocumentMapType>());
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
		StylesheetHandler::ElemTextLiteralStackType	theVector;

		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemTextLiteral>());
	}

	{
		const VariablesStack::ParamsVectorType	theVector;
		VariablesStack 				theVariablesStack;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 VariablesStack::PushParamFunctor(theVariablesStack));
	}

	{
		ElemForEach::SortElemsVector	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<ElemSort>());
	}

	{
		AVT::AVTPartPtrVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<AVTPart>());
	}

	{
		ElemLiteralResult::AVTVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<AVT>());
	}

	{
		StylesheetConstructionContextDefault::StylesheetSetType	theSet;
		
		for_each(
			theSet.begin(),
			theSet.end(),
			DeleteFunctor<StylesheetRoot>());
	}

	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::sort;
		using std::less;
#endif

		StylesheetRoot::QNameVectorType		theVector;

		sort(
				theVector.begin(),
				theVector.end(),
				less<QName>());
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
		XalanDOMCharVectorType	theVector;

		replace(
			theVector.begin(),
			theVector.end(),
			XalanDOMCharVectorType::value_type(XalanUnicode::charReverseSolidus),
			XalanDOMCharVectorType::value_type(XalanUnicode::charSolidus));
	}
	
	{
		NodeRefList::NodeListVectorType theVector;

		remove(	
			theVector.begin(),
			theVector.end(),
			NodeRefList::NodeListVectorType::value_type(0));
	}

	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::back_inserter;
		using std::copy;
#endif

		typedef MutableNodeRefList::addNodeInDocOrderFunctor	addNodeInDocOrderFunctor;

		MutableNodeRefList				theList;
		NodeRefList::NodeListVectorType theVector;

		copy(
			nodelist.m_nodeList.rbegin(),
			nodelist.m_nodeList.rend(),
			back_inserter(m_nodeList));

		addNodeInDocOrderFunctor	theFunctor(theList, executionContext)

		for_each(
			theVector.begin(),
			theVector.end(),
			theFunctor);

		for_each(
			nodelist.m_nodeList.rbegin(),
			nodelist.m_nodeList.rend(),
			theFunctor);
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
}



#include <stl/_alloc.h>
#include <stl/_alloc.c>


__node_alloc<0,0> alloc1;
__node_alloc<1,0> alloc2;



#endif
