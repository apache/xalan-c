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
#if !defined(PRINTWRITER_HEADER_GUARD_1357924680)
#define PRINTWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/Writer.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT PrintWriter : public Writer
{
public:

	/**
	 * Constructor
	 * 
	 * @param fAutoFlush if true, the output will not be buffered
	 */
	PrintWriter( bool	fAutoFlush = false, MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

    MemoryManagerType& 
    getMemoryManager()
    {
        return m_memoryManager;
    }

	virtual
	~PrintWriter();

	/**
	 * Flush the stream, then check the error status.
	 * 
	 * @return true if no errors
	 */
	virtual bool
    checkError() const = 0;

	// Functions inherited from Writer...

	virtual void
	close() = 0;

	virtual void
	flush() = 0;


	// Output functions inherited from Writer...

	virtual void
	write(
			const char*		s,
			size_t			theOffset = 0,
			size_t			theLength = npos) = 0;

	virtual void
	write(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	virtual void
	write(XalanDOMChar	c) = 0;

	virtual void
	write(
			const XalanDOMString&		s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	// Output functions which are new...

#if !defined(XALAN_BOOL_AS_INT)
	virtual void
	print(bool	b) = 0;
#endif

	virtual void
	print(char	c) = 0;

	virtual void
	print(
			const char*		s,
			size_t			theLength = npos) = 0;

	virtual void
	print(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	virtual void
	print(double	d) = 0;

	virtual void
	print(int	i) = 0;

	virtual void
	print(long	l) = 0;

	virtual void
	print(const XalanDOMString&		s) = 0;

	virtual void
	println() = 0;

#if !defined(XALAN_BOOL_AS_INT)
	virtual void
	println(bool	b) = 0;
#endif

	virtual void
	println(char	x) = 0;

	virtual void
	println(
			const char*		s,
		    size_t			theLength = npos) = 0;

	virtual void
	println(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	virtual void
	println(double	x) = 0;

	virtual void
	println(int		x) = 0;

	virtual void
	println(long	x) = 0;

	virtual void
	println(const XalanDOMString&	s) = 0;

protected:

	const bool						m_fAutoFlush;

    MemoryManagerType&              m_memoryManager;

	// Some static strings to help derived classes...
	static const XalanDOMChar	s_trueString[];

	static const XalanDOMChar	s_falseString[];

	static const XalanDOMChar	s_newlineString[];

private:

	// Not implemented
	PrintWriter(const PrintWriter&);

	PrintWriter&
	operator=(const PrintWriter&);

	bool
	operator==(const PrintWriter&);
};



XALAN_CPP_NAMESPACE_END



#endif	// PRINTWRITER_HEADER_GUARD_1357924680
