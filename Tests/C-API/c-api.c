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

#include <xalanc/XalanTransformer/XalanCAPI.h>



#include <assert.h>
#include <stdio.h>



// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



void
Usage()
{
    fprintf(stderr, "Usage: c-api <xml file> [<xsl file>]\n");
}



CallbackSizeType
OutputHandler(
            const char*         data,
            CallbackSizeType    length,
            void*               handle)
{
    FILE*   theHandle = (FILE*)handle;

    return fwrite(data, sizeof(char), length, theHandle);
}



void
FlushHandler(void*  handle)
{
    FILE*   theHandle = (FILE*)handle;

    fflush(theHandle);
}



int
main(
            int     argc,
            char*   argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    if (argc < 2 || argc > 3)
    {
        Usage();
    }
    else
    {
        int         error = 0;
        XalanHandle xalan = NULL;

        XalanInitialize();

        xalan = CreateXalanTransformer();

        if (xalan == 0)
        {
            fprintf(stderr, "Unable to create transformer...\n");
        }
        else
        {
            if (argc == 2)
            {
                error = XalanTransformToHandler(argv[1], NULL, xalan, stdout, OutputHandler, FlushHandler);
            }
            else
            {
                error = XalanTransformToHandler(argv[1], argv[2], xalan, stdout, OutputHandler, FlushHandler);
            }

            if (error != 0)
            {
                fprintf(
                    stderr,
                    "Error transforming.  The error code is %d.  The message is %s.\n",
                    error,
                    XalanGetLastError(xalan));
            }

            DeleteXalanTransformer(xalan);
        }

        XalanTerminate(1);
    }

    return 0;
}
