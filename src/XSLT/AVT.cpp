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
 */
#include "AVT.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "AVTPartSimple.hpp"
#include "AVTPartXPath.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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



const XalanDOMString	AVT::s_emptyString;



/**
 * Construct an AVT by parsing the string, and either 
 * constructing a vector of AVTParts, or simply hold 
 * on to the string if the AVT is simple.
 */
AVT::AVT(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver) :
		m_parts(0),
		m_partsSize(0),
		m_simpleString(0),
		m_simpleStringLength(0),
		m_name(constructionContext.getPooledString(name))
{
	StringTokenizer		tokenizer(stringedValue, theTokenDelimiterCharacters, true);

	const StringTokenizer::size_type	nTokens = tokenizer.countTokens();

	if(nTokens < 2)
	{
		// Do the simple thing
		m_simpleStringLength = length(stringedValue);

		m_simpleString = constructionContext.allocateXalanDOMCharVector(stringedValue, m_simpleStringLength, false);
	}
	else
	{
		// This over-allocates, but we probably won't waste that much space.  If necessary,
		// we could tokenize twice, just counting the numbers of AVTPart instances we
		// will need the first time.
		m_parts = constructionContext.allocateAVTPartPointerVector(nTokens + 1);

		XalanDOMString	buffer;
		XalanDOMString	exprBuffer;
		XalanDOMString	t; // base token
		XalanDOMString	lookahead; // next token

		while(tokenizer.hasMoreTokens())
		{
			if(length(lookahead))
			{
				t = lookahead;

				clear(lookahead);
			}
			else
			{
				nextToken(constructionContext, locator, tokenizer, t);
			}

			if(length(t) == 1)
			{
				const XalanDOMChar	theChar = charAt(t, 0);

				switch(theChar)
				{
					case(XalanUnicode::charLeftCurlyBracket):
					{
						// Attribute Value Template start
						nextToken(constructionContext, locator, tokenizer, lookahead);

						if(equals(lookahead, theLeftCurlyBracketString))
						{
							// Double braces mean escape to show brace
							append(buffer, lookahead);

							clear(lookahead);

							break; // from switch
						}
						else
						{
							if(length(buffer) > 0)
							{
								assert(m_partsSize + 1 < nTokens);

								m_parts[m_partsSize++] =
									constructionContext.createAVTPart(
										c_wstr(buffer),
										length(buffer));

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

											const XalanDOMChar	quote[2] =
											{
												lookahead[0],
												0
											};

											// Consume stuff 'till next quote
											nextToken(constructionContext, locator, tokenizer, lookahead);

											while(!equals(lookahead, quote))
											{
												append(exprBuffer, lookahead);

												nextToken(constructionContext, locator, tokenizer, lookahead);
											}

											append(exprBuffer,lookahead);

											break;
										}

										case XalanUnicode::charLeftCurlyBracket:
											// What's another brace doing here?
											constructionContext.error("\"{\" cannot appear within an expression.");

											break;

										default:
											// part of the template stuff, just add it.
											append(exprBuffer, lookahead);
											break;

									} // end inner switch
								} // end if lookahead length == 1
								else
								{
									// part of the template stuff, just add it.
									append(exprBuffer,lookahead);
								}

								nextToken(constructionContext, locator, tokenizer, lookahead);
							} // end while(!equals(lookahead, "}"))
							assert(equals(lookahead, theRightCurlyBracketString));

							// Proper close of attribute template. Evaluate the
							// expression.
							clear(buffer);

							assert(m_partsSize + 1 < nTokens);

							m_parts[m_partsSize++] =
								constructionContext.createAVTPart(
									locator,
									c_wstr(exprBuffer),
									length(exprBuffer),
									resolver);

							clear(lookahead); // breaks out of inner while loop
						}
						break;
					}
					case(XalanUnicode::charRightCurlyBracket):
					{
						nextToken(constructionContext, locator, tokenizer, lookahead);

						if(equals(lookahead, theRightCurlyBracketString))
						{
							// Double brace mean escape to show brace
							append(buffer, lookahead);

							clear(lookahead); // swallow
						}
						else
						{
							constructionContext.error("An unmatched \"}\" was found!");
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
		} // end while(tokenizer.hasMoreTokens())

		if(length(buffer) > 0)
		{
			assert(m_partsSize + 1 < nTokens);

			m_parts[m_partsSize++] = constructionContext.createAVTPart(c_wstr(buffer), length(buffer));

			clear(buffer);
		}
	} // end else nTokens > 1
}



AVT::~AVT()
{
}



void
AVT::doEvaluate(
			XalanDOMString&			buf,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	clear(buf);

	if(m_partsSize != 0)
	{
		for(size_type i = 0; i < m_partsSize; i++)
		{
			assert(m_parts[i] != 0);

			m_parts[i]->evaluate(buf, contextNode, prefixResolver, executionContext);
		}
	}
}



void
AVT::nextToken(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator,
			StringTokenizer&				tokenizer,
			XalanDOMString&					token)
{
	if (tokenizer.hasMoreTokens() == false)
	{
		constructionContext.error(
			"The attribute value template has a missing a '}'",
			0,
			locator);
	}
	else
	{
		tokenizer.nextToken(token);
	}
}



XALAN_CPP_NAMESPACE_END
