/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(XALAN_VARIABLESSTACK_HEADER_GUARD)
#define XALAN_VARIABLESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <cassert>



#include <xalanc/Include/XalanVector.hpp>



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

    typedef unsigned long   size_type;

	/**
	 * Constructor for a variable stack.
	 */
	explicit
	VariablesStack(MemoryManagerType& theManager);

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

	typedef XalanVector<ParamsVectorEntry>			ParamsVectorType;
	typedef XalanVector<const ElemVariable*>			RecursionGuardStackType;
	typedef XalanVector<const ElemTemplateElement*>	ElemTemplateElementStackType;

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
	setCurrentStackFrameIndex(size_type     currentStackFrameIndex = ~0u)
	{
		if (currentStackFrameIndex == ~0u)
        {
            assert(size_type(m_stack.size()) == m_stack.size());

			m_currentStackFrameIndex = size_type(m_stack.size());
        }
		else
        {
			m_currentStackFrameIndex = currentStackFrameIndex;
        }
	}

	/**
	 * Get the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @return current value of index
	 */
	size_type
	getCurrentStackFrameIndex() const
	{
		return m_currentStackFrameIndex;
	}

	/**
	 * Get the top of the global stack frame.
	 *
	 * @return current value of index
	 */
	size_type
	getGlobalStackFrameIndex() const
	{
		return m_globalStackFrameIndex;
	}

	class InvalidStackContextException : public XSLTProcessorException
	{
	public:

		InvalidStackContextException(XalanDOMString& theResult);

		virtual
		~InvalidStackContextException();


		virtual const XalanDOMChar*
			getType() const
		{
			return m_type;
		}
		
	private:
		
		static const XalanDOMChar	m_type[];

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

	typedef XalanVector<StackEntry>			VariableStackStackType;

	size_type
	getStackSize() const
	{
		return size_type(m_stack.size());
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

	size_type
	findEntry(
			const XalanQName&	name,
			bool				fIsParam,
			bool				fSearchGlobalSpace);


	VariableStackStackType		m_stack;

	size_type				    m_globalStackFrameIndex;

	bool						m_globalStackFrameMarked;

	/**
	 * This is the top of the stack frame from where a search 
	 * for a variable or param should take place.  It may not 
	 * be the real stack top.
	 */
	size_type				    m_currentStackFrameIndex;	

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
