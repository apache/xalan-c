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



// Class header file.
#include "VariablesStack.hpp"



#include <algorithm>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "ElemVariable.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



VariablesStack::VariablesStack(MemoryManager& theManager) :
    m_stack(theManager),
    m_globalStackFrameIndex(~0u),
    m_globalStackFrameMarked(false),
    m_currentStackFrameIndex(0),
    m_guardStack(theManager),
    m_elementFrameStack(theManager)
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
    m_globalStackFrameIndex = ~0u;
}



bool
VariablesStack::elementFrameAlreadyPushed(const ElemTemplateElement*    elem) const
{
    const VariableStackStackType::size_type     nElems = m_stack.size();
    assert(nElems > 0);

    // There is guaranteed to be a context marker at
    // the bottom of the stack, so i should stop at
    // 1.
    for(VariableStackStackType::size_type i = nElems - 1; i > 0; --i)
    {
        const StackEntry&   theEntry = m_stack[i];

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
    VariableStackStackType::iterator    i = m_stack.end();

    for(; ;)
    {
        assert(i != m_stack.begin());

        const StackEntry&           theEntry = *--i;
        assert(theEntry == back());

        const StackEntry::eType     type = theEntry.getType();
        assert(type < StackEntry::eNextValue && type >= 0);

        pop();

        if (type == StackEntry::eContextMarker)
        {
            break;
        }
    }

    // This introduced a regression into 1.7, because of all the
    // external fiddling with the stack frame index.  We need to
    // re-write that code so no external meddling is necessary,
    // so we can track the stack frame index properly.  In particular,
    // this is an issue with xsl:apply-templates, because a context
    // marker is pushed before its select attribute is evaluated, even
    // though the select expression really should be evaluated in
    // the previous stack context.
//  m_currentStackFrameIndex = size_type(m_stack.size());
}



void
VariablesStack::push(const StackEntry&  theEntry)
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
VariablesStack::PushParamFunctor::operator()(const VariablesStack::ParamsVectorType::value_type&    theEntry) const
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



VariablesStack::CommitPushParams::CommitPushParams(VariablesStack&  theVariablesStack) :
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
VariablesStack::pushParams(const ParamsVectorType&  theParams)
{
    // This object will push the params and pop them
    // if we don't call it's commit() member function.  So
    // if an exception is thrown while transferring the
    // parameters, the stack stays in a consistent state.
    CommitPushParams    thePusher(*this);

    XALAN_USING_STD(for_each)

    for_each(theParams.begin(), theParams.end(), PushParamFunctor(*this));

    thePusher.commit();
}



void
VariablesStack::pushVariable(
            const XalanQName&           name,
            const ElemVariable*         var,
            const ElemTemplateElement*  e)
{
    if(elementFrameAlreadyPushed(e) == false)
    {
        pushElementFrame(e);
    }

    push(StackEntry(&name, var));
}



void
VariablesStack::pushVariable(
            const XalanQName&           name,
            const XObjectPtr&           val,
            const ElemTemplateElement*  e)
{
    if(elementFrameAlreadyPushed(e) == false)
    {
        XalanDOMString theBuffer(m_stack.getMemoryManager());

        throw InvalidStackContextException(theBuffer);
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
    const size_type     nElems = getCurrentStackFrameIndex();
    assert(nElems > 0);

    // There is guaranteed to be a context marker at
    // the bottom of the stack, so i should stop at
    // 1.
    for(size_type i = nElems - 1; i > 0; --i)
    {
        StackEntry&     theEntry = m_stack[i];

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
    m_globalStackFrameIndex = size_type(m_stack.size());

    m_globalStackFrameMarked = true;

    pushContextMarker();
}



void
VariablesStack::unmarkGlobalStackFrame()
{
    popContextMarker();

    m_globalStackFrameIndex = ~0u;

    m_globalStackFrameMarked = false;
}



const XObjectPtr
VariablesStack::findXObject(
            const XalanQName&               name,
            StylesheetExecutionContext&     executionContext,
            bool                            fIsParam,
            bool                            fSearchGlobalSpace,
            bool&                           fNameFound)
{
    typedef VariableStackStackType::size_type   size_type;

    // findEntry() returns an index into the stack.  We should
    // _never_ take the address of anything in the stack, since
    // the address could change at unexpected times.
    const size_type     theEntryIndex =
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

        const XObjectPtr&   theValue = m_stack[theEntryIndex].getValue();

        if (theValue.null() == false)
        {
            return theValue;
        }
        else
        {
            const ElemVariable* const   var = m_stack[theEntryIndex].getVariable();

            XObjectPtr                  theNewValue;

            if (var != 0)
            {
                XalanNode* const    doc = executionContext.getRootDocument();
                assert(doc != 0);

                XALAN_USING_STD(find)

                // See if the ElemVariable instance is already being evaluated...
                if (find(m_guardStack.begin(), m_guardStack.end(), var) != m_guardStack.end())
                {
                    const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

                    executionContext.problem(
                        StylesheetExecutionContext::eXSLTProcessor,
                        StylesheetExecutionContext::eError,
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::CircularVariableDefWasDetected),
                        var->getLocator(),
                        doc);
                }

                m_guardStack.push_back(var);

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
                executionContext.pushContextMarker();
#else
                // We need to set up a stack frame for the variable's execution...
                typedef StylesheetExecutionContext::PushAndPopContextMarker PushAndPopContextMarker;

                const PushAndPopContextMarker   theContextMarkerPushPop(executionContext);
#endif

                theNewValue = var->getValue(executionContext, doc);
                assert(theNewValue.null() == false);

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
                executionContext.popContextMarker();
#endif

                assert(m_guardStack.empty() == false);

                m_guardStack.pop_back();

                m_stack[theEntryIndex].setValue(theNewValue);
                m_stack[theEntryIndex].activate();
            }

            return theNewValue;
        }
    }
}



VariablesStack::size_type
VariablesStack::findEntry(
            const XalanQName&   qname,
            bool                fIsParam,
            bool                fSearchGlobalSpace)
{
    size_type   theEntryIndex = size_type(m_stack.size());

    const size_type     nElems = getCurrentStackFrameIndex();

    // There is guaranteed to be a context marker at
    // the bottom of the stack, so i should stop at
    // 1.
    for(size_type i = nElems - 1; i > 0; --i)
    {
        StackEntry&                 theEntry = m_stack[i];

        const StackEntry::eType     theType = theEntry.getType();

        if(theType == StackEntry::eVariable ||
           theType == StackEntry::eActiveParam)
        {
            assert(theEntry.getName() != 0);

            if(theEntry.getName()->equals(qname))
            {
                theEntryIndex = i;

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

                    theEntryIndex = i;

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
        for(size_type i = m_globalStackFrameIndex - 1; i > 0; i--)
        {
            StackEntry&     theEntry = m_stack[i];

            const StackEntry::eType     theType = theEntry.getType();

            if(theType == StackEntry::eVariable)
            {
                assert(theEntry.getName() != 0);

                if(theEntry.getName()->equals(qname))
                {
                    theEntryIndex = i;

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
VariablesStack::pushElementFrame(const ElemTemplateElement* elem)
{
    push(StackEntry(elem));

#if !defined(NDEBUG)
    m_elementFrameStack.push_back(elem);
#endif
}



class EnsurePop
{
public:

    EnsurePop(VariablesStack&   theVariablesStack) :
        m_variablesStack(theVariablesStack)
    {
    }

    ~EnsurePop()
    {
        m_variablesStack.pop();
    }

private:

    VariablesStack&     m_variablesStack;
};



void
VariablesStack::popElementFrame()
{
    const VariableStackStackType::size_type     nElems = m_stack.size();
    assert(nElems > 0);

    // There is guaranteed to be a context marker at
    // the bottom of the stack, so i should stop at
    // 1.
    for(VariableStackStackType::size_type i = nElems - 1; i > 0; --i)
    {
        const StackEntry&   theEntry = m_stack[i];

        // Guarantee that it will be popped when we're done.
        const EnsurePop     theEnsurePop(*this);

        if(theEntry.getType() == StackEntry::eContextMarker)
        {
            XalanDOMString theBuffer(m_stack.getMemoryManager());

            throw InvalidStackContextException(theBuffer);
        }
        else if (theEntry.getType() == StackEntry::eElementFrameMarker)
        {
#if !defined(NDEBUG)
            const ElemTemplateElement* const    theElement =
                theEntry.getElement();

            if (m_elementFrameStack.empty() == true)
            {
                XalanDOMString theBuffer(m_stack.getMemoryManager());

                throw InvalidStackContextException(theBuffer);
            }

            const ElemTemplateElement* const    theStackBack =
                m_elementFrameStack.back();

            m_elementFrameStack.pop_back();

            if (theElement != theStackBack)
            {
                XalanDOMString theBuffer(m_stack.getMemoryManager());

                throw InvalidStackContextException(theBuffer);
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
            const XalanQName*   name,
            const XObjectPtr&   val,
            bool                isParam) :
    m_type(isParam == true ? eParam : eVariable),
    m_qname(name),
    m_value(val),
    m_variable(0),
    m_element(0)
{
}



VariablesStack::StackEntry::StackEntry(
            const XalanQName*       name,
            const ElemVariable*     var,
            bool                    isParam) :
    m_type(isParam == true ? eParam : eVariable),
    m_qname(name),
    m_value(),
    m_variable(var),
    m_element(0)
{
}



VariablesStack::StackEntry::StackEntry(const ElemTemplateElement*   elem) :
    m_type(eElementFrameMarker),
    m_qname(0),
    m_value(),
    m_variable(0),
    m_element(elem)
{
}



VariablesStack::StackEntry::StackEntry(const StackEntry&    theSource) :
    m_type(theSource.m_type),
    m_qname(theSource.m_qname),
    m_value(theSource.m_value),
    m_variable(theSource.m_variable),
    m_element(theSource.m_element)
{
}



VariablesStack::StackEntry::~StackEntry()
{
}



VariablesStack::StackEntry&
VariablesStack::StackEntry::operator=(const StackEntry&     theRHS)
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
VariablesStack::StackEntry::operator==(const StackEntry&    theRHS) const
{
    bool    fResult = false;

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


const XalanDOMChar  VariablesStack::InvalidStackContextException::m_type[] = 
{   
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_k,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



VariablesStack::InvalidStackContextException::InvalidStackContextException(XalanDOMString& theResult) :
    XSLTProcessorException(
        theResult.getMemoryManager(),
        XalanMessageLoader::getMessage(
            theResult,
            XalanMessages::InvalidStackContext))
{
}



VariablesStack::InvalidStackContextException::~InvalidStackContextException()
{
}



XALAN_CPP_NAMESPACE_END
