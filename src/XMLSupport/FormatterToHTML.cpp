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
 * $Id$
 * 
 * $State$
 * 
 * @author David N. Bertoni <david_n_bertoni@lotus.com>
 */



// Class header file.
#include "FormatterToHTML.hpp"



#include <cassert>
#include <climits>


#include <sax/SAXException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/Writer.hpp>



const FormatterToHTML::EmptiesSetType				FormatterToHTML::s_empties =
						FormatterToHTML::createEmpties();

const FormatterToHTML::EmptiesSetType				FormatterToHTML::s_attrempties =
						FormatterToHTML::createAttrEmpties();

const FormatterToHTML::HTMLAttributesVectorType		FormatterToHTML::s_HTMLlat1 =
						FormatterToHTML::createAttributes();

const FormatterToHTML::HTMLSymbolsVectorType		FormatterToHTML::s_HTMLsymbol1 =
						FormatterToHTML::createSymbols();

const FormatterToHTML::StringSetType FormatterToHTML::s_nonblockelems =
						FormatterToHTML::createNonBlockElems();

const FormatterToHTML::StringSetType FormatterToHTML::s_escapetb =
						FormatterToHTML::createEscapeElems();

const FormatterToHTML::AttributesMapType FormatterToHTML::s_attruris =
						FormatterToHTML::createAttributesMap();


FormatterToHTML::FormatterToHTML(
	  Writer&				writer,
	  const DOMString& version,
	  bool doIndent, 
	  int indent,
	  const DOMString& encoding, 
	  const DOMString& mediaType,
	  const DOMString& doctypeSystem,
	  const DOMString& doctypePublic,
	  bool xmlDecl,
	  const DOMString& standalone, 
	  const QNameVectorType* const cdataSectionElems) :
	FormatterToXML( writer, version, doIndent, indent,
	  encoding, mediaType, doctypeSystem, doctypePublic,
	  xmlDecl, standalone, cdataSectionElems),
	m_currentElementName()
{
}



FormatterToHTML::~FormatterToHTML()
{
}


void
FormatterToHTML::startDocument()
{
    m_needToOutputDocTypeDecl = true;
    m_startNewLine = false;
    
    if(true == m_needToOutputDocTypeDecl)
    {
		// Output the header if either the System or Public attributes are
		// specified
		if((! isEmpty(m_doctypeSystem)) || (! isEmpty(m_doctypePublic)))
		{
			m_writer.write("<!DOCTYPE HTML");          
			if(! isEmpty(m_doctypePublic))
			{
				m_writer.write(" PUBLIC \"");
				m_writer.write(m_doctypePublic);
				m_writer.write("\"");
			}
			if(! isEmpty(m_doctypeSystem))
			{
				if(isEmpty(m_doctypePublic))
					m_writer.write(" SYSTEM \"");
				else
					m_writer.write(" \"");
				m_writer.write(m_doctypeSystem);
				m_writer.write("\"");
			}
			m_writer.write(">");
			m_writer.write(m_lineSep);
      }              
    }
    m_needToOutputDocTypeDecl = false;
}

void
FormatterToHTML::endDocument()
{
	try
	{
		m_writer.write("\n");
	}
	catch(...)
	{
		throw SAXException();
	}

	FormatterToXML::endDocument();
}



void
FormatterToHTML::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
	DOMString theName(name);
	if(true == m_needToOutputDocTypeDecl)
	{
		try
		{
			if((! isEmpty(m_doctypeSystem)) || (! isEmpty(m_doctypePublic)))
			{
				m_writer.write("<!DOCTYPE ");
				m_writer.write(name);
				if(! isEmpty(m_doctypePublic))
				{
					m_writer.write(" PUBLIC \"");
					m_writer.write(m_doctypePublic);
					m_writer.write("\"");
				}
				if(! isEmpty(m_doctypeSystem))
				{
					if(isEmpty(m_doctypePublic))
						m_writer.write(" SYSTEM \"");
					else
						m_writer.write(" \"");
					m_writer.write(m_doctypeSystem);
					m_writer.write("\"");
				}
				m_writer.write(">");
				m_writer.write(m_lineSep);
			}
		}
		// java: catch(IOException ioe)
		catch(...)
		{
			throw SAXException("IO error");
		}
	}
	m_needToOutputDocTypeDecl = false;
	bool savedDoIndent = m_doIndent;

	// If the previous element is a non-block element or the next 
	// element is a non-block element, then do not indent.
	m_doIndent =
	((s_nonblockelems.end() != s_nonblockelems.find(toUpperCase(theName))) ||
	((! isEmpty(m_currentElementName) &&
		(s_nonblockelems.end() !=
		 s_nonblockelems.find(toUpperCase(m_currentElementName))))))
			? false : true;

	m_currentElementName = name;
	try
	{      
		writeParentTagEnd();
		m_ispreserve = false;

		if (shouldIndent() && !m_elemStack.empty()) 
		{
			m_startNewLine = true;
			indent(m_writer, m_currentIndent);
		}

		m_writer.write('<');
		m_writer.write(name);

		int nAttrs = attrs.getLength();
		for (int i = 0;  i < nAttrs ;  i++)
		{
			processAttribute(attrs.getName(i), attrs.getValue(i));
		}
		// Flag the current element as not yet having any children.
		openElementForChildren();

		m_currentIndent += m_indent;
		m_isprevtext = false;
	}
	// java: catch(IOException ioe)
	catch(...)
	{
		throw SAXException("IO error");
	}
	m_doIndent = savedDoIndent;
}
 


void
FormatterToHTML::endElement(
			const	XMLCh* const	name)
{
	try
	{
 		m_currentIndent -= m_indent;
		// name = name.toUpperCase();
		const bool	hasChildNodes = childNodesWereAdded();

		if (hasChildNodes == true) 
		{
			if (shouldIndent() == true)
				indent(m_writer, m_currentIndent);
			m_writer.write("</");
			m_writer.write(name);
			m_writer.write(">");
		}
		else
		{
			if(s_empties.find(toUpperCase(name)) == s_empties.end())
			{
				m_writer.write("></");
				m_writer.write(name);
				m_writer.write(">");
			}
			else
			{
				m_writer.write(">");
			}
		}
		if (hasChildNodes == true) 
		{
			m_ispreserve = m_preserves.top();
			m_preserves.pop();
		}
		m_isprevtext = false;
	}
	catch(...)
	{
	  throw SAXException();
	}
}



void
FormatterToHTML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
		if(0 == length) return;

	if(m_nextIsRaw)
	{
		m_nextIsRaw = false;
		charactersRaw (chars, length);
		return;
	}

	if((! isEmpty(m_currentElementName)) &&
			(equalsIgnoreCase(m_currentElementName, "SCRIPT") ||
			 equalsIgnoreCase(m_currentElementName, "STYLE")))
	{
		try
		{
			writeParentTagEnd();
			m_ispreserve = true;
			if (shouldIndent())
				indent(m_writer, m_currentIndent);
			// m_writer.write("<![CDATA[");
			// m_writer.write(chars, 0, length);
			writeNormalizedChars(chars, 0, length, false);
			// m_writer.write("]]>");
			return;
		}
		// java: catch(IOException ioe)
		catch(...)
		{
			throw SAXException("IO error");
		}
	}

	try
	{
		writeParentTagEnd();
		m_ispreserve = true;
		int pos = 0;
		int end = length;
		for (int i = 0;  i < end;  i ++) 
		{
			const XMLCh ch = chars[i];
			int chNum = ch;
			if ('\n' == ch) 
			{
				m_charBuf[pos++] = m_lineSep;
			}
			else if ('<' == ch) 
			{
				pos = copyEntityIntoBuf("lt", pos);
			}
			else if ('>' == ch) 
			{
				pos = copyEntityIntoBuf("gt", pos);
			}
			else if ('&' == ch) 
			{
				pos = copyEntityIntoBuf("amp", pos);
			}
			else if((chNum >= 9) && (chNum <= 126))
			{
				m_charBuf[pos++] = ch;
			}
			else if((chNum >= 160) && (chNum <= 255))
			{
				pos = copyEntityIntoBuf(s_HTMLlat1[ch-160], pos);
			}
			else if((chNum >= 913) && (chNum <= 982))
			{
				pos = copyEntityIntoBuf(s_HTMLsymbol1[ch-913], pos);
			}
			else if (402 == ch) 
			{
				pos = copyEntityIntoBuf("fnof", pos);
			}
			else if (m_isUTF8 && (0xd800 <= chNum && chNum < 0xdc00)) 
			{
				// UTF-16 surrogate
				copyUTF16IntoBuf( chars, i, pos, length);
			}
			else if((ch >= 0x7F) && (ch <= m_maxCharacter))
			{
				// Hope this is right...
				m_charBuf[pos++] = ch;
			}
			else
			{
				DOMString ds;
				m_charBuf[pos++] = '&';
				m_charBuf[pos++] = '#';
				ds = LongToDOMString(chNum);
				const XMLCh* pb = c_wstr(ds);
				int nIntStr = ds.length();
				for(int k = 0; k < nIntStr; k++)
				{
					m_charBuf[pos++] = *(pb+k);
				}
				m_charBuf[pos++] = ';';
			}
			// Use 80 as a best guess safe buffer
			if(pos > MAXSAFECHARBUF)
			{
				m_writer.write(m_charBuf, 0, pos);
				pos = 0;
			}
		}
		m_writer.write(m_charBuf, 0, pos);
		m_isprevtext = true;
	}
	// java: catch(IOException ioe)
	catch(...)
	{
		throw SAXException("IO error");
	}
}


void
FormatterToHTML::entityReference(const XMLCh* const	name)
{
	try
	{
		m_writer.write("&");
		m_writer.write(name);
		m_writer.write(";");
	}
	catch(...)
	{
		throw SAXException();
	}
}



void
FormatterToHTML::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{
	if(equalsIgnoreCase(m_currentElementName, "SCRIPT") ||
		equalsIgnoreCase(m_currentElementName, "STYLE"))
	{
		try
		{
			writeParentTagEnd();
			m_ispreserve = true;
			if (shouldIndent() == true)
			{
				indent(m_writer, m_currentIndent);
			}
			// was: m_writer.write(ch, 0, length);
			writeNormalizedChars(ch, 0, length, true);
		}
		catch(...)
		{
			throw SAXException();
		}
	}
	else if(m_stripCData == true)
	{
		try
		{
			writeParentTagEnd();

			m_ispreserve = true;

			if (shouldIndent() == true)
			{
				indent(m_writer, m_currentIndent);
			}

			// m_writer.write("<![CDATA[");
			m_writer.write(ch, 0, length);
			// m_writer.write("]]>");
		}
		catch(...)
		{
			throw SAXException();
		}
	}
	else
	{
		characters(ch, length);
	}
}


void
FormatterToHTML::processAttribute(
			const DOMString&	name,
			const DOMString&	value)
{
	try
	{
		if(!name.equals("xmlns") && !startsWith(name, "xmlns:"))
		{
			DOMString pval;
			DOMString aname = toLowerCase(name);
			if (equals(aname, "xml:lang"))  aname = "lang";
			// qualify with an element??
			AttributesMapType::const_iterator it = 
				(s_attruris.find(toLowerCase(m_currentElementName)));
			if (it != s_attruris.end())
			{
				const StringSetType val = (*it).second;
				// Determine if value is in the set of strings
				if(val.find(aname) != val.end())
					pval = prepAttrURI(value, m_attrSpecialChars, m_encoding);
				else
					pval = prepAttrString(value, m_attrSpecialChars, m_encoding);
			}
			else
				pval = prepAttrString(value, m_attrSpecialChars, m_encoding);

			if(s_attrempties.find(aname) == s_attrempties.end())
			{
				m_writer.write(' ');
				m_writer.write(name);
				m_writer.write("=\"");
				m_writer.write(pval);
				m_writer.write('\"');
			}
			else
			{
				m_writer.write(' ');
				if((pval.length() == 0) || equalsIgnoreCase(pval, aname))
				{
					m_writer.write(name);
				}
				else
				{
					m_writer.write(name);
					m_writer.write("=\"");
					m_writer.write(pval);
					m_writer.write('\"');
				}
			}
		}
	}
	catch(...)
	{
		throw SAXException();
	}
}


/**
 * Returns the specified <var>string</var> after substituting non ASCII characters,
 * with <CODE>%HH</CODE>, where HH is the hex of the byte value.
 *
 * @param   string      String to convert to XML format.
 * @param   specials    Characters, should be represented in character references.
 * @param   encoding    CURRENTLY NOT IMPLEMENTED.
 * @return              XML-formatted string.
 */
const DOMString FormatterToHTML::prepAttrURI(
			const DOMString& string,
			const DOMString& /* specials */,
			const DOMString& /* encoding */)
	// java: throws SAXException
{
	DOMString sb;
	const unsigned int	theLength = length(string);

	for (unsigned int i = 0;  i < theLength;  i ++)
	{
		const XMLCh ch = charAt(string, i);
		DOMString sch(&ch, 1);
		const int ich = ch;
		if(((ch > 0x1F) &&   // X'00 - 1F' not valid
					(ch < 0x7F)) &&   // X'7F' not valid
				(s_escapetb.find(sch)== s_escapetb.end())  ) // characters in the table
		{
			sb += sch;   // valid character, append it
		}
		else
		{
			// need to escape the character
			const int mask1  = 0xFF00;
			const int mask2  = 0x00FF;
			assert (ich < 0xFFFF);
			const int b1 = (int)(((ich) & mask1) >> 8);
			const int b2 = (int)((ich) & mask2);

			// if first 8 bytes are 0, no need to append them.
			if (b1 != 0)
			{	 
				sb += "%";
				sb += LongToHexDOMString(b1);
			}	
			sb += "%";
			sb += LongToHexDOMString(b2);
		}
	}
	return sb;
}
  


FormatterToHTML::EmptiesSetType
FormatterToHTML::createEmpties()
{
	EmptiesSetType	theEmpties;

	theEmpties.insert(XALAN_STATIC_UCODE_STRING("AREA"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("BASE"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("BR"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("COL"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("HR"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("IMG"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("INPUT"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("LINK"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("META"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("PARAM"));

	// HTML 4.0 loose DTD
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("BASEFONT"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("FRAME"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("ISINDEX"));

	return theEmpties;
}

FormatterToHTML::EmptiesSetType
FormatterToHTML::createAttrEmpties()
{
	EmptiesSetType	theEmpties;
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("checked"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("disabled"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("readonly"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("multiple"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("disabled"));
	theEmpties.insert(XALAN_STATIC_UCODE_STRING("selected"));
	return theEmpties;
}

FormatterToHTML::StringSetType
FormatterToHTML::createNonBlockElems()
{
	StringSetType	theElems;
	theElems.insert(XALAN_STATIC_UCODE_STRING("FONT"));
	// s_nonblockelems.insert(XALAN_STATIC_UCODE_STRING("A"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("TD"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("IMG"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("B"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("I"));
	return theElems;
}

FormatterToHTML::StringSetType
FormatterToHTML::createEscapeElems()
{
	StringSetType	theElems;
	theElems.insert(XALAN_STATIC_UCODE_STRING("%"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("<"));
	theElems.insert(XALAN_STATIC_UCODE_STRING(">"));
//	theElems.insert(XALAN_STATIC_UCODE_STRING(" "));
//	theElems.insert(XALAN_STATIC_UCODE_STRING("#"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("{"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("}"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("["));
	theElems.insert(XALAN_STATIC_UCODE_STRING("]"));
//	theElems.insert(XALAN_STATIC_UCODE_STRING("\\"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("|"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("^"));
	theElems.insert(XALAN_STATIC_UCODE_STRING("\""));
//	theElems.insert(XALAN_STATIC_UCODE_STRING("'")); 	  
	return theElems;
}

FormatterToHTML::AttributesMapType
FormatterToHTML::createAttributesMap()
{
	AttributesMapType	theAtts;
	StringSetType URLAttrsHREFSingle;
	URLAttrsHREFSingle.insert(XALAN_STATIC_UCODE_STRING("href"));
	StringSetType URLAttrsCITESingle;
	URLAttrsCITESingle.insert(XALAN_STATIC_UCODE_STRING("cite"));

	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("base")), URLAttrsHREFSingle));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("link")), URLAttrsHREFSingle));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("area")), URLAttrsHREFSingle));
	// From the HTML 4.0 spec: Note. The same conversion based on UTF-8 
	// should be applied to values of the name attribute for the A element. 

	StringSetType URLAttrs_A;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("href"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("name"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("a")), URLAttrs_A));

	StringSetType URLAttrs_INPUT;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("src"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("usemap"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("input")), URLAttrs_INPUT));

	StringSetType URLAttrs_SCRIPT;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("src"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("for"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("script")), URLAttrs_SCRIPT));

	StringSetType URLAttrs_IMG;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("src"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("longdesc"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("usemap"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("img")), URLAttrs_IMG));

	StringSetType URLAttrs_OBJECT;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("classid"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("codebase"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("data"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("archive"));
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("usemap"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("object")), URLAttrs_OBJECT));

	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("q")), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("blockquote")), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ins")), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("del")), URLAttrsCITESingle));

	StringSetType URLAttrs_FORM;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("action"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("form")), URLAttrs_FORM));

	StringSetType URLAttrs_HEAD;
	URLAttrs_A.insert(XALAN_STATIC_UCODE_STRING("profile"));
	theAtts.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("head")), URLAttrs_HEAD));
	return theAtts;
}

FormatterToHTML::HTMLAttributesVectorType
FormatterToHTML::createAttributes()
{
#if defined(XALAN_WIDE_STRING_UCODE_PROBLEM)
	static const char* const	theHTMLAttributes[] = 
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
#else
	static const XMLCh* const	theHTMLAttributes[] = 
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

	static const size_t		theArraySize =
			sizeof(theHTMLAttributes) / sizeof(theHTMLAttributes[0]);

	HTMLAttributesVectorType	theAttributes;

	theAttributes.reserve(theArraySize);

	for(size_t	i = 0; i < theArraySize; i++)
	{
#if defined(XALAN_WIDE_STRING_UCODE_PROBLEM)
		theAttributes.push_back(initializeAndTranscode(theHTMLAttributes[i]));
#else
		theAttributes.push_back(theHTMLAttributes[i]);
#endif
	}

	return theAttributes;
}



FormatterToHTML::HTMLSymbolsVectorType
FormatterToHTML::createSymbols()
{
#if defined(XALAN_WIDE_STRING_UCODE_PROBLEM)
	static const char* const	theHTMLSymbols[] = 
	{
		"Alpha",    "Beta",
		"Gamma",    "Delta",    "Epsilon",    "Zeta",
		"Eta",    "Theta",    "Iota",    "Kappa",
		"Lambda",    "Mu",    "Nu",    "Xi",
		"Omicron",    "Pi",    "Rho",    "Sigma",
		"Tau",    "Upsilon",    "Phi",    "Chi",
		"Psi",    "Omega",    "alpha",    "beta",
		"gamma",    "delta",    "epsilon",    "zeta",
		"eta",    "theta",    "iota",    "kappa",
		"lambda",    "mu",    "nu",    "xi",
		"omicron",    "pi",    "rho",    "sigmaf",
		"sigma",    "tau",    "upsilon",    "phi",
		"chi",    "psi",    "omega",    "thetasym",
		"upsih",    "piv"
	};
#else
	static const XMLCh* const	theHTMLSymbols[] = 
	{
		L"Alpha",    L"Beta",
		L"Gamma",    L"Delta",    L"Epsilon",    L"Zeta",
		L"Eta",    L"Theta",    L"Iota",    L"Kappa",
		L"Lambda",    L"Mu",    L"Nu",    L"Xi",
		L"Omicron",    L"Pi",    L"Rho",    L"Sigma",
		L"Tau",    L"Upsilon",    L"Phi",    L"Chi",
		L"Psi",    L"Omega",    L"alpha",    L"beta",
		L"gamma",    L"delta",    L"epsilon",    L"zeta",
		L"eta",    L"theta",    L"iota",    L"kappa",
		L"lambda",    L"mu",    L"nu",    L"xi",
		L"omicron",    L"pi",    L"rho",    L"sigmaf",
		L"sigma",    L"tau",    L"upsilon",    L"phi",
		L"chi",    L"psi",    L"omega",    L"thetasym",
		L"upsih",    L"piv"
	};
#endif

	static const size_t		theArraySize =
		sizeof(theHTMLSymbols) / sizeof(theHTMLSymbols[0]);

	HTMLSymbolsVectorType	theSymbols;

	theSymbols.reserve(theArraySize);

	for(size_t	i = 0; i < theArraySize; i++)
	{
#if defined(XALAN_WIDE_STRING_UCODE_PROBLEM)
		theSymbols.push_back(initializeAndTranscode(theHTMLSymbols[i]));
#else
		theSymbols.push_back(theHTMLSymbols[i]);
#endif
	}

	return theSymbols;
}
