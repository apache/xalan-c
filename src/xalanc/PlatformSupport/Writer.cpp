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
// Class header file.
#include "Writer.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#if !defined(XALAN_INLINE_INITIALIZATION)
const size_t    Writer::npos = ~0u;
#elif !defined(XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG)
const size_t    Writer::npos;
#endif



Writer::Writer()
{
}



Writer::~Writer()
{
}



XalanOutputStream*
Writer::getStream()
{
    return 0;
}



const XalanOutputStream*
Writer::getStream() const
{
    return 0;
}



XALAN_CPP_NAMESPACE_END
