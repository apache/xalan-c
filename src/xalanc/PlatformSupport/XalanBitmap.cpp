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

#include "XalanBitmap.hpp"



#include <algorithm>



XALAN_CPP_NAMESPACE_BEGIN



// Pre-constructed masks for bit twiddling.  Update these if not using chars for storing the bits.
const int   XalanBitmap::s_setMasks[XalanBitmap::eBitsPerUnit] = { 1, 2, 4, 8, 16, 32, 64, 128 };
const int   XalanBitmap::s_clearMasks[XalanBitmap::eBitsPerUnit] = { ~1, ~2, ~4, ~8, ~16, ~32, ~64, ~128 };



XalanBitmap::XalanBitmap(MemoryManager& theManager, size_type   theSize) :
    m_size(theSize),
    m_bitmap(size_type((theSize + eBitsPerUnit) / eBitsPerUnit), BitmapVectorType::value_type(0), theManager)
{
}



XalanBitmap::~XalanBitmap()
{
}



void
XalanBitmap::clearAll()
{
    XALAN_STD_QUALIFIER fill(m_bitmap.begin(), m_bitmap.end(), '\0');
}



XALAN_CPP_NAMESPACE_END
