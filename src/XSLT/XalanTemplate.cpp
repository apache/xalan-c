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

#include <limits>
#include <stl/_limits.c>

#include <algorithm>
#include <stl/_algo.c>
#include <stl/_algobase.c>

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
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrintWriter.hpp>
#include <PlatformSupport/XalanOutputStream.hpp>
#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanTranscodingServices.hpp>	



#include <DOMSupport/NamespaceResolver.hpp>
#include <DOMSupport/NSInfo.hpp>



#include <XMLSupport/FormatterListener.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToDOM.hpp>



#include <XPath/FunctionID.hpp>
#include <XPath/NodeRefList.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathExpression.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathFunctionTable.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include <XercesParserLiaison/XercesDocumentBridge.hpp>
#include <XercesParserLiaison/XercesNamedNodeListCache.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



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



static numeric_limits<double> theDoubleLimits;
static string theString;
static vector<XalanDOMString> theDOMStringVector;
static vector<char> theCharVector;
static vector<unsigned char> theUnsignedCharVector;
static AttributeListImpl::AttributeVectorType theAttributeVectorEntryVector;
static allocator<DOMString> theAllocator;
static vector<pair<const XalanNode*,NSInfo> > theXalanNodeVector;
static FormatterToHTML::ElemDesc theElemDesc;
static FormatterToHTML::ElementFlagsMapType theElementFlagsMapType;
static XPathExpression::TokenQueueType theTokenQueueType;
static set<const XalanNode*,less<const XalanNode*> > theXalanNodeSet;
static XPathExpression::OpCodeLengthMapType theOpCodeLengthMapType;
static XPathExpression::OpCodeMapValueType theOpCodeMapValueType;
static XPathExpression::NodeTestSetType theNodeTestSetType;
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

static void
foo()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
	using std::replace;
#endif

	{		
		XObjectFactoryDefault::CollectionType 	theVector;
		XObjectFactoryDefault			theFactory;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 XObjectFactoryDefault::DeleteXObjectFunctor(theFactory, true));
	}
	
	{		
		StylesheetExecutionContextDefault::LiveVariablesStackType 	theStack;
		XObjectFactoryDefault			theFactory;
		
		for_each(theStack.back().begin(),
			 theStack.back().end(),
			 XObjectFactoryDefault::DeleteXObjectFunctor(theFactory));
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
		AttributeListImpl::AttributeVectorType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<AttributeListImpl::AttributeVectorEntry>());
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
		XPathExecutionContextDefault::NodeRefListCacheType	theVector;
		
		for_each(theVector.begin(),
			 theVector.end(),
			 DeleteFunctor<MutableNodeRefList>());
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
		Stylesheet::AttributeSetMapType	theMap;
		
		for_each(theMap.begin(),
			 theMap.end(),
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
		
		for_each(theSet.begin(),
			 theSet.end(),
			 DeleteFunctor<StylesheetRoot>());
	}

	{
		StylesheetExecutionContextDefault::FormatterListenerSetType	theSet;
		
		for_each(theSet.begin(),
			 theSet.end(),
			 DeleteFunctor<FormatterListener>());
	}

	{
		StylesheetExecutionContextDefault::PrintWriterSetType	theSet;
		
		for_each(theSet.begin(),
			 theSet.end(),
			 DeleteFunctor<PrintWriter>());
	}

	{
		StylesheetExecutionContextDefault::OutputStreamSetType	theSet;
		
		for_each(theSet.begin(),
			 theSet.end(),
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
		vector<XalanNode*> theVector;
		NodeSorter::NodeSortKeyCompare* theComparer;
		
		stable_sort(	
			theVector.begin(),
			theVector.end(),
			*theComparer);
	}
	
	{
		ostream* const	theStream = 0;
		const string	theString;

		*theStream << theString;
	}
}



#include <stl/_alloc.h>
//#undef __SGI_STL_OWN_IOSTREAMS
#include <stl/_alloc.c>


__node_alloc<0,0> alloc1;
__node_alloc<1,0> alloc2;

#endif