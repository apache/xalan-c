/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XERCESBRIDGETYPES_HEADER_GUARD_1357924680)
#define XERCESBRIDGETYPES_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



XALAN_DECLARE_XERCES_CLASS(DOM_Attr)
XALAN_DECLARE_XERCES_CLASS(DOM_CharacterData)
XALAN_DECLARE_XERCES_CLASS(DOM_Comment)
XALAN_DECLARE_XERCES_CLASS(DOM_CDATASection)
XALAN_DECLARE_XERCES_CLASS(DOM_Document)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentFragment)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentType)
XALAN_DECLARE_XERCES_CLASS(DOM_DOMImplementation)
XALAN_DECLARE_XERCES_CLASS(DOM_Element)
XALAN_DECLARE_XERCES_CLASS(DOM_Entity)
XALAN_DECLARE_XERCES_CLASS(DOM_EntityReference)
XALAN_DECLARE_XERCES_CLASS(DOM_Node)
XALAN_DECLARE_XERCES_CLASS(DOM_Text)
XALAN_DECLARE_XERCES_CLASS(DOM_NamedNodeMap)
XALAN_DECLARE_XERCES_CLASS(DOM_NodeList)
XALAN_DECLARE_XERCES_CLASS(DOM_Notation)
XALAN_DECLARE_XERCES_CLASS(DOM_ProcessingInstruction)
XALAN_DECLARE_XERCES_CLASS(DOM_Text)
XALAN_DECLARE_XERCES_CLASS(DOM_DocumentType)
XALAN_DECLARE_XERCES_CLASS(DOM_DOMException)
XALAN_DECLARE_XERCES_CLASS(DOMString)
XALAN_DECLARE_XERCES_CLASS(AttrImpl)
XALAN_DECLARE_XERCES_CLASS(ElementImpl)
XALAN_DECLARE_XERCES_CLASS(NodeImpl)
XALAN_DECLARE_XERCES_CLASS(TextImpl)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr				DOM_AttrType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_CharacterData	DOM_CharacterDataType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Comment			DOM_CommentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_CDATASection		DOM_CDATASectionType;
// These next two have an extra '_' because they collide without it...
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DocumentType		DOM_DocumentType_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Document			DOM_Document_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DocumentFragment	DOM_DocumentFragmentType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DOMImplementation	DOM_DOMImplementationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element			DOM_ElementType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Entity			DOM_EntityType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_EntityReference	DOM_EntityReferenceType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node				DOM_NodeType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text				DOM_TextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_NamedNodeMap		DOM_NamedNodeMapType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_NodeList			DOM_NodeListType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Notation			DOM_NotationType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_ProcessingInstruction	DOM_ProcessingInstructionType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text				DOM_TextType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_DOMException		DOM_DOMExceptionType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER AttrImpl				AttrImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ElementImpl			ElementImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER NodeImpl				NodeImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER TextImpl				TextImplType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMString			DOMStringType;





XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESBRIDGETYPES_HEADER_GUARD_1357924680)
