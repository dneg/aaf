//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Errors.h>
#include <Files.h>
#include <Aliases.h>
#include <Resources.h>
#include <Memory.h>
#include <Script.h>
#include <ErrMgr.h>

static void usage (const char * command);
static void checkResult(OSErr result);
static OSErr pathToFSSpec(const char *path, FSSpec * spec);
static OSErr makeAliasFileSpec(FSSpec *destFolderSpec, ConstStrFileNameParam aliasName, FSSpec *newSpec);
static OSErr MakeRelativeAliasFile(FSSpec *targetFile, FSSpec *aliasDest);

void main(int argc, char *argv[], char */*envp[]*/)

{
  int result = noErr;
  const char * targetPath = NULL;
  const char * destFolderPath = NULL;
  FSSpec targetSpec, destFolderSpec, aliasSpec;
  
  memset(&targetSpec, 0, sizeof(targetSpec));
  memset(&destFolderSpec, 0, sizeof(destFolderSpec));
  memset(&aliasSpec, 0, sizeof(aliasSpec));
  

  // There must be exactly "4" arguments after the tool name.
  if (5 != argc)
    usage(argv[0]);

  for (int i = 1; i < argc; i++)
  {
    // Extract the target file path.  
  	if (0 == strcmp("-t", argv[i]) || 0 == strcmp("--target", argv[i]))
  	  targetPath = argv[++i];
    // Extract the alias file path.
  	else if (0 == strcmp("-a", argv[i]) || 0 == strcmp("--alias", argv[i]))
  	  destFolderPath = argv[++i];
  }

  // Must have arguments for both the target and alias paths.
  if (!targetPath || !destFolderPath)
    usage(argv[0]);

  InitErrMgr(NULL, NULL, true);

  // Convert the given target path into a file specification. This file must
  // exist!
  checkResult(pathToFSSpec(targetPath, &targetSpec));
  
  // Covert the given destination alias folder path into a spec.
  checkResult(pathToFSSpec(destFolderPath, &destFolderSpec));
  
  // Create the alias file spec.
  checkResult(makeAliasFileSpec(&destFolderSpec, targetSpec.name, &aliasSpec));
  
  
  // Attempt to create the relative alias file.
  checkResult(MakeRelativeAliasFile(&targetSpec, &aliasSpec));
  
  // Normal termination.
  CloseErrMgr();
  exit (0);
}


void usage (const char * command)
{
  fprintf (stderr, "Usage: %s -t targetPath -a destFolderPath\n", command);
  exit (1);
}


void checkResult(OSErr result)
{
  if (noErr != result)
  {
    char errMsg[255];
    GetSysErrText(result, errMsg);
    fprintf(stderr, "%s\n", errMsg);
    
    if (0 > result)
      result *= -1;
      
    CloseErrMgr();
    exit (result);
  }  
}


OSErr pathToFSSpec(const char *path, FSSpec * spec)
{
  OSErr result = noErr;
  AliasHandle alias = NULL;
  Boolean wasChanged = false;

  result = NewAliasMinimalFromFullPath(strlen(path), path, NULL, NULL, &alias);
  if (noErr != result)
    goto cleanup;
  
  result = ResolveAlias(NULL, alias, spec, &wasChanged);
  if (noErr != result)
    goto cleanup;

cleanup:
  if (NULL != alias)
    DisposeHandle((Handle)alias);
    
  return result;
}

OSErr makeAliasFileSpec(FSSpec *destFolderSpec, ConstStrFileNameParam fileName, FSSpec *newSpec)
{
  // We need to find the dirID for destFolderSpec. This will be the parID in newSpec.
  OSErr result = noErr;
  
  // Check to see if the given fileSpec points to a file or directory.
  CInfoPBRec cInfo;

  memset(&cInfo, 0, sizeof(cInfo));
  cInfo.dirInfo.ioVRefNum = destFolderSpec->vRefNum;
  cInfo.dirInfo.ioDrDirID = destFolderSpec->parID;
  cInfo.dirInfo.ioNamePtr = destFolderSpec->name;

  result = PBGetCatInfoSync(&cInfo);
  if (noErr != result)
    return result;

  // If the destination folder spec is not a directory then
  if (0 == ((1 << ioDirFlg) & cInfo.dirInfo.ioFlAttrib))
    return dirNFErr; // return Directory name not found. 
  
  // dupFNErr;
  newSpec->vRefNum = destFolderSpec->vRefNum;
  newSpec->parID = cInfo.dirInfo.ioDrDirID;
  memcpy(newSpec->name, fileName, ((size_t)fileName[0]) + 1);
  
  return noErr;
}


// The following code was copied from the Apple tn1188.pdf.
// Modified for easier debugging: use multi-line if statements etc.

/* MakeRelativeAliasFile creates a new alias file located at
   aliasDest referring to the targetFile. relative path
   information is stored in the new file. */
OSErr MakeRelativeAliasFile(FSSpec *targetFile, FSSpec *aliasDest)
{
  FInfo fndrInfo;
  AliasHandle theAlias;
  Boolean fileCreated;
  short rsrc;
  OSErr err;
  
    /* set up locals */
  memset(&fndrInfo, 0, sizeof(fndrInfo));
  theAlias = NULL;
  fileCreated = false;
  rsrc = -1;
  
    /* set up our the alias' file information */
  err = FSpGetFInfo(targetFile, &fndrInfo);
  if (err != noErr)
  	goto bail;
  if (fndrInfo.fdType == 'APPL')
    fndrInfo.fdType = kApplicationAliasType;
  fndrInfo.fdFlags = kIsAlias; /* implicitly clear the inited bit */
    /* create the new file */
  FSpCreateResFile(aliasDest, 'TEMP', 'TEMP', smSystemScript);
  if ((err = ResError()) != noErr)
  	goto bail;
  fileCreated = true;
    /* set the file information or the new file */
  err = FSpSetFInfo(aliasDest, &fndrInfo);
  if (err != noErr)
  	goto bail;
    /* create the alias record, relative to the new alias file */
  err = NewAlias(aliasDest, targetFile, &theAlias);
  if (err != noErr)
  	goto bail;
    /* save the resource */
  rsrc = FSpOpenResFile(aliasDest, fsRdWrPerm);
  if (rsrc == -1)
  { 
  	err = ResError(); 
  	goto bail; 
  }
  UseResFile(rsrc);
  AddResource((Handle) theAlias, rAliasType, 0, aliasDest->name);
  if ((err = ResError()) != noErr)
  	goto bail;
  theAlias = NULL;
  CloseResFile(rsrc);
  rsrc = -1;
  if ((err = ResError()) != noErr)
  	goto bail;
    /* done */
  return noErr;

bail:
  if (rsrc != -1)
  	CloseResFile(rsrc);
  if (fileCreated)
  	FSpDelete(aliasDest);
  if (theAlias != NULL)
  	DisposeHandle((Handle) theAlias);
  return err;
}
