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


#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/StringTokenizer.hpp>


#include "AVTPartSimple.hpp"
#include "AVTPartXPath.hpp"
#include "StylesheetConstructionContext.hpp"


/**
 * Construct an AVT by parsing the string, and either 
 * constructing a vector of AVTParts, or simply hold 
 * on to the string if the AVT is simple.
 */
AVT::AVT(
			const DOMString&				name,
			const XMLCh*					type,
			const XMLCh*					stringedValue,
			const PrefixResolver&			resolver,
			StylesheetConstructionContext&	constructionContext) :
		AVTPart(),
		m_name(name),
		m_pcType(type)
{
	StringTokenizer tokenizer(stringedValue, "{}\"\'", true);

	const int	nTokens = tokenizer.countTokens();

	if(nTokens < 2)
	{
		m_simpleString = stringedValue; // then do the simple thing
	}
	else
	{
		m_parts.reserve(nTokens + 1);

		DOMString buffer;
		DOMString exprBuffer;
		DOMString t; // base token
		DOMString lookahead; // next token
		DOMString error; // if non-null, break from loop

		while(tokenizer.hasMoreTokens())
		{
			if(length(lookahead))
			{
				t = lookahead;
				lookahead = DOMString();
			}
			else t = tokenizer.nextToken();
					
			if(length(t) == 1)
			{
				switch(t.charAt(0))
				{
					case('\"'):
					case('\''):
					{
						// just keep on going, since we're not in an attribute template
						append(buffer,t);
						break;
					}
					case('{'):
					{
						// Attribute Value Template start
						lookahead = tokenizer.nextToken();
						if(equals(lookahead, "{"))
						{
							// Double curlys mean escape to show curly
							append(buffer,lookahead);
							lookahead = DOMString();
							break; // from switch
						}
						else
						{
							if(length(buffer) > 0)
							{
								m_parts.push_back(new AVTPartSimple(buffer));
								buffer = DOMString();
							}
									
							exprBuffer = DOMString();
							while(length(lookahead) > 0 && !equals(lookahead, "}"))
							{
								if(length(lookahead) == 1)
								{
									switch(lookahead.charAt(0))
									{
										case '\'':
										case '\"':
										{
											// String start
											append(exprBuffer,lookahead);
											const DOMString quote = lookahead;
											// Consume stuff 'till next quote
											lookahead = tokenizer.nextToken();
											while(!equals(lookahead, quote))
											{
												append(exprBuffer,lookahead);
												lookahead = tokenizer.nextToken();
											}
											append(exprBuffer,lookahead);
											break;
										}
										case '{':
										{
											// What's another curly doing here?
											error = "Error: Can not have \"{\" within expression.";
											break;
										}
										default:
										{
											// part of the template stuff, just add it.
											append(exprBuffer,lookahead);
										}
									} // end inner switch
								} // end if lookahead length == 1
								else
								{
									// part of the template stuff, just add it.
									append(exprBuffer,lookahead);
								}
								lookahead = tokenizer.nextToken();
							} // end while(!lookahead.equals("}"))
							assert(equals(lookahead, "}"));
							
							// Proper close of attribute template. Evaluate the
							// expression.
							buffer = DOMString();
							XPath *xpath = constructionContext.createXPath(exprBuffer, resolver);
							m_parts.push_back(new AVTPartXPath(xpath));
							lookahead = DOMString(); // breaks out of inner while loop
							if(length(error) > 0)
							{
								break; // from inner while loop
							}
						}
						break;
					}
					case('}'):
					{
						lookahead = tokenizer.nextToken();
						if(equals(lookahead, "}"))
						{
							// Double curlys mean escape to show curly
							append(buffer,lookahead);
							lookahead = DOMString(); // swallow
						}
						else
						{
							// Illegal, I think...
							constructionContext.warn("Found \"}\" but no attribute template open!");
							append(buffer,"}");
							// leave the lookahead to be processed by the next round.
						}
						break;
					}
					default:
					{
						// Anything else just add to string.
						append(buffer,t);
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
				constructionContext.warn("Attr Template, "+error);
				break;
			}
		} // end while(tokenizer.hasMoreTokens())

		if(length(buffer) > 0)
		{
			m_parts.push_back(new AVTPartSimple(buffer));

			buffer = DOMString();
		}

	} // end else nTokens > 1

	if(m_parts.empty() && length(m_simpleString) == 0)
	{
		// Error?
		m_simpleString = DOMString();
	}
}



AVT::~AVT()
{
	for (unsigned i = 0; i < m_parts.size(); ++i)
	{
		delete (m_parts[i]);
	}

	m_pcType = 0;
}



void
AVT::evaluate(
			DOMString&				buf,
			const DOM_Node&			contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	if(length(m_simpleString) > 0)
	{
		buf = m_simpleString;
	}
	else if(!m_parts.empty())
	{
		buf = DOMString();

		const int	n = m_parts.size();

		for(int i = 0; i < n; i++)
		{
			assert(m_parts[i] != 0);

			m_parts[i]->evaluate(buf, contextNode, prefixResolver, executionContext);
		}
	}
	else
	{
		buf = DOMString();
	}
}
