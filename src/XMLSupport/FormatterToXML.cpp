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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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



static const XalanDOMChar 	theDefaultAttrSpecialChars[] = {'<', '>', '&', '"', '\r', '\n', 0 };


const XalanDOMCharVectorType	FormatterToXML::s_xsltNextIsRawString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("xslt-next-is-raw")));

const XalanDOMCharVectorType	FormatterToXML::s_formatterToDOMString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("formatter-to-dom")));

const XalanDOMCharVectorType	FormatterToXML::s_defaultMIMEEncoding = 
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("UTF-8")));

const XalanDOMCharVectorType	FormatterToXML::s_doctypeHeaderStartString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("<!DOCTYPE ")));

const XalanDOMCharVectorType	FormatterToXML::s_doctypeHeaderPublicString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING(" PUBLIC \"")));

const XalanDOMCharVectorType	FormatterToXML::s_doctypeHeaderSystemString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING(" SYSTEM \"")));

const XalanDOMCharVectorType	FormatterToXML::s_defaultVersionString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("1.0")));

const XalanDOMCharVectorType	FormatterToXML::s_xmlHeaderStartString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("<?xml version=\"")));

const XalanDOMCharVectorType	FormatterToXML::s_xmlHeaderEncodingString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("\" encoding=\"")));

const XalanDOMCharVectorType	FormatterToXML::s_xmlHeaderStandaloneString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("\" standalone=\"")));

const XalanDOMCharVectorType	FormatterToXML::s_xmlHeaderEndString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("\"?>")));

const XalanDOMCharVectorType	FormatterToXML::s_windows1250EncodingString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")));

const XalanDOMCharVectorType	FormatterToXML::s_usASCIIEncodingString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("US-ASCII")));

const XalanDOMCharVectorType	FormatterToXML::s_asciiEncodingString =
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("ASCII")));

const XalanDOMCharVectorType	FormatterToXML::s_utf8EncodingString=
		MakeXalanDOMCharVector(c_wstr(XALAN_STATIC_UCODE_STRING("UTF-8")));

XalanDOMChar				FormatterToXML::s_lineSep = '\n';


bool						FormatterToXML::s_javaEncodingIsISO = false; 


const FormatterToXML::DOMCharBufferType::size_type	FormatterToXML::s_maxBufferSize = 512;


FormatterToXML::FormatterToXML(
			Writer& 				writer,
			const XalanDOMString&	version,
			bool					doIndent,
			int 					indent,
			const XalanDOMString&	encoding, 
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone,
			eFormat					format) :
	FormatterListener(format),
	m_writer(writer),
	m_maxCharacter(0x7Fu),
	m_attrCharsMap(),
	m_charsMap(),
	m_shouldWriteXMLHeader(xmlDecl),
	m_ispreserve(false),
	m_doIndent(doIndent),
	m_startNewLine(false),
	m_needToOutputDocTypeDecl(true),
	m_isprevtext(false),
	m_stripCData(false),
	m_nextIsRaw(false),
	m_inCData(false),
	m_isUTF8(false),
	m_doctypeSystem(doctypeSystem),
	m_doctypePublic(doctypePublic),
	m_encoding(isEmpty(encoding) == false ? encoding :
			XalanDOMString(&s_defaultMIMEEncoding[0], s_defaultMIMEEncoding.size() - 1)),
	m_currentIndent(0),
	m_indent(indent),
	m_preserves(),
	m_shouldFlush(true),
	m_bytesEqualChars(false),
	m_spaceBeforeClose(false),
	m_escapeCData(false),
	m_inEntityRef(false),
	m_version(version),
	m_standalone(standalone),
	m_mediaType(mediaType),
	m_attrSpecialChars(theDefaultAttrSpecialChars),
	m_charBuf(),
	m_byteBuf(),
	m_pos(0),
	m_level(0),
	m_elemStack()
{
	initCharsMap();

	if(isEmpty(m_doctypePublic) == false)
	{
		if(startsWith(
			m_doctypePublic,
			XALAN_STATIC_UCODE_STRING("-//W3C//DTD XHTML")) == true)
		{
			m_spaceBeforeClose = true;
		}
	}

	m_isUTF8 = equals(m_encoding, s_utf8EncodingString); // || isEmpty(m_encoding);

	if (equals(m_encoding, s_windows1250EncodingString) == true ||
        equals(m_encoding, s_usASCIIEncodingString) == true ||
		equals(m_encoding, s_asciiEncodingString) == true)
	{
		m_bytesEqualChars = true;

		m_byteBuf.resize(s_maxBufferSize + 1);
	}
	else
	{
		m_charBuf.resize(s_maxBufferSize + 1);
	}

#if 0
	DOMString2IntMapType::const_iterator it =
		s_revsize.find(toUpperCase(m_encoding));

	if (it != s_revsize.end())
		m_maxCharacter = (*it).second;
	m_isUTF8 = equals(m_encoding, XALAN_STATIC_UCODE_STRING("UTF-8"));
	it = s_revsize.find(toUpperCase(m_encoding));
	if (it != s_revsize.end())
	{
	  m_maxCharacter = (*it).second;
	}
#endif
}



FormatterToXML::~FormatterToXML()
{
}



void
FormatterToXML::initAttrCharsMap()
{
	memset(m_attrCharsMap, 0, sizeof(m_attrCharsMap));

	const unsigned int	nSpecials = length(m_attrSpecialChars);

	for(unsigned int i = 0; i < nSpecials; ++i)
	{
		m_attrCharsMap[charAt(m_attrSpecialChars, i)] = 'S';
	}

	m_attrCharsMap[0x0A] = 'S';
	m_attrCharsMap[0x0D] = 'S';
}



void
FormatterToXML::initCharsMap()
{
	initAttrCharsMap();

	memset(m_charsMap, 0, sizeof(m_charsMap));

	m_charsMap['\n'] = 'S';
	m_charsMap['<'] = 'S';
	m_charsMap['>'] = 'S';
	m_charsMap['&'] = 'S';

	memset(m_charsMap, 'S', 20);

	m_charsMap[0x0A] = 'S';
	m_charsMap[0x0D] = 'S';
	m_charsMap[9] = '\0';

	for(int i = m_maxCharacter; i < SPECIALSSIZE; ++i)
	{
		m_charsMap[i] = 'S';
	}
}



void
FormatterToXML::outputDocTypeDecl(const XalanDOMString& 	name)
{
	accum(s_doctypeHeaderStartString);	// "<!DOCTYPE "

	accum(name);
	  
	if(length(m_doctypePublic) != 0)
	{
		accum(s_doctypeHeaderPublicString); // " PUBLIC \""
		accum(m_doctypePublic);
		accum('"');
		accum(' ');
		accum('"');
	}
	else
	{
		accum(s_doctypeHeaderSystemString); // " SYSTEM \""
	}

	accum(m_doctypeSystem);
	accum('"');
	accum('>');

	outputLineSep();
}



void
FormatterToXML::accum(char	ch)
{
	if(m_bytesEqualChars == true)
	{
		m_byteBuf[m_pos++] = ch;

		if(m_pos == s_maxBufferSize)
		{
			flushBytes();
		}
	}
	else
	{
		m_charBuf[m_pos++] = ch;

		if(m_pos == s_maxBufferSize)
		{
			flushChars();
		}
	}
}



void
FormatterToXML::accum(XalanDOMChar	ch)
{
	if(m_bytesEqualChars == true)
	{
		m_byteBuf[m_pos++] = static_cast<ByteBufferType::value_type>(ch);

		if(m_pos == s_maxBufferSize)
		{
			flushBytes();
		}
	}
	else
	{
		m_charBuf[m_pos++] = ch;

		if(m_pos == s_maxBufferSize)
		{
			flushChars();
		}
	}
}



void
FormatterToXML::accum(
			const XalanDOMChar	chars[],
			unsigned int		start,
			unsigned int		length)
{
	const DOMCharBufferType::size_type	n = start + length;

	if(m_bytesEqualChars == true)
	{
		for(DOMCharBufferType::size_type i = start; i < n; ++i)
		{
			m_byteBuf[m_pos++] = static_cast<ByteBufferType::value_type>(chars[i]);

			if(m_pos == s_maxBufferSize)
			{
				flushBytes();
			}
		}
	}
	else
	{
		for(DOMCharBufferType::size_type i = start; i < n; ++i)
		{
			m_charBuf[m_pos++] = chars[i];

			if(m_pos == s_maxBufferSize)
			{
				flushChars();
			}
		}
	}
}



void
FormatterToXML::accum(const XalanDOMString&		str)
{
	accum(c_wstr(str), 0, length(str));
}



void
FormatterToXML::accum(const XalanDOMCharVectorType& 	theVector)
{
	accum(c_wstr(theVector), 0, theVector.size() - 1);
}



void
FormatterToXML::throwInvalidUTF16SurrogateException(XalanDOMChar	ch)
{
	const XalanDOMString	theMessage("Invalid UTF-16 surrogate detected: " +
									   UnsignedLongToHexDOMString(ch) +
									   " ?");

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXML::throwInvalidUTF16SurrogateException(
			XalanDOMChar	ch,
			unsigned int	next)
{
	const XalanDOMString	theMessage("Invalid UTF-16 surrogate detected: " +
									   UnsignedLongToHexDOMString(ch) +
									   UnsignedLongToHexDOMString(next) +
									   " ?");

	throw SAXException(c_wstr(theMessage));
}



void
FormatterToXML::accumDefaultEscape(
			XalanDOMChar		ch,
			unsigned int		i,
			const XalanDOMChar	chars[],
			unsigned int		len,
			bool				escLF)
{
	if(!accumDefaultEntity(ch, i, chars, len, escLF))
	{
		if (0xd800 <= ch && ch < 0xdc00) 
		{
			// UTF-16 surrogate
			unsigned int next = 0;

			if (i + 1 >= len) 
			{
				throwInvalidUTF16SurrogateException(ch);
			}
			else 
			{
				next = chars[++i];

				if (!(0xdc00 <= next && next < 0xe000))
				{
					throwInvalidUTF16SurrogateException(ch, next);
				}

				next = ((ch-0xd800) << 10) + next - 0xdc00 + 0x00010000;
			}

			writeNumberedEntityReference(next);
		}
		else 
		{
			if(ch > m_maxCharacter || (ch < SPECIALSSIZE && m_attrCharsMap[ch] == 'S'))
			{
				writeNumberedEntityReference(ch);
			}
			else
			{
				accum(ch);
			}
		}
	}
}



bool
FormatterToXML::accumDefaultEntity(
			XalanDOMChar		ch,
			unsigned int		i,
			const XalanDOMChar	chars[],
			unsigned int		len,
			bool				escLF)
{
	if (escLF == false &&
		0x0D == ch &&
		i + 1 < len &&
		0x0A == chars[i + 1]) 
	{
		outputLineSep();

		i++;
	}
	else if (escLF == false &&
			 0x0A == ch &&
			 i + 1 < len &&
			 0x0D == chars[i + 1])
	{
		outputLineSep();

		i++;
	}
	else if (escLF == false && 0x0D == ch) 
	{
		outputLineSep();

		i++;
	}
	else if (escLF == false && '\n' == ch) 
	{
		outputLineSep();
	}
	else if ('<' == ch) 
	{
		accum('&');
		accum('l');
		accum('t');
		accum(';');
	}
	else if ('>' == ch) 
	{
		accum('&');
		accum('g');
		accum('t');
		accum(';');
	}
	else if ('&' == ch) 
	{
		accum('&');
		accum('a');
		accum('m');
		accum('p');
		accum(';');
	}
	else if ('"' == ch) 
	{
		accum('&');
		accum('q');
		accum('u');
		accum('o');
		accum('t');
		accum(';');
	}
	else if ('\'' == ch) 
	{
		accum('&');
		accum('a');
		accum('p');
		accum('o');
		accum('s');
		accum(';');
	}
	else
	{
		return false;
	}

	return true;
}



void
FormatterToXML::flushBytes()
{
	assert(m_byteBuf.size() > 0 && m_byteBuf.size() > m_pos);

	m_byteBuf[m_pos] = '\0';

	m_writer.write(&m_byteBuf[0]);

	m_pos = 0;
}



void
FormatterToXML::flushChars()
{
	assert(m_charBuf.size() > 0 && m_charBuf.size() > m_pos);

	m_charBuf[m_pos] = 0;

	m_writer.write(&m_charBuf[0]);

	m_pos = 0;
}



void
FormatterToXML::flush()
{
	if(m_bytesEqualChars == true)
	{
		flushBytes();
	}
	else
	{
		flushChars();
	}
}



void
FormatterToXML::flushWriter()
{
	m_writer.flush();
}



void
FormatterToXML::setDocumentLocator(const Locator* const 	/* locator */)
{
	// I don't do anything with this yet.
}



void
FormatterToXML::startDocument()
{
	if(m_inEntityRef == false)
	{
		m_needToOutputDocTypeDecl = true;
		m_startNewLine = false;

		if(m_shouldWriteXMLHeader == true)
		{
			XalanDOMString	encoding = m_encoding;

			if(isEmpty(encoding) == true)
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
				encoding = XALAN_STATIC_UCODE_STRING("ISO-8859-1");
			}

			accum(s_xmlHeaderStartString);	// "<?xml version=\""

			if (length(m_version) != 0)
			{
				accum(m_version);
			}
			else
			{
				accum(s_defaultVersionString);
			}

			accum(s_xmlHeaderEncodingString);	// "\" encoding=\""
			accum(encoding);

			if (length(m_standalone) != 0)
			{
				accum(s_xmlHeaderStandaloneString);
				accum(m_standalone);
			}

			accum(s_xmlHeaderEndString);

			outputLineSep();
		}	   
	}
}



void
FormatterToXML::endDocument()
{
	if(m_doIndent == true && m_isprevtext == false)
	{
		outputLineSep();
	}

	flush();
	flushWriter();
}



void
FormatterToXML::startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs)
{
    if(m_inEntityRef == false)
	{
		if(true == m_needToOutputDocTypeDecl &&
		   isEmpty(m_doctypeSystem) == false)
		{
			outputDocTypeDecl(name);

			m_needToOutputDocTypeDecl = false;
		}

		writeParentTagEnd();

		m_ispreserve = false;

		if (shouldIndent() == true &&
			m_startNewLine == true) 
		{
			indent(m_currentIndent);
		}

		m_startNewLine = true;

		accum('<');
		accum(name);

		const unsigned int	nAttrs = attrs.getLength();

		for (unsigned int i = 0;  i < nAttrs ;  i++)
		{
			processAttribute(attrs.getName(i), attrs.getValue(i));
		}

		// Flag the current element as not yet having any children.
		openElementForChildren();

		m_currentIndent += m_indent;

		m_isprevtext = false;
	}
}



void
FormatterToXML::endElement(const XMLCh* const	name)
{
	m_currentIndent -= m_indent;

	const bool	hasChildNodes = childNodesWereAdded();

	if (hasChildNodes == true) 
	{
		if (shouldIndent() == true)
		{
			indent(m_currentIndent);
		}

		accum('<');
		accum('/');
		accum(name);
	}
	else
	{
		if(m_spaceBeforeClose == true)
		{
			accum(' ');
		}

		accum('/');
	}

	accum('>');

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



void
FormatterToXML::processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
{
	if(m_inEntityRef == false)
	{
		// Use a fairly nasty hack to tell if the next node is supposed to be 
		// unescaped text.
		if(equals(target, c_wstr(s_xsltNextIsRawString)) == true
			&& equals(data, c_wstr(s_formatterToDOMString)) == true)
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

			accum('<');
			accum('?');
			accum(target);

			const unsigned int	len = length(data);

			if ( len > 0 && !isSpace(data[0]))
			{
				accum(' ');
			}

			accumNormalizedPIData(data, len);

			accum('?');
			accum('>');

			m_startNewLine = true;
		}
	}
}



void
FormatterToXML::characters(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if(m_inEntityRef == false && length != 0)
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
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			for (unsigned int i = 0; i < length; ++i) 
			{
				const XalanDOMChar	ch = chars[i];

				if(ch < SPECIALSSIZE && m_charsMap[ch] != 'S')
				{
					accum(ch);
				}
				else
				{
					accumDefaultEscape(ch, i, chars, length, false);
				}
			}

			if (m_isprevtext == false)
			{
				m_isprevtext = true;
			}
		}
	}
}




void
FormatterToXML::charactersRaw(
		const XMLCh* const	chars,
		const unsigned int	length)
{
	if(m_inEntityRef == false)
	{
		writeParentTagEnd();

		m_ispreserve = true;

		accum(chars, 0, length);
	}
}



void
FormatterToXML::writeAttrString(
			const XalanDOMChar*		string,
			const XalanDOMString&	/* encoding */)
{
    const unsigned int	len = length(string);

    for (unsigned int i = 0;  i < len;  i ++) 
    {
		const XalanDOMChar	ch = string[i];

		if(ch < SPECIALSSIZE &&
		   m_attrCharsMap[ch] != 'S')
		{
			accum(ch);
		}
		else
		{
			accumDefaultEscape(ch, i, string, len, true);
		}
    }
}



void
FormatterToXML::writeNormalizedChars(
			const XalanDOMChar	ch[],
			unsigned int		start,
			unsigned int		length,
			bool				isCData)
{
    unsigned int	end = start + length;

    for(unsigned int i = start; i < end; i++)
    {
		const XalanDOMChar	c = ch[i];

		if (0x0D == c &&
			i + 1 < end &&
			0x0A == ch[i + 1])
		{
			outputLineSep();

			i++;
		}
		else if (0x0A == c &&
				 i + 1 < end &&
				 0x0D == ch[i + 1]) 
		{
			outputLineSep();

			i++;
		}
		else if('\n' == c)
		{
			outputLineSep();
		}
		else if(isCData == true && c > m_maxCharacter)
		{
			if(i != 0)
			{
				accum(XALAN_STATIC_UCODE_STRING("]]>"));
			}

			// This needs to go into a function... 
			if (0xd800 <= ((int)c) && ((int)c) < 0xdc00) 
			{
				// UTF-16 surrogate
				unsigned int	next = 0;

				if (i + 1 >= end) 
				{
					throwInvalidUTF16SurrogateException(c);
				}
				else 
				{
					next = ch[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(c, next);
					}

					next = ((c-0xd800) << 10) + next - 0xdc00 + 0x00010000;
				}

				writeNumberedEntityReference(next);
			}
			else
			{
				writeNumberedEntityReference(c);
			}

			if(i != 0 && i < end - 1)
			{
				accum(XALAN_STATIC_UCODE_STRING("<![CDATA["));
			}
		}
		else if(isCData == true &&
				i < end - 2 &&
				']' == c &&
                ']' == ch[i + 1] &&
				'>' == ch[ i + 2])
		{
			accum(XALAN_STATIC_UCODE_STRING("]]]]><![CDATA[>"));

			i += 2;
		}
		else
		{
			if(c <= m_maxCharacter)
			{
				accum(c);
			}
			// This needs to go into a function...
			else if (0xd800 <= c && c < 0xdc00)
			{
				// UTF-16 surrogate
				unsigned int	next = 0;

				if (i + 1 >= end) 
				{
					throwInvalidUTF16SurrogateException(c);
				}
				else
				{
					next = ch[++i];

					if (!(0xdc00 <= next && next < 0xe000))
					{
						throwInvalidUTF16SurrogateException(c, next);
					}

					next = ((c - 0xd800) << 10) + next - 0xdc00 + 0x00010000;
				}

				writeNumberedEntityReference(next);
			}
			else
			{
				writeNumberedEntityReference(c);
			}
		}
    }
}



void
FormatterToXML::writeNumberedEntityReference(unsigned long	theNumber)
{
	accum('&');
	accum('#');
	accum(UnsignedLongToDOMString(theNumber));
	accum(';');
}



void
FormatterToXML::entityReference(const XMLCh* const	name)
{
	writeParentTagEnd();
	  
	if (shouldIndent() == true)  
	{
		indent(m_currentIndent);
	}

	m_writer.write('&');
	m_writer.write(name);
	m_writer.write(';');
}



void
FormatterToXML::ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
{
	if (length > 0)
	{
		characters(chars, length);
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
	if(m_inEntityRef == false)
	{
		writeParentTagEnd();

		if (shouldIndent() == true)  
		{
			indent(m_currentIndent);
		}

		accum('<');
		accum('!');
		accum('-');
		accum('-');

		accum(data);

		accum('-');
		accum('-');
		accum('>');

		m_startNewLine = true;
	}
}



void
FormatterToXML::cdata(
			const XMLCh* const	ch,
			const unsigned int	length)
{
	if(m_nextIsRaw == true)
	{
		m_nextIsRaw = false;

		charactersRaw(ch, length);
	}
	else
	{
		if(m_escapeCData) // Should normally always be false.
		{
			characters(ch, length);
		}
		else
		{
			writeParentTagEnd();

			m_ispreserve = true;

			if (shouldIndent() == true)
			{
				indent(m_currentIndent);
			}

			if(m_stripCData == false)
			{
				if(length >= 1 &&
				   ch[0] <= m_maxCharacter)
				{
					accum(XALAN_STATIC_UCODE_STRING("<![CDATA["));
				}
			}

			writeNormalizedChars(ch, 0, length, !m_stripCData);

			if(m_stripCData == false)
			{
				if(length >= 1 &&
				   ch[length - 1] <= m_maxCharacter)
				{
					accum(']');
					accum(']');
					accum('>');
				}
			}
		}
	}
}



void
FormatterToXML::writeParentTagEnd()
{
	if(!m_elemStack.empty())
	{
		// See if the parent element has already been flagged as having children.
		if(false == m_elemStack.top())
		{
			accum('>');
			m_isprevtext = false;

			m_elemStack.pop();
			m_elemStack.push(true);

			m_preserves.push(m_ispreserve);
		}
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
			const XalanDOMChar* 	name,
			const XalanDOMChar* 	value)
{
	accum(' ');
	accum(name);
	accum('=');
	accum('"');
	writeAttrString(value, m_encoding);
	accum('"');
}



void
FormatterToXML::indent(int 	n)
{
	if(m_startNewLine == true)
	{
		outputLineSep();
	}

	if(m_doIndent == true)
	{
		printSpace(n);
	}
}



void
FormatterToXML::accumNormalizedPIData(
			const XalanDOMChar*		theData,
			unsigned int			theLength)
{
	// If there are any "?>" pairs in the string,
	// we have to normalize them to "? >", so they
	// won't be confused with the end tag.

	for (unsigned int i = 0; i < theLength; ++i)
	{
		const XalanDOMChar	theChar = theData[i];

		if (theChar == '?' && i + 1 < theLength && theData[i + 1] == '>')
		{
			accum('?');
			accum(' ');
		}
		else
		{
			accum(theChar);
		}
	}
}



#if 0



FormatterToXML::DOMStringMapType  FormatterToXML::s_enchash;
FormatterToXML::DOMStringMapType FormatterToXML::s_revhash;
FormatterToXML::DOMString2IntMapType FormatterToXML::s_revsize;

const XalanDOMString FormatterToXML::convertMime2JavaEncoding(
		const XalanDOMString& mimeCharsetName)
{
	if (m_javaEncodingIsISO) return mimeCharsetName;
	DOMStringMapType::const_iterator it =
		s_enchash.find(toUpperCase(mimeCharsetName));
	return (*it).second;
}		

const XalanDOMString FormatterToXML::convertJava2MimeEncoding(
		const XalanDOMString& encoding)
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

		unsigned int dashindex = (encoding != null ? encoding.indexOf('-') : -1);
		if(3 == dashindex)
		{
			String ISOprefix =	new String(encoding.toCharArray(), 0, 3);
			if (ISOprefix.equals("ISO") == true)
				javaEncodingIsISO = true;
		}
	}
	catch(SecurityException se)
	{
	}
*/

#if !defined(XALAN_NO_NAMESPACES)
	using std::make_pair;
#endif

	// Make a table to maximum character sizes before we 
	// need to resort to escaping in the XML.
	// TODO: To tell the truth, I'm guessing a bit here. 
	// s_revsize.insert(make_pair("CP1252", 		 0xFF)); // Windows Latin-1 
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")),	 0xFF)); // Windows 1250 Peter Smolik
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF-8")),			 0xFFFF)); // Universal Transformation Format 8
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),		 0x7F));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")), 	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP")),	 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("SHIFT_JIS")),		 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-JP")), 		 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312")), 		 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("BIG5")),			 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-KR")), 		 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR")),	 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8-R")), 		 0xFFFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE")), 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS")),	 0xFF));
	s_revsize.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2")),	 0xFF));

	//	  <preferred MIME name>, <Java encoding name>
	// s_enchash.insert(make_pair("ISO 8859-1", "CP1252")); // Close enough, I guess
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("WINDOWS-1250")), XalanDOMString(XALAN_STATIC_UCODE_STRING("CP1250")))); // Peter Smolik
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF-8")), XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF8"))));
	if(useISOPrefix)
	{
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1"))));	 // ?
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_2"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_3"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_4"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_5"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_6"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_7"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_8"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_9"))));
	}
	else
	{
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("US-ASCII")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_1"))));	  // ?
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_1"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_2"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_3"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_4"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_5"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_6"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_7"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_8"))));
		s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9")),
		XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_9"))));
	}
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("JIS"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("SHIFT_JIS")),		 XalanDOMString(XALAN_STATIC_UCODE_STRING("SJIS"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-JP")), 		 XalanDOMString(XALAN_STATIC_UCODE_STRING("EUCJIS"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312")), 		 XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("BIG5")),			 XalanDOMString(XALAN_STATIC_UCODE_STRING("Big5"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-KR")), 		 XalanDOMString(XALAN_STATIC_UCODE_STRING("KSC5601"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO2022KR"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8-R")), 		 XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8_R"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP277"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP277"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP278"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP278"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP280"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP284"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP285"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP297"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP420"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP424"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP500"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE")), XalanDOMString(XALAN_STATIC_UCODE_STRING("CP870"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP870"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP871"))));
	s_enchash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2")),	 XalanDOMString(XALAN_STATIC_UCODE_STRING("CP918"))));

	// j:CNS11643 -> EUC-TW?
	// ISO-2022-CN? ISO-2022-CN-EXT?

	//	  <Java encoding name>, <preferred MIME name>
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP1252")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1")))); // Close enough, I guess
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP1250")), XalanDOMString(XALAN_STATIC_UCODE_STRING("windows-1250")))); // Peter Smolik
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF8")), XalanDOMString(XALAN_STATIC_UCODE_STRING("UTF-8"))));
	//s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1")), XalanDOMString(XALAN_STATIC_UCODE_STRING("US-ASCII"))));	 // ?
	if(useISOPrefix)
	{
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_1")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_2")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_3")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_4")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_5")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_6")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_7")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_8")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO8859_9")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9"))));
	}
	else
	{
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_1")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-1"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_2")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-2"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_3")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-3"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_4")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-4"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_5")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-5"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_6")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-6"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_7")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-7"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_8")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-8"))));
		s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("8859_9")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-8859-9"))));
	}
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("JIS")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-JP"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("SJIS")), XalanDOMString(XALAN_STATIC_UCODE_STRING("Shift_JIS"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("EUCJIS")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-JP"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312")), XalanDOMString(XALAN_STATIC_UCODE_STRING("GB2312"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("BIG5")), XalanDOMString(XALAN_STATIC_UCODE_STRING("Big5"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("KSC5601")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EUC-KR"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO2022KR")), XalanDOMString(XALAN_STATIC_UCODE_STRING("ISO-2022-KR"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8_R")), XalanDOMString(XALAN_STATIC_UCODE_STRING("KOI8-R"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-US"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CA"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP037")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NL"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP277")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-DK"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP277")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-NO"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP278")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FI"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP278")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-SE"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP280")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IT"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP284")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ES"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP285")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-GB"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP297")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-FR"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP420")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR1"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP424")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-HE"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP500")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-CH"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP870")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-ROECE"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP870")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-YU"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP871")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-IS"))));
	s_revhash.insert(make_pair(XalanDOMString(XALAN_STATIC_UCODE_STRING("CP918")), XalanDOMString(XALAN_STATIC_UCODE_STRING("EBCDIC-CP-AR2"))));
}
#endif