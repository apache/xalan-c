/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_VARIABLESSTACK_HEADER_GUARD)
#define XALAN_VARIABLESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <cassert>
#include <vector>



#include <XPath/QName.hpp>



#include <XSLT/XSLTProcessorException.hpp>



class Arg;
class ElemTemplateElement;
class StylesheetExecutionContext;
class XObject;
class XalanNode;



/**
 * Defines a class to keep track of a stack for macro arguments.
 */
class XALAN_XSLT_EXPORT VariablesStack
{
public:

	/**
	 * Constructor for a variable stack.
	 */
	explicit
	VariablesStack();

	~VariablesStack();

	
	/**
	 * Reset the stack.
	 */
	void
	reset();

	/**
	 * Push a frame marker for an element.
	 *
	 * @param elem the element
	 */
	void
	pushElementFrame(const ElemTemplateElement*		elem);

	/**
	 * Pop a frame marker for an element.
	 *
	 * @param elem the element
	 */
	void
	popElementFrame(const ElemTemplateElement*	elem);

	/**
	 * Push a context marker onto the stack to let us know when to stop
	 * searching for a var.
	 *
	 * @param caller	 caller node
	 * @param sourceNode source node
	 */
	void
	pushContextMarker();

	/**
	 * Pop the current context from the current context stack.
	 */
	void
	popContextMarker();

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<pair<const QName*, XObject*> >				ParamsVectorType;
#else
	typedef std::vector<std::pair<const QName*, XObject*> >		ParamsVectorType;
#endif

	/**
	 * Push the provided objects as parameters.  You must call
	 * popContextMarker() when you are done with the arguments.
	 *
	 * @param theParam The vector containing the parameters.
	 * @param targetTemplate target template for the parameters
	 */
	void
	pushParams(
			const ParamsVectorType&		theParams,
			const ElemTemplateElement*	targetTemplate);

	/**
	 * Given a name, return a string representing the value, but don't look
	 * in the global space.
	 *
	 * @param theName name of variable
	 * @return pointer to XObject for variable
	 */
	XObject*
	getParamVariable(const QName& qname) const
	{
		return findXObject(qname, false);
	}

	/**
	 * Given a name, find the corresponding XObject.
	 *
	 * @param qname name of variable
	 * @return pointer to the corresponding XObject
	 */
	XObject*
	getVariable(const QName& 	name) const
	{
		return findXObject(name, true);
	}

	/**
	 * Push a named variable onto the processor variable stack. Don't forget
	 * to call startContext before pushing a series of arguments for a given
	 * macro call.
	 *
	 * @param name	  name of variable
	 * @param val	  pointer to XObject value
	 * @param e 	  element marker for variable
	 */
	void
	pushVariable(
			const QName&				name,
			XObject*					val,
			const ElemTemplateElement*	e);

	/**
	 * Mark the top of the stack.
	 */
	void
	start();

	/**
	 * Mark the top of the global stack frame.
	 */
	void
	markGlobalStackFrame();

	/**
	 * Set the top of the stack frame from where a search for a variable or
	 * param should take place.  Calling with no parameter will cause the
	 * index to be set to the size of the stack.
	 *
	 * @param currentStackFrameIndex new value of index
	 */
	void
	setCurrentStackFrameIndex(int	currentStackFrameIndex = -1)
	{
		if (currentStackFrameIndex == -1)
			m_currentStackFrameIndex = m_stack.size();
		else
			m_currentStackFrameIndex = currentStackFrameIndex;
	}

	/**
	 * Get the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @return current value of index
	 */
	int
	getCurrentStackFrameIndex() const
	{
		return m_currentStackFrameIndex;
	}

	class InvalidStackContextException : public XSLTProcessorException
	{
	public:

		InvalidStackContextException();

		virtual
		~InvalidStackContextException();

	private:

	};

private:

	class StackEntry;

	/**
	 * Check to see if an element frame for the particular element has already
	 * been pushed.
	 * 
	 * @param elem element in question
	 * @return true if it has been pushed already
	 */
	bool
	elementFrameAlreadyPushed(const ElemTemplateElement*	elem) const;

	/**
	 * Push an entry onto the stack.
	 *
	 * @param stack entry to push
	 */
	void
	push(const StackEntry&	theEntry);

	/**
	 * Pop an entry from the top of the stack.
	 */
	void
	pop();

	/**
	 * Get a reference to the entry at the back (top) of the stack.
	 *
	 * @return a reference to the back of the stack.
	 */
	const StackEntry&
	back() const
	{
		assert(m_stack.empty() == false);

		return m_stack.back();
	}

	friend class CommitPushElementFrame;
	friend class EnsurePop;
	friend class PopPushStackEntry;
	friend class PushFunctor;
	friend class PushParamFunctor;

	class StackEntry
	{
	public:

		/**
		 * Enumeration for types of stack entries, one of context state, context
		 * marker, element marker, or argument.
		 */
		enum eStackEntryType { eContextMarker,
							   eVariable,
							   eElementFrameMarker,
							   eNextValue };

		/**
		 * Construct a context marker.
		 */
		explicit
		StackEntry();

		/**
		 * Construct a variable.
		 */
		StackEntry(
			const QName*	name,
			XObject*		val);

		/**
		 * Construct an element frame marker.
		 */
		StackEntry(const ElemTemplateElement*	elem);


		/**
		 * Copy constructor...
		 */
		StackEntry(const StackEntry&	theSource);

		/**
		 * Destructor...
		 */
		~StackEntry();

		/**
		 * Determine type of stack entry
		 * 
		 * @return enumeration value for type
		 */
		eStackEntryType
		getType() const
		{
			return m_type;
		}

		/**
		 * Retrieve object name.  Valid only for variables
		 * 
		 * @return qualified name of object
		 */
		const QName*
		getName() const
		{
			return variable.m_qname;
		}

		/**
		 * Retrieve object's XObject pointer.  Valid only for variables
		 * 
		 * @return pointer to XObject
		 */
		XObject*
		getVariable() const
		{
			return variable.m_value;
		}

		/**
		 * Retrieve the ElemTemplateElem where frame begins.  Valid only for element frame markers
		 *
		 * @return ElemTemplateElement corresponding to marker
		 */
		const ElemTemplateElement*
		getElement() const
		{
			return elementMarker.m_element;
		}

		StackEntry&
		operator=(const StackEntry&		theRHS);

		bool
		operator==(const StackEntry&	theRHS) const;

	private:

		// Data members...
		eStackEntryType				m_type;

		union
		{
			struct
			{
				const QName*				m_qname;

				XObject*					m_value;
			} variable;

			struct
			{
				const ElemTemplateElement*	m_element;
			} elementMarker;
		};
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<StackEntry>			VariableStackStackType;
#else
	typedef std::vector<StackEntry>		VariableStackStackType;
#endif

	enum { eDefaultStackSize = 100 };


	XObject*
	findXObject(
			const QName&	name,
			bool			fSearchGlobalSpace) const;

	const StackEntry*
	findVariable(
			const QName&	name,
			bool			fSearchGlobalSpace) const;


	VariableStackStackType			m_stack;

	int								m_globalStackFrameIndex;

	/**
	 * This is the top of the stack frame from where a search 
	 * for a variable or param should take place.  It may not 
	 * be the real stack top.
	 */
	unsigned int					m_currentStackFrameIndex;	
};



#endif	// #if !defined(XALAN_VARIABLESSTACK_HEADER_GUARD)
