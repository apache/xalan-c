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
#if !defined(XALAN_KEYTABLE_HEADER_GUARD)
#define XALAN_KEYTABLE_HEADER_GUARD 

/**
 * $Id$
 * 
 * $State$
 * 
 * @author Myriam Midy (Myriam_Midy @lotus.com 
 */



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
#include <hash_map>
#else
#include <map>
#endif

#include<vector>



// Base class header file.
//#include "UnImplNode.hpp"
//#include "PrefixResolver.hpp"
//#include "Constants.hpp"



// Xerces XML4C header files.
#include <dom/DOM_Node.hpp>
#include <dom/DOMString.hpp>



#include <XPath/MutableNodeRefList.hpp>



class DOM_Element;
class DOM_Node;
class KeyDeclaration;
class NodeRefListBase;
class PrefixResolver;
class XPathExecutionContext;



/**
 * Table of element keys, keyed by document node.  An instance of this 
 * class is keyed by a Document node that should be matched with the 
 * root of the current context.  It contains a table of name mappings 
 * to tables that contain mappings of identifier values to nodes.
 */
class KeyTable
{
public:

	/**
	 * Build a keys table.
	 * @param doc The owner document key (normally the same as startNode).
	 * @param startNode The node to start itterating from to build the keys index.
	 * @param nscontext The stylesheet's namespace context.
	 * @param keyDeclarations The stylesheet's xsl:key declarations.
	 * @param executionContext The current execution context.
	 */
	KeyTable(
			const DOM_Node&						doc,
			const DOM_Node&						startNode,
			const PrefixResolver&				resolver,
			const std::vector<KeyDeclaration>&	keyDeclarations,
			XPathExecutionContext&				executionContext);

	virtual
	~KeyTable();

	/**
	 * Given a valid element key, return the corresponding node list. 
	 * @param The name of the key, which must match the 'name' attribute on xsl:key.
	 * @param ref The value that must match the value found by the 'match' attribute on xsl:key.
	 * @return If the name was not declared with xsl:key, this will return null, 
	 * if the identifier is not found, it will return an empty node set, 
	 * otherwise it will return a nodeset of nodes.
	 */
	const NodeRefListBase*
	getNodeSetByKey(
				  const DOMString&	name, 
				  const DOMString&	ref) const;

	DOM_Node
	getDocKey() const
	{
		return m_docKey;
	}

private:

	/**
	 * The document key.  This table should only be used with contexts
	 * whose Document roots match this key.
	 */
	DOM_Node	m_docKey;

	/**
	 * Hashtable of keys.
	 * The table is:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *    the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */

	/**
	 * Table of element keys.  The table will be built on demand, 
	 * when a key is requested, or set by the XMLParserLiaison or 
	 * the caller.  The table is:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *    the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */

#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
	typedef hash_map<DOMString,
					 MutableNodeRefList,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>	NodeListMapType;

	typedef hash_map<DOMString,
					 NodeListMapType,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>	KeysMapType;
#else
	typedef std::map<DOMString,
					 MutableNodeRefList>	NodeListMapType;

	typedef std::map<DOMString,
					 NodeListMapType>	KeysMapType;
#endif

	KeysMapType		m_keys;
};



#endif	// XALAN_KEYTABLE_HEADER_GUARD
