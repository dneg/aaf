#if defined(macintosh) || defined(_MAC)
////////////////////////////////////////////////////////////////////////////////
// DataInput.cpp
//
////////////////////////////////////////////////////////////////////////////////
#include "DataInput.h"

#include <fstream.h> // obsolete
#include <iostream.h>

#include <assert.h>
#include <ctype.h>
#include <console.h> /* Mac command line window */
#include <Processes.h>
#include <Script.h>
#include <Errors.h>

/* Make sure we don't redefine our local main. */
#undef main

/* Use the correct external declaration used by the MSL __initialize code. */
extern "C" int main(void);

////////////////////////////////////////////////////////////////////////////////
// Local utilities
//

typedef enum _aafStdioFile
{
	kAAFStdOut = 1,
	kAAFStdErr = 2,
	kAAFStdOutErr = 3
} aafStdioFile;

static OSErr GetApplicationInfo(FSSpecPtr appFileSpec);
static OSErr AddFileNameSuffix(StringPtr name, const StringPtr suffix);
static OSErr initialize_stdio_file(FSSpecPtr fileSpec, aafStdioFile file);


////////////////////////////////////////////////////////////////////////////////
// Utility class to cleanup the command line data. We use a static instance to 
// cleanup to handle the case where the renamed "Main" routine calls the ansi 
// exit() routine instead of just returning normally.
//
class MacCommandLine
{
public:
	MacCommandLine();
	~MacCommandLine();

	// Save the given string for argv[0].
	OSErr SetAppName(const StringPtr appName);
	
	// Note returned buffer is owned by this
	// object.
	char* AllocateArgumentBuffer (size_t bufferSize);
	
	// Create the argument buffer from the contents of the given file.
	OSErr InitializeArgumentBuffer(ConstFSSpecPtr fileSpec);
	
	// This method was copied (shamelessly from the ccommand sources) and modified
	// for this class.
	OSErr ParseArgumentBuffer(void);
	
	// Call the external Main routine with the current argc and argv.
	int CallMain(void);
	
private:

	// The string storage to hold the current application name. */
	char * _appName;

	// Raw buffer for the command line data.
	char * _argumentBuffer;
	
	// The internal size of the argument buffer
	size_t _argumentBufferSize;
	
	// The buffer that will hold the parsed arguments from the raw argument buffer.
	char * _argvBuffer;
	
	// Buffer for argument/option string pointers (into _argmentBuffer)
	char ** _argv;
	
	// The number of string pointers allocated for _argv.
	int _argc;
};



////////////////////////////////////////////////////////////////////////////////
// Global data:
const StringPtr kDefaultStdOutSuffix = "\p.out";
const StringPtr kCommandLineFileSuffix = "\p.arg";
const StringPtr kInputFileSuffix = "\p.inp";


// Instance must be declared in global scope so that it will be called by the
// runtime even if client "Main" calls exit.
static MacCommandLine sMacCommandLine;



////////////////////////////////////////////////////////////////////////////////
// This implementation of the main application entrypoint will 
//
int main(void)
{
	int exitCode = 0;	
	OSErr err = noErr;
	FSSpec appFileSpec;
	char *argumentBuffer = NULL;
	bool redirectStdio = true;
	
	/* Get the file spec of the currently running application. */
	err = GetApplicationInfo(&appFileSpec);   
	if (noErr == err)
	{
		/* Get the global application name as a null terminated string. */
		err = sMacCommandLine.SetAppName(appFileSpec.name);
		if (noErr == err)
		{
			/* Process the input, ".inp" file if one exists. If the ".inp" exists then use the contents to 
			   build argc and argv but do NOT redirect stdout and stderr. */
			FSSpec argumentFileSpec = appFileSpec;
			err = AddFileNameSuffix(argumentFileSpec.name, kInputFileSuffix);
			if (noErr == err)
			{
				err = sMacCommandLine.InitializeArgumentBuffer(&argumentFileSpec);
				if (noErr == err)
					redirectStdio = false;
			}

			if (noErr != err)
			{
				/* Process the argument file if one exists */
				argumentFileSpec = appFileSpec;
				err = AddFileNameSuffix(argumentFileSpec.name, kCommandLineFileSuffix);
				if (noErr == err)
				{
					err = sMacCommandLine.InitializeArgumentBuffer(&argumentFileSpec);
				}
			}
			
			if (noErr == err)
			{
				/* Build the command line arguments from the given argument
				   file. */   
				err = sMacCommandLine.ParseArgumentBuffer();
				if (noErr == err && redirectStdio)
				{
					/* Create the new stdout */
					FSSpec stdoutSpec = appFileSpec;
					err = AddFileNameSuffix(stdoutSpec.name, kDefaultStdOutSuffix);
					if (noErr == err)
					{
						err = initialize_stdio_file(&stdoutSpec, kAAFStdOutErr);
						
						// Make sure that our re-assigned stdout and stderr will be used 
						// by cout and cerr static stream objects.
						ios_base::sync_with_stdio();
					}
				}
			}
		}			
	}
		
	
	if (noErr == err)
	{
		/* Call the original "main" routine. */
		exitCode = sMacCommandLine.CallMain();
	}
	else
	{	
		/* If we failed for any reason to read the arguments from the external
		   file then just fall back on the good old ccomand dialog. */
		char **tempargv = NULL;
		int tempargc = 0;

		/* console window for mac */
		tempargc = ccommand(&tempargv);

		/* Call the original "main" routine. */
		exitCode = Main(tempargc, tempargv);
	}
	
	return exitCode;	
}


////////////////////////////////////////////////////////////////////////////////
// Default constructor
//
MacCommandLine::MacCommandLine() :
	_appName(NULL),
	_argumentBuffer(NULL),
	_argumentBufferSize(0),
	_argvBuffer(NULL),
	_argv(NULL),
	_argc(0)
{
}

////////////////////////////////////////////////////////////////////////////////
// Destructor (for buffer cleanup)
//
MacCommandLine::~MacCommandLine()
{
	if (_argv)
	{
		delete [] _argv;
		_argv = NULL;
	}

	if (_argvBuffer)
	{
		delete [] _argvBuffer;
		_argvBuffer = NULL;
	}

	if (_argumentBuffer)
	{
		delete [] _argumentBuffer;
		_argumentBuffer = NULL;
	}
	
	if (_appName)
	{
		delete [] _appName;
		_appName = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Save the given string for argv[0].
//
OSErr MacCommandLine::SetAppName(const StringPtr appName)
{
	OSErr err = noErr;
	
	if (NULL == appName)
		return paramErr;
		
	if (_appName)
	{
		delete [] _appName;
		_appName = NULL;	
	}
	
	_appName = new char[appName[0] + 1];
	err = (NULL != _appName) ? noErr : memFullErr;
	if (noErr == err)
	{
		/* Get the global application name as a null terminated string. */
		int idx;
		for (idx = 0; idx < appName[0]; ++idx)
			_appName[idx] = appName[idx + 1];
		_appName[idx] = 0;	
	}
	
	return err;
}

////////////////////////////////////////////////////////////////////////////////
// Allocate the buffer that will be used to hold the command line
// Make sure the string is double null terminated. After ParseArguments
// the buffer will contain embedded c strings that must be termined
// by an additional null "\0\0".
//
char*  MacCommandLine::AllocateArgumentBuffer (size_t bufferSize)
{
	if (_argumentBuffer)
	{
		delete [] _argumentBuffer;
		_argumentBuffer = NULL;	
	}
	
	if (_argvBuffer)
	{
		delete [] _argvBuffer;
		_argvBuffer = NULL;	
	}

	_argumentBufferSize = bufferSize + 1;
		
	_argvBuffer = new char[_argumentBufferSize + 1];
	if (_argvBuffer)
	{
		memset(_argvBuffer, 0, _argumentBufferSize + 1);
		
		_argumentBuffer = new char[_argumentBufferSize];
		if (_argumentBuffer)
		{
			memset(_argumentBuffer, 0, _argumentBufferSize);
		}
	}
		
	return (_argumentBuffer);
}

////////////////////////////////////////////////////////////////////////////////
// Allocate the argument buffer and initialize it with the contents of the 
// given file.
//
OSErr MacCommandLine::InitializeArgumentBuffer(ConstFSSpecPtr fileSpec)
{
	OSErr err = noErr;
	short refNum = 0;
	char *argumentBuffer = NULL;
	
	
	if (NULL == fileSpec)
		return paramErr;
	
	err = FSpOpenDF(fileSpec, fsRdPerm, &refNum);
	if (noErr == err)
	{		
		long logicalSize = 0;
		err = GetEOF(refNum, &logicalSize);
		if (noErr == err)
		{
			/* Allocate the argument buffer large enough for the entire file */
			argumentBuffer = sMacCommandLine.AllocateArgumentBuffer(logicalSize);
			err = (NULL != argumentBuffer) ? noErr : memFullErr;
			if (noErr == err)
			{
				long count = logicalSize;
				err = FSRead(refNum, &count, argumentBuffer);
			}
		}
		
		FSClose(refNum);
	}				
	return err;	
}

////////////////////////////////////////////////////////////////////////////////
// The following function (now method) was copied from ccommand.c and then tailored to the 
//  environment of this module (based on ParseArgs).
OSErr MacCommandLine::ParseArgumentBuffer(void)
{
	assert (NULL != _argumentBuffer); // precondition

	OSErr err = noErr;
	int  n = 1, Quote = 0;
	//char *p = s, *p1, c;
	char c;
	char *p = _argumentBuffer;
	char *p1 = _argvBuffer;
			
	
	//p1 = (char *) argStr;
	while ((c = *p++) != 0) {
		//if (c==' ') continue;
		if (isspace(c)) continue;
		n++; //argv[n++] = p1;
//		if (n > MAX_ARGS)				/* mm 970404 */
//			return (n-1);				/* mm 970404 */
		do {
			// if (c=='\\' && *p++) 
			if (c=='\\' && *p)  /*tr 20000604 bugfix: \"a\" b was one argument, {a b}, instead of two, {"a", b}*/
				c = *p++;
			else
				if ((c=='"') || (c == '\'')) {
					if (!Quote) {
						Quote = c;
						continue;
					}
					if (c == Quote) {
						Quote = 0;
						continue;
					}
				}
			*p1++ = c;
		//} while (*p && ((c = *p++) != ' ' || Quote));
		} while (*p && (!isspace(c = *p++) || Quote));
		*p1++ = 0;
		
	}
	//return n;
	
	
	
	// Allocate the argv string array.
	if (_argv)
	{
		delete [] _argv;
		_argv = NULL;
		_argc = 0;
	}
	
	_argv = new char*[n];
	err = (NULL != _argv) ? noErr : memFullErr;
	if (noErr == err)
	{
		/* Install the application name as arg[0]. */
		_argv[_argc++] = _appName;
		
		p = _argvBuffer;
		
		while (0 != *p)
		{
			/* Copy the next argument */
			_argv[_argc++] = p;
			
			/* skip to the next argument...*/
			while (0 != *p++)
				;
			//++p; /* skip over the trailing null */
		}
	}
	
	return err;
}


	
////////////////////////////////////////////////////////////////////////////////
// Call the external Main routine with the current argc and argv.
//
int MacCommandLine::CallMain(void)
{
	return Main(_argc, _argv);
}


////////////////////////////////////////////////////////////////////////////////
// Simple utility to append a suffix to a mac file name.
//
OSErr AddFileNameSuffix(StringPtr name, const StringPtr suffix)
{
	int maxMacBaseFileName = ((sizeof(StrFileName) / 2) - 1) - suffix[0];
	
	if (0 >= maxMacBaseFileName)
		return paramErr;
		
	if (maxMacBaseFileName < name[0])
		name[0] = maxMacBaseFileName;
		
	BlockMove(suffix + 1, name + name[0] + 1, suffix[0]);
	name[0] = name[0] + suffix[0];
	
	return noErr;
}

////////////////////////////////////////////////////////////////////////////////
// Simple utility to initialize the given file specifiction from the
//  currently running process.
//
OSErr GetApplicationInfo(FSSpecPtr appFileSpec)
{
	OSErr err = noErr;
	ProcessSerialNumber psn;
	ProcessInfoRec pinfo;
	
	if (NULL == appFileSpec)
		return paramErr;
	
	memset(&pinfo, 0, sizeof(pinfo));
	err = GetCurrentProcess(&psn);	
	if (noErr == err)
	{
		pinfo.processInfoLength = sizeof(pinfo);
		pinfo.processAppSpec = appFileSpec;
		err = GetProcessInformation(&psn,&pinfo);
	}
	
	return err;
}


////////////////////////////////////////////////////////////////////////////////
// Automatically redirect stdout to the given file.
//
OSErr initialize_stdio_file(FSSpecPtr fileSpec, aafStdioFile file)
{
	OSErr err = noErr;
	short refNum = 0;
	FILE* file1 = NULL;
	FILE* file2 = NULL;
	
	if (NULL == fileSpec)
		return paramErr;
		
	switch (file)
	{
		case kAAFStdOut:
			file1 = stdout;
			break;

		case kAAFStdErr:
			file1 = stderr;
			break;

		case kAAFStdOutErr:
			file1 = stdout;
			file2 = stderr;
			break;
			
		default:
			return paramErr;
			break;
	}
					
	err = FSpDelete(fileSpec);
	if ((noErr == err) || (fnfErr == err))
	{
		err = FSpCreate(fileSpec, 'ttxt' /*'CWIE'*/, 'TEXT', smRoman);
		if (noErr == err)
		{
			err = FSpOpenDF(fileSpec, fsWrPerm, &refNum);
			if (noErr == err)
			{
				__handle_reopen(refNum, "w", file1);
				
				// If there is second file then we are probably assigning 
				// stdout and stderr to the same file.
				if (file2)
				{
					__handle_reopen(refNum, "w", file2);
				}
			}
		}
	}
	
	return err;	
}



/* The following routines are obsolete. */
void getInputData(int *pargc, char **argv, char *inputFile)
{		
	char *newstring = NULL;
	
	/* try to open inputFile for read */
	ifstream ifsCommandLine(inputFile);
		
	/* Test to make sure it is open.  If not it doesn't exist */
	if (ifsCommandLine.is_open()) 
	{
		char inputstr[100];
		int  count = 0;

		/* Determine the number of lines */
		while (	ifsCommandLine >> inputstr )
			++count;
 
		/* *pargc = count + 1 extra for appName */
		*pargc = count + 1;  
		
		/* Create the array of char pointers */
		*argv = new char [*pargc];

		/* Reset file pointer to beginning of file */
		ifsCommandLine.clear();
		ifsCommandLine.seekg(0);

		count = 0;
		/* Allocate for argv[0] assuming inputFile = AppName.inp */
		int inputFileLength = strlen(inputFile);
		newstring = new char [inputFileLength-3];
		newstring[inputFileLength-4] = NULL;
		strncpy(newstring, inputFile, inputFileLength-4);
		argv[0] = newstring;

		/* Get Data */
		while (	ifsCommandLine >> inputstr )	
		{
			newstring = new char [strlen(inputstr)+1];
			strcpy(newstring, inputstr);
			argv[++count] = newstring;
		}
		ifsCommandLine.close();
	}
	else	
	{	
		char **tempargv = NULL;
		int tempargc = 0;

		/* console window for mac */
		tempargc = ccommand(&tempargv);

		*pargc = tempargc;
		
		/* Create the array of char pointers */
		*argv = new char [*pargc];

		/* Fill it in */
		for (int i = 0; i<*pargc; ++i)
		{
			newstring = new char [strlen(tempargv[i])+1];
			strcpy(newstring, tempargv[i]);
			argv[i] = newstring;
		}
	}
}


void cleanUpInputData(int pargc, char **argv)
{
	for ( int i=0; i<pargc; ++i)
		delete [] argv[i];
}


#endif // #if defined(macintosh) || defined(_MAC)
