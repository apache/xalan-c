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
#include "AVT.hpp"



#include <algorithm>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVTPartSimple.hpp"
#include "AVTPartXPath.hpp"
#include "StylesheetConstructionContext.hpp"



static const XalanDOMChar	theTokenDelimiterCharacters[] =
{
		XalanUnicode::charLeftCurlyBracket,
		XalanUnicode::charRightCurlyBracket,
		XalanUnicode::charApostrophe,
		XalanUnicode::charQuoteMark,
		0
};



static const XalanDOMChar	theLeftCurlyBracketString[] =
{
		XalanUnicode::charLeftCurlyBracket,
		0
};



static const XalanDOMChar	theRightCurlyBracketString[] =
{
		XalanUnicode::charRightCurlyBracket,
		0
};



/**
 * Construct an AVT by parsing the string, and either 
 * constructing a vector of AVTParts, or simply hold 
 * on to the string if the AVT is simple.
 */
AVT::AVT(
			const Locator*					locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				type,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver,
			StylesheetConstructionContext&	constructionContext) :
		m_parts(),
		m_simpleString(),
		// $$$ ToDo: Explicit XalanDOMString constructor
		m_name(XalanDOMString(name)),
		m_prefix(getPrefix(name)),
		m_pcType(type)
{
	StringTokenizer		tokenizer(stringedValue, theTokenDelimiterCharacters, true);

	const StringTokenizer::size_type	nTokens = tokenizer.countTokens();

	if(nTokens < 2)
	{
		m_simpleString = stringedValue; // then do the simple thing
	}
	else
	{
		m_parts.reserve(nTokens + 1);

		XalanDOMString	buffer;
		XalanDOMString	exprBuffer;
		XalanDOMString	t; // base token
		XalanDOMString	lookahead; // next token
		XalanDOMString	error; // if non-null, break from loop

		while(tokenizer.hasMoreTokens())
		{
			if(length(lookahead))
			{
				t = lookahead;

				clear(lookahead);
			}
			else
			{
				tokenizer.nextToken(t);
			}

			if(length(t) == 1)
			{
				const XalanDOMChar	theChar = charAt(t, 0);

				switch(theChar)
				{
					case(XalanUnicode::charLeftCurlyBracket):
					{
						// Attribute Value Template start
						tokenizer.nextToken(lookahead);

						if(equals(lookahead, theLeftCurlyBracketString))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);

							clear(lookahead);

							break; // from switch
						}
						else
						{
							if(length(buffer) > 0)
							{
								m_parts.push_back(new AVTPartSimple(buffer));

								clear(buffer);
							}
									
							clear(exprBuffer);

							while(length(lookahead) > 0 && !equals(lookahead, theRightCurlyBracketString))
							{
								if(length(lookahead) == 1)
								{
									switch(charAt(lookahead, 0))
									{
										case XalanUnicode::charApostrophe:
										case XalanUnicode::charQuoteMark:
										{
											// String start
											append(exprBuffer, lookahead);

											const XalanDOMString	quote = lookahead;

											// Consume stuff 'till next quote
											tokenizer.nextToken(lookahead);

											while(!equals(lookahead, quote))
											{
												append(exprBuffer, lookahead);

												 tokenizer.nextToken(lookahead);
											}

											append(exprBuffer,lookahead);

											break;
										}
										case XalanUnicode::charLeftCurlyBracket:
										{
											// What's another curly doing here?
											error = TranscodeFromLocalCodePage("Error: Can not have \"{\" within expression.");

											break;
										}
										default:
										{
											// part of the template stuff, just add it.
											append(exprBuffer, lookahead);
										}
									} // end inner switch
								} // end if lookahead length == 1
								else
								{
									// part of the template stuff, just add it.
									append(exprBuffer,lookahead);
								}

								tokenizer.nextToken(lookahead);
							} // end while(!equals(lookahead, "}"))
							assert(equals(lookahead, theRightCurlyBracketString));

							// Proper close of attribute template. Evaluate the
							// expression.
							clear(buffer);

							const XPath* const	xpath =
								constructionContext.createXPath(
											locator,
											exprBuffer,
											resolver);
							assert(xpath != 0);

							m_parts.push_back(new AVTPartXPath(xpath));

							clear(lookahead); // breaks out of inner while loop

							if(length(error) > 0)
							{
								break; // from inner while loop
							}
						}
						break;
					}
					case(XalanUnicode::charRightCurlyBracket):
					{
						tokenizer.nextToken(lookahead);

						if(equals(lookahead, theRightCurlyBracketString))
						{
							// Double curlys mean escape to show curly
							append(buffer, lookahead);

							clear(lookahead); // swallow
						}
						else
						{
							// Illegal, I think...
							constructionContext.warn("Found \"}\" but no attribute template open!");

							append(buffer, theRightCurlyBracketString);
							// leave the lookahead to be processed by the next round.
						}
						break;
					}
					default:
					{
						const XalanDOMString	s(&theChar, 1);

						// Anything else just add to string.
						append(buffer, s);
					}
				} // end switch t
			} // end if length == 1
			else
			{
				// Anything else just add to string.
				append(buffer,t);
			}

			if(length(error) > 0)
			{
				constructionContext.warn("Attr Template, " + error);

				break;
			}
		} // end while(tokenizer.hasMoreTokens())

		if(length(buffer) > 0)
		{
			m_parts.push_back(new AVTPartSimple(buffer));

			clear(buffer);
		}

	} // end else nTokens > 1

	if(m_parts.empty() && length(m_simpleString) == 0)
	{
		// Error?
		clear(m_simpleString);
	}
	else if (m_parts.size() < m_parts.capacity())
	{
		AVTPartPtrVectorType(m_parts).swap(m_parts);
	}

}



AVT::~AVT()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the vector.
	for_each(m_parts.begin(),
			 m_parts.end(),
			 DeleteFunctor<AVTPart>());
}



void
AVT::evaluate(
			XalanDOMString&			buf,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	if(length(m_simpleString) > 0)
	{
		buf = m_simpleString;
	}
	else
	{
		clear(buf);

		if(m_parts.empty() == false)
		{
			const AVTPartPtrVectorType::size_type	n = m_parts.size();

			for(AVTPartPtrVectorType::size_type i = 0; i < n; i++)
			{
				assert(m_parts[i] != 0);

				m_parts[i]->evaluate(buf, contextNode, prefixResolver, executionContext);
			}
		}
	}
}



XalanDOMString
AVT::getPrefix(const XalanDOMChar*	theName)
{
	if (startsWith(theName, DOMServices::s_XMLNamespaceWithSeparator) == true)
	{
		return substring(theName, DOMServices::s_XMLNamespaceWithSeparatorLength);
	}
	else
	{
		return XalanDOMString();
	}
}
