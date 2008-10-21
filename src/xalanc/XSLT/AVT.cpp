/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



static const XalanDOMChar   theTokenDelimiterCharacters[] =
{
        XalanUnicode::charLeftCurlyBracket,
        XalanUnicode::charRightCurlyBracket,
        XalanUnicode::charApostrophe,
        XalanUnicode::charQuoteMark,
        0
};



static const XalanDOMChar   theLeftCurlyBracketString[] =
{
        XalanUnicode::charLeftCurlyBracket,
        0
};



static const XalanDOMChar   theRightCurlyBracketString[] =
{
        XalanUnicode::charRightCurlyBracket,
        0
};



const XalanDOMString    AVT::s_emptyString(XalanMemMgrs::getDummyMemMgr());



typedef XPathConstructionContext::GetCachedString   GetCachedString;



/**
 * Construct an AVT by parsing the string, and either 
 * constructing a vector of AVTParts, or simply hold 
 * on to the string if the AVT is simple.
 */
AVT::AVT(
            StylesheetConstructionContext&  constructionContext,
            const Locator*                  locator,
            const XalanDOMChar*             name,
            const XalanDOMChar*             stringedValue,
            const PrefixResolver&           resolver) :
        m_parts(0),
        m_partsSize(0),
        m_simpleString(0),
        m_simpleStringLength(0),
        m_name(constructionContext.getPooledString(name))
{
    StringTokenizer     tokenizer(stringedValue, theTokenDelimiterCharacters, true);

    const StringTokenizer::size_type    nTokens = tokenizer.countTokens();

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

        XalanDOMString  buffer(constructionContext.getMemoryManager());
        XalanDOMString  exprBuffer(constructionContext.getMemoryManager());
        XalanDOMString  t(constructionContext.getMemoryManager()); // base token
        XalanDOMString  lookahead(constructionContext.getMemoryManager()); // next token

        while (tokenizer.hasMoreTokens())
        {
            if (lookahead.empty() == false)
            {
                t = lookahead;

                lookahead.clear();
            }
            else
            {
                nextToken(constructionContext, locator, tokenizer, t);
            }

            if (t.length() == 1)
            {
                const XalanDOMChar  theChar = t[0];

                switch(theChar)
                {
                    case(XalanUnicode::charLeftCurlyBracket):
                    {
                        // Attribute Value Template start
                        nextToken(constructionContext, locator, tokenizer, lookahead);

                        if(equals(lookahead, theLeftCurlyBracketString))
                        {
                            // Double braces mean escape to show brace
                            buffer.append(lookahead);

                            lookahead.clear();

                            break; // from switch
                        }
                        else
                        {
                            if(buffer.empty() == false)
                            {
                                assert(m_partsSize + 1 < nTokens);

                                m_parts[m_partsSize++] =
                                    constructionContext.createAVTPart(
                                        buffer.c_str(),
                                        buffer.length());

                                buffer.clear();
                            }
                                    
                            exprBuffer.clear();

                            while (lookahead.empty() == false && !equals(lookahead, theRightCurlyBracketString))
                            {
                                if (lookahead.length() == 1)
                                {
                                    switch(lookahead[0])
                                    {
                                        case XalanUnicode::charApostrophe:
                                        case XalanUnicode::charQuoteMark:
                                        {
                                            // String start
                                            exprBuffer.append(lookahead);

                                            const XalanDOMChar  quote[2] =
                                            {
                                                lookahead[0],
                                                0
                                            };

                                            // Consume stuff 'till next quote
                                            nextToken(constructionContext, locator, tokenizer, lookahead);

                                            while(!equals(lookahead, quote))
                                            {
                                                exprBuffer.append(lookahead);

                                                nextToken(constructionContext, locator, tokenizer, lookahead);
                                            }

                                            exprBuffer.append(lookahead);

                                            break;
                                        }

                                        case XalanUnicode::charLeftCurlyBracket:
                                            {
                                                GetCachedString     theGuard(constructionContext);

                                                // What's another brace doing here?
                                                constructionContext.problem(
                                                    StylesheetConstructionContext::eXSLTProcessor,
                                                    StylesheetConstructionContext::eError,
                                                    XalanMessageLoader::getMessage(
                                                        theGuard.get(),
                                                        XalanMessages::LeftBraceCannotAppearWithinExpression),
                                                    locator,
                                                    0);
                                                break;
                                            }

                                        default:
                                            // part of the template stuff, just add it.
                                            exprBuffer.append(lookahead);
                                            break;

                                    } // end inner switch
                                } // end if lookahead length == 1
                                else
                                {
                                    // part of the template stuff, just add it.
                                    exprBuffer.append(lookahead);
                                }

                                nextToken(constructionContext, locator, tokenizer, lookahead);
                            } // end while(!equals(lookahead, "}"))
                            assert(equals(lookahead, theRightCurlyBracketString));

                            // Proper close of attribute template. Evaluate the
                            // expression.
                            buffer.clear();

                            assert(m_partsSize + 1 < nTokens);

                            m_parts[m_partsSize++] =
                                constructionContext.createAVTPart(
                                    locator,
                                    exprBuffer.c_str(),
                                    exprBuffer.length(),
                                    resolver);

                            lookahead.clear(); // breaks out of inner while loop
                        }
                        break;
                    }
                    case(XalanUnicode::charRightCurlyBracket):
                    {
                        nextToken(constructionContext, locator, tokenizer, lookahead);

                        if (equals(lookahead, theRightCurlyBracketString))
                        {
                            // Double brace mean escape to show brace
                            buffer.append(lookahead);

                            lookahead.clear(); // swallow
                        }
                        else
                        {
                            GetCachedString     theGuard(constructionContext);

                            constructionContext.problem(
                                StylesheetConstructionContext::eXSLTProcessor,
                                StylesheetConstructionContext::eError,
                                XalanMessageLoader::getMessage(
                                    theGuard.get(),
                                    XalanMessages::UnmatchedWasFound),
                                locator,
                                0);

                        }
                        break;
                    }
                    default:
                    {
                        // Anything else just add to string.
                        buffer.append(1, theChar);
                    }
                } // end switch t
            } // end if length == 1
            else
            {
                // Anything else just add to string.
                buffer.append(t);
            }
        } // end while(tokenizer.hasMoreTokens())

        if (buffer.empty() == false)
        {
            assert(m_partsSize + 1 < nTokens);

            m_parts[m_partsSize++] =
                constructionContext.createAVTPart(
                    buffer.c_str(),
                    buffer.length());

            buffer.clear();
        }
    } // end else nTokens > 1
}



AVT::~AVT()
{
}



void
AVT::doEvaluate(
            XalanDOMString&         buf,
            XalanNode*              contextNode,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const
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
            XalanDOMString&         buf,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const
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
            StylesheetConstructionContext&  constructionContext,
            const Locator*                  locator,
            StringTokenizer&                tokenizer,
            XalanDOMString&                 token)
{
    if (tokenizer.hasMoreTokens() == false)
    {
        GetCachedString     theGuard(constructionContext);

        constructionContext.problem(
            StylesheetConstructionContext::eXSLTProcessor,
            StylesheetConstructionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::AttributeValueTemplateHasMissing),
            locator,
            0);
    }
    else
    {
        tokenizer.nextToken(token);
    }
}



XALAN_CPP_NAMESPACE_END
