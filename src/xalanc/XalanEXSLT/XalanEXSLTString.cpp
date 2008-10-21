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
#include "XalanEXSLTString.hpp"
#include "XalanEXSLTStringImpl.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar  XalanEXSLTFunctionAlign::s_centerString[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    0
};



const XalanDOMChar  XalanEXSLTFunctionAlign::s_rightString[] =
{
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    0
};



XObjectPtr
XalanEXSLTFunctionAlign::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    const XObjectArgVectorType::size_type   theSize = args.size();

    if (theSize != 2 && theSize != 3)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(
        args[0].null() == false &&
        args[1].null() == false &&
        (theSize == 2 || args[2].null() == false));

    const XalanDOMString&   theTargetString = args[0]->str(executionContext);
    const XalanDOMString&   thePaddingString = args[1]->str(executionContext);

    const XalanDOMString::size_type     theTargetStringLength = theTargetString.length();
    const XalanDOMString::size_type     thePaddingStringLength = thePaddingString.length();

    if (theTargetStringLength == thePaddingStringLength)
    {
        return XObjectPtr(args[0]);
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     theResult = theGuard.get();

        if (theTargetStringLength > thePaddingStringLength)
        {
            theResult.assign(theTargetString, 0, thePaddingStringLength);
        }
        else
        {
            theResult.reserve(thePaddingStringLength);

            enum eAlignment { eCenter, eLeft, eRight };

            eAlignment  theAlignment = eLeft;

            if (theSize == 3)
            {
                const XalanDOMString&   theAlignmentString = args[2]->str(executionContext);

                if (equals(
                            s_centerString,
                            theAlignmentString.c_str(),
                            sizeof(s_centerString) / sizeof(s_centerString[0]) - 1) == true)
                {
                    theAlignment = eCenter;
                }
                else if (equals(
                            s_rightString,
                            theAlignmentString.c_str(),
                            sizeof(s_rightString) / sizeof(s_rightString[0]) - 1) == true)
                {
                    theAlignment = eRight;
                }
            }

            if (theAlignment == eLeft)
            {
                theResult = theTargetString;

                theResult.append(thePaddingString, theTargetStringLength, thePaddingStringLength - theTargetStringLength);
            }
            else if (theAlignment == eRight)
            {
                theResult.assign(thePaddingString, 0, thePaddingStringLength - theTargetStringLength);
                theResult.append(theTargetString);
            }
            else if (theAlignment == eCenter)
            {
                const XalanDOMString::size_type     theStartIndex =
                    (thePaddingStringLength - theTargetStringLength) / 2;

                theResult.assign(thePaddingString, 0, theStartIndex);

                theResult.append(theTargetString);

                theResult.append(
                    thePaddingString,
                    theTargetStringLength + theStartIndex,
                    thePaddingStringLength - theTargetStringLength - theStartIndex);
            }
        }

        return executionContext.getXObjectFactory().createString(theResult);
    }
}



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XObjectPtr
XalanEXSLTFunctionConcat::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    if (args.size() != 1)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(args[0].null() == false);

    const NodeRefListBase&              theNodeSet = args[0]->nodeset();
    const NodeRefListBase::size_type    theLength = theNodeSet.getLength();

    if (theLength == 0)
    {
        return executionContext.getXObjectFactory().createStringReference(s_emptyString);
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     theResult = theGuard.get();

        for(NodeRefListBase::size_type i = 0; i < theLength; ++i)
        {
            assert(theNodeSet.item(i) != 0);

            DOMServices::getNodeData(*theNodeSet.item(i), executionContext, theResult);
        }

        return executionContext.getXObjectFactory().createString(theResult);
    }
}



const XalanDOMChar  XalanEXSLTFunctionPadding::s_spaceString[] =
{
    XalanUnicode::charSpace,
    0
};



XObjectPtr
XalanEXSLTFunctionPadding::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    assert(m_space == s_spaceString);

    const XObjectArgVectorType::size_type   theSize = args.size();

    if (theSize != 1 && theSize != 2)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(
        args[0].null() == false &&
        (theSize == 1 || args[1].null() == false));

    const double                        theLength = DoubleSupport::round(args[0]->num(executionContext));
    const XalanDOMString&               thePaddingString = theSize == 2 ? args[1]->str(executionContext) : m_space;
    const XalanDOMString::size_type     thePaddingStringLength = thePaddingString.length();

    if (theLength == 0.0 || thePaddingStringLength == 0)
    {
        return executionContext.getXObjectFactory().createStringReference(s_emptyString);
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&     theResult = theGuard.get();

        if (thePaddingStringLength == 1)
        {
            theResult.assign(XalanDOMString::size_type(theLength), thePaddingString[0]);
        }
        else
        {
            XalanDOMString::size_type   theRemainingLength = XalanDOMString::size_type(theLength);

            for(;;)
            {
                if (theRemainingLength > thePaddingStringLength)
                {
                    theResult.append(thePaddingString);

                    theRemainingLength -= thePaddingStringLength;
                }
                else
                {
                    theResult.append(
                        thePaddingString,
                        0,
                        XalanDOMString::size_type(theRemainingLength));

                    break;
                }
            }
        }

        return executionContext.getXObjectFactory().createString(theResult);
    }
}

// Reserved URI characters
const XalanDOMChar  
XalanEXSLTFunctionEncodeURI::s_reservedChars[] = 
{
    XalanUnicode::charDollarSign,
    XalanUnicode::charAmpersand,
    XalanUnicode::charPlusSign,
    XalanUnicode::charComma,
    XalanUnicode::charSolidus,
    XalanUnicode::charColon,
    XalanUnicode::charSemicolon,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuestionMark,
    XalanUnicode::charCommercialAt,
    XalanUnicode::charLeftSquareBracket,
    XalanUnicode::charRightSquareBracket
};
const XalanDOMString::size_type 
XalanEXSLTFunctionEncodeURI::s_reservedCharsSize = sizeof(s_reservedChars) / sizeof(s_reservedChars[0]);

// Excluded URI characters
const XalanDOMChar
XalanEXSLTFunctionEncodeURI::s_excludedChars[] =
{
    XalanUnicode::charSpace,
    XalanUnicode::charQuoteMark,
    XalanUnicode::charNumberSign,
    XalanUnicode::charPercentSign,
    XalanUnicode::charLessThanSign,
    XalanUnicode::charGreaterThanSign,
    XalanUnicode::charReverseSolidus,
    XalanUnicode::charCircumflexAccent,
    XalanUnicode::charGraveAccent,
    XalanUnicode::charLeftCurlyBracket,
    XalanUnicode::charVerticalLine,
    XalanUnicode::charRightCurlyBracket
};
const XalanDOMString::size_type
XalanEXSLTFunctionEncodeURI::s_excludedCharsSize = sizeof(s_excludedChars) / sizeof(s_excludedChars[0]);

XObjectPtr
XalanEXSLTFunctionEncodeURI::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    const XObjectArgVectorType::size_type   theSize = args.size();

    if (theSize != 2 && theSize != 3)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(
        args[0].null() == false && 
        args[1].null() == false &&
        (theSize == 2 || args[2].null() == false));

    const XalanDOMString& theString = args[0]->str(executionContext);
    const bool            escapeReserved = args[1]->boolean(executionContext);

    // We only support UTF-8, which is the default when there are only two arguments.
    const bool  fSupportedEncoding =
                    theSize == 2 ||
                    XalanTranscodingServices::encodingIsUTF8(args[2]->str(executionContext));

    if (theString.length() == 0 ||
        !fSupportedEncoding)
    {
        return executionContext.getXObjectFactory().createStringReference(s_emptyString);
    }   

    const GetCachedString   theGuard(executionContext);

    XalanDOMString&     theResult = theGuard.get();

    const GetCachedString   theGuard1(executionContext);
    XalanDOMString&     theBuffer = theGuard1.get();

    XALAN_USING_STD(find)

    for (XalanDOMString::size_type i = 0; i < theString.length(); ++i)
    {
        const XalanDOMChar  ch = theString[i];
        
        // ASCII characters (C0 control and basic Latin)
        if (ch <= 0x7F) {
            // Escape all control and excluded URI characters.
            // Reserved URI characters are optionally escaped.
            if (ch < 32 || ch > 126 || 
                find(s_excludedChars,s_excludedChars+s_excludedCharsSize,ch)
                    != s_excludedChars+s_excludedCharsSize || 
                (escapeReserved && 
                 find(s_reservedChars,s_reservedChars+s_reservedCharsSize,ch) 
                     != s_reservedChars+s_reservedCharsSize))
            {           
                theResult+= escapedOctet(ch, theBuffer);
            }
            else 
            {
                theResult+= ch;
            }
        } 
        // Character is in the BMP
        else if(ch <= 0x7FF)
        {
            const XalanDOMChar  highByte = XalanDOMChar((ch >> 6) | 0xC0);
            const XalanDOMChar  lowByte = XalanDOMChar((ch & 0x3F) | 0x80);


            theResult+= escapedOctet(highByte,theBuffer);
            theResult+= escapedOctet(lowByte,theBuffer);
        }
        // Character is in a higher supplementary plane
        else if((ch & 0xFC00) == 0xD800) // high surrogate
        {   
            const XalanDOMChar highSurrogate = XalanDOMChar((ch & 0x03FF) + 0x40);

            const XalanDOMChar nextChar = theString[++i];
            const XalanDOMChar lowSurrogate = XalanDOMChar(nextChar & 0x03FF);

            const XalanDOMChar byte1 = XalanDOMChar(0xF0 + ((highSurrogate & 0x0300) >> 8));
            const XalanDOMChar byte2 = XalanDOMChar(0x80 + ((highSurrogate & 0x00FC) >> 2));
            const XalanDOMChar byte3 = XalanDOMChar(0x80 + ((highSurrogate & 0x0003) << 4) + ((lowSurrogate & 0x03C0) >> 6));
            const XalanDOMChar byte4 = XalanDOMChar(0x80 + (lowSurrogate & 0x003F));

            theResult+= escapedOctet(byte1,theBuffer);
            theResult+= escapedOctet(byte2,theBuffer);
            theResult+= escapedOctet(byte3,theBuffer);
            theResult+= escapedOctet(byte4,theBuffer);
        }
        else
        {
            const XalanDOMChar  highByte = XalanDOMChar((ch >> 12) | 0xE0);
            const XalanDOMChar  middleByte = XalanDOMChar(((ch & 0x0FC0) >> 6) | 0x80);
            const XalanDOMChar  lowByte = XalanDOMChar((ch & 0x3F) | 0x80);

            theResult+= escapedOctet(highByte,theBuffer);
            theResult+= escapedOctet(middleByte,theBuffer);
            theResult+= escapedOctet(lowByte,theBuffer);
        }   
    }

    return executionContext.getXObjectFactory().createString(theResult);

}



const XalanDOMString&
XalanEXSLTFunctionEncodeURI::escapedOctet(
            XalanDOMChar        theChar,
            XalanDOMString&     theResult) const
{
    theResult = XalanUnicode::charPercentSign;

    if (theChar < XalanDOMChar(16))
    {
        theResult += XalanDOMChar(XalanUnicode::charDigit_0);
    }

    return NumberToHexDOMString(theChar, theResult);
}



const XalanDOMString::size_type XalanEXSLTFunctionDecodeURI::s_octetSize = 3;



XObjectPtr
XalanEXSLTFunctionDecodeURI::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      context,
            const XObjectArgVectorType&     args,
            const Locator*                  locator) const
{
    const XObjectArgVectorType::size_type   theSize = args.size();

    if (theSize != 1 && theSize != 2)
    {
        generalError(
            executionContext,
            context,
            locator);
    }

    assert(
        args[0].null() == false && 
        (theSize == 1 || args[1].null() == false));

    const XalanDOMString& theString = args[0]->str(executionContext);

    // We only support UTF-8, which is the default when there's only one argument.
    const bool  fSupportedEncoding =
        theSize == 1 ||
        XalanTranscodingServices::encodingIsUTF8(args[1]->str(executionContext));

    if (theString.length() == 0 || !fSupportedEncoding)
    {
        return executionContext.getXObjectFactory().createStringReference(s_emptyString);
    } 
        
    const XPathExecutionContext::GetCachedString    theGuard(executionContext);

    XalanDOMString&     theResult = theGuard.get();

    for (XalanDOMString::size_type i = 0; i < theString.length(); ++i)
    {
        const XalanDOMChar  ch = theString[i];
            
        if (ch != XalanUnicode::charPercentSign) 
        {
            theResult+= ch;
            continue;
        }

        // escaped character

        // count number of escaped octets
        XalanDOMString::size_type numOctets = 0;
        XalanDOMString::size_type j = i;
        //assert(theString[j] == XalanUnicode::charPercentSign);
        //assert(j + 2 < theString.length());
        while (theString[j] == XalanUnicode::charPercentSign &&
               j + 2 < theString.length()) 
        {
            ++numOctets;
            j+= s_octetSize;
        }

        // no complete sequences found
        if (numOctets < 1) {
            break;
        }

        XalanDOMChar byte1 = hexCharsToByte(executionContext, context, locator,
                                            theString[i+1],theString[i+2]);

        if ((byte1 & 0x80) == 0) 
        {
            i += 2;
            theResult+=byte1;
        }
        else if ((byte1 & 0xE0) == 0xC0 && numOctets >= 2) 
        {
            i += s_octetSize;
            XalanDOMChar byte2 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+=2;

            if ((byte2 & 0x80) == 0) 
            {
                // invalid byte, bypass rest of this sequence
                i+= (numOctets - 2) * s_octetSize;
            }
            theResult+= XalanDOMChar(((byte1 & 0x1F) << 6) + (byte2 & 0x3F));
        } 
        else if ((byte1 & 0xF0) == 0xE0 && numOctets >= 3)
        {
            i+= s_octetSize;
            XalanDOMChar byte2 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+= s_octetSize;
            XalanDOMChar byte3 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+=2;
            
            if ((byte2 & 0x80) == 0 || (byte3 & 0x80) == 0)
            {
                // invalid byte, bypass rest of this sequence
                i+= (numOctets - 3) * s_octetSize;
            }
            theResult+= XalanDOMChar(((byte1 & 0x0F) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F));
        }
        else if ((byte1 & 0xF8) == 0xF0 && numOctets >=4)
        {
            i+= s_octetSize;
            XalanDOMChar byte2 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+= s_octetSize;
            XalanDOMChar byte3 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+= s_octetSize;
            XalanDOMChar byte4 = hexCharsToByte(executionContext, context, locator,
                                                theString[i+1],theString[i+2]);
            i+=2;
    
            if ((byte2 & 0x80) == 0 || (byte3 & 0x80) == 0 || (byte4 & 0x80) == 0)
            {
                // invalid byte, bypass rest of this sequence
                i+= (numOctets - 4) * s_octetSize;
            }
            theResult+= XalanDOMChar(0xD800 + 
                                     ((byte1 & 0x07) << 8) + ((byte2 & 0x3F) << 2) + 
                                     ((byte3 & 0x30) >> 4) - 0x40);
            theResult+= XalanDOMChar(0xDC00 + ((byte3 & 0x0F) << 6) + (byte4 & 0x3F));
        } 
        else 
        {
            // invalid byte, bypass rest of this sequence
            i+=numOctets * s_octetSize -1;
        }
    }

    return executionContext.getXObjectFactory().createString(theResult);
}



XalanDOMChar
XalanEXSLTFunctionDecodeURI::hexCharsToByte(
            XPathExecutionContext&      executionContext,
            XalanNode*                  context,
            const Locator*              locator,
            const XalanDOMChar          highHexChar,
            const XalanDOMChar          lowHexChar) const
{
    XalanDOMChar byte = 0;
    
    XalanDOMChar curChar = lowHexChar;

    for (int place = 0; place < 2; ++place)
    {
        if (curChar >= XalanUnicode::charDigit_0 
            && curChar <= XalanUnicode::charDigit_9) // Digit 
        {
            byte = byte + XalanDOMChar((curChar - XalanUnicode::charDigit_0) << (place * 4));
        }
        else if (curChar >= XalanUnicode::charLetter_A 
                 && curChar <= XalanUnicode::charLetter_F) // Uppercase
        {
            byte = byte + XalanDOMChar((curChar - XalanUnicode::charLetter_A + 10) << (place * 4));
        }
        else if (curChar >= XalanUnicode::charLetter_a
                 && curChar <= XalanUnicode::charLetter_f)  // Lowercase
        {
            byte = byte + XalanDOMChar((curChar - XalanUnicode::charLetter_a + 10) << place);
        }
        else 
        {
            const XPathExecutionContext::GetCachedString    theGuard(executionContext);

            executionContext.problem(
                XPathExecutionContext::eXPath,
                XPathExecutionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::InvalidURI), 
                locator,
                context);
        }

        curChar = highHexChar;
    }

    return byte;
}


static const XalanDOMChar   s_stringNamespace[] =
{
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_p,
    XalanUnicode::charColon,
    XalanUnicode::charSolidus,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charFullStop,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_g,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_s,
    0
};



static const XalanDOMChar   s_alignFunctionName[] =
{
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_n,
    0
};



static const XalanDOMChar   s_concatFunctionName[] =
{
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    0
};



static const XalanDOMChar   s_paddingFunctionName[] =
{
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};

static const XalanDOMChar   s_encodeUriFunctionName[] =
{
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    0
};

static const XalanDOMChar   s_decodeUriFunctionName[] =
{
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    0
};

static const XalanEXSLTFunctionAlign    s_alignFunction;
static const XalanEXSLTFunctionConcat   s_concatFunction;
// Note this is a special constructor of XalanEXSLTFunctionPadding which
// allocates no memory.  It is only used here, so we can have table-based
// initialization, but not have any memory allocation.

static const XalanEXSLTFunctionPadding  s_paddingFunction(XalanMemMgrs::getDummyMemMgr(), 1);

static const XalanEXSLTFunctionEncodeURI s_encodeUriFunction;
static const XalanEXSLTFunctionDecodeURI s_decodeUriFunction;



static const XalanEXSLTStringFunctionsInstaller::FunctionTableEntry     theFunctionTable[] =
{
    { s_alignFunctionName, &s_alignFunction },
    { s_concatFunctionName, &s_concatFunction },
    { s_paddingFunctionName, &s_paddingFunction },
    { s_encodeUriFunctionName, &s_encodeUriFunction },
    { s_decodeUriFunctionName, &s_decodeUriFunction },
    { 0, 0 }
};



void
XalanEXSLTStringFunctionsInstaller::installLocal(XPathEnvSupportDefault&        theSupport)
{
    doInstallLocal(s_stringNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTStringFunctionsInstaller::installGlobal(MemoryManager& theManager)
{
    doInstallGlobal(theManager, s_stringNamespace, theFunctionTable);
}



void
XalanEXSLTStringFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&  theSupport)
{
    doUninstallLocal(s_stringNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTStringFunctionsInstaller::uninstallGlobal(MemoryManager& theManager)
{
    doUninstallGlobal(theManager, s_stringNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
