// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/
/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// AAF Utilities Infra-structure 
#include "UtlConsole.h"
// OMF Includes


#include "AafOmf.h"

#include "Aaf2Omf.h"
#include "Omf2Aaf.h"

//#include "AAFUtils.h"
AAFRESULT aafMobIDNew(aafUID_t *mobID);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafUID_t *mobID);     /* OUT - Newly created Mob ID */


// ============================================================================
// Global Variables and functions
// ============================================================================
AafOmfGlobals*	gpGlobals = NULL;
// ============================================================================
// Delete a file given its  name
// ============================================================================
void deleteFile(char* fileName)
{

	int result = remove(fileName);
	//.. ignore error which typically happens if the file doesn;t exist

}

char* baseName(char* fullName)
{
	char* result;
#if defined(WIN32)
	const int delimiter = '\\';
#elif defined(_MAC) || defined(macintosh)
	const int delimiter = ':';
#else
	const in delimiter = '/';
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
// InitGlobalVars
// 
//		Allocates memory and intializes the global variables
//
// ============================================================================
HRESULT InitGlobalVars( void )
{
	HRESULT hr = UTLMemoryAlloc(sizeof(AafOmfGlobals),(void**)&gpGlobals);
	if (FAILED(hr))
		return hr;

	gpGlobals->bAAFFileOpen = AAFFalse;
	gpGlobals->bConvertAllObjects = AAFFalse;
	gpGlobals->bCreateTOCFile = AAFFalse;
	gpGlobals->bDefFile = AAFFalse;
	gpGlobals->bLogFile = AAFFalse;
	gpGlobals->bOMFFileOpen = AAFFalse;
	gpGlobals->bVerboseMode = AAFFalse;
	gpGlobals->numIndents = 0;
	gpGlobals->pProgramName = NULL;

	gpGlobals->nNumAAFMobs = 0;
	gpGlobals->nNumAAFObjects = 0;
	gpGlobals->nNumAAFProperties = 0;
	gpGlobals->nNumOMFMobs = 0;
	gpGlobals->nNumOMFObjects = 0;
	gpGlobals->nNumOMFProperties  = 0;
	gpGlobals->nNumUndefinedOMFObjects = 0;
	gpGlobals->nNumUndefinedOMFProperties = 0;
	gpGlobals->nNumUndefinedAAFObjects = 0;
	gpGlobals->nNumUndefinedAAFProperties = 0;

	memset(gpGlobals->sDefinitionFileName, 0,sizeof(gpGlobals->sDefinitionFileName));
	memset(gpGlobals->indentLeader, 0, sizeof(gpGlobals->indentLeader));
	memset(gpGlobals->sInFileName, 0, sizeof(gpGlobals->sInFileName));
	memset(gpGlobals->sLogFileName, 0, sizeof(gpGlobals->sLogFileName));
	memset(gpGlobals->sOutFileName, 0, sizeof(gpGlobals->sOutFileName));
	memset(gpGlobals->sTOCFileName, 0, sizeof(gpGlobals->sTOCFileName));

	return AAFRESULT_SUCCESS;
}

// ============================================================================
// Usage
// 
//		Displays short help text with the program arguments.
//
// ============================================================================
void Usage( void )
{
	UTLstdprintf("\n*******************\n\n");
	UTLstdprintf("%s : OMF/AAF file conversion Version 0.01.00\n\n", gpGlobals->pProgramName);
	UTLstdprintf("Usage: \n");
	UTLstdprintf("%s [-v] [-s] [-p logfile] [-d deffile] [-t tocfile] <infile> [outfile]\n\n", gpGlobals->pProgramName);
	UTLstdprintf("-v         = Verbose - give progress report (optional)\n" );
	UTLstdprintf("-s         = Straight conversion. Do NOT discard unnecessary objects (optional)\n");
	UTLstdprintf("-p logfile = Log file name(optional)\n");
	UTLstdprintf("-d deffile = Definition file (optional)\n");
	UTLstdprintf("-t tocfile = Dump OMFI Table of contents (optional)\n");
	UTLstdprintf("infile     = input file name (required)\n");
	UTLstdprintf("outfile    = output file name (optional)\n");
	UTLstdprintf("\n*******************\n\n");
}
/******************** IncIndentLevel *******************
	Increases the incrementation used for readability

	Inputs:		none
				  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void IncIndentLevel( void )
{
	gpGlobals->numIndents++;
	if (gpGlobals->numIndents <= MAX_INDENT)
		gpGlobals->indentLeader[gpGlobals->numIndents-1] = '\t';
}

/******************** DecIndentLevel *******************
	Lessens the incrementation used for readability

	Inputs:		none
			  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void DecIndentLevel(void)
{
	gpGlobals->numIndents--;
	if (gpGlobals->numIndents < 0)
		gpGlobals->numIndents = 0;
	if (gpGlobals->numIndents >= 0 && gpGlobals->numIndents < MAX_INDENT)
		gpGlobals->indentLeader[gpGlobals->numIndents] = 0;

}

// ============================================================================
// DisplaySummary
// 
//		Displays summary of execution at the end of the run.
//
// ============================================================================
void DisplaySummary( void )
{
	UTLstdprintf("\n*******************\n\n");
	UTLstdprintf("%s Summary for the file :%s\n\n",gpGlobals->pProgramName, gpGlobals->sOutFileName);
	UTLstdprintf("\tTotal OMF Mobs read \t\t: %ld\n", gpGlobals->nNumOMFMobs);
	UTLstdprintf("\tTotal AAF Mobs written \t\t: %ld\n", gpGlobals->nNumAAFMobs);
	UTLstdprintf("\tTotal OMF Objects read \t\t: %ld\n", gpGlobals->nNumOMFObjects);
	UTLstdprintf("\tTotal AAF Objects written \t: %ld\n", gpGlobals->nNumAAFObjects);
	UTLstdprintf("\tTotal OMF Properties read \t: %ld\n", gpGlobals->nNumOMFProperties);
	UTLstdprintf("\tTotal AAF Properties written \t: %ld\n", gpGlobals->nNumAAFProperties);
	UTLstdprintf("\tTotal OMF Objects NOT found\t: %ld\n", gpGlobals->nNumUndefinedOMFObjects);
	UTLstdprintf("\n*******************\n\n");
}
// ============================================================================
// GetUserInput
//
//		Parse program arguments and extract names of OMF and AAF files.
//
// ============================================================================
HRESULT GetUserInput(int argc, char* argv[])
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

			if ((c == '-') && (strlen(pNextArgument) >= 2))
			{
				char flag = pNextArgument[1];
				switch( flag )
				{
					case 'v':
						gpGlobals->bVerboseMode = AAFTrue;
						break;
					case 's':
						gpGlobals->bConvertAllObjects = AAFTrue;
						break;
					case 'p':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								gpGlobals->bLogFile = AAFTrue;
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
								gpGlobals->bDefFile = AAFTrue;
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
								gpGlobals->bCreateTOCFile = AAFTrue;
								strcpy(gpGlobals->sTOCFileName, pFileName);
							}
						}
						break;
					default:
						rc = AAFRESULT_BAD_FLAGS;
						break;
				}
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
// IsOMFFile
//			This function returns AAFTrue if the given file is an OMF file.
//
// ============================================================================
HRESULT IsOMFFile (char * pFileName, aafBool* pReturn)
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	FILE*		pStream = NULL;

	int			result = 0;
	char		ReadBuffer[8];
	char		CompBuffer[8];


	CompBuffer[0] = (char)0xA4;
	CompBuffer[1] = 'C';
	CompBuffer[2] = 'M';
	CompBuffer[3] = (char)0xA5;
	CompBuffer[4] = 'H';
	CompBuffer[5] = 'd';
	CompBuffer[6] = 'r';
	CompBuffer[7] = 0x00;

	// verify if file exists
	if (strlen(pFileName) > 0)
	{
		pStream = fopen(pFileName, "r");
		if (pStream != NULL)
		{
			result = fseek(pStream, -24, SEEK_END);
			if (result == 0)
			{
				result = fread(ReadBuffer, sizeof( char ), sizeof(ReadBuffer), pStream);
				if (result > 0)
				{
					if (memcmp(CompBuffer, ReadBuffer, strlen(CompBuffer)) == 0)
						*pReturn = AAFTrue;
				}
			}
			fclose(pStream);
		}
		else
		{
			rc = AAFRESULT_FILE_NOT_FOUND;
		}
	}
	else
	{
		rc = AAFRESULT_NULL_PARAM;
	}
			
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

	CComInitialize	comInit;
	Aaf2Omf			AAFMain;
	Omf2Aaf			OMFMain;

	aafBool			bIsOMFFile = AAFFalse;

#ifdef macintosh
	argc = ccommand(&argv);	// calls up a command line window
#endif 

	hr = UTLInitFileIO();
	if (FAILED(hr))
		return UTLEcFromHr(hr);

	hr = InitGlobalVars();
	if (FAILED(hr))
		return UTLEcFromHr(hr);

	gpGlobals->pProgramName = baseName(argv[0]);

	UTLstdprintf("%s: Version 0.01.00\n", gpGlobals->pProgramName);
	hr = GetUserInput(argc, argv);
	if (FAILED(hr))
	{
		Usage();
		return UTLEcFromHr(hr);
	}
	hr = IsOMFFile(gpGlobals->sInFileName, &bIsOMFFile);
	if (FAILED(hr))
	{
		Usage();
		return UTLEcFromHr(hr);
	}
	if (bIsOMFFile)
	{
		// Convert OMF to AAF
		OMFMain.ConvertFile();
	}
	else
	{
		// Convert AAF to OMF
		AAFMain.ConvertFile();
	}

	// We are done, just
	DisplaySummary();

	// clean up memory
	if(gpGlobals)
		UTLMemoryFree(gpGlobals);

	// close fileio
	hr = UTLExitFileIO();

	return UTLEcFromHr(hr);
}