/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <cassert>
#include <vector>



#include <xalanc/XPath/XalanQName.hpp>
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XSLT/XSLTProcessorException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Arg;
class ElemTemplateElement;
class ElemVariable;
class StylesheetExecutionContext;
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
	popElementFrame();

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

	struct ParamsVectorEntry
	{
		ParamsVectorEntry() :
			m_qname(0),
			m_value(),
			m_variable(0)
		{
		}

		ParamsVectorEntry(
				const XalanQName*	qname,
				const XObjectPtr	value) :
			m_qname(qname),
			m_value(value),
			m_variable(0)
		{
		}

		ParamsVectorEntry(
				const XalanQName*		qname,
				const ElemVariable*		variable) :
			m_qname(qname),
			m_value(),
			m_variable(variable)
		{
		}

		const XalanQName*		m_qname;

		XObjectPtr				m_value;

		const ElemVariable*		m_variable;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<ParamsVectorEntry>			ParamsVectorType;
	typedef vector<const ElemVariable*>			RecursionGuardStackType;
	typedef vector<const ElemTemplateElement*>	ElemTemplateElementStackType;
#else
	typedef std::vector<ParamsVectorEntry>			ParamsVectorType;
	typedef std::vector<const ElemVariable*>		RecursionGuardStackType;
	typedef std::vector<const ElemTemplateElement*>	ElemTemplateElementStackType;
#endif

	/**
	 * Push the provided objects as parameters.  You must call
	 * popContextMarker() when you are done with the arguments.
	 *
	 * @param theParam The vector containing the parameters.
	 */
	void
	pushParams(const ParamsVectorType&	theParams);

	/**
	 * Given a name, return a string representing the value, but don't look
	 * in the global space.  Since the variable may not yet have been
	 * evaluated, this may return a null XObjectPtr.
	 *
	 * @param theName name of variable
	 * @param exeuctionContext the current execution context
	 * @param fNameFound set to true if the name was found, false if not.
	 * @return pointer to XObject for variable
	 */
	const XObjectPtr
	getParamVariable(
			const XalanQName&				qname,
			StylesheetExecutionContext&		executionContext,
			bool&							fNameFound)
	{
		return findXObject(qname, executionContext, true, false, fNameFound);
	}

	/**
	 * Given a name, find the corresponding XObject.  If the variable
	 * exists, but has not yet been evaluated, the variable will be
	 * evaluated and the result returned.  This may return a null XObjectPtr,
	 * if the variable was not found.
	 *
	 * @param qname name of variable
	 * @param exeuctionContext the current execution context
	 * @param fNameFound set to true if the name was found, false if not.
	 * @return pointer to the corresponding XObject
	 */
	const XObjectPtr
	getVariable(
			const XalanQName& 				qname,
			StylesheetExecutionContext&		executionContext,
			bool&							fNameFound)
	{
		return findXObject(qname, executionContext, false, true, fNameFound);
	}

	/**
	 * Push a named variable onto the processor variable stack. Don't forget
	 * to call startContext before pushing a series of arguments for a given
	 * template.
	 *
	 * @param name	  name of variable
	 * @param val	  pointer to ElemVariable
	 * @param e 	  element marker for variable
	 */
	void
	pushVariable(
			const XalanQName&			name,
			const ElemVariable*			var,
			const ElemTemplateElement*	e);

	/**
	 * Push a named variable onto the processor variable stack. Don't forget
	 * to call startContext before pushing a series of arguments for a given
	 * template.
	 *
	 * @param name	  name of variable
	 * @param val	  pointer to XObject value
	 * @param e 	  element marker for variable
	 */
	void
	pushVariable(
			const XalanQName&			name,
			const XObjectPtr&			val,
			const ElemTemplateElement*	e);

	/**
	 * Mark the top of the stack.
	 */
	void
	start();

	/**
	 * Reset all params in the current stack frame.
	 */
	void
	resetParams();

	/**
	 * Mark the top of the global stack frame.
	 */
	void
	markGlobalStackFrame();

	/**
	 * Clear the marking of the global stack frame.
	 */
	void
	unmarkGlobalStackFrame();

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

	/**
	 * Get the top of the global stack frame.
	 *
	 * @return current value of index
	 */
	int
	getGlobalStackFrameIndex() const
	{
		return m_globalStackFrameIndex;
	}

	class InvalidStackContextException : public XSLTProcessorException
	{
	public:

		InvalidStackContextException();

		virtual
		~InvalidStackContextException();

	private:

	};

	class PushParamFunctor
	{
	public:

		PushParamFunctor(VariablesStack&	theVariablesStack) :
			m_variablesStack(theVariablesStack)
		{
		}

		void
		operator()(const ParamsVectorType::value_type&	theEntry) const;

	private:

		VariablesStack&		m_variablesStack;
	};

	class XALAN_XSLT_EXPORT StackEntry
	{
	public:

		/**
		 * Enumeration for types of stack entries, one of context state, context
		 * marker, element marker, or argument.
		 */
		enum eType { eContextMarker,
				    eVariable,
					eParam,
					eActiveParam,
					eElementFrameMarker,
					eNextValue };

		/**
		 * Construct a context marker.
		 */
		explicit
		StackEntry();

		/**
		 * Construct a variable that is already evaluated.
		 */
		StackEntry(
			const XalanQName*	name,
			const XObjectPtr&	val,
			bool				isParam = false);

		/**
		 * Construct a variable that has not been evaluated yet.
		 */
		StackEntry(
			const XalanQName*		name,
			const ElemVariable*		var,
			bool					isParam = false);

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
		eType
		getType() const
		{
			return m_type;
		}

		/**
		 * Retrieve object name.  Valid only for variables
		 * 
		 * @return qualified name of object
		 */
		const XalanQName*
		getName() const
		{
			return m_qname;
		}

		/**
		 * Retrieve object's XObject pointer.  Valid only for variables
		 * 
		 * @return pointer to XObject
		 */
		const XObjectPtr&
		getValue() const
		{
			return m_value;
		}

		/**
		 * Retrieve object's XObject pointer.  Valid only for variables
		 * 
		 * @return pointer to XObject
		 */
		void
		setValue(const XObjectPtr&	theValue)
		{
			m_value = theValue;
		}

		/**
		 * Retrieve object's XObject pointer.  Valid only for variables
		 * 
		 * @return pointer to XObject
		 */
		const ElemVariable*
		getVariable() const
		{
			return m_variable;
		}

		void
		activate();

		void
		deactivate();

		/**
		 * Retrieve the ElemTemplateElem where frame begins.  Valid only for element frame markers
		 *
		 * @return ElemTemplateElement corresponding to marker
		 */
		const ElemTemplateElement*
		getElement() const
		{
			return m_element;
		}

		StackEntry&
		operator=(const StackEntry&		theRHS);

		bool
		operator==(const StackEntry&	theRHS) const;

	private:

		// Data members...
		eType						m_type;

		const XalanQName*			m_qname;

		XObjectPtr					m_value;

		const ElemVariable*			m_variable;

		const ElemTemplateElement*	m_element;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<StackEntry>			VariableStackStackType;
#else
	typedef std::vector<StackEntry>		VariableStackStackType;
#endif

	typedef VariableStackStackType::size_type	size_type;

	size_type
	getStackSize() const
	{
		return m_stack.size();
	}

	enum { eDefaultStackSize = 100 };

private:

	class CommitPushParams
	{
	public:

		CommitPushParams(VariablesStack&	theVariablesStack);

		~CommitPushParams();

		void
		commit()
		{
			m_variablesStack = 0;
		}

	private:

		VariablesStack*		m_variablesStack;

		size_type			m_stackSize;
	};

	friend class CommitPushParams;

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
	friend class PushParamFunctor;
	friend class SetAndRestoreForceGlobalSearch;

	const XObjectPtr
	findXObject(
			const XalanQName&				name,
			StylesheetExecutionContext&		executionContext,
			bool							fIsParam,
			bool							fSearchGlobalSpace,
			bool&							fNameFound);

	VariableStackStackType::size_type
	findEntry(
			const XalanQName&	name,
			bool				fIsParam,
			bool				fSearchGlobalSpace);


	VariableStackStackType		m_stack;

	int							m_globalStackFrameIndex;

	bool						m_globalStackFrameMarked;

	/**
	 * This is the top of the stack frame from where a search 
	 * for a variable or param should take place.  It may not 
	 * be the real stack top.
	 */
	unsigned int				m_currentStackFrameIndex;	

	/**
	 * This will be a stack for any variable definitions
	 * that are being evaluated dynamically, to protect
	 * against circular definitions.
	 */
	RecursionGuardStackType			m_guardStack;

	/**
	 * This will be a stack for tracking element frames.
	 * This is only used in debug builds.
	 */
	ElemTemplateElementStackType	m_elementFrameStack;
};



XALAN_CPP_NAMESPACE_END



#endif	// #if !defined(XALAN_VARIABLESSTACK_HEADER_GUARD)
