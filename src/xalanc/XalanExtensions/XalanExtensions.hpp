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
#if !defined(XALANEXTENSIONS_HEADER_GUARD_1357924680)
#define XALANEXTENSIONS_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XalanExtensions/XalanExtensionsDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Function;
class XPathEnvSupportDefault;



class XALAN_XALANEXTENSIONS_EXPORT XalanExtensionsInstaller
{
public:

	struct FunctionTableEntry
	{
		const XalanDOMChar*		theFunctionName;
		const Function*			theFunction;
	};

	static void
	installLocal(XPathEnvSupportDefault&	theSupport);

	static void
	installGlobal(MemoryManagerType& theManager);

	static void
	uninstallLocal(XPathEnvSupportDefault&	theSupport);

	static void
	uninstallGlobal(MemoryManagerType& theManager);

protected:

	static void
	doInstallLocal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[],
			XPathEnvSupportDefault&		theSupport);

	static void
	doInstallGlobal(
            MemoryManagerType&          theManager,
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[]);

	static void
	doUninstallLocal(
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[],
			XPathEnvSupportDefault&		theSupport);

	static void
	doUninstallGlobal(
            MemoryManagerType&          theManager,
			const XalanDOMChar*			theNamespace,
			const FunctionTableEntry	theFunctionTable[]);
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANEXTENSIONS_HEADER_GUARD_1357924680
