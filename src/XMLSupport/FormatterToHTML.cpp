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
 */

 /**
 * @author David N. Bertoni <david_n_bertoni@lotus.com>
 */



// Class header file.
#include "FormatterToHTML.hpp"



#include <cassert>
#include <climits>
#include <map>



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/Writer.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#if !defined(XALAN_LSTRSUPPORT)

#include <vector>

#if defined(XALAN_NO_NAMESPACES)
typedef vector<XalanDOMString>			XalanDOMStringVectorType;
#else
typedef std::vector<XalanDOMString>		XalanDOMStringVectorType;
#endif

static const char* const	theHTMLSymbols1Narrow[] = 
{
	"Alpha",    "Beta",
	"Gamma",    "Delta",    "Epsilon",    "Zeta",
	"Eta",    "Theta",    "Iota",    "Kappa",
	"Lambda",    "Mu",    "Nu",    "Xi",
	"Omicron",    "Pi",    "Rho",    "",  "Sigma",
	"Tau",    "Upsilon",    "Phi",    "Chi",
	"Psi",    "Omega"
};

static const char* const	theHTMLSymbols2Narrow[] = 
{
	"alpha",    "beta",
	"gamma",    "delta",    "epsilon",    "zeta",
	"eta",    "theta",    "iota",    "kappa",
	"lambda",    "mu",    "nu",    "xi",
	"omicron",    "pi",    "rho",    "sigmaf",
	"sigma",    "tau",    "upsilon",    "phi",
	"chi",    "psi",    "omega",    "thetasym",
	"upsih",    "piv"
};

static XalanDOMStringVectorType		theHTMLSymbols1;
static XalanDOMStringVectorType		theHTMLSymbols2;

#else
static const XalanDOMChar* const	theHTMLSymbols1[] =
{
	L"Alpha",    L"Beta",
	L"Gamma",    L"Delta",    L"Epsilon",    L"Zeta",
	L"Eta",    L"Theta",    L"Iota",    L"Kappa",
	L"Lambda",    L"Mu",    L"Nu",    L"Xi",
	L"Omicron",    L"Pi",    L"Rho",   L"",   L"Sigma",
	L"Tau",    L"Upsilon",    L"Phi",    L"Chi",
	L"Psi",    L"Omega"
};

static const XalanDOMChar* const	theHTMLSymbols2[] = 
{
    L"alpha",    L"beta",
	L"gamma",    L"delta",    L"epsilon",    L"zeta",
	L"eta",    L"theta",    L"iota",    L"kappa",
	L"lambda",    L"mu",    L"nu",    L"xi",
	L"omicron",    L"pi",    L"rho",    L"sigmaf",
	L"sigma",    L"tau",    L"upsilon",    L"phi",
	L"chi",    L"psi",    L"omega",    L"thetasym",
	L"upsih",    L"piv"
};
#endif


#if !defined(XALAN_LSTRSUPPORT)

static const char* const	theHTMLLatin1SymbolsNarrow[] = 
{
	"nbsp",    "iexcl",    "cent",    "pound",
	"curren",    "yen",    "brvbar",    "sect",
	"uml",    "copy",    "ordf",    "laquo", 
	"not",    "shy",    "reg",    "macr",    "deg",
	"plusmn",    "sup2",    "sup3",    "acute",
	"micro",    "para",    "middot",    "cedil",
	"sup1",    "ordm",    "raquo",    "frac14",
	"frac12",    "frac34",    "iquest",
	"Agrave",    "Aacute",    "Acirc",
	"Atilde",    "Auml",    "Aring",    "AElig",
	"Ccedil",    "Egrave",    "Eacute",    "Ecirc",    
	"Euml",    "Igrave",    "Iacute",    "Icirc",
	"Iuml",    "ETH",    "Ntilde",    "Ograve",
	"Oacute",    "Ocirc",    "Otilde",    "Ouml",
	"times",    "Oslash",    "Ugrave",    "Uacute",
	"Ucirc",    "Uuml",    "Yacute",    "THORN",
	"szlig",    "agrave",    "aacute",    "acirc",
	"atilde",    "auml",    "aring",    "aelig",
	"ccedil",    "egrave",    "eacute",    "ecirc",
	"euml",    "igrave",    "iacute",    "icirc",
	"iuml",    "eth",    "ntilde",    "ograve",
	"oacute",    "ocirc",    "otilde",    "ouml",
	"divide",    "oslash",    "ugrave",    "uacute",
	"ucirc",    "uuml",    "yacute",    "thorn",
	"yuml"
};


static XalanDOMStringVectorType		theHTMLLatin1Symbols;


#else
static const XMLCh* const	theHTMLLatin1Symbols[] = 
{
	L"nbsp",    L"iexcl",    L"cent",    L"pound",
	L"curren",    L"yen",    L"brvbar",    L"sect",
	L"uml",    L"copy",    L"ordf",    L"laquo", 
	L"not",    L"shy",    L"reg",    L"macr",    L"deg",
	L"plusmn",    L"sup2",    L"sup3",    L"acute",
	L"micro",    L"para",    L"middot",    L"cedil",
	L"sup1",    L"ordm",    L"raquo",    L"frac14",
	L"frac12",    L"frac34",    L"iquest",
	L"Agrave",    L"Aacute",    L"Acirc",
	L"Atilde",    L"Auml",    L"Aring",    L"AElig",
	L"Ccedil",    L"Egrave",    L"Eacute",    L"Ecirc",    
	L"Euml",    L"Igrave",    L"Iacute",    L"Icirc",
	L"Iuml",    L"ETH",    L"Ntilde",    L"Ograve",
	L"Oacute",    L"Ocirc",    L"Otilde",    L"Ouml",
	L"times",    L"Oslash",    L"Ugrave",    L"Uacute",
	L"Ucirc",    L"Uuml",    L"Yacute",    L"THORN",
	L"szlig",    L"agrave",    L"aacute",    L"acirc",
	L"atilde",    L"auml",    L"aring",    L"aelig",
	L"ccedil",    L"egrave",    L"eacute",    L"ecirc",
	L"euml",    L"igrave",    L"iacute",    L"icirc",
	L"iuml",    L"eth",    L"ntilde",    L"ograve",
	L"oacute",    L"ocirc",    L"otilde",    L"ouml",
	L"divide",    L"oslash",    L"ugrave",    L"uacute",
	L"ucirc",    L"uuml",    L"yacute",    L"thorn",
	L"yuml"
};
#endif



FormatterToHTML::FormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding, 
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					doIndent,
			int						indent,
			const XalanDOMString&	version,
			const XalanDOMString&	standalone,
			bool xmlDecl) :
	FormatterToXML(
			writer,
			version,
			doIndent,
			indent,
			encoding,
			mediaType,
			doctypeSystem,
			doctypePublic,
			xmlDecl,
			standalone,
			OUTPUT_METHOD_HTML),
	m_currentElementName(),
	m_inBlockElem(false),
	m_isRawStack(),
	m_isScriptOrStyleElem(false),
	m_isFirstElem(true)
{
	initCharsMap();
}



FormatterToHTML::~FormatterToHTML()
{
}



void
FormatterToHTML::initAttrCharsMap()
{
	FormatterToXML::initAttrCharsMap();

	m_attrCharsMap[XalanUnicode::charLF] = 'S';

	// These should _not_ be escaped...
	m_attrCharsMap[XalanUnicode::charLessThanSign] = 0;
	m_attrCharsMap[XalanUnicode::charGreaterThanSign] = 0;

	for(unsigned int i = 160; i < SPECIALSSIZE; i++)
	{
		m_attrCharsMap[i] = 'S';
	}
}



void
FormatterToHTML::initCharsMap()
{
	initAttrCharsMap();

	memset(m_charsMap, 0, sizeof(m_charsMap));

	m_charsMap[XalanUnicode::charLF] = 'S';
	m_charsMap[XalanUnicode::charLessThanSign] = 'S';
	m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
	m_charsMap[XalanUnicode::charAmpersand] = 'S';

	memset(m_charsMap, 'S', 10);

	m_charsMap[0x0A] = 'S';
	m_charsMap[0x0D] = 'S';

	for(int i = 160; i < SPECIALSSIZE; ++i)
	{
		m_charsMap[i] = 'S';
	}

    for(int j = m_maxCharacter; j < SPECIALSSIZE; ++j)
    {
		m_charsMap[j] = 'S';
    }
}



const FormatterToHTML::ElemDesc&
FormatterToHTML::getElemDesc(const XalanDOMChar*	name)
{
	const ElementFlagsMapType::const_iterator	i =
		s_elementFlags.find(name);

	if (i == s_elementFlags.end())
	{
		return s_dummyDesc;
	}
	else
	{
		return (*i).second;
	}
}



void
FormatterToHTML::startDocument()
{
    m_startNewLine = false;
	m_shouldWriteXMLHeader = false;

	const bool				isEmptySystem =
			isEmpty(m_doctypeSystem);

	const bool				isEmptyPublic =
			isEmpty(m_doctypePublic);

	// Output the header if either the System or Public attributes are
	// specified
	if(isEmptySystem == false || isEmptyPublic == false)
	{
		accumContent(s_doctypeHeaderStartString);

		if(isEmptyPublic == false)
		{
			accumContent(s_doctypeHeaderPublicString);
			accumContent(m_doctypePublic);
			accumContent(XalanUnicode::charQuoteMark);
		}

		if(isEmptySystem == false)
		{
			if(isEmptyPublic == true)
			{
				accumContent(s_doctypeHeaderSystemString);
			}

			accumContent(XalanUnicode::charSpace);
			accumContent(XalanUnicode::charQuoteMark);

			accumContent(m_doctypeSystem);
			accumContent(XalanUnicode::charQuoteMark);
		}

		accumContent(XalanUnicode::charGreaterThanSign);

		outputLineSep();
	}

	m_needToOutputDocTypeDecl = false;
}



void
FormatterToHTML::endDocument()
{
	m_isFirstElem = true;

	FormatterToXML::endDocument();
}



void
FormatterToHTML::startElement(
			const XMLCh* const	name,
			AttributeList&		attrs)
{
	writeParentTagEnd();

	const ElemDesc&		elemDesc =
		getElemDesc(name);

    bool	isBlockElement = elemDesc.is(ElemDesc::BLOCK);
	bool	isHeadElement = elemDesc.is(ElemDesc.HEADELEM);

	m_isScriptOrStyleElem = 
		equalsIgnoreCaseASCII(name, c_wstr(s_scriptString)) ||
		equalsIgnoreCaseASCII(name, c_wstr(s_styleString));

	if(m_ispreserve == true)
	{
		m_ispreserve = false;
	}
    else if(m_doIndent &&
			m_isFirstElem == false &&
			(m_inBlockElem == false || isBlockElement == true))
    {
		m_startNewLine = true;

		indent(m_currentIndent);
    }

	m_inBlockElem = !isBlockElement;

	m_isRawStack.push_back(elemDesc.is(ElemDesc::RAW));

	accumContent(XalanUnicode::charLessThanSign);

	accumName(name);

	const unsigned int	nAttrs = attrs.getLength();

    for (unsigned int i = 0;  i < nAttrs ;  i++)
    {
		processAttribute(attrs.getName(i), attrs.getValue(i), elemDesc);
    }

    // Flag the current element as not yet having any children.
    openElementForChildren();

    m_currentIndent += m_indent;
    
    m_isprevtext = false;

	m_isFirstElem = false;

	if (isHeadElement)
    {
      writeParentTagEnd();

      if (m_doIndent)
        indent(m_currentIndent);

      accumContent(s_metaString);
      accumContent(getEncoding());      
      accumContent(XalanUnicode::charQuoteMark);
      accumContent(XalanUnicode::charGreaterThanSign);
    }
}



void
FormatterToHTML::endElement(const XMLCh* const	name)
{
    m_currentIndent -= m_indent;

    const bool	hasChildNodes = childNodesWereAdded();

    m_isRawStack.pop_back();
    
    const ElemDesc&		elemDesc =
		getElemDesc(name);

    const bool	isBlockElement = elemDesc.is(ElemDesc::BLOCK);

    bool shouldIndent = false;

    if(m_ispreserve == true)
    {
		m_ispreserve = false;
    }
    else if(m_doIndent == true && (m_inBlockElem == false || isBlockElement == true))
    {
		m_startNewLine = true;

		shouldIndent = true;
    }

    m_inBlockElem = !isBlockElement;

    if (hasChildNodes) 
    {
		if (shouldIndent == true)
		{
			indent(m_currentIndent);
		}

		accumContent(XalanUnicode::charLessThanSign);
		accumContent(XalanUnicode::charSolidus);
		accumName(name);
		accumContent(XalanUnicode::charGreaterThanSign);
	}
    else
    {
		if(elemDesc.is(ElemDesc::EMPTY) == false)
		{
			accumContent(XalanUnicode::charGreaterThanSign);

			accumContent(XalanUnicode::charLessThanSign);
			accumContent(XalanUnicode::charSolidus);
			accumName(name);
			accumContent(XalanUnicode::charGreaterThanSign);
		}
		else
		{
			accumContent(XalanUnicode::charGreaterThanSign);
		}
    }

    if (elemDesc.is(ElemDesc::WHITESPACESENSITIVE) == true)
	{
		m_ispreserve = true;
	}

    if (hasChildNodes == true)
    {
		if (m_preserves.empty() == false)
		{
			m_preserves.pop_back();
		}
    }

	m_isFirstElem = false;

    m_isprevtext = false;
}



void
FormatterToHTML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(length != 0)
	{
		if(m_inCData == true)
		{
			cdata(chars, length);
		}
		else if(m_nextIsRaw)
		{
			m_nextIsRaw = false;

			charactersRaw(chars, length);
		}
		else if (m_isRawStack.empty() == false &&
				 m_isRawStack.back() == true)
		{
			writeParentTagEnd();

			m_ispreserve = true;

			if (shouldIndent() == true)
			{
				indent(m_currentIndent);
			}

			writeNormalizedChars(chars, 0, length, false);
		}
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			writeCharacters(chars, length);
		}
	}

	if (m_isprevtext == false)
	{
		m_isprevtext = true;
	}
}



void
FormatterToHTML::entityReference(const XMLCh* const		name)
{
	accumContent(XalanUnicode::charAmpersand);
	accumName(name);
	accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	if(m_isScriptOrStyleElem == true)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		writeNormalizedChars(ch, 0, length, true);
	}
	else if(m_stripCData == true)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accumContent(ch, 0, length);
	}
	else
	{
		FormatterToXML::cdata(ch, length);
	}
}



void
FormatterToHTML::processingInstruction(
		const XMLCh* const	target,
		const XMLCh* const	data)

{
	// Use a fairly nasty hack to tell if the next node is supposed to be 
	// unescaped text.
	if(equals(target, c_wstr(s_xsltNextIsRawString)) == true &&
	   equals(data, c_wstr(s_formatterToDOMString)) == true)
	{
		m_nextIsRaw = true;
	}
	else
	{
		writeParentTagEnd();

		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accumContent(XalanUnicode::charLessThanSign);
		accumContent(XalanUnicode::charQuestionMark);
		accumName(target);

		if (length(data) > 0)
		{
			if(isXMLWhitespace(data[0]) == false)
			{
				accumContent(XalanUnicode::charSpace);
			}

			accumName(data);
		}

		accumContent(XalanUnicode::charGreaterThanSign); // different from XML

		m_startNewLine = true;
	}
}



void
FormatterToHTML::writeCharacters(const XalanDOMString&	theString)
{
	writeCharacters(toCharArray(theString), length(theString));
}



void
FormatterToHTML::writeCharacters(
			const XalanDOMChar*		theString,
			unsigned int			theLength)
{
	assert(theString != 0);

	if (theLength == unsigned(-1))
	{
		theLength = length(theString);
	}

	for (unsigned int i = 0; i < theLength; ++i) 
	{
		const XalanDOMChar	ch = theString[i];

		if(ch < SPECIALSSIZE && m_charsMap[ch] != 'S')
		{
			accumContent(ch);
		}
		else if (XalanUnicode::charLF == ch) 
		{
			outputLineSep();
		}
		else if (XalanUnicode::charLessThanSign == ch) 
		{
			copyEntityIntoBuffer(s_ltString);
		}
		else if (XalanUnicode::charGreaterThanSign == ch) 
		{
			copyEntityIntoBuffer(s_gtString);
		}
		else if (XalanUnicode::charAmpersand == ch) 
		{
			copyEntityIntoBuffer(s_ampString);
		}
		else if(ch >= 160 && ch <= 255)
		{
			copyEntityIntoBuffer(theHTMLLatin1Symbols[ch - 160]);
		}
		else if(ch >= 913 && ch <= 937 && ch != 930)
		{
			copyEntityIntoBuffer(theHTMLSymbols1[ch - 913]);
		}
		else if(ch >= 945 && ch <= 969)
		{
			copyEntityIntoBuffer(theHTMLSymbols2[ch - 945]);
		}
		else if(ch >= 977 && ch <= 978)
		{
			// subtract the unused characters 
			copyEntityIntoBuffer(theHTMLSymbols2[ch - 945 - 7]);
		}
		else if(ch == 982)
		{
			// subtract the unused characters
			copyEntityIntoBuffer(theHTMLSymbols2[ch - 945 - 10]);
		}
		else if (338 == ch)
		{
			copyEntityIntoBuffer(s_oeligString);
		}
		else if (402 == ch) 
		{
			copyEntityIntoBuffer(s_fnofString);
		}
		else if (m_isUTF8 == true && 0xd800 <= ch && ch < 0xdc00)
		{
			// UTF-16 surrogate
			unsigned int	next = 0;

			if (i + 1 >= theLength) 
			{
				throwInvalidUTF16SurrogateException(ch);
			}
			else
			{
				next = theString[++i];

				if (!(0xdc00 <= next && next < 0xe000))
				{
					throwInvalidUTF16SurrogateException(ch, next);
				}

				next = ((ch - 0xd800) << 10) + next - 0xdc00 + 0x00010000;
			}

			writeNumberedEntityReference(next);
		}
		else if(ch >= 0x007Fu && ch <= m_maxCharacter)
		{
			// Hope this is right...
			accumContent(ch);
		}
		else
		{
			writeNumberedEntityReference(ch);
		}
	}
}



void
FormatterToHTML::writeAttrString(
			const XalanDOMChar*		string,
			const XalanDOMString&	/* encoding */)
{
    const unsigned int	strLen = length(string);

    for (unsigned int i = 0;  i < strLen;  i ++)
    {
		const XalanDOMChar	ch = string[i];

		if(ch < SPECIALSSIZE && m_attrCharsMap[ch] != 'S')
		{
			accumContent(ch);
		}
		else if(XalanUnicode::charAmpersand == ch &&
				i + 1 < strLen &&
				XalanUnicode::charLeftCurlyBracket == string[i + 1])
		{
			accumContent(ch); // no escaping in this case, as specified in 15.2
		}
		else if (accumDefaultEntity(ch, i, string, strLen, false) == false)
		{
			if (0xd800 <= ch && ch < 0xdc00) 
			{
				// UTF-16 surrogate

				unsigned int next = 0;

				if (i + 1 >= strLen) 
				{
					throwInvalidUTF16SurrogateException(ch);
				}
				else 
				{
					next = string[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(ch, next);
					}

					next = ((ch - 0xd800) << 10) + next -0xdc00 + 0x00010000;
				}

				accumContent(XalanUnicode::charAmpersand);
				accumContent(XalanUnicode::charNumberSign);
				accumContent(UnsignedLongToDOMString(next));
				accumContent(XalanUnicode::charSemicolon);
			}
			else if(ch >= 160 && ch <= 255)
			{
				accumContent(XalanUnicode::charAmpersand);
				accumContent(theHTMLLatin1Symbols[ch - 160]);
				accumContent(XalanUnicode::charSemicolon);
			}
			else if(ch >= 913 && ch <= 937 && ch != 930)
			{
				accumContent(XalanUnicode::charAmpersand);
				accumContent(theHTMLSymbols1[ch - 913]);
				accumContent(XalanUnicode::charSemicolon);
			}
			else if(ch >= 945 && ch <= 969)
			{
				accumContent(XalanUnicode::charAmpersand);
				accumContent(theHTMLSymbols2[ch - 945]);
				accumContent(XalanUnicode::charSemicolon);
			}
			else if(ch >= 977 && ch <= 978)
			{
				accumContent(XalanUnicode::charAmpersand);
				// subtracting the number of unused characters
				accumContent(theHTMLSymbols2[ch - 945 - 7]);
				accumContent(XalanUnicode::charSemicolon);
			}
			else if(ch == 982)
			{
				accumContent(XalanUnicode::charAmpersand);
				// subtracting the number of unused characters
				accumContent(theHTMLSymbols2[ch - 945 - 10]);
				accumContent(XalanUnicode::charSemicolon);
			}
			else if (338 == ch)
			{
				copyEntityIntoBuffer(s_oeligString);
			}
			else if (402 == ch) 
			{
				copyEntityIntoBuffer(s_fnofString);
			}
			else
			{
				writeNumberedEntityReference(ch);
			}
		}
    }
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMChar*	s)
{
	const unsigned int	len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(unsigned int i = 0; i < len; ++i)
    {
		accumContent(s[i]);
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMString&		s)
{
	const unsigned int	len = length(s);

    accumContent(XalanUnicode::charAmpersand);

    for(unsigned int i = 0; i < len; ++i)
    {
		accumContent(charAt(s, i));
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMCharVectorType&		s)
{
    accumContent(XalanUnicode::charAmpersand);

    for(XalanDOMCharVectorType::const_iterator i = s.begin(); *i != 0; ++i)
    {
		accumContent(*i);
    }

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::processAttribute(
			const XalanDOMChar*		name,
			const XalanDOMChar*		value,
			const ElemDesc&			elemDesc)
{
    accumContent(XalanUnicode::charSpace);

    if(elemDesc.isAttrFlagSet(name, ElemDesc::ATTREMPTY) == true &&
       (length(value) == 0) || equalsIgnoreCaseASCII(value, name) == true)
    {
		accumName(name);
    }
    else
    {
		accumName(name);
		accumContent(XalanUnicode::charEqualsSign);
		accumContent(XalanUnicode::charQuoteMark);

		if(elemDesc.isAttrFlagSet(name, ElemDesc::ATTRURL) == true)
		{
			writeAttrURI(value, m_encoding);
		}
		else
		{
			writeAttrString(value, m_encoding);
		}

		accumContent(XalanUnicode::charQuoteMark);
    }
}



void
FormatterToHTML::writeAttrURI(
			const XalanDOMChar*		string,
			const XalanDOMString	encoding)
{
	const unsigned int	len = length(string);

    for (unsigned int i = 0; i < len; ++i)
    {
		const XalanDOMChar	ch = string[i];

		// if first 8 bytes are 0, no need to append them.
		if (ch < 9 || ch > 127 || ch == XalanUnicode::charQuoteMark || ch == XalanUnicode::charSpace)
		{
			const unsigned int	b1 = (ch & 0xFF00) >> 8;
			const unsigned int	b2 = ch & 0x00FF;

			if(b1 != 0)
			{
				accumContent(XalanUnicode::charPercentSign);

				accumContent(UnsignedLongToHexDOMString(b1));
			}

			accumContent(XalanUnicode::charPercentSign);
			accumContent(UnsignedLongToHexDOMString(b2));		
		}	
		else
		{
			accumContent(ch);
		}
	}
}



void
FormatterToHTML::initializeElementFlagsMap(ElementFlagsMapType&		theElementFlags)
{
#if defined(XALAN_NO_NAMESPACES)
	typedef pair<ElementFlagsMapType::iterator, bool>	PairType;
#else
	typedef std::pair<ElementFlagsMapType::iterator, bool>	PairType;
#endif

	// HTML 4.0 loose DTD
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BASEFONT")),
			ElemDesc(0|ElemDesc::EMPTY)));

	PairType	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FRAME")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FRAMESET")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOFRAMES")),
			ElemDesc(0|ElemDesc::BLOCK)));
 
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ISINDEX")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("APPLET")),
			ElemDesc(0|ElemDesc::WHITESPACESENSITIVE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CENTER")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DIR")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("MENU")),
			ElemDesc(0|ElemDesc::BLOCK)));


	// HTML 4.0 strict DTD
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TT")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("I")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("B")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BIG")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SMALL")),
			ElemDesc(0|ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("EM")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STRONG")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DFN")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CODE")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SAMP")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("KBD")),
			ElemDesc(0|ElemDesc::PHRASE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("VAR")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CITE")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ABBR")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ACRONYM")),
			ElemDesc(0|ElemDesc::PHRASE)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SUP")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SUB")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SPAN")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BDO")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BR")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BODY")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("ADDRESS")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DIV")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("A")),
			ElemDesc(0|ElemDesc::SPECIAL)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("HREF")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("NAME")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("MAP")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("AREA")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LINK")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("IMG")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::EMPTY|ElemDesc::WHITESPACESENSITIVE)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("LONGDESC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OBJECT")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::HEADMISC|ElemDesc::WHITESPACESENSITIVE)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CLASSID")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CODEBASE")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DATA")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ARCHIVE")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("PARAM")),
			ElemDesc(0|ElemDesc::EMPTY)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HR")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET|ElemDesc::EMPTY)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("P")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H1")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H2")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H3")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H4")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H5")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("H6")),
			ElemDesc(0|ElemDesc::HEAD|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("PRE")),
			ElemDesc(0|ElemDesc::PREFORMATTED|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("Q")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BLOCKQUOTE")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("INS")),
			ElemDesc(0)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DEL")),
			ElemDesc(0)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CITE")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DL")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DT")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("DD")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OL")),
			ElemDesc(0|ElemDesc::LIST|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("UL")),
			ElemDesc(0|ElemDesc::LIST|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LI")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FORM")),
			ElemDesc(0|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("ACTION")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LABEL")),
			ElemDesc(0|ElemDesc::FORMCTRL)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("INPUT")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL|ElemDesc::EMPTY)));
	
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("USEMAP")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("CHECKED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("READONLY")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SELECT")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("READONLY")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("MULTIPLE")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OPTGROUP")),
			ElemDesc(0)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("OPTION")),
			ElemDesc(0)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SELECTED")), ElemDesc::ATTREMPTY);

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TEXTAREA")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("READONLY")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FIELDSET")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("LEGEND")),
			ElemDesc(0)));
	
	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BUTTON")),
			ElemDesc(0|ElemDesc::FORMCTRL|ElemDesc::INLINELABEL)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("DISABLED")), ElemDesc::ATTREMPTY);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TABLE")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("CAPTION")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("THEAD")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TFOOT")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TBODY")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("COLGROUP")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("COL")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TR")),
			ElemDesc(0|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TH")),
			ElemDesc(0)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TD")),
			ElemDesc(0)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HEAD")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::HEADELEM)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("PROFILE")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("TITLE")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("BASE")),
			ElemDesc(0|ElemDesc::EMPTY|ElemDesc::BLOCK)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("HREF")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("META")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::EMPTY|ElemDesc::BLOCK)));
	
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STYLE")),
			ElemDesc(0|ElemDesc::HEADMISC|ElemDesc::RAW|ElemDesc::BLOCK)));

	theResult =
	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("SCRIPT")),
			ElemDesc(0|ElemDesc::SPECIAL|ElemDesc::ASPECIAL|ElemDesc::HEADMISC|ElemDesc::RAW)));

	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("SRC")), ElemDesc::ATTRURL);
	(*theResult.first).second.setAttr(c_wstr(XALAN_STATIC_UCODE_STRING("FOR")), ElemDesc::ATTRURL);

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOSCRIPT")),
			ElemDesc(0|ElemDesc::BLOCK|ElemDesc::BLOCKFORM|ElemDesc::BLOCKFORMFIELDSET)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("HTML")),
			ElemDesc(0|ElemDesc::BLOCK)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("FONT")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("S")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("STRIKE")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("U")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));

	theElementFlags.insert(
		ElementFlagsMapType::value_type(
			c_wstr(XALAN_STATIC_UCODE_STRING("NOBR")),
			ElemDesc(0 | ElemDesc::FONTSTYLE)));
}



static FormatterToHTML::ElementFlagsMapType		s_elementFlags;


const FormatterToHTML::ElementFlagsMapType&		FormatterToHTML::s_elementFlags = ::s_elementFlags;


const FormatterToHTML::ElemDesc					FormatterToHTML::s_dummyDesc(FormatterToHTML::ElemDesc::BLOCK);


static XalanDOMCharVectorType	s_doctypeHeaderStartString;

static XalanDOMCharVectorType	s_doctypeHeaderPublicString;

static XalanDOMCharVectorType	s_doctypeHeaderSystemString;

static XalanDOMCharVectorType	s_scriptString;

static XalanDOMCharVectorType	s_styleString;

static XalanDOMCharVectorType	s_ltString;

static XalanDOMCharVectorType	s_gtString;

static XalanDOMCharVectorType	s_ampString;

static XalanDOMCharVectorType	s_fnofString;

static XalanDOMCharVectorType	s_oeligString;

static XalanDOMCharVectorType	s_metaString;


const XalanDOMCharVectorType&	FormatterToHTML::s_doctypeHeaderStartString =
			::s_doctypeHeaderStartString;

const XalanDOMCharVectorType&	FormatterToHTML::s_doctypeHeaderPublicString =
			::s_doctypeHeaderPublicString;

const XalanDOMCharVectorType&	FormatterToHTML::s_doctypeHeaderSystemString =
			::s_doctypeHeaderSystemString;

const XalanDOMCharVectorType&	FormatterToHTML::s_scriptString =
			::s_scriptString;

const XalanDOMCharVectorType&	FormatterToHTML::s_styleString =
			::s_styleString;

const XalanDOMCharVectorType&	FormatterToHTML::s_ltString =
			::s_ltString;

const XalanDOMCharVectorType&	FormatterToHTML::s_gtString =
			::s_gtString;

const XalanDOMCharVectorType&	FormatterToHTML::s_ampString =
			::s_ampString;

const XalanDOMCharVectorType&	FormatterToHTML::s_fnofString =
			::s_fnofString;

const XalanDOMCharVectorType&	FormatterToHTML::s_oeligString =
			::s_oeligString;

const XalanDOMCharVectorType&	FormatterToHTML::s_metaString =
			::s_metaString;

#if !defined(XALAN_LSTRSUPPORT)
void
pushStringsOnVector(
			const char*	const			theStrings[],
			size_t						theStringsSize,
			XalanDOMStringVectorType&	theVector)
{
	theVector.reserve(theStringsSize);

	for(size_t i = 0; i < theStringsSize; ++i)
	{
		theVector.push_back(XalanDOMString(theStrings[i]));
	}
}
#endif



void
FormatterToHTML::initialize()
{
	initializeElementFlagsMap(::s_elementFlags);

	::s_doctypeHeaderStartString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("<!DOCTYPE HTML")));

	::s_doctypeHeaderPublicString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING(" PUBLIC \"")));

	::s_doctypeHeaderSystemString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING(" SYSTEM")));

	::s_scriptString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("SCRIPT")));

	::s_styleString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("STYLE")));

	::s_ltString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("lt")));

	::s_gtString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("gt")));

	::s_ampString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("amp")));

	::s_fnofString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("fnof")));

	::s_oeligString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("OElig")));
	
	::s_metaString = MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("<META http-equiv=\"Content-Type\" content=\"text/html; charset=")));

#if !defined(XALAN_LSTRSUPPORT)
	pushStringsOnVector(
			theHTMLSymbols1Narrow,
			sizeof(theHTMLSymbols1Narrow) / sizeof(theHTMLSymbols1Narrow[0]),
			theHTMLSymbols1);

	pushStringsOnVector(
			theHTMLSymbols2Narrow,
			sizeof(theHTMLSymbols2Narrow) / sizeof(theHTMLSymbols2Narrow[0]),
			theHTMLSymbols2);

	pushStringsOnVector(
			theHTMLLatin1SymbolsNarrow,
			sizeof(theHTMLLatin1SymbolsNarrow) / sizeof(theHTMLLatin1SymbolsNarrow[0]),
			theHTMLLatin1Symbols);
#endif
}



void
FormatterToHTML::terminate()
{
	ElementFlagsMapType().swap(::s_elementFlags);

	XalanDOMCharVectorType().swap(::s_doctypeHeaderStartString);

	XalanDOMCharVectorType().swap(::s_doctypeHeaderPublicString);

	XalanDOMCharVectorType().swap(::s_doctypeHeaderSystemString);

	XalanDOMCharVectorType().swap(::s_scriptString);

	XalanDOMCharVectorType().swap(::s_styleString);

	XalanDOMCharVectorType().swap(::s_ltString);

	XalanDOMCharVectorType().swap(::s_gtString);

	XalanDOMCharVectorType().swap(::s_ampString);

	XalanDOMCharVectorType().swap(::s_fnofString);

	XalanDOMCharVectorType().swap(::s_oeligString);

	XalanDOMCharVectorType().swap(::s_metaString);		

#if !defined(XALAN_LSTRSUPPORT)
	XalanDOMStringVectorType().swap(theHTMLSymbols1);

	XalanDOMStringVectorType().swap(theHTMLSymbols2);

	XalanDOMStringVectorType().swap(theHTMLLatin1Symbols);
#endif
}
