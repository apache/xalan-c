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
#if !defined(XALAN_DECIMALTOROMAN_HEADER_GUARD)
#define XALAN_DECIMALTOROMAN_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Structure to help in converting integrals to roman numerals
 */
struct XALAN_XSLT_EXPORT DecimalToRoman 
{
    enum { eMaxLetter = 2 };

    typedef XalanSize_t     ValueType;

    ValueType       m_postValue;

    XalanDOMChar    m_postLetter[eMaxLetter + 1];

    ValueType       m_preValue;

    XalanDOMChar    m_preLetter[eMaxLetter + 1];
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_DECIMALTOROMAN_HEADER_GUARD
