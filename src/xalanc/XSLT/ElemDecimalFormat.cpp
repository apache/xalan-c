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
#include "ElemDecimalFormat.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"


XALAN_CPP_NAMESPACE_BEGIN



static const XalanQNameByValue	s_empty;



ElemDecimalFormat::ElemDecimalFormat(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet& 					stylesheetTree,
			const AttributeListType&		atts,
			int 							lineNumber,
			int 							columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_DECIMAL_FORMAT),
	m_countMatchPattern(0),
	m_fromMatchPattern(0),
	m_valueExpr(0),
	m_qname(&s_empty),
	m_decimalFormatSymbols()
{
	m_decimalFormatSymbols.setInfinity(XalanDOMString());
	m_decimalFormatSymbols.setNaN(XalanDOMString());

	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const		aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			assert(atts.getValue(i) != 0);

			m_qname = constructionContext.createXalanQName(
						atts.getValue(i),
						getStylesheet().getNamespaces(),
						getLocator());

			if (m_qname->isValid() == false)
			{
				constructionContext.error(
						XalanMessageLoader::getMessage(
							XalanMessages::AttributeValueNotValidQName_2Param,
							Constants::ATTRNAME_NAME.c_str(),
							atts.getValue(i)),
						0,
						this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_DECIMALSEPARATOR))
		{
			const XalanDOMChar* const	decimalSepValue = atts.getValue(i);
			assert(decimalSepValue != 0);

			if (length(decimalSepValue) == 1)
			{
				m_decimalFormatSymbols.setDecimalSeparator(decimalSepValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_DECIMALSEPARATOR.c_str(),
						decimalSepValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_GROUPINGSEPARATOR))
		{
			const XalanDOMChar* const	sepValue = atts.getValue(i);
			assert(sepValue!= 0);

			if (length(sepValue) == 1)
			{
				m_decimalFormatSymbols.setGroupingSeparator(sepValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_GROUPINGSEPARATOR.c_str(),
						sepValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_INFINITY))
		{
			assert(atts.getValue(i) != 0);

			m_decimalFormatSymbols.setInfinity(atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_MINUSSIGN))
		{
			const XalanDOMChar* const	minusValue = atts.getValue(i);
			assert(minusValue != 0);

			if (length(minusValue) == 1)
			{
				m_decimalFormatSymbols.setMinusSign(minusValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_MINUSSIGN.c_str(),
						minusValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_NAN))
		{
			assert(atts.getValue(i) != 0);

			m_decimalFormatSymbols.setNaN(atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_PERCENT))
		{
			const XalanDOMChar* const	percentValue = atts.getValue(i);
			assert(percentValue != 0);

			if (length(percentValue) == 1)
			{
				m_decimalFormatSymbols.setPercent(percentValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_PERCENT.c_str(),
						percentValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_PERMILLE))
		{
			const XalanDOMChar* const	permilleValue = atts.getValue(i);
			assert(permilleValue != 0);

			if (length(permilleValue) == 1)
			{
				m_decimalFormatSymbols.setPerMill(permilleValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_PERMILLE.c_str(),
						permilleValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_ZERODIGIT))
		{
			const XalanDOMChar* const	zeroDigitValue = atts.getValue(i);
			assert(zeroDigitValue != 0);

			if (length(zeroDigitValue) == 1)
			{
				m_decimalFormatSymbols.setZeroDigit(zeroDigitValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_ZERODIGIT.c_str(),
						zeroDigitValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_DIGIT))
		{
			const XalanDOMChar* const	digitValue = atts.getValue(i);
			assert(digitValue != 0);

			if (length(digitValue) == 1)
			{
				m_decimalFormatSymbols.setDigit(digitValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_DIGIT.c_str(),
						digitValue),
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_PATTERNSEPARATOR))
		{
			const XalanDOMChar* const	patternSepValue = atts.getValue(i);
			assert(patternSepValue != 0);

			if (length(patternSepValue) == 1)
			{
				m_decimalFormatSymbols.setPatternSeparator(patternSepValue[0]);
			}
			else
			{
				constructionContext.warn(
					XalanMessageLoader::getMessage(
						XalanMessages::ElementHasIllegalValue_3Param,
						Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
						Constants::ATTRNAME_PATTERNSEPARATOR.c_str(),
						patternSepValue),
					0,
					this);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
				XalanMessageLoader::getMessage(
					XalanMessages::TemplateHasIllegalAttribute_2Param,
					Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING.c_str(),
					aname),
				0,
				this);
		}
	}

	if (length(m_decimalFormatSymbols.getInfinity()) == 0)
	{
		m_decimalFormatSymbols.setInfinity(Constants::ATTRVAL_INFINITY);
	}

	if (length(m_decimalFormatSymbols.getNaN()) == 0)
	{
		m_decimalFormatSymbols.setNaN(Constants::ATTRVAL_NAN);
	}

	// Look for duplicate decimal-format names
	const XalanDecimalFormatSymbols* const	theOther =
			stylesheetTree.getDecimalFormatSymbols(*m_qname);

	if (theOther != 0 && *theOther != m_decimalFormatSymbols)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::DuplicateDefinitions_1Param,
				Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING),
			0,
			this);
	}
}



ElemDecimalFormat::~ElemDecimalFormat()
{
}



const XalanDOMString&
ElemDecimalFormat::getElementName() const
{
	return Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING;
}



void
ElemDecimalFormat::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);
}



const XPath*
ElemDecimalFormat::getXPath(unsigned int	index) const
{
	const XPath*	result = 0;

	switch(index)
	{
	case 0:
		result = m_valueExpr;
		break;

	case 1:
		result = m_countMatchPattern;
		break;

	case 2:
		result = m_fromMatchPattern;
		break;

	default:
		break;
	}

	return result;
}



XALAN_CPP_NAMESPACE_END
