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
#include "AVT.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



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
											constructionContext.error(XalanMessageLoader::getMessage(XalanMessages::LeftBraceCannotAppearWithinExpression));
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
							constructionContext.error(XalanMessageLoader::getMessage(XalanMessages::UnmatchedWasFound));

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
AVT::doEvaluate(
			XalanDOMString&			buf,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	if(m_partsSize != 0)
	{
		for(size_type i = 0; i < m_partsSize; i++)
		{
			assert(m_parts[i] != 0);

			m_parts[i]->evaluate(buf, prefixResolver, executionContext);
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
		constructionContext.error(XalanMessageLoader::getMessage(XalanMessages::AttributeValueTemplateHasMissing),0,locator);
	}
	else
	{
		tokenizer.nextToken(token);
	}
}



XALAN_CPP_NAMESPACE_END
