// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef COMPILE_AS_DLL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
using namespace std;

#ifdef macintosh
	#include "DataInput.h"
#endif 
#include "omPublic.h"
#include "omMedia.h"

#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"

// OMF Includes


#include "AafOmf.h"

#include "AAFDomainUtils.h"
#include "OMFDomainUtils.h"
#if AVID_SPECIAL
#include "ConvertAvid.h"
#include "AAFDomainAvidUtils.h"
#include "OMFDomainAvidUtils.h"
#else
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#include "Extensions.h"
#endif
#include "Aaf2Omf.h"
#include "Omf2Aaf.h"
#include "AAFClassDefUIDs.h"
#include "EffectTranslate.h"

static char* baseName(char* fullName);
static char* baseName(char* fullName)
{
	char* result;
#if defined(WIN32)
	const int delimiter = '\\';
#elif defined(_MAC) || defined(macintosh)
	const int delimiter = ':';
#else
	const int delimiter = '/';
#endif
	result = strrchr(fullName, delimiter);
	if (result == 0)
	{
		result = fullName;
	}
	else if (strlen(result) == 0) 
	{
		result = fullName;
	}
	else 
	{
		result++;
	}

	return result;
}

// ============================================================================
// Usage
// 
//		Displays short help text with the program arguments.
//
// ============================================================================
static void Usage( void );
static void Usage( void )
{
	printf("\n*******************\n\n");
	printf("%s : OMF/AAF file conversion Version 0.01.00\n\n", gpGlobals->pProgramName);
	printf("Usage: \n");
//	printf("%s [-v] [-s] [-p logfile] [-d deffile] [-t tocfile] <infile> [outfile]\n\n", gpGlobals->pProgramName);
	printf("%s [-v] [-s] [-nr] [-OMF] <infile> [outfile]\n\n", gpGlobals->pProgramName);
	printf("-v         = Verbose - give progress report (optional)\n" );
	printf("-s         = Straight conversion. Do NOT discard unnecessary objects (optional)\n");
	printf("-nr        = DO NOT replace Output file. If Output file is present, give an error (optional)!!\n");
//	printf("-p logfile = Log file name(optional)\n");
//	printf("-d deffile = Definition file (optional)\n");
//	printf("-t tocfile = Dump OMFI Table of contents (optional)\n");
	printf("-OMF       = Convert an AAF file to OMF 2.1 version\n");
	printf("infile     = input file name (required)\n");
	printf("outfile    = output file name (optional)\n");
	printf("\n*******************\n\n");
}

// ============================================================================
// DisplaySummary
// 
//		Displays summary of execution at the end of the run.
//
// ============================================================================
static void DisplaySummary( void );
static void DisplaySummary( void )
{
	printf("\n*******************\n\n");
	printf("%s Summary for the file :%s\n\n",gpGlobals->pProgramName, gpGlobals->sOutFileName);
	printf("\tTotal OMF Mobs \t\t\t: %ld\n", gpGlobals->nNumOMFMobs);
	printf("\tTotal AAF Mobs \t\t\t: %ld\n", gpGlobals->nNumAAFMobs);
	printf("\tTotal OMF Objects \t\t: %ld\n", gpGlobals->nNumOMFObjects);
	printf("\tTotal AAF Objects \t\t: %ld\n", gpGlobals->nNumAAFObjects);
	printf("\tTotal OMF Properties \t\t: %ld\n", gpGlobals->nNumOMFProperties);
	printf("\tTotal AAF Properties \t\t: %ld\n", gpGlobals->nNumAAFProperties);
	printf("\tTotal OMF Objects NOT found\t: %ld\n", gpGlobals->nNumUndefinedOMFObjects);
	printf("\n*******************\n\n");
}
// ============================================================================
// GetUserInput
//
//		Parse program arguments and extract names of OMF and AAF files.
//
// ============================================================================
static HRESULT GetUserInput(int argc, char* argv[]);
static HRESULT GetUserInput(int argc, char* argv[])
{
	HRESULT			rc = AAFRESULT_SUCCESS;
	aafInt32		nFileCount = 0;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			char*	pNextArgument = argv[i];
			char	c = pNextArgument[0];
			char*	pFileName;

			if ((c == '-') && (strlen(pNextArgument) == 2))
			{
				char flag = pNextArgument[1];
				switch( flag )
				{
					case 'v':
						gpGlobals->bVerboseMode = kAAFTrue;
						break;
					case 's':
						gpGlobals->bConvertAllObjects = kAAFTrue;
						break;
					case 'p':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								gpGlobals->bLogFile = kAAFTrue;
								strcpy(gpGlobals->sLogFileName, pFileName);
							}
						}
						break;
					case 'd':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								gpGlobals->bDefFile = kAAFTrue;
								strcpy(gpGlobals->sDefinitionFileName, pFileName);
							}
						}
						break;
					case 't':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								gpGlobals->bCreateTOCFile = kAAFTrue;
								strcpy(gpGlobals->sTOCFileName, pFileName);
							}
						}
						break;
					default:
						rc = AAFRESULT_BAD_FLAGS;
						break;
				}
			}
			else if ((c == '-') && (strlen(pNextArgument) > 2))
			{
				char* pArg = &pNextArgument[1];
				char  lc[4];
				memset(lc, 0, sizeof(lc));
				unsigned int i ;
				unsigned int j = strlen(pArg);
				if (strlen(pArg) <=3)
				{
					for (i = 0; i < j; i++, pArg++)
					{
						lc[i] = tolower(*pArg);
					}
					if (strcmp(lc, "nr") == 0)
					{
						gpGlobals->bDeleteOutput = kAAFFalse;
					}
					else if ( strcmp(lc, "omf") == 0 ) 
					{
						gpGlobals->bConvertAAFFile = kAAFTrue;
					}
					else
						rc = AAFRESULT_BAD_FLAGS;
				}
				else
					rc = AAFRESULT_BAD_FLAGS;
			}
			else
			{
				nFileCount += 1;
				if (nFileCount == 1)
					strcpy(gpGlobals->sInFileName, argv[i]);
				else
					strcpy(gpGlobals->sOutFileName, argv[i]);
			}
		}
	}
	else
		rc = AAFRESULT_BAD_FLAGS;

	return rc;
}

// ============================================================================
// MAIN Module 
//
//		This is the application's main controlling routine.  
//
// ============================================================================
int main(int argc, char *argv[])
{
	HRESULT			hr;

	hr = InitGlobalVars();
	if (FAILED(hr))
		return 1; //!!!UTLEcFromHr(hr);

	gpGlobals->pProgramName = baseName(argv[0]);

	printf("%s: Version 0.01.00\n", gpGlobals->pProgramName);
	hr = GetUserInput(argc, argv);
	if (FAILED(hr))
	{
		Usage();
		return 1; //!!!UTLEcFromHr(hr);
	}

	// ************** Set up the logging utility for the application.**********

	// If in verbose mode we will have 3 levels of logging: error (0), warning (1),
	// and info (2). Otherwise we just log warnings and errors.
	unsigned logLevel = gpGlobals->bVerboseMode ? kLogInfo : kLogWarn;
	if(  gpGlobals->bLogFile )
	{
		// The user specified a log file on the command line.
		try
		{
			// Clobber any previous log file that was hanging around.
			gpGlobals->pLogger = new FileStreamLogger( gpGlobals->sLogFileName, "w", logLevel );
		}
		catch( FileStreamLogger::LogStreamNULL )
		{
			// Couldn't open log file for writing. Send log output to stdout.
			gpGlobals->pLogger = new StreamLogger( stdout, logLevel );
			gpGlobals->pLogger->Log( kLogError, "Could not open log file %s."
			"Logging to the console instead.\n", gpGlobals->sLogFileName );
		}
	}
	else
	{
		// Use stdout.
		gpGlobals->pLogger = new StreamLogger( stdout, logLevel );
	}

	// Wire up excepion handling to the logger. .
	ExceptionBase::SetLogger( gpGlobals->pLogger );
	// **************************************************************************

	AAFDomainExtensionUtils	*aafDomain = new AAFDomainExtensionUtils;
	OMFDomainExtensionUtils	*omfDomain = new OMFDomainExtensionUtils;
	EffectTranslate			*effectTranslate = new EffectTranslate;

	try
	{
		AAFCheck check;
		if (gpGlobals->bConvertAAFFile)
		{
			// User indicated input file must be an AAF 
			// Convert AAF to OMF
			ExtendedAaf2Omf		AAFMain( aafDomain, omfDomain, effectTranslate );
			AAFMain.ConvertFile();
		}
		else
		{
			// User indicated Input file must be an OMF file
			// Conert OMF to AAF
			check = IsOMFFile(gpGlobals->sInFileName);
			ExtendedOmf2Aaf		OMFMain ( aafDomain, omfDomain, effectTranslate );
			OMFMain.ConvertFile();
		}

		// We are done, just display a summary of results
		DisplaySummary();
	}
	catch( ExceptionBase &e )
	{
		// Ultimately we want the exception to print itself out instead
		// of doing this case analysis...
		if( e.Code() == AAFRESULT_FILE_NOT_OMF )
		{
			gpGlobals->pLogger->Log( kLogError, 
				"File \"%s\" is not a valid OMF file.\n", gpGlobals->sInFileName );
		}
		else if( e.Code() == AAFRESULT_BADOPEN )
		{
			gpGlobals->pLogger->Log( kLogError, 
				"Cannot open file \"%s\".\n", gpGlobals->sInFileName );
		}
		else
		{
			gpGlobals->pLogger->Log( kLogError, 
				"main(): %s exception %0lx\n", e.Type(), e.Code() );
		}
		hr = e.Code();
	}

	// If we get here then the gpGlobals was created.
	delete gpGlobals;
	delete aafDomain;
	delete omfDomain;
	delete effectTranslate;

	return( hr );
}

#endif // ifndef COMPILE_AS_DLL

