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
#if !defined(XALANNULLOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANNULLOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanNullOutputStream : public XalanOutputStream
{
public :

	explicit
    XalanNullOutputStream(MemoryManagerType&      theManager);

    virtual
	~XalanNullOutputStream();

protected:

    // These are inherited from XalanOutputStream...
	virtual void
	writeData(
			const char*		theBuffer,
			size_type		theBufferLength);

	virtual void
	doFlush();

private:

    // These are not implemented...
    XalanNullOutputStream(const XalanNullOutputStream&);

    XalanNullOutputStream&
	operator=(const XalanNullOutputStream&);

    bool
	operator==(const XalanNullOutputStream&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// NULLTEXTOUTPUTSTREAM_HEADER_GUARD_1357924680
