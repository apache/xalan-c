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
#include "XalanHTMLElementsProperties.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>




XALAN_CPP_NAMESPACE_BEGIN



XalanHTMLElementsProperties::XalanHTMLElementsProperties()
{
}



XalanHTMLElementsProperties::ElementProperties
XalanHTMLElementsProperties::find(const XalanDOMChar*   theElementName)
{
    const InternalElementProperties&    theProperties =
        findProperties(theElementName);

    return ElementProperties(&theProperties);
}



bool
XalanHTMLElementsProperties::InternalElementProperties::isAttribute(
                const XalanDOMChar*     theAttributeName,
                AttributeFlagsType      theFlags) const
{
    const InternalAttributeProperties&  theProperties =
                findProperties(theAttributeName);

    return theProperties.is(theFlags);
}



const XalanHTMLElementsProperties::InternalAttributeProperties&
XalanHTMLElementsProperties::InternalElementProperties::findProperties(const XalanDOMChar*  theAttributeName) const
{
    const InternalAttributeProperties*  theCurrent = m_attributes;

    for(;;)
    {
        if (theCurrent->m_name[0] == 0)
        {
            return s_dummyProperties;
        }

        const int   theResult = compareIgnoreCaseASCII(theAttributeName, theCurrent->m_name);

        if (theResult == 0)
        {
            break;
        }
        else if (theResult < 0)
        {
            return s_dummyProperties;
        }
        else
        {
            ++theCurrent;
        }
    }

    assert(theCurrent != 0);

    return *theCurrent;
}



const XalanHTMLElementsProperties::InternalAttributeProperties  XalanHTMLElementsProperties::InternalElementProperties::s_dummyProperties =
                 { { 0 }, 0 };



const XalanHTMLElementsProperties::InternalElementProperties&
XalanHTMLElementsProperties::findProperties(const XalanDOMChar*     theElementName)
{
    assert(theElementName != 0);

    // Find the InternalElementProperties instance, if any...
    const InternalElementProperties*    theFirst = s_elementProperties;
    const InternalElementProperties*    theLast = s_lastProperties;

    while(theFirst <= theLast)
    {
        const InternalElementProperties*    theCurrent = theFirst + (theLast - theFirst) / 2;
        assert(theCurrent->m_name[0] != 0);

        const int   theResult = compareIgnoreCaseASCII(theElementName, theCurrent->m_name);

        if (theResult < 0)
        {
            theLast = theCurrent - 1;
        }
        else if (theResult > 0)
        {
            theFirst = theCurrent + 1;
        }
        else
        {
            return *theCurrent;
        }
    }

    return *s_dummyProperties;
}



typedef XalanHTMLElementsProperties     ElemDesc;

const XalanHTMLElementsProperties::InternalElementProperties    XalanHTMLElementsProperties::s_elementProperties[] =
{
    // Note that these are in a particular order, which is not a collation order.  It is
    // the ordering that is imposed by the function compareIgnoreCaseASCII().  Basically,
    // strings are first collated by length, and the by their characters.  Additions
    // must obey the same rules, or the code will not work correctly.

    // Also, should new entries be added, you must make sure that all static sizes are
    // still adequate to hold the additions.  You must consider the length of the
    // name of the element or attribute, and the number of attributes.  There are enums
    // defined in the header file which indicate the maximum size of each.
    {
        { XalanUnicode::charLetter_A, 0 },
        ElemDesc::SPECIAL,
        {
            {
                { XalanUnicode::charLetter_H, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            // This is a dummy entry and it must be the last one!!! Each array of
            // InternalAttributesProperties must be terminated by a dummy entry.
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_I, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_P, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_Q, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_U, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_R, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK | ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_D, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_L, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_T, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_E, XalanUnicode::charLetter_M, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_1, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_2, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_3, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_4, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_5, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charDigit_6, 0 },
        ElemDesc::BLOCK | ElemDesc::HEAD,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charLetter_R, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
            
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_S, XalanUnicode::charLetter_H, XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_L, XalanUnicode::charLetter_I, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_O, XalanUnicode::charLetter_L, 0 },
        ElemDesc::BLOCK | ElemDesc::LIST,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_D, 0 },
        0,
        {
            {
            
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_W, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, 0 },
        0,
        {
            {
            
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_W, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_R, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_T, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_U, XalanUnicode::charLetter_L, 0 },
        ElemDesc::BLOCK | ElemDesc::LIST,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_D, XalanUnicode::charLetter_O, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_I, XalanUnicode::charLetter_G, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_L, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_L, 0 },
        0,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_F, XalanUnicode::charLetter_N, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_R, 0 },
        ElemDesc::BLOCK,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_V, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_M, XalanUnicode::charLetter_G, 0 },
        ElemDesc::EMPTY | ElemDesc::SPECIAL | ElemDesc::ASPECIAL | ElemDesc::WHITESPACESENSITIVE,
        {
            {
            
                { XalanUnicode::charLetter_S, XalanUnicode::charLetter_R, XalanUnicode::charLetter_C, 0 },
                ElemDesc::ATTRURL,
            },
            {
            
                { XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
            
                { XalanUnicode::charLetter_U, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTRURL,
            },
            {
            
                { XalanUnicode::charLetter_L, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_G, XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_S, XalanUnicode::charLetter_C, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_N, XalanUnicode::charLetter_S, 0 },
        0,
        {
            {
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_K, XalanUnicode::charLetter_B, XalanUnicode::charLetter_D, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
        ElemDesc::BLOCK | ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_P, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, 0 },
        ElemDesc::BLOCK | ElemDesc::PREFORMATTED,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_U, XalanUnicode::charLetter_B, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_U, XalanUnicode::charLetter_P, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_V, XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_B, XalanUnicode::charLetter_R, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK,
        {
            {
                { XalanUnicode::charLetter_H, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_H, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_A, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK,
        {
            {
                { XalanUnicode::charLetter_H, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_O, XalanUnicode::charLetter_D, XalanUnicode::charLetter_Y, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_F, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_T, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_F, XalanUnicode::charLetter_O, XalanUnicode::charLetter_R, XalanUnicode::charLetter_M, 0 },
        ElemDesc::BLOCK,
        {
            {
                { XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, XalanUnicode::charLetter_I, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, 0 },
        ElemDesc::BLOCK | ElemDesc::HEADELEM,
        {
            {
                { XalanUnicode::charLetter_P, XalanUnicode::charLetter_R, XalanUnicode::charLetter_O, XalanUnicode::charLetter_F, XalanUnicode::charLetter_I, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_H, XalanUnicode::charLetter_T, XalanUnicode::charLetter_M, XalanUnicode::charLetter_L, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_L, XalanUnicode::charLetter_I, XalanUnicode::charLetter_N, XalanUnicode::charLetter_K, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK | ElemDesc::HEADMISC,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, XalanUnicode::charLetter_N, XalanUnicode::charLetter_U, 0 },
        ElemDesc::BLOCK,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, XalanUnicode::charLetter_A, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK | ElemDesc::HEADMISC,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_B, XalanUnicode::charLetter_R, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, XalanUnicode::charLetter_P, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_N, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_F, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK,
        {
            {
                { XalanUnicode::charLetter_S, XalanUnicode::charLetter_R, XalanUnicode::charLetter_C, 0 },
                ElemDesc::ATTRURL,
            },
            {
            
                { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_S, XalanUnicode::charLetter_I, XalanUnicode::charLetter_Z, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_N, XalanUnicode::charLetter_P, XalanUnicode::charLetter_U, XalanUnicode::charLetter_T, 0 },
        ElemDesc::EMPTY | ElemDesc::INLINELABEL | ElemDesc::FORMCTRL,
        {
            {
                { XalanUnicode::charLetter_S, XalanUnicode::charLetter_R, XalanUnicode::charLetter_C, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { XalanUnicode::charLetter_U, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_H, XalanUnicode::charLetter_E, XalanUnicode::charLetter_C, XalanUnicode::charLetter_K, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
                },
            {
                { XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_L, XalanUnicode::charLetter_Y, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_L, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_E, XalanUnicode::charLetter_L, 0 },
        ElemDesc::FORMCTRL,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_P, XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, 0 },
        ElemDesc::EMPTY,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_L, XalanUnicode::charLetter_L, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_T, XalanUnicode::charLetter_Y, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, 0 },
        ElemDesc::BLOCK | ElemDesc::RAW | ElemDesc::HEADMISC | ElemDesc::STYLEELEM,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_B, XalanUnicode::charLetter_O, XalanUnicode::charLetter_D, XalanUnicode::charLetter_Y, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_F, XalanUnicode::charLetter_O, XalanUnicode::charLetter_O, XalanUnicode::charLetter_T, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, XalanUnicode::charLetter_P, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, 0 },
        ElemDesc::WHITESPACESENSITIVE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_U, XalanUnicode::charLetter_T, XalanUnicode::charLetter_T, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, 0 },
        ElemDesc::INLINELABEL | ElemDesc::FORMCTRL,
        {
            {
            
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_E, XalanUnicode::charLetter_N, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, XalanUnicode::charLetter_R, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_G, XalanUnicode::charLetter_E, XalanUnicode::charLetter_N, XalanUnicode::charLetter_D, 0 },
        0,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_O, XalanUnicode::charLetter_B, XalanUnicode::charLetter_J, XalanUnicode::charLetter_E, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
        ElemDesc::SPECIAL | ElemDesc::ASPECIAL | ElemDesc::HEADMISC | ElemDesc::WHITESPACESENSITIVE,
        {
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_A, XalanUnicode::charLetter_T, XalanUnicode::charLetter_A, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_U, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_M, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, XalanUnicode::charLetter_C, XalanUnicode::charLetter_H, XalanUnicode::charLetter_I, XalanUnicode::charLetter_V, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_L, XalanUnicode::charLetter_A, XalanUnicode::charLetter_S, XalanUnicode::charLetter_S, XalanUnicode::charLetter_I, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_C, XalanUnicode::charLetter_L, XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_B, XalanUnicode::charLetter_A, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_O, XalanUnicode::charLetter_P, XalanUnicode::charLetter_T, XalanUnicode::charLetter_I, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, 0 },
        0,
        {
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {   
                { XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_C, XalanUnicode::charLetter_R, XalanUnicode::charLetter_I, XalanUnicode::charLetter_P, XalanUnicode::charLetter_T, 0 },
        ElemDesc::RAW | ElemDesc::SPECIAL | ElemDesc::ASPECIAL | ElemDesc::HEADMISC | ElemDesc::SCRIPTELEM,
        {
            {
                { XalanUnicode::charLetter_F, XalanUnicode::charLetter_O, XalanUnicode::charLetter_R, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_S, XalanUnicode::charLetter_R, XalanUnicode::charLetter_C, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, XalanUnicode::charLetter_E, XalanUnicode::charLetter_R, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_C, XalanUnicode::charLetter_T, 0 },
        ElemDesc::INLINELABEL | ElemDesc::FORMCTRL,
        {
            {
            
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
            
                { XalanUnicode::charLetter_M, XalanUnicode::charLetter_U, XalanUnicode::charLetter_L, XalanUnicode::charLetter_T, XalanUnicode::charLetter_I, XalanUnicode::charLetter_P, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_T, XalanUnicode::charLetter_R, XalanUnicode::charLetter_I, XalanUnicode::charLetter_K, XalanUnicode::charLetter_E, 0 },
        ElemDesc::FONTSTYLE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_S, XalanUnicode::charLetter_T, XalanUnicode::charLetter_R, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_G, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_A, XalanUnicode::charLetter_C, XalanUnicode::charLetter_R, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_Y, XalanUnicode::charLetter_M, 0 },
        ElemDesc::PHRASE,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, XalanUnicode::charLetter_D, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_S, XalanUnicode::charLetter_S, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_A, XalanUnicode::charLetter_P, XalanUnicode::charLetter_T, XalanUnicode::charLetter_I, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_I, XalanUnicode::charLetter_N, XalanUnicode::charLetter_D, XalanUnicode::charLetter_E, XalanUnicode::charLetter_X, 0 },
        ElemDesc::EMPTY | ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_A, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_F, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_T, 0 },
        ElemDesc::EMPTY,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_C, XalanUnicode::charLetter_O, XalanUnicode::charLetter_L, XalanUnicode::charLetter_G, XalanUnicode::charLetter_R, XalanUnicode::charLetter_O, XalanUnicode::charLetter_U, XalanUnicode::charLetter_P, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_F, XalanUnicode::charLetter_I, XalanUnicode::charLetter_E, XalanUnicode::charLetter_L, XalanUnicode::charLetter_D, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_F, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, XalanUnicode::charLetter_S, XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_F, XalanUnicode::charLetter_R, XalanUnicode::charLetter_A, XalanUnicode::charLetter_M, XalanUnicode::charLetter_E, XalanUnicode::charLetter_S, 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_N, XalanUnicode::charLetter_O, XalanUnicode::charLetter_S, XalanUnicode::charLetter_C, XalanUnicode::charLetter_R, XalanUnicode::charLetter_I, XalanUnicode::charLetter_P, XalanUnicode::charLetter_T, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_O, XalanUnicode::charLetter_P, XalanUnicode::charLetter_T, XalanUnicode::charLetter_G, XalanUnicode::charLetter_R, XalanUnicode::charLetter_O, XalanUnicode::charLetter_U, XalanUnicode::charLetter_P, 0 },
        0,
        {
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, XalanUnicode::charLetter_X, XalanUnicode::charLetter_T, XalanUnicode::charLetter_A, XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, 0 },
        ElemDesc::INLINELABEL | ElemDesc::FORMCTRL,
        {
            {
                { XalanUnicode::charLetter_D, XalanUnicode::charLetter_I, XalanUnicode::charLetter_S, XalanUnicode::charLetter_A, XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_E, XalanUnicode::charLetter_D, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { XalanUnicode::charLetter_R, XalanUnicode::charLetter_E, XalanUnicode::charLetter_A, XalanUnicode::charLetter_D, XalanUnicode::charLetter_O, XalanUnicode::charLetter_N, XalanUnicode::charLetter_L, XalanUnicode::charLetter_Y, 0 },
                ElemDesc::ATTREMPTY,
            },
            {
                { 0 },
                0,
            },
        },
    },
    {
        { XalanUnicode::charLetter_B, XalanUnicode::charLetter_L, XalanUnicode::charLetter_O, XalanUnicode::charLetter_C, XalanUnicode::charLetter_K, XalanUnicode::charLetter_Q, XalanUnicode::charLetter_U, XalanUnicode::charLetter_O, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
        ElemDesc::BLOCK | ElemDesc::BLOCKFORM | ElemDesc::BLOCKFORMFIELDSET,
        {
            {
            
                { XalanUnicode::charLetter_C, XalanUnicode::charLetter_I, XalanUnicode::charLetter_T, XalanUnicode::charLetter_E, 0 },
                ElemDesc::ATTRURL,
            },
            {
                { 0 },
                0,
            },
        },
    },
    // This is a dummy entry and it must be the last one!!!
    {
        { 0 },
        ElemDesc::BLOCK,
        {
            {
                { 0 },
                0,
            },
        },
    },
};



const XalanHTMLElementsProperties::InternalElementProperties* const     XalanHTMLElementsProperties::s_lastProperties =
            s_elementProperties + (sizeof(s_elementProperties) / sizeof(s_elementProperties[0]) - 2);

const XalanHTMLElementsProperties::InternalElementProperties* const     XalanHTMLElementsProperties::s_dummyProperties =
            s_lastProperties + 1;



XALAN_CPP_NAMESPACE_END
