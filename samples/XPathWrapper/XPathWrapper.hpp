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
#if !defined(XPATH_WRAPPER_HEADER_GUARD_1357924680)
#define XPATH_WRAPPER_HEADER_GUARD_1357924680



#include <xalanc/Include/PlatformDefinitions.hpp>

#include <xalanc/Include/XalanVector.hpp>


#include <xalanc/PlatformSupport/DOMStringHelper.hpp>




#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
#include <stl/_vector.c>
#endif



class XPathWrapperImpl;



class XPathWrapper
{

public:

    typedef XALAN_CPP_NAMESPACE_QUALIFIER CharVectorType    ChVectorType;


    typedef XALAN_CPP_NAMESPACE_QUALIFIER XalanVector<ChVectorType>     CharVectorTypeVectorType;   


    XPathWrapper();

    virtual
    ~XPathWrapper();

    // Given an xml document and an xpath context and expression in the form of (ascii) string objects,
    // this function parses the XML document, evaluates the xpath and returns the result, as a list of 
    // string objects
    void
    evaluate(
        const char*     xml, 
        const char*     context, 
        const char*     path,
                CharVectorTypeVectorType& theResult);

private:

    // not implemented
    XPathWrapper(const XPathWrapper&);

    XPathWrapper&
    operator=(const XPathWrapper&);

    bool
    operator==(const XPathWrapper&) const;

    XPathWrapperImpl* const     pImpl;
};


#endif  // XPATH_WRAPPER_HEADER_GUARD_1357924680
