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
 
#include <xalanc/Utils/XalanMsgLib/XalanMsgLib.hpp>


#include <xalanc/PlatformSupport/XalanUnicode.hpp>


#include <LocalMsgData.hpp>

XALAN_CPP_NAMESPACE_BEGIN


static const XalanDOMChar	s_errString[] =
{
	XalanUnicode::charLetter_M,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_t,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_d,
	0
};



unsigned int
XalanMsgContainer::getNumbOfMsgs()
{
	return gXalanMsgArraySize;
}



const XalanDOMChar*
XalanMsgContainer::getMessage(unsigned int	msgInd)
{
	if ( msgInd < gXalanMsgArraySize )
		return msgArray[msgInd];
	else
		return s_errString;
}




XALAN_CPP_NAMESPACE_END


