// Base header file.  Must be first.
#include <XPathCAPI/XPathCAPI.h>



#include <assert.h>
#include <malloc.h>
#include <stdio.h>



int
CreateXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			XalanXPathHandle*			theXPathHandle)
{
	int					theResult = 0;

	assert(theXalanHandle);
	assert(theXPathHandle);

	theResult = XalanCreateXPath(theXalanHandle, theXPathExpression, "foo-8", theXPathHandle);

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
					fprintf(stdout, "The result of the expression was '%s'.", theBoolean == 0 ? "false" : "true");
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
