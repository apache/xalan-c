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
#if !defined(NULLPRINTWRITER_HEADER_GUARD_1357924680)
#define NULLPRINTWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/PrintWriter.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT NullPrintWriter : public PrintWriter
{
public:

	explicit
	NullPrintWriter(MemoryManagerType& theManager);

	virtual
	~NullPrintWriter();

	// Functions inherited from PrintWriter...

	virtual bool
    checkError() const;

	virtual void
	close();

	virtual void
	flush();

	virtual void
	write(
			const char*		s,
			size_t			theOffset = 0,
			size_t			theLength = npos);

	virtual void
	write(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual void
	write(XalanDOMChar		c);

	virtual void
	write(
			const XalanDOMString&		s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

#if !defined(XALAN_BOOL_AS_INT)
	virtual void
	print(bool	b);
#endif

	virtual void
	print(char	c);

	virtual void
	print(
			const char*		s,
			size_t			theLength = npos);

	virtual void
	print(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual void
	print(double	d);

	virtual void
	print(int	i);

	virtual void
	print(long	l);

	virtual void
	print(const XalanDOMString&		s);

	virtual void
	println();

#if !defined(XALAN_BOOL_AS_INT)
	virtual void
	println(bool	x);
#endif

	virtual void
	println(char	x);

	virtual void
	println(
			const char*		s,
		    size_t			theLength = npos);

	virtual void
	println(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual void
	println(double	x);

	virtual void
	println(int		x);

	virtual void
	println(long	x);

	virtual void
	println(const XalanDOMString&	s);

private:

	// Not implemented
	NullPrintWriter(const NullPrintWriter&);

	NullPrintWriter&
	operator=(const NullPrintWriter&);

	bool
	operator==(const NullPrintWriter&);
};



XALAN_CPP_NAMESPACE_END



#endif	// NULLPRINTWRITER_HEADER_GUARD_1357924680
