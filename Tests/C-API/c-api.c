#include <XalanTransformer/XalanCAPI.h>



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
			const char*			data,
			CallbackSizeType	length,
			void*				handle)
{
	FILE*	theHandle = (FILE*)handle;

	return fwrite(data, sizeof(char), length, theHandle);
}



void
FlushHandler(void*	handle)
{
	FILE*	theHandle = (FILE*)handle;

	fflush(theHandle);
}



int
main(
			int				argc,
			const char*		argv[])
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
		int			error = 0;
		XalanHandle	xalan = NULL;

		XalanInitialize();

		xalan = CreateXalanTransformer();

		if (xalan == 0)
		{
			fprintf(stderr, "Unable to create transformer...\n");
		}
		else if (argc == 2)
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

		XalanTerminate(1);
	}

	return 0;
}
