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
#if !defined(XALAN_TraceListener_HEADER_GUARD)
#define XALAN_TraceListener_HEADER_GUARD 

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class TracerEvent;
class SelectionEvent;
class GenerateEvent;



class XALAN_XSLT_EXPORT TraceListener
{
public:

	TraceListener();

	virtual
	~TraceListener();

	/**
	 * Method that is called when a trace event occurs. The method is blocking.
	 * It must return before processing continues.
	 *
	 * @param ev trace event
	 */
	virtual void
	trace(const TracerEvent&	ev) = 0;

	/**
	 * Method that is called just after the formatter listener is called.
	 *
	 * @param ev generate event
	 */
	virtual void
	selected(const SelectionEvent&	ev) = 0;

	/**
	 * Method that is called just after the formatter listener is called.
	 *
	 * @param ev generate event
	 */
	virtual void
	generated(const GenerateEvent&	ev) = 0;

	struct TraceListenerTraceFunctor
	{
		TraceListenerTraceFunctor(const TracerEvent&	theEvent) :
			m_event(theEvent)
		{
		}

		void
		operator()(TraceListener*	theListener) const
		{
			theListener->trace(m_event);
		}

		const TracerEvent&	m_event;
	};

	struct TraceListenerSelectFunctor
	{
		TraceListenerSelectFunctor(const SelectionEvent&		theEvent) :
			m_event(theEvent)
		{
		}

		void
		operator()(TraceListener*	theListener) const
		{
			theListener->selected(m_event);
		}

		const SelectionEvent&	m_event;
	};

	struct TraceListenerGenerateFunctor
	{
		TraceListenerGenerateFunctor(const GenerateEvent&	theEvent) :
			m_event(theEvent)
		{
		}

		void
		operator()(TraceListener*	theListener) const
		{
			theListener->generated(m_event);
		}

		const GenerateEvent&	m_event;
	};

};



XALAN_CPP_NAMESPACE_END



#endif //XALAN_TraceListener_HEADER_GUARD 
