/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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



// Class header file.
#include "VariablesStack.hpp"



#include <algorithm>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "ElemVariable.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



VariablesStack::VariablesStack() :
	m_stack(),
	m_globalStackFrameIndex(-1),
	m_globalStackFrameMarked(false),
	m_currentStackFrameIndex(0),
	m_guardStack(),
	m_elementFrameStack()
{
	m_stack.reserve(eDefaultStackSize);
}



VariablesStack::~VariablesStack()
{
}



void
VariablesStack::reset()
{
	while(m_stack.empty() == false)
	{
		pop();
	}

	m_stack.clear();
	m_guardStack.clear();
	m_elementFrameStack.clear();

	m_globalStackFrameMarked = false;
	m_globalStackFrameIndex = -1;
}



bool
VariablesStack::elementFrameAlreadyPushed(const ElemTemplateElement*	elem) const
{
	const VariableStackStackType::size_type		nElems = m_stack.size();
	assert(nElems > 0);

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(VariableStackStackType::size_type i = nElems - 1; i > 0; --i)
	{
		const StackEntry&	theEntry = m_stack[i];

		if(theEntry.getType() == StackEntry::eElementFrameMarker)
		{
			if(theEntry.getElement() == elem)
			{
				return true;
			}
		}
	}

	return false;
}



void
VariablesStack::pushContextMarker()
{
	push(StackEntry());
}



void
VariablesStack::popContextMarker()
{
	VariableStackStackType::iterator	i = m_stack.end();

	for(; ;)
	{
		assert(i != m_stack.begin());

		const StackEntry&			theEntry = *--i;
		assert(theEntry == back());

		const StackEntry::eType		type = theEntry.getType();
		assert(type < StackEntry::eNextValue && type >= 0);

		pop();

		if (type == StackEntry::eContextMarker)
		{
			break;
		}
	}

	m_currentStackFrameIndex = m_stack.size();
}



void
VariablesStack::push(const StackEntry&	theEntry)
{
	assert(theEntry.getType() < StackEntry::eNextValue && theEntry.getType() >= 0);

	if(m_currentStackFrameIndex == m_stack.size())
	{
		++m_currentStackFrameIndex;
	}

	m_stack.push_back(theEntry);

	// Increment the global stack frame index as long as we're pushing variables, and
	// it already hasn't been marked.  This is a temporary work-around for problems
	// with evaluating top-level variables as they're pushed, rather than as they're
	// used.
	if (m_globalStackFrameMarked == false && theEntry.getType() == StackEntry::eVariable)
	{
		m_globalStackFrameIndex = m_currentStackFrameIndex;
	}
}



void
VariablesStack::pop()
{
	assert(m_stack.empty() == false);

	if(m_currentStackFrameIndex == m_stack.size())
	{
		--m_currentStackFrameIndex;
	}

	m_stack.pop_back();
}



void
VariablesStack::PushParamFunctor::operator()(const VariablesStack::ParamsVectorType::value_type&	theEntry) const
{
	assert(theEntry.m_qname != 0);

	if (theEntry.m_value.null() == false)
	{
		m_variablesStack.push(VariablesStack::StackEntry(theEntry.m_qname, theEntry.m_value, true));
	}
	else
	{
		assert(theEntry.m_variable != 0);

		m_variablesStack.push(VariablesStack::StackEntry(theEntry.m_qname, theEntry.m_variable, true));
	}
}



VariablesStack::CommitPushParams::CommitPushParams(VariablesStack&	theVariablesStack) :
	m_variablesStack(&theVariablesStack),
	m_stackSize(theVariablesStack.getStackSize())
{
}



VariablesStack::CommitPushParams::~CommitPushParams()
{
	if (m_variablesStack != 0)
	{
		while(m_variablesStack->getStackSize() > m_stackSize)
		{
			m_variablesStack->pop();
		}
	}
}



void
VariablesStack::pushParams(const ParamsVectorType&	theParams)
{
	// This object will push the params and pop them
	// if we don't call it's commit() member function.  So
	// if an exception is thrown while transferring the
	// parameters, the stack stays in a consistent state.
	CommitPushParams	thePusher(*this);

	XALAN_USING_STD(for_each)

	for_each(theParams.begin(), theParams.end(), PushParamFunctor(*this));

	thePusher.commit();
}



void
VariablesStack::pushVariable(
			const XalanQName&			name,
			const ElemVariable*			var,
			const ElemTemplateElement*	e)
{
	if(elementFrameAlreadyPushed(e) == false)
	{
		pushElementFrame(e);
	}

	push(StackEntry(&name, var));
}



void
VariablesStack::pushVariable(
			const XalanQName&			name,
			const XObjectPtr&			val,
			const ElemTemplateElement*	e)
{
	if(elementFrameAlreadyPushed(e) == false)
	{
		throw InvalidStackContextException();
	}

	push(StackEntry(&name, val));
}



void
VariablesStack::start()
{
}



void
VariablesStack::resetParams()
{
	const unsigned int	nElems = getCurrentStackFrameIndex();

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(unsigned int i = nElems - 1; i > 0; --i)
	{
		StackEntry&		theEntry = m_stack[i];

		if(theEntry.getType() == StackEntry::eContextMarker)
		{
			break;
		}
		else
		{
			theEntry.deactivate();
		}
	}
}



void
VariablesStack::markGlobalStackFrame()
{
	m_globalStackFrameIndex = m_stack.size();

	m_globalStackFrameMarked = true;

	pushContextMarker();
}



void
VariablesStack::unmarkGlobalStackFrame()
{
	popContextMarker();

	m_globalStackFrameIndex = -1;

	m_globalStackFrameMarked = false;
}



const XObjectPtr
VariablesStack::findXObject(
			const XalanQName&				name,
			StylesheetExecutionContext&		executionContext,
			bool							fIsParam,
			bool							fSearchGlobalSpace,
			bool&							fNameFound)
{
	typedef VariableStackStackType::size_type	size_type;

	// findEntry() returns an index into the stack.  We should
	// _never_ take the address of anything in the stack, since
	// the address could change at unexpected times.
	const size_type		theEntryIndex =
		findEntry(name, fIsParam, fSearchGlobalSpace);

	if (theEntryIndex == m_stack.size())
	{
		fNameFound = false;

		return XObjectPtr();
	}
	else
	{
		assert(theEntryIndex < m_stack.size());

		fNameFound = true;

		assert(m_stack[theEntryIndex].getType() == StackEntry::eVariable ||
			   m_stack[theEntryIndex].getType() == StackEntry::eParam ||
			   m_stack[theEntryIndex].getType() == StackEntry::eActiveParam);

		const XObjectPtr&	theValue = m_stack[theEntryIndex].getValue();

		if (theValue.null() == false)
		{
			return theValue;
		}
		else
		{
			const ElemVariable* const	var = m_stack[theEntryIndex].getVariable();

			XObjectPtr					theNewValue;

			if (var != 0)
			{
				XalanNode* const	doc = executionContext.getRootDocument();
				assert(doc != 0);

				XALAN_USING_STD(find)

				// See if the ElemVariable instance is already being evaluated...
				if (find(m_guardStack.begin(), m_guardStack.end(), var) != m_guardStack.end())
				{
					executionContext.error(
						XalanMessageLoader::getMessage(XalanMessages::CircularVariableDefWasDetected),
						doc,
						var->getLocator());
				}

				m_guardStack.push_back(var);

				// We need to set up a stack frame for the variable's execution...
				typedef StylesheetExecutionContext::PushAndPopContextMarker	PushAndPopContextMarker;

				const PushAndPopContextMarker	theContextMarkerPushPop(executionContext);

				theNewValue = var->getValue(executionContext, doc);
				assert(theNewValue.null() == false);

				assert(m_guardStack.empty() == false);

				m_guardStack.pop_back();

				m_stack[theEntryIndex].setValue(theNewValue);
				m_stack[theEntryIndex].activate();
			}

			return theNewValue;
		}
	}
}



VariablesStack::VariableStackStackType::size_type
VariablesStack::findEntry(
			const XalanQName&	qname,
			bool				fIsParam,
			bool				fSearchGlobalSpace)
{
	typedef VariableStackStackType::size_type	size_type;

	size_type	theEntryIndex = m_stack.size();

	const unsigned int	nElems = getCurrentStackFrameIndex();

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(unsigned int i = nElems - 1; i > 0; --i)
	{
		StackEntry&					theEntry = m_stack[i];

		const StackEntry::eType		theType = theEntry.getType();

		if(theType == StackEntry::eVariable ||
		   theType == StackEntry::eActiveParam)
		{
			assert(theEntry.getName() != 0);

			if(theEntry.getName()->equals(qname))
			{
				theEntryIndex = size_type(i);

				break;
			}
		}
		else if (theType == StackEntry::eParam)
		{
			if (fIsParam == true)
			{
				if(theEntry.getName()->equals(qname))
				{
					theEntry.activate();

					theEntryIndex = size_type(i);

					break;
				}
			}
		}
		else if(theType == StackEntry::eContextMarker)
		{
			break;
		}
	}

	if(theEntryIndex == m_stack.size() && fIsParam == false && true == fSearchGlobalSpace && m_globalStackFrameIndex > 1)
	{
		// Look in the global space
		for(unsigned int i = m_globalStackFrameIndex - 1; i > 0; i--)
		{
			StackEntry&		theEntry = m_stack[i];

			const StackEntry::eType		theType = theEntry.getType();

			if(theType == StackEntry::eVariable)
			{
				assert(theEntry.getName() != 0);

				if(theEntry.getName()->equals(qname))
				{
					theEntryIndex = size_type(i);

					break;
				}
			}
			else if(theType == StackEntry::eContextMarker)
			{
				break;
			}
		}
	}

	return theEntryIndex;
}



void
VariablesStack::pushElementFrame(const ElemTemplateElement*	elem)
{
	push(StackEntry(elem));

#if !defined(NDEBUG)
	m_elementFrameStack.push_back(elem);
#endif
}



class EnsurePop
{
public:

	EnsurePop(VariablesStack&	theVariablesStack) :
		m_variablesStack(theVariablesStack)
	{
	}

	~EnsurePop()
	{
		m_variablesStack.pop();
	}

private:

	VariablesStack&		m_variablesStack;
};



void
VariablesStack::popElementFrame()
{
	const VariableStackStackType::size_type		nElems = m_stack.size();
	assert(nElems > 0);

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(VariableStackStackType::size_type i = nElems - 1; i > 0; --i)
	{
		const StackEntry&	theEntry = m_stack[i];

		// Guarantee that it will be popped when we're done.
		const EnsurePop		theEnsurePop(*this);

		if(theEntry.getType() == StackEntry::eContextMarker)
		{
			throw InvalidStackContextException();
		}
		else if (theEntry.getType() == StackEntry::eElementFrameMarker)
		{
#if !defined(NDEBUG)
			const ElemTemplateElement* const	theElement =
				theEntry.getElement();

			if (m_elementFrameStack.empty() == true)
			{
				throw InvalidStackContextException();
			}

			const ElemTemplateElement* const	theStackBack =
				m_elementFrameStack.back();

			m_elementFrameStack.pop_back();

			if (theElement != theStackBack)
			{
				throw InvalidStackContextException();
			}
#endif

			break;
		}
    }
}



VariablesStack::StackEntry::StackEntry() :
	m_type(eContextMarker),
	m_qname(0),
	m_value(),
	m_variable(0),
	m_element(0)
{
}



VariablesStack::StackEntry::StackEntry(
			const XalanQName*	name,
			const XObjectPtr&	val,
			bool				isParam) :
	m_type(isParam == true ? eParam : eVariable),
	m_qname(name),
	m_value(val),
	m_variable(0),
	m_element(0)
{
}



VariablesStack::StackEntry::StackEntry(
			const XalanQName*		name,
			const ElemVariable*		var,
			bool					isParam) :
	m_type(isParam == true ? eParam : eVariable),
	m_qname(name),
	m_value(),
	m_variable(var),
	m_element(0)
{
}



VariablesStack::StackEntry::StackEntry(const ElemTemplateElement*	elem) :
	m_type(eElementFrameMarker),
	m_qname(0),
	m_value(),
	m_variable(0),
	m_element(elem)
{
}



VariablesStack::StackEntry::StackEntry(const StackEntry&	theSource) :
	m_type(theSource.m_type),
	m_qname(theSource.m_qname),
	m_value(theSource.m_value),
	m_variable(theSource.m_variable),
	m_element(theSource.m_element)
{
}



VariablesStack::StackEntry::~StackEntry()
{
#if !defined(NDEBUG)
	m_qname = (const XalanQName*) 0xDEADBEEF;

	m_variable = (const ElemVariable*) 0xDEADBEEF;

	m_element = (const ElemTemplateElement*) 0xDEADBEEF;
#endif
}



VariablesStack::StackEntry&
VariablesStack::StackEntry::operator=(const StackEntry&		theRHS)
{
	if (this != &theRHS)
	{
		m_type = theRHS.m_type;

		m_qname = theRHS.m_qname;

		m_value = theRHS.m_value;

		m_variable = theRHS.m_variable;

		m_element = theRHS.m_element;
	}

	return *this;
}



// Equality for StackEntry instances is probably bogus,
// so it might be worthwhile to just get rid of this.
bool
VariablesStack::StackEntry::operator==(const StackEntry&	theRHS) const
{
	bool	fResult = false;

	if (m_type == theRHS.m_type)
	{
		if (m_type == eContextMarker)
		{
			if (&theRHS == this)
			{
				fResult = true;
			}
		}
		else if (m_type == eVariable || m_type == eParam || m_type == eActiveParam)
		{
			// We only need to compare the variable related members...
			if (m_value == theRHS.m_value ||
				m_variable == theRHS.m_variable)
			{
				fResult = true;
			}
		}
		else if (m_type == eElementFrameMarker)
		{
			if (m_element == theRHS.m_element)
			{
				fResult = true;
			}
		}
		else
		{
			assert(0);
		}
	}

	return fResult;
}



void
VariablesStack::StackEntry::activate()
{
	if (m_type == eParam)
	{
		m_type = eActiveParam;
	}
}



void
VariablesStack::StackEntry::deactivate()
{
	if (m_type == eActiveParam)
	{
		m_type = eParam;
	}
}



VariablesStack::InvalidStackContextException::InvalidStackContextException() :
	XSLTProcessorException(XalanMessageLoader::getMessage(XalanMessages::InvalidStackContext),
						   TranscodeFromLocalCodePage("InvalidStackContextException"))
{
}



VariablesStack::InvalidStackContextException::~InvalidStackContextException()
{
}



XALAN_CPP_NAMESPACE_END
