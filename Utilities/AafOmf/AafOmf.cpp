// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

#ifdef macintosh
	#include <console.h>
#endif 
namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

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
#include "aafclassdefuids.h"
#include "EffectTranslate.h"

//#include "AAFUtils.h"
//AAFRESULT aafMobIDNew(aafMobID_t *mobID);
//AAFRESULT aafMobIDFromMajorMinor(
//        aafUInt32	major,
//		aafUInt32	minor,
//		aafMobID_t *mobID);     /* OUT - Newly created Mob ID */

// ============================================================================
// Global Variables and functions
// ============================================================================
AafOmfGlobals*	gpGlobals = NULL;
// ============================================================================
// Delete a file given its  name
// ============================================================================
int deleteFile(char* fileName)
{

	return(remove(fileName));

}

static char* baseName(char* fullName)
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

void AUIDtoString(aafUID_t *uid, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}

void MobIDtoString(aafMobID_t *mobID, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			mobID->Data1, mobID->Data2, mobID->Data3, (int)mobID->Data4[0],
			(int)mobID->Data4[1], (int)mobID->Data4[2], (int)mobID->Data4[3], (int)mobID->Data4[4],
			(int)mobID->Data4[5], (int)mobID->Data4[6], (int)mobID->Data4[7]);
}



struct SMPTELabel
{
	aafUInt32	MobIDMajor;
	aafUInt32	MobIDMinor;
	aafUInt8	oid;
	aafUInt8	size;
	aafUInt8	ulcode;
	aafUInt8	SMPTE;
	aafUInt8	Registry;
	aafUInt8	unused;
	aafUInt16	MobIDPrefix;
};

union label
{
	aafMobID_t			mobID;
	struct SMPTELabel	smpte;
};

AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
	union label		aLabel;
	
	aLabel.smpte.oid = 0x06;
	aLabel.smpte.size = 0x0E;
	aLabel.smpte.ulcode = 0x2B;
	aLabel.smpte.SMPTE = 0x34;
	aLabel.smpte.Registry = 0x02;
	aLabel.smpte.unused = 0;
	aLabel.smpte.MobIDPrefix = 42;		// Means its an OMF Uid

	aLabel.smpte.MobIDMajor = major;
	aLabel.smpte.MobIDMinor = minor;

	*mobID = aLabel.mobID;
	return(AAFRESULT_SUCCESS);
}

// ============================================================================
// InitGlobalVars
// 
//		Allocates memory and intializes the global variables
//
// ============================================================================
static HRESULT InitGlobalVars( void )
{
	gpGlobals = new AafOmfGlobals;
	if (gpGlobals == NULL)
		return AAFRESULT_NOMEMORY;

	gpGlobals->bAAFFileOpen = AAFFalse;
	gpGlobals->bConvertAllObjects = AAFFalse;
	gpGlobals->bCreateTOCFile = AAFFalse;
	gpGlobals->bDefFile = AAFFalse;
	gpGlobals->bLogFile = AAFFalse;
	gpGlobals->pLogger = NULL;
	gpGlobals->bOMFFileOpen = AAFFalse;
	gpGlobals->bVerboseMode = AAFFalse;
	gpGlobals->numIndents = 0;
	gpGlobals->pProgramName = NULL;
	gpGlobals->bDeleteOutput  = AAFTrue;
	gpGlobals->bConvertAAFFile  = AAFFalse;

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
static void Usage( void )
{
	printf("\n*******************\n\n");
	printf("%s : OMF/AAF file conversion Version 0.01.00\n\n", gpGlobals->pProgramName);
	printf("Usage: \n");
	printf("%s [-v] [-s] [-p logfile] [-d deffile] [-t tocfile] <infile> [outfile]\n\n", gpGlobals->pProgramName);
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
						gpGlobals->bDeleteOutput = AAFFalse;
					}
					else if ( strcmp(lc, "omf") == 0 ) 
					{
						gpGlobals->bConvertAAFFile = AAFTrue;
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
// IsOMFFile
//  This function returns AAFRESULT_SUCCESS if the given file is an OMF file.
//  and the error code otherwise.
//
// ============================================================================
HRESULT IsOMFFile (char * pFileName )
{
	// verify if file exists
	gpGlobals->pLogger->Log( kLogInfo, "Verifying that \"%s\" is an OMF file\n", 
		pFileName );
	if ( strlen(pFileName) == 0)
	{
		return  AAFRESULT_NULL_PARAM;
	}

	FILE *pStream = fopen(pFileName, "r");
	if( pStream == NULL )
	{
		return AAFRESULT_BADOPEN;
	}

	HRESULT rc = AAFRESULT_FILE_NOT_OMF;
	int result = fseek(pStream, -24, SEEK_END);
	if( result == 0 )
	{	
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
		result = fread(ReadBuffer, sizeof( char ), sizeof(ReadBuffer), pStream);
		if (result > 0 && memcmp(CompBuffer, ReadBuffer, strlen(CompBuffer)) == 0)
		{
				rc = AAFRESULT_SUCCESS;
		}
	}

	fclose(pStream);
	return rc;
}

void RegisterCodecProperties(AafOmfGlobals *globals, OMF2::omfSessionHdl_t OMFSession)
{
	OMFCheck	OMFError;

	// To get the CDCI codec related properties we first reister them in OMF
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ComponentWidth", OMClassCDCI, 
									   OMF2::OMVersionType, OMF2::kPropRequired, 
									   &(globals->omCDCIComponentWidth));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "HorizontalSubsampling", OMClassCDCI, 
									   OMF2::OMBoolean, OMF2::kPropRequired, 
									   &(globals->omCDCIHorizontalSubsampling));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ColorSiting", OMClassCDCI, 
									   OMF2::OMBoolean, OMF2::kPropRequired, 
									   &(globals->omCDCIColorSiting));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "BlackReferenceLevel", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIBlackReferenceLevel));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "WhiteReferenceLevel", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIWhiteReferenceLevel));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "ColorRange", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIColorRange));
	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither, 
									   "PaddingBits", OMClassCDCI, 
									   OMF2::OMInt32, OMF2::kPropRequired, 
									   &(globals->omCDCIPaddingBits));
}

#ifndef COMPILE_AS_DLL
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
	ExtendedAaf2Omf		AAFMain;
	ExtendedOmf2Aaf		OMFMain;

#ifdef macintosh
	argc = ccommand(&argv);	// calls up a command line window
#endif 

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

	try
	{
		AAFCheck check;
		if (gpGlobals->bConvertAAFFile)
		{
			// User indicated input file must be an AAF 
			// Convert AAF to OMF
			AAFMain.ConvertFile();
		}
		else
		{
			// User indicated Input file must be an OMF file
			// Conert OMF to AAF
			check = IsOMFFile(gpGlobals->sInFileName);
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

	return( hr );
}
#endif

