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

#include "XalanMatchPatternData.hpp"



#include "xalanc/PlatformSupport/DoubleSupport.hpp"



#include "ElemTemplate.hpp"



XALAN_CPP_NAMESPACE_BEGIN



double
XalanMatchPatternData::getPriorityOrDefault() const
{
    const double    templatePriority =
        m_template->getPriority();

    if (DoubleSupport::isNegativeInfinity(templatePriority) == true)
    {
        return XPath::getMatchScoreValue(m_priority);
    }
    else
    {
        return templatePriority;
    }
}



XALAN_CPP_NAMESPACE_END
