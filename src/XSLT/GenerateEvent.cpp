/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#include "GenerateEvent.hpp"



/** 
* Constructor for startDocument, endDocument events.
* 
* @param processor The XSLT Processor instance.
* @param eventType One of the EVENTTYPE_XXX constants.
*/
GenerateEvent::GenerateEvent(XSLTEngineImpl* processor, EventType eventType) :  m_pProcessor(processor), 
	m_eventType(eventType), m_start(0), m_length(0), m_pAtts(0)
{
}

/** 
* Constructor for startElement, endElement events.
* 
* @param processor The XSLT Processor Instance.
* @param eventType One of the EVENTTYPE_XXX constants.
* @param name The name of the element.
* @param atts The SAX attribute list.
*/
GenerateEvent::GenerateEvent(XSLTEngineImpl* processor, EventType eventType, DOMString name, AttributeList* atts):	
	m_name(name), m_pAtts(atts), m_pProcessor(processor), m_eventType(eventType), m_start(0), m_length(0)
{
}

/** 
* Constructor for characters, cdate events.
* 
* @param processor The XSLT Processor instance.
* @param eventType One of the EVENTTYPE_XXX constants.
* @param ch The char array from the SAX event.
* @param start The start offset to be used in the char array.
* @param length The end offset to be used in the chara array.
*/
GenerateEvent::GenerateEvent(XSLTEngineImpl* processor, EventType eventType, DOMString ch, int start, int length) :	
	m_characters(ch), m_start(start), m_length(length), m_pProcessor(processor), m_eventType(eventType), m_pAtts(0)
{
}

/** 
* Constructor for processingInstruction events.
* 
* @param processor The instance of the XSLT processor.
* @param eventType One of the EVENTTYPE_XXX constants.
* @param name The name of the processing instruction.
* @param data The processing instruction data.
*/
GenerateEvent::GenerateEvent(XSLTEngineImpl* processor, EventType eventType, DOMString name, DOMString data): 
	m_name(name), m_data(data), m_pProcessor(processor), m_eventType(eventType)
{
}

/** 
* Constructor for comment and entity ref events.
* 
* @param processor The XSLT processor instance.
* @param eventType One of the EVENTTYPE_XXX constants.
* @param data The comment or entity ref data.
*/
GenerateEvent::GenerateEvent(XSLTEngineImpl* processor, EventType eventType, DOMString data):
	m_data(data), m_pProcessor(processor), m_eventType(eventType)
{
}


