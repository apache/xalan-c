/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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

#include <xalanc/XPathCAPI/XPathCAPI.h>



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



int
CreateXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			XalanXPathHandle*			theXPathHandle)
{
	int					theResult = 0;

	assert(theXalanHandle);
	assert(theXPathHandle);

	theResult = XalanCreateXPath(theXalanHandle, theXPathExpression, 0, theXPathHandle);

	if (theResult != XALAN_XPATH_API_SUCCESS)
	{
		fprintf(stderr, "Error creating XPath.  Error code was %d.\n", theResult);
	}

	return theResult;
}



int
DestroyXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			XalanXPathHandle			theXPathHandle)
{
	int					theResult = 0;

	theResult = XalanDestroyXPath(theXalanHandle, theXPathHandle);

	if (theResult != XALAN_XPATH_API_SUCCESS)
	{
		fprintf(stderr, "Error creating XPath.  Error code was %d.\n", theResult);
	}

	return theResult;
}



char*
ReadFile(const char*	theXMLFileName)
{
	char*	theBuffer = 0;
	FILE*	theFile = fopen(theXMLFileName, "rb");

	if (theFile == 0)
	{
		fprintf(stderr, "Unable to open input file %s\n", theXMLFileName);
	}
	else
	{
		fseek(theFile, 0, SEEK_END);

		{
			const int	theSize = ftell(theFile);

			if (theSize == -1)
			{
				fprintf(stderr, "Unable to determine the size of the input file\n");
			}
			else
			{
				fseek(theFile, SEEK_SET, 0);

				theBuffer = (char*)malloc(theSize + 1);

				if (theBuffer == 0)
				{
					fprintf(stderr, "Unable to allocate enough memory.  The input file size is %d\n", theSize);
				}
				else
				{
					fread(theBuffer, 1, theSize, theFile);

					theBuffer[theSize] = '\0';
				}
			}
		}
	}

	return theBuffer;
}



int
EvaluateXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXMLFileName,
			const char*					theXPathExpression,
			int*						theBoolean)
{
	XalanXPathHandle	theXPathHandle;

	int					theResult = CreateXPath(theXalanHandle, theXPathExpression, &theXPathHandle);

	if (theResult == XALAN_XPATH_API_SUCCESS)
	{
		char*	theBuffer = ReadFile(theXMLFileName);

		if (theBuffer != NULL)
		{
			theResult = XalanEvaluateXPathAsBoolean(theXalanHandle, theXPathHandle, theBuffer, theBoolean);

			if (theResult != XALAN_XPATH_API_SUCCESS)
			{
				fprintf(stderr, "Unable to evaluate XPath expression.  Error code was %d.\n", theResult);
			}

			free(theBuffer);
		}

		DestroyXPath(theXalanHandle, theXPathHandle);
	}

	return theResult;
}



int
main(
			int				argc,
			const char*		argv[])
{
	int		theResult = 0;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: SimpleXPathAPI XMLFilePath XPathExpression\n");

		theResult = -1;
	}
	else
	{
		theResult = XalanXPathAPIInitialize();

		if (theResult != XALAN_XPATH_API_SUCCESS)
		{
			fprintf(stderr, "Unable to initialize the API.  Error code was %d.\n", theResult);
		}
		else
		{
			XalanXPathEvaluatorHandle	theXalanHandle;

			theResult = XalanCreateXPathEvaluator(&theXalanHandle);

			if (theResult != XALAN_XPATH_API_SUCCESS)
			{
				fprintf(stderr, "Error creating evaluator.  Error code was %d.\n", theResult);
			}
			else
			{
				int	theBoolean = 0;

				theResult = EvaluateXPath(theXalanHandle, argv[1], argv[2], &theBoolean);

				if (theResult == XALAN_XPATH_API_SUCCESS)
				{
					fprintf(stdout, "The result of the expression was '%s'.\n", theBoolean == 0 ? "false" : "true");
				}

				theResult = XalanDestroyXPathEvaluator(theXalanHandle);

				if (theResult != XALAN_XPATH_API_SUCCESS)
				{
					fprintf(stderr, "Error destroying evaluator.  Error code was %d.\n", theResult);
				}
			}

			theResult = XalanXPathAPITerminate();

			if (theResult != XALAN_XPATH_API_SUCCESS)
			{
				fprintf(stderr, "Error terminating the API.  Error code was %d.\n", theResult);
			}
		}
	}

	return theResult;
}
