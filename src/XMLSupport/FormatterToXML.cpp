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


// Class header file.
#include "FormatterToXML.hpp"



#include <sax/AttributeList.hpp>
#include <sax/SAXException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/Writer.hpp>

#include <string>


static const char* const	theDefaultAttrSpecialChars = "<>&\"\'\r\n";


const DOMString FormatterToXML::DEFAULT_MIME_ENCODING = XALAN_STATIC_UCODE_STRING("UTF-8");


XMLCh				FormatterToXML::m_charBuf[MAXCHARBUF];
// This should be OK on all platforms ??
XMLCh 			FormatterToXML::m_lineSep = '\n';
bool				FormatterToXML::m_javaEncodingIsISO = false; 


FormatterToXML::FormatterToXML(
			Writer&				writer,
			const DOMString& /* version */,
			bool doIndent, 
			int indent,
			const DOMString& encoding, 
			const DOMString& /*mediaType */,
			const DOMString& doctypeSystem,
			const DOMString& doctypePublic,
			bool xmlDecl,
			const DOMString& /* standalone */, 
			const QNameVectorType* const /* cdataSectionElems */) :
	FormatterListener(),
	m_attrSpecialChars(theDefaultAttrSpecialChars),
	m_currentIndent(0),
	m_doctypePublic(doctypePublic),
	m_doctypeSystem(doctypeSystem),
	m_doIndent(doIndent),
	m_elemStack(),
	m_encoding(encoding),
	m_escapeCData(false),
	m_indent(indent),
	m_ispreserve(false),
	m_isprevtext(false),
	m_level(0),
	m_maxCharacter(0x007F),
	m_needToOutputDocTypeDecl(true),
	m_nextIsRaw(false),
	m_preserves(),
	m_shouldWriteXMLHeader(xmlDecl),
	m_spaceBeforeClose(false),
	m_startNewLine(true),
	m_stripCData(false),
	m_version(""),
	m_writer(writer)
{
	if(! isEmpty(m_doctypePublic))
	{
		if(startsWith(m_doctypePublic, "-//W3C//DTD XHTML"))
			m_spaceBeforeClose = true;
	}

	// Determine the last printable character based on the output format
	// @@ JMD: We don't have no OutputFormat class yet ...
	// java: m_maxCharacter = format.getLastPrintable();

	m_isUTF8 = equals(m_encoding, "UTF-8") || isEmpty(m_encoding);
	if(isEmpty(m_encoding))
	{
/*
	@@ JMD: Not supported yet:
		java:
		encoding = System.getProperty("file.encoding");
		encoding = (null != encoding) ?
			FormatterToXML.convertJava2MimeEncoding(encoding ) : DEFAULT_MIME_ENCODING; 
*/
		m_encoding = DEFAULT_MIME_ENCODING;
	}

	DOMString2IntMapType::const_iterator it =
		s_revsize.find(toUpperCase(m_encoding));

	if (it != s_revsize.end())
		m_maxCharacter = (*it).second;
	m_isUTF8 = equals(m_encoding, "UTF-8");
	it = s_revsize.find(toUpperCase(m_encoding));
	if (it != s_revsize.end())
	{
      m_maxCharacter = (*it).second;
	}
}



FormatterToXML::~FormatterToXML()
{
}



void
FormatterToXML::setDocumentLocator(const Locator* const	/* locator */)
{
	// I don't do anything with this yet.
}




void
FormatterToXML::startDocument()
{
	try
	{
		m_needToOutputDocTypeDecl = true;
		m_startNewLine = false;

		if(m_shouldWriteXMLHeader)
		{
			DOMString  encoding = m_encoding;
			if(isEmpty(encoding))
			{
				/*
				java:
				try
				{
					encoding = System.getProperty("file.encoding");
					encoding = FormatterToXML.convertJava2MimeEncoding( encoding ); 
				}
				catch(SecurityException se)
				{
					encoding = "ISO-8859-1";
				}
				 */
				encoding = "ISO-8859-1";
			}

			DOMString version = (isEmpty(m_version)) ? "1.0" : m_version;

			m_writer.write("<?xml version=\""+version+"\" encoding=\""+
					encoding + "\"?>");
			m_writer.write(m_lineSep);
		}      
	}
	// java: catch(IOException ioe)
	catch(...)
	{
		throw SAXException("IO error");
	}
}



void
FormatterToXML::endDocument()
{
	try
	{
		m_writer.flush();
	}
	catch(...)
	{
		throw SAXException();
	}
}



void
FormatterToXML::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
	try
	{
		if((true == m_needToOutputDocTypeDecl) && (! isEmpty(m_doctypeSystem)))
		{
			m_writer.write("<!DOCTYPE ");
			m_writer.write(name);
			if(! isEmpty(m_doctypePublic))
			{
				m_writer.write(" PUBLIC \"");
				m_writer.write(m_doctypePublic);
				m_writer.write("\"");
			}
			if(isEmpty(m_doctypePublic))
				m_writer.write(" SYSTEM \"");
			else
				m_writer.write(" \"");
			m_writer.write(m_doctypeSystem);
			m_writer.write("\">");
			m_writer.write(m_lineSep);
		}
		m_needToOutputDocTypeDecl = false;
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
}



void
FormatterToXML::endElement(const XMLCh* const	name)
{
	try
	{
		m_currentIndent -= m_indent;
		const bool	hasChildNodes = childNodesWereAdded();
		if (hasChildNodes == true) 
		{
			if (shouldIndent() == true)
				indent(m_writer, m_currentIndent);
			m_writer.write(DOMString("</") + name + DOMString(">"));
		}
		else
		{
			if(m_spaceBeforeClose)
				m_writer.write(" />");
			else
				m_writer.write("/>");
		}
		if (hasChildNodes == true) 
		{
			if (m_preserves.size() == 0)
			{
				m_ispreserve = false;
			}
			else
			{
				m_ispreserve = m_preserves.top();
				m_preserves.pop();
			}
		}
		m_isprevtext = false;
	}
	catch(...)
	{
		throw SAXException();
	}
}



void
FormatterToXML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(0 == length)
		return;

	if(m_nextIsRaw)
	{
		m_nextIsRaw = false;
		charactersRaw (chars, length);
		return;
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
			const int chNum = ch;
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
			// Regular old ASCII character
			else if((((chNum >= 20) && (chNum <= 126)) 
						|| (chNum == 10)
						|| (chNum == 13)
						|| (chNum == 9)))
			{
				// System.out.println("ch: "+ch);
				m_charBuf[pos++] = ch;
			}
			else if((chNum >= 20) && (ch <= m_maxCharacter))
			{
				// System.out.println("ch(2): "+ch);
				// Hope this is right...
				m_charBuf[pos++] = ch;
			}
			else
			{
				copyBigCharIntoBuf(chars, i, pos, length);
			}
			// Use 80 as a best guess safe buffer
			if(pos > MAXSAFECHARBUF)
			{
				m_writer.write(m_charBuf, 0, pos);
				pos = 0;
			}
		}

		// System.out.println(new String(m_charBuf, 0, pos));
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
FormatterToXML::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	try
	{
		writeParentTagEnd();
		m_ispreserve = true;
		m_writer.write(chars, 0, length);
		m_writer.flush();
	}
	// java: catch(IOException ioe)
	catch(...)
	{
		throw SAXException();
	}
}

void FormatterToXML::copyBigCharIntoBuf(
			const XMLCh* const chars,		// Character string to process
			int&					i,				// Index into 'chars'
			int&					pos,			// Index in m_charBuf
			int					length,		// Length of 'chars' string
			XMLCh* theBuffer /* m_charBuf	*/		// Buffer to write to
			)
/*
 * Processes a non-ASCII character either in hexadecimal format (&#Xnnn;) if
 * the character is in the range 0xd800 to 0xdc00 or decimal format (&#nnn;),
 * and places the result in member 'm_charBuf.'  Side effect, indices 'i' and
 * 'pos' are incremented.  On exit, 'theBuffer' will contain a null terminated
 * Unicode string.
 */
{
	DOMString msg("Invalid UTF-16 surrogate detected: ");
	DOMString ds;
	int c = chars[i];
	if (0xd800 <= c && c < 0xdc00) 
	{
		// UTF-16 surrogate
		int next;
		if (i+1 >= length) 
		{
			msg = append(msg, LongToHexDOMString(c));
			msg = append(msg, " ?");
			throw SAXException(c_wstr(msg));
		}
		else 
		{
			next = chars[++i];
			if (!(0xdc00 <= next && next < 0xe000))
			{
				msg = append(msg, LongToHexDOMString(c));
				msg = append(msg, " ");
				msg = append(msg, LongToHexDOMString(next));
				throw SAXException(c_wstr(msg));
			}
			next = ((c-0xd800)<<10)+next-0xdc00+0x00010000;
		}
		theBuffer[pos++] = '&';
		theBuffer[pos++] = '#';
		theBuffer[pos++] = 'x';
		ds = LongToHexDOMString(next);
		const XMLCh* pb = c_wstr(ds);
		int nIntStr = ds.length();
		for(int k = 0; k < nIntStr; k++)
		{
			theBuffer[pos++] = *(pb+k);
		}
		theBuffer[pos++] = ';';
	}
	else
	{
		theBuffer[pos++] = '&';
		theBuffer[pos++] = '#';
		ds = LongToDOMString(c);
		const XMLCh* pb = c_wstr(ds);
		int nIntStr = ds.length();
		for(int k = 0; k < nIntStr; k++)
		{
			theBuffer[pos++] = *(pb+k);
		}
		theBuffer[pos++] = ';';
	}
	theBuffer[pos] = 0;	// null terminate
}

void FormatterToXML::writeBigChar(const XMLCh* const ch, int& i, int end)
/*
 * Writes a non-ASCII character either in hexadecimal format (&#Xnnn;) if the
 * character is in the range 0xd800 to 0xdc00 or decimal format (&#nnn;); as a
 * side effect the current character index (i) is incremented
 */
{
	XMLCh buffer[32];	// Should be big enough
	int pos = 0;
	copyBigCharIntoBuf(ch, i, pos, end, buffer);
	m_writer.write(buffer);
}

void FormatterToXML::writeNormalizedChars(
		const XMLCh*  const ch,
		int start, int length,
		bool isCData)
{
	int end = start+length;
	for(int i = start; i < end; i++)
	{
		XMLCh c = ch[i];
		if('\n' == c)
		{
			m_writer.write(m_lineSep);
		}
		else if(isCData && (c > m_maxCharacter))
		{
			if(i != 0)
				m_writer.write("]]>");
			writeBigChar(ch, i, end);
			// @@ JMD: this differs from java
			// java: if((i != 0) && (i < (end-1)))
			if( (i < (end-1)))
				m_writer.write("<![CDATA[");
		}
		else if(isCData && ((i < (end-2)) && (']' == c) && 
					(']' == ch[i+1]) && ('>' == ch[i+2])))
		{
			m_writer.write("]]]]><![CDATA[>");
			i+=2;
		}
		else
		{
			if(c <= m_maxCharacter)
			{
				m_writer.write(c);
			}
			else
				writeBigChar(ch, i, end);
		}
	}
}



void
FormatterToXML::entityReference(const XMLCh* const	name)
{
	try
	{
		writeParentTagEnd();
	  
		if (shouldIndent() == true)  
		{
			indent(m_writer, m_currentIndent);
		}

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
FormatterToXML::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	characters(chars, length);
}



void
FormatterToXML::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	try
	{
		writeParentTagEnd();

		if (shouldIndent() == true)  
		{
			indent(m_writer, m_currentIndent);
		}

		m_writer.write("<?");
		m_writer.write(target);

		if (length(data) > 0 && !isSpace(data[0]))
		{
			m_writer.write(" ");
		}

		m_writer.write(data);
		m_writer.write("?>");

		m_startNewLine = true;
	}
	catch(...)
	{
	  throw SAXException();
	}
}



void
FormatterToXML::resetDocument()
{
	// I don't do anything with this yet.
}



void
FormatterToXML::comment(const XMLCh* const	data)
{
	try
	{
		writeParentTagEnd();

		if (shouldIndent() == true)  
		{
			indent(m_writer, m_currentIndent);
		}

		m_writer.write("<!--");
		m_writer.write(data);
		m_writer.write("-->");

		m_startNewLine = true;
	}
	catch(...)
	{
	  throw SAXException();
	}
}



void
FormatterToXML::cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
{

	try
	{
		if(m_nextIsRaw)
		{
			m_nextIsRaw = false;
			charactersRaw (ch, length);
			return;
		}
		if(m_escapeCData) // Should normally always be false.
		{
			characters(ch, length);
		}
		else
		{
			writeParentTagEnd();
			m_ispreserve = true;
			if (shouldIndent())
				indent(m_writer, m_currentIndent);
			if(!m_stripCData)
			{
				if(((length >= 1) && (ch[0] <= m_maxCharacter)))
				{
					m_writer.write("<![CDATA[");
				}
			}
			// m_writer.write(ch, 0, length);
			writeNormalizedChars(ch, 0, length, !m_stripCData);
			if(!m_stripCData)
			{
				if(((length >= 1) && (ch[(length)-1] <= m_maxCharacter)))
				{
					m_writer.write("]]>");
				}
			}
		}
	}
	// java: catch(IOException ioe)
	catch(...)
	{
		throw SAXException("IO error");
	}
}



void
FormatterToXML::writeParentTagEnd()
{
	try
	{
		if(!m_elemStack.empty())
		{
			// See if the parent element has already been flagged as having children.
			if(false == m_elemStack.top())
			{
				m_writer.write(">");

				m_elemStack.pop();
				m_elemStack.push(true);

				m_preserves.push(m_ispreserve);
			}
		}
	}
	catch(...)
	{
		throw SAXException();
	}
}

  

void
FormatterToXML::openElementForChildren()
{
	m_elemStack.push(false);
}



bool
FormatterToXML::childNodesWereAdded()
{
	bool	fResult = false;

	if (m_elemStack.size() > 0)
	{
		fResult = m_elemStack.top();

		m_elemStack.pop();
	}

	return fResult;
}



void
FormatterToXML::processAttribute(
			const DOMString&	name,
			const DOMString&	value)
{
	try
	{
		m_writer.write(" ");
		m_writer.write(name);
		m_writer.write("=\"");
		m_writer.write(prepAttrString(value, m_attrSpecialChars, m_encoding));
		m_writer.write("\"");
	}
	catch(...)
	{
		throw SAXException();
	}
}



DOMString
FormatterToXML::prepAttrString(
			const DOMString& 	string,
			const DOMString& 	specials,
			const DOMString& 	/* encoding */)
{
	const unsigned int	theLength = string.length();

	// we'll do the work into a buffer pre-allocated to
	// twice the size of the original string, giving some
	// room to grow without reallocating.
	//
	// Whem DOMString has gets a better += operator
	// for XMLCh and XMLCh*, a more straightforward
	// solution will give good performance
	std::vector<XMLCh> vec;
	vec.reserve(theLength*2);

	for (unsigned int i = 0;  i < theLength;  i ++) 
	{
		const XMLCh		ch = charAt(string, i);

		const int		chNum = static_cast<int>(ch);

		const int		index = indexOf(specials, ch);

		if (index >= 0) 
		{
			vec.push_back('&');
			vec.push_back('#');
			vec.push_back(ch);
			vec.push_back(';');
		}
		else if (0xd800 <= chNum && chNum < 0xdc00) 
		{
			// UTF-16 surrogate
			int		next = 0;

			if (i + 1 >= theLength) 
			{
				throw SAXException();

				// $$$ ToDo: Fix this!!!
//				throw new SAXException("Invalid UTF-16 surrogate detected: "
//				+Integer.toHexString(ch)+ " ?");
			}
			else 
			{
				next = charAt(string, ++i);

				if (!(0xdc00 <= next && next < 0xe000))
				{
					throw SAXException();

					// $$$ ToDo: Fix this!!!
//					throw new SAXException("Invalid UTF-16 surrogate detected: "
//					  +Integer.toHexString(ch)+" "+Integer.toHexString(next));
				}

				next = ((ch-0xd800)<<10) + next - 0xdc00 + 0x00010000;
			}

			vec.push_back('&');
			vec.push_back('#');
			vec.push_back('x');

			const DOMString num = LongToHexDOMString(next);

			for (unsigned int k=0; k < length(num); k++)
				vec.push_back(charAt(num, k));

			vec.push_back(';');
		}
/*
		else if (null != ctbc && !ctbc.canConvert(ch))
		{
			sb.append("&#x");
			sb.append(Integer.toString((int)ch, 16));
			sb.append(";");
		}
*/
		else 
		{
			vec.push_back(ch);
		}
	}

	vec.push_back('\0');

	return DOMString(vec.begin());
}


	
bool
FormatterToXML::shouldIndent() const
{
	return m_doIndent && (!m_ispreserve && !m_isprevtext);
}



void
FormatterToXML::printSpace(
			Writer&		pw,
			int			n)
{
	try
	{
		for (int i = 0;  i < n;  i++)
		{
			pw.write(' ');
		}
	}
	catch(...)
	{
		throw SAXException();
	}
}



void
FormatterToXML::indent(
			Writer&		pw,
			int			n)
{
	try
	{
		if(m_startNewLine == true)
		{
			pw.write('\n');
		}

		if(m_doIndent == true)
		{
			printSpace(pw, n);
		}
	}
	catch(...)
	{
		throw SAXException();
	}
}

int FormatterToXML::copyEntityIntoBuf(
			const DOMString&	s,
			int					pos)
{
	const int	l = length(s);
	m_charBuf[pos++] = '&';
	for(int i = 0; i < l; i++)
	{
		m_charBuf[pos++] = charAt(s, i);
	}
	m_charBuf[pos++] = ';';
	return pos;
}

int FormatterToXML::copyUTF16IntoBuf(
			const XMLCh* const chars,		// Character string to process
			int&					i,				// Index into 'chars'
			int&					pos,			// Index in m_charBuf
			int					length		// Length of 'chars' string
			)
/*
 * Processes a UTF-16 surrogate character sequence, and places the result in
 * member 'm_charBuf.'  Side effect, indices 'i' and 'pos' are incremented.
 * Method consumes two characters from the input buffer 
 */
{
	DOMString msg("Invalid UTF-16 surrogate detected: ");
	// UTF-16 surrogate
	int next;
	int ch = chars[i];
	if (i+1 >= length) 
	{
		msg = append(msg, LongToHexDOMString(ch));
		throw SAXException(c_wstr(msg));
	}
	else 
	{
		next = chars[++i];
		if (!(0xdc00 <= next && next < 0xe000))
		{
			msg = append(msg, LongToHexDOMString(ch));
			msg = append(msg, " ");
			msg = append(msg, LongToHexDOMString(next));
		throw SAXException(c_wstr(msg));
		}
		next = ((ch-0xd800)<<10)+next-0xdc00+0x00010000;
	}
	m_charBuf[pos++] = '&';
	m_charBuf[pos++] = '#';
	DOMString ds;
	ds = LongToDOMString(ch);
	const XMLCh* pb = c_wstr(ds);
	int nIntStr = ds.length();
	for(int k = 0; k < nIntStr; k++)
	{
		m_charBuf[pos++] = pb[k];
	}
	m_charBuf[pos++] = ';';
	return pos;
}

FormatterToXML::DOMStringMapType  FormatterToXML::s_enchash;
FormatterToXML::DOMStringMapType FormatterToXML::s_revhash;
FormatterToXML::DOMString2IntMapType FormatterToXML::s_revsize;

const DOMString FormatterToXML::convertMime2JavaEncoding(
		const DOMString& mimeCharsetName)
{
	if (m_javaEncodingIsISO) return mimeCharsetName;
	DOMStringMapType::const_iterator it =
		s_enchash.find(toUpperCase(mimeCharsetName));
	return (*it).second;
}		

const DOMString FormatterToXML::convertJava2MimeEncoding(
		const DOMString& encoding)
{
	if (m_javaEncodingIsISO) return encoding;
	DOMStringMapType::const_iterator it =
		s_revhash.find(toUpperCase(encoding));
	return (*it).second;
}

void FormatterToXML::initEncodings() 
{
	if(0 != s_enchash.size())
		return;

	bool useISOPrefix = false;
/*
	java:
	try 
	{
		java.io.ByteArrayOutputStream os = new java.io.ByteArrayOutputStream();
		os.write(32);
		String s = os.toString("ISO8859_1");
		// Just in case it doesn't throw an exception...
		if(null == s)
			useISOPrefix = false;
		else
			useISOPrefix = true;
	}
	catch (java.io.UnsupportedEncodingException e) 
	{
		useISOPrefix = false;
	}
*/

	// A bit of a hack for the blackdown VMs (and probably some others).
/*
	java:
	try
	{
		String encoding = System.getProperty("file.encoding");

		int dashindex = (encoding != null ? encoding.indexOf('-') : -1);
		if(3 == dashindex)
		{
			String ISOprefix =  new String(encoding.toCharArray(), 0, 3);
			if (ISOprefix.equals("ISO") == true)
				javaEncodingIsISO = true;
		}
	}
	catch(SecurityException se)
	{
	}
*/

	// Make a table to maximum character sizes before we 
	// need to resort to escaping in the XML.
	// TODO: To tell the truth, I'm guessing a bit here. 
	// s_revsize.insert(std::make_pair("CP1252",          0xFF)); // Windows Latin-1 
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")),    0xFF)); // Windows 1250 Peter Smolik
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("UTF-8")),           0xFFFF)); // Universal Transformation Format 8
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),        0x7F));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")),      0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP")),     0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("SHIFT_JIS")),       0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EUC-JP")),          0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("GB2312")),          0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("BIG5")),            0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EUC-KR")),          0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR")),     0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("KOI8-R")),          0xFFFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1")),   0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE")), 0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS")),    0xFF));
	s_revsize.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2")),   0xFF));

	//    <preferred MIME name>, <Java encoding name>
	// s_enchash.insert(std::make_pair("ISO 8859-1", "CP1252")); // Close enough, I guess
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")), DOMString(XALAN_STATIC_UCODE_STRING("CP1250")))); // Peter Smolik
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("UTF-8")), DOMString(XALAN_STATIC_UCODE_STRING("UTF8"))));
	if(useISOPrefix)
	{
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1"))));    // ?
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_2"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_3"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_4"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_5"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_6"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_7"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_8"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")),
		DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_9"))));
	}
	else
	{
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_1"))));    // ?
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_1"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_2"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_3"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_4"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_5"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_6"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_7"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_8"))));
		s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")),
		DOMString(XALAN_STATIC_UCODE_STRING("8859_9"))));
	}
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP")),     DOMString(XALAN_STATIC_UCODE_STRING("JIS"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("SHIFT_JIS")),       DOMString(XALAN_STATIC_UCODE_STRING("SJIS"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EUC-JP")),          DOMString(XALAN_STATIC_UCODE_STRING("EUCJIS"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("GB2312")),          DOMString(XALAN_STATIC_UCODE_STRING("GB2312"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("BIG5")),            DOMString(XALAN_STATIC_UCODE_STRING("Big5"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EUC-KR")),          DOMString(XALAN_STATIC_UCODE_STRING("KSC5601"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR")),     DOMString(XALAN_STATIC_UCODE_STRING("ISO2022KR"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("KOI8-R")),          DOMString(XALAN_STATIC_UCODE_STRING("KOI8_R"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US")),    DOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA")),    DOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL")),    DOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK")),    DOMString(XALAN_STATIC_UCODE_STRING("CP277"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO")),    DOMString(XALAN_STATIC_UCODE_STRING("CP277"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI")),    DOMString(XALAN_STATIC_UCODE_STRING("CP278"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE")),    DOMString(XALAN_STATIC_UCODE_STRING("CP278"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT")),    DOMString(XALAN_STATIC_UCODE_STRING("CP280"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES")),    DOMString(XALAN_STATIC_UCODE_STRING("CP284"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB")),    DOMString(XALAN_STATIC_UCODE_STRING("CP285"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR")),    DOMString(XALAN_STATIC_UCODE_STRING("CP297"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1")),   DOMString(XALAN_STATIC_UCODE_STRING("CP420"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE")),    DOMString(XALAN_STATIC_UCODE_STRING("CP424"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH")),    DOMString(XALAN_STATIC_UCODE_STRING("CP500"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE")), DOMString(XALAN_STATIC_UCODE_STRING("CP870"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU")),    DOMString(XALAN_STATIC_UCODE_STRING("CP870"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS")),    DOMString(XALAN_STATIC_UCODE_STRING("CP871"))));
	s_enchash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2")),   DOMString(XALAN_STATIC_UCODE_STRING("CP918"))));

	// j:CNS11643 -> EUC-TW?
	// ISO-2022-CN? ISO-2022-CN-EXT?

	//    <Java encoding name>, <preferred MIME name>
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP1252")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")))); // Close enough, I guess
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP1250")), DOMString(XALAN_STATIC_UCODE_STRING("windows-1250")))); // Peter Smolik
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("UTF8")), DOMString(XALAN_STATIC_UCODE_STRING("UTF-8"))));
	//s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1")), DOMString(XALAN_STATIC_UCODE_STRING("US-ASCII"))));    // ?
	if(useISOPrefix)
	{
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_2")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_3")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_4")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_5")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_6")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_7")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_8")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO8859_9")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9"))));
	}
	else
	{
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_1")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_2")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_3")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_4")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_5")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_6")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_7")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_8")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8"))));
		s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("8859_9")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9"))));
	}
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("JIS")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("SJIS")), DOMString(XALAN_STATIC_UCODE_STRING("Shift_JIS"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("EUCJIS")), DOMString(XALAN_STATIC_UCODE_STRING("EUC-JP"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("GB2312")), DOMString(XALAN_STATIC_UCODE_STRING("GB2312"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("BIG5")), DOMString(XALAN_STATIC_UCODE_STRING("Big5"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("KSC5601")), DOMString(XALAN_STATIC_UCODE_STRING("EUC-KR"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("ISO2022KR")), DOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("KOI8_R")), DOMString(XALAN_STATIC_UCODE_STRING("KOI8-R"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP037")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP037")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP037")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP277")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP277")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP278")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP278")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP280")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP284")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP285")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP297")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP420")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP424")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP500")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP870")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP870")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP871")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS"))));
	s_revhash.insert(std::make_pair(DOMString(XALAN_STATIC_UCODE_STRING("CP918")), DOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2"))));
}
