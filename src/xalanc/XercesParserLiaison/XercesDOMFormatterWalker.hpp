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
#if !defined(XERCESDOMFORMATTERWALKER_HEADER_GUARD_1357924680)
#define XERCESDOMFORMATTERWALKER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <xalanc/XercesParserLiaison/XercesDOMWalker.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMFormatterWalker : public XercesDOMWalker
{
public:

    typedef XercesDOMWalker     ParentType;

    /**
     * Constructor.
     */
    XercesDOMFormatterWalker(FormatterListener&     theFormatterListener);

    virtual
    ~XercesDOMFormatterWalker();

protected:

    virtual bool
    startNode(const DOMNodeType*    node);

    virtual bool
    endNode(const DOMNodeType*  node);

    using ParentType::startNode;
    using ParentType::endNode;

private:

    FormatterListener&  m_formatterListener;


};



XALAN_CPP_NAMESPACE_END



#endif  // XERCESDOMFORMATTERWALKER_HEADER_GUARD_1357924680
