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


#include <sax/SaxException.hpp>



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
			else
			{
				m_writer.write("<!DOCTYPE ");
				if(equals(theName, "HTML"))
					m_writer.write("html");  // match Clark
				else
					m_writer.write(name);
				if(! isEmpty(m_version))
				{
					// Not totally sure about this.
					m_writer.write(" PUBLIC \"-//W3C//DTD "+m_version+" //EN\">");
					m_writer.write(m_lineSep);
				}
				else
				{
					m_writer.write(" PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">");
					m_writer.write(m_lineSep);
				}
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
	// @@ JMD: What's this ??
	bool noLineBreak;
	// If the previous element is a non-block element or the next 
	// element is a non-block element, then do not indent.
	m_doIndent =
	((s_nonblockelems.end() != s_nonblockelems.find(toUpperCase(theName))) ||
	((! isEmpty(m_currentElementName)) &&
		(s_nonblockelems.end() != s_nonblockelems.find(toUpperCase(m_currentElementName)))))
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
      bool isWhitespaceSensitive 
        = (s_nonblockelems.end() != s_nonblockelems.find(toUpperCase(name)));

		if (hasChildNodes == true) 
		{
			if (shouldIndent() == true && ! isWhitespaceSensitive)
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

	std::basic_string<XMLCh> msg(L"Invalid UTF-16 surrogate detected: ");
	XMLCh buffer[32];	// Should be big enough

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
			int ch = chars[i];
			int chNum = ch;
			if ('\n' == ch) 
			{
			/*
				java:
				for(int k = 0; k < m_lineSepLen; k++)
				{
					m_charBuf[pos++] = m_lineSep.charAt(k);
				}
			*/
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

			if(s_attrempties.find(toLowerCase(aname)) == s_attrempties.end())
			{
				m_writer.write(' ');
				m_writer.write(aname);
				m_writer.write("=\"");
				m_writer.write(pval);
				m_writer.write('\"');
			}
			else
			{
				m_writer.write(' ');
				if((pval.length() == 0) || equalsIgnoreCase(pval, aname))
				{
					m_writer.write(aname);
				}
				else
				{
					m_writer.write(aname);
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
 * @see #backReference
 * NOTE: return value destroyed on subsequent calls
 */
const DOMString& FormatterToHTML::prepAttrURI(
			const DOMString& string,
			const DOMString& specials,
			const DOMString& encoding)
	// java: throws SAXException
{
	static DOMString sb;
	sb = "";
	for (int i = 0;  i < string.length();  i ++)
	{
		XMLCh ch = charAt(string, i);
		DOMString sch(&ch, 1);
		int ich = ch;
		if(((ch > 0x1F) &&   // X'00 - 1F' not valid
					(ch < 0x7F)) &&   // X'7F' not valid
				(s_escapetb.find(sch)== s_escapetb.end())  ) // characters in the table
		{
			sb += ch;   // valid character, append it
		}
		else
		{
			// need to escape the character
			int mask1  = 0xFF00;
			int mask2  = 0x00FF;
			assert (ich < 0xFFFF);
			int b1 = (int)(((ich) & mask1) >> 8);
			int b2 = (int)((ich) & mask2);
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

	theEmpties.insert("AREA");
	theEmpties.insert("BASE");
	theEmpties.insert("BR");
	theEmpties.insert("COL");
	theEmpties.insert("HR");
	theEmpties.insert("IMG");
	theEmpties.insert("INPUT");
	theEmpties.insert("LINK");
	theEmpties.insert("META");
	theEmpties.insert("PARAM");

	// HTML 4.0 loose DTD
	theEmpties.insert("BASEFONT");
	theEmpties.insert("FRAME");
	theEmpties.insert("ISINDEX");

	return theEmpties;
}

FormatterToHTML::EmptiesSetType
FormatterToHTML::createAttrEmpties()
{
	EmptiesSetType	theEmpties;
	theEmpties.insert("checked");
	theEmpties.insert("disabled");
	theEmpties.insert("readonly");
	theEmpties.insert("multiple");
	theEmpties.insert("disabled");
	theEmpties.insert("selected");
	return theEmpties;
}

FormatterToHTML::StringSetType
FormatterToHTML::createNonBlockElems()
{
	StringSetType	theElems;
	theElems.insert("FONT");
	// s_nonblockelems.insert("A");
	theElems.insert("TD");
	theElems.insert("IMG");
	theElems.insert("B");
	theElems.insert("I");
	return theElems;
}

FormatterToHTML::StringSetType
FormatterToHTML::createEscapeElems()
{
	StringSetType	theElems;
	theElems.insert("%");
	theElems.insert("<");
	theElems.insert(">");
//	theElems.insert(" ");
//	theElems.insert("#");
	theElems.insert("{");
	theElems.insert("}");
	theElems.insert("[");
	theElems.insert("]");
//	theElems.insert("\\");
	theElems.insert("|");
	theElems.insert("^");
	theElems.insert("\"");
//	theElems.insert("'"); 	  
	return theElems;
}

FormatterToHTML::AttributesMapType
FormatterToHTML::createAttributesMap()
{
	AttributesMapType	theAtts;
	StringSetType URLAttrsHREFSingle;
	URLAttrsHREFSingle.insert(DOMString("href"));
	StringSetType URLAttrsCITESingle;
	URLAttrsCITESingle.insert(DOMString("cite"));

	theAtts.insert(std::make_pair(DOMString("base"), URLAttrsHREFSingle));
	theAtts.insert(std::make_pair(DOMString("link"), URLAttrsHREFSingle));
	theAtts.insert(std::make_pair(DOMString("area"), URLAttrsHREFSingle));
	// From the HTML 4.0 spec: Note. The same conversion based on UTF-8 
	// should be applied to values of the name attribute for the A element. 

	StringSetType URLAttrs_A;
	URLAttrs_A.insert(DOMString("href"));
	URLAttrs_A.insert(DOMString("name"));
	theAtts.insert(std::make_pair(DOMString("a"), URLAttrs_A));

	StringSetType URLAttrs_INPUT;
	URLAttrs_A.insert(DOMString("src"));
	URLAttrs_A.insert(DOMString("usemap"));
	theAtts.insert(std::make_pair(DOMString("input"), URLAttrs_INPUT));

	StringSetType URLAttrs_SCRIPT;
	URLAttrs_A.insert(DOMString("src"));
	URLAttrs_A.insert(DOMString("for"));
	theAtts.insert(std::make_pair(DOMString("script"), URLAttrs_SCRIPT));

	StringSetType URLAttrs_IMG;
	URLAttrs_A.insert(DOMString("src"));
	URLAttrs_A.insert(DOMString("longdesc"));
	URLAttrs_A.insert(DOMString("usemap"));
	theAtts.insert(std::make_pair(DOMString("img"), URLAttrs_IMG));

	StringSetType URLAttrs_OBJECT;
	URLAttrs_A.insert(DOMString("classid"));
	URLAttrs_A.insert(DOMString("codebase"));
	URLAttrs_A.insert(DOMString("data"));
	URLAttrs_A.insert(DOMString("archive"));
	URLAttrs_A.insert(DOMString("usemap"));
	theAtts.insert(std::make_pair(DOMString("object"), URLAttrs_OBJECT));

	theAtts.insert(std::make_pair(DOMString("q"), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString("blockquote"), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString("ins"), URLAttrsCITESingle));
	theAtts.insert(std::make_pair(DOMString("del"), URLAttrsCITESingle));

	StringSetType URLAttrs_FORM;
	URLAttrs_A.insert(DOMString("action"));
	theAtts.insert(std::make_pair(DOMString("form"), URLAttrs_FORM));

	StringSetType URLAttrs_HEAD;
	URLAttrs_A.insert(DOMString("profile"));
	theAtts.insert(std::make_pair(DOMString("head"), URLAttrs_HEAD));
	return theAtts;
}

FormatterToHTML::HTMLAttributesVectorType
FormatterToHTML::createAttributes()
{
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

	static const size_t		theArraySize =
			sizeof(theHTMLAttributes) / sizeof(theHTMLAttributes[0]);

	HTMLAttributesVectorType	theAttributes;

	theAttributes.reserve(theArraySize);

	for(size_t	i = 0; i < theArraySize; i++)
	{
		theAttributes.push_back(theHTMLAttributes[i]);
	}

	return theAttributes;
}



FormatterToHTML::HTMLSymbolsVectorType
FormatterToHTML::createSymbols()
{
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
	static const size_t		theArraySize =
		sizeof(theHTMLSymbols) / sizeof(theHTMLSymbols[0]);
	HTMLSymbolsVectorType	theSymbols;
	theSymbols.reserve(theArraySize);
	for(size_t	i = 0; i < theArraySize; i++)
	{
		theSymbols.push_back(theHTMLSymbols[i]);
	}
	return theSymbols;
}
