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



VariablesStack::VariablesStack() :
	m_stack(),
	m_globalStackFrameIndex(-1),
	m_currentStackFrameIndex(0)
{
	m_stack.reserve(eDefaultStackSize);
}



VariablesStack::~VariablesStack()
{
}



void
VariablesStack::reset()
{
	while(m_stack.size() > 0)
	{
		pop();
	}

	m_stack.clear();
}



bool
VariablesStack::elementFrameAlreadyPushed(const ElemTemplateElement*	elem) const
{
	const unsigned int	nElems = m_stack.size();

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(unsigned int i = nElems - 1; i > 0; --i)
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
	const int	nElems = m_stack.size();

	for(int i = (nElems - 1); i >= 0 && m_stack.empty() == false; --i)
	{
		const StackEntry&	theEntry = m_stack[i];
		assert(theEntry == back());

		const StackEntry::eStackEntryType	type = theEntry.getType();
		assert(type < StackEntry::eNextValue && type >= 0);

		pop();

		if (type == StackEntry::eContextMarker)
		{
			break;
		}
	}
}



class PopPushStackEntry
{
public:

	PopPushStackEntry(VariablesStack&	theVariablesStack) :
		m_variablesStack(theVariablesStack),
		m_stackEntry(theVariablesStack.back())
	{
		assert(m_stackEntry.getType() == VariablesStack::StackEntry::eContextMarker);

		theVariablesStack.pop();
	}

	~PopPushStackEntry()
	{
		m_variablesStack.push(m_stackEntry);
	}

private:

	VariablesStack&						m_variablesStack;

	const VariablesStack::StackEntry	m_stackEntry;
};



class CommitPushElementFrame
{
public:

	CommitPushElementFrame(
			VariablesStack&								theVariableStack,
			const ElemTemplateElement*					targetTemplate) :
		m_variableStack(&theVariableStack),
		m_targetTemplate(targetTemplate)
	{
		theVariableStack.pushElementFrame(targetTemplate);
	}

	~CommitPushElementFrame()
	{
		if (m_variableStack != 0)
		{
			m_variableStack->popElementFrame(m_targetTemplate);
		}
	}

	void
	commit()
	{
		m_variableStack = 0;
	}

private:

	VariablesStack*								m_variableStack;

	const ElemTemplateElement* const			m_targetTemplate;
};



void
VariablesStack::push(const StackEntry&	theEntry)
{
	assert(theEntry.getType() < StackEntry::eNextValue && theEntry.getType() >= 0);

	if(m_currentStackFrameIndex == m_stack.size())
	{
		++m_currentStackFrameIndex;
	}

	m_stack.push_back(theEntry);
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



const void
VariablesStack::PushParamFunctor::operator()(const VariablesStack::ParamsVectorType::value_type&	theEntry)
{
	assert(theEntry.first != 0);

	m_variablesStack.push(VariablesStack::StackEntry(theEntry.first, theEntry.second));
}



void
VariablesStack::pushParams(
			const ParamsVectorType&		theParams,
			const ElemTemplateElement*	targetTemplate)
{
	// This object will push an element marker, and pop it
	// if we don't call it's commit() member function.  So
	// if an exception is thrown while transferring the
	// parameters, the element marker will be popped.
	// This keeps the stack in a consistent state.
	// It will also delete things left in the temp stack
	// as well.
	CommitPushElementFrame		thePusher(*this,
										  targetTemplate);

#if !defined (XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(theParams.begin(), theParams.end(), PushParamFunctor(*this));

	thePusher.commit();
}



void
VariablesStack::pushVariable(
			const QName&				name,
			const XObject*				val,
			const ElemTemplateElement*	e)
{
	if(elementFrameAlreadyPushed(e) == false)
	{
		pushElementFrame(e);
	}

	push(StackEntry(&name, val));
}



void
VariablesStack::start()
{
}



void
VariablesStack::markGlobalStackFrame()
{
	m_globalStackFrameIndex = m_stack.size();

	pushContextMarker();
}



const XObject*
VariablesStack::findXObject(
			const QName&	name,
			bool			fSearchGlobalSpace) const
{
	const XObject*		theXObject = 0;

	const StackEntry*	theVariable = findVariable(name, fSearchGlobalSpace);

	if (theVariable != 0)
	{
		assert(theVariable->getType() == StackEntry::eVariable);

		theXObject = theVariable->getVariable();
	}

	return theXObject;
}



const VariablesStack::StackEntry*
VariablesStack::findVariable(
			const QName&	qname,
			bool			fSearchGlobalSpace) const
{
	const StackEntry*	theResult = 0;

	const unsigned int	nElems = getCurrentStackFrameIndex();

	// There is guaranteed to be a context marker at
	// the bottom of the stack, so i should stop at
	// 1.
	for(unsigned int i = nElems - 1; i > 0; --i)
	{
		const StackEntry&	theEntry =
			m_stack[i];

		if(theEntry.getType() == StackEntry::eVariable)
		{
			assert(theEntry.getName() != 0);

			if(theEntry.getName()->equals(qname))
			{
				theResult = &theEntry;

				break;
			}
		}
		else if(theEntry.getType() == StackEntry::eContextMarker)
		{
			break;
		}
	}

	if(0 == theResult && true == fSearchGlobalSpace)
	{
		// Look in the global space
		for(unsigned int i = m_globalStackFrameIndex - 1; i > 0; i--)
		{
			const StackEntry&	theEntry = m_stack[i];

			if(theEntry.getType() == StackEntry::eVariable)
			{
				assert(theEntry.getName() != 0);

				if(theEntry.getName()->equals(qname))
				{
					theResult = &theEntry;

					break;
				}
			}
			else if(theEntry.getType() == StackEntry::eContextMarker)
			{
				break;
			}
		}
	}

	return theResult;
}



void
VariablesStack::pushElementFrame(const ElemTemplateElement*	elem)
{
	push(StackEntry(elem));
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
VariablesStack::popElementFrame(const ElemTemplateElement*	elem)
{
	const unsigned int	nElems = getCurrentStackFrameIndex();

	// Sub 1 extra for the context marker.
	for(unsigned int i = nElems - 1; i > 0; --i)
	{
		const StackEntry&	theEntry = m_stack[i];

		// Guarantee that it will be popped when we're done.
		EnsurePop	theEnsurePop(*this);

		if(theEntry.getType() == StackEntry::eContextMarker)
		{
			throw InvalidStackContextException();
		}
		else if (theEntry.getType() == StackEntry::eElementFrameMarker)
		{
			const ElemTemplateElement* const	theElement =
				theEntry.getElement();

			if (theElement != elem)
			{
				throw InvalidStackContextException();
			}

			break;
		}
    }
}



VariablesStack::StackEntry::StackEntry() :
	m_type(eContextMarker)
{
}



VariablesStack::StackEntry::StackEntry(
		const QName*	name,
		const XObject*	val) :
	m_type(eVariable)
{
	variable.m_qname = name;
	variable.m_value = val;
}



VariablesStack::StackEntry::StackEntry(const ElemTemplateElement*	elem) :
	m_type(eElementFrameMarker)
{
	elementMarker.m_element = elem;
}



VariablesStack::StackEntry::StackEntry(const StackEntry&	theSource) :
	m_type(theSource.m_type)
{
	// Use operator=() to do the work...
	*this = theSource;
}



VariablesStack::StackEntry::~StackEntry()
{
}



VariablesStack::StackEntry&
VariablesStack::StackEntry::operator=(const StackEntry&		theRHS)
{
	m_type = theRHS.m_type;

	if (m_type == eVariable)
	{
		variable.m_qname = theRHS.variable.m_qname;

		variable.m_value = theRHS.variable.m_value;
	}
	else if (m_type == eElementFrameMarker)
	{
		elementMarker.m_element = theRHS.elementMarker.m_element;
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
		else if (m_type == eVariable)
		{
			// We only need to compare the variable pointer..
			if (variable.m_value == theRHS.variable.m_value)
			{
				fResult = true;
			}
		}
		else if (m_type == eElementFrameMarker)
		{
			if (elementMarker.m_element == theRHS.elementMarker.m_element)
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



VariablesStack::InvalidStackContextException::InvalidStackContextException() :
	XSLTProcessorException(TranscodeFromLocalCodePage("Invalid stack context"),
						   TranscodeFromLocalCodePage("InvalidStackContextException"))
{
}



VariablesStack::InvalidStackContextException::~InvalidStackContextException()
{
}
