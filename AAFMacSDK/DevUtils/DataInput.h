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

#ifndef _DATAINPUT_H
#define _DATAINPUT_H

#ifdef __cplusplus
extern "C" {
#endif


/* Redefine the normal "main" entrypoint to "Main" so that we can
   re-implement main to process a command line and redirect stdout
   before calling the renamed "Main" with the correct values for 
   argc and argv */
#define main Main


/* The prototype for the newly defined (renamed) global function. */
int Main(int argc, char* argv[]);


/* The following routine are obsolete. */
void getInputData(int *pargc, char **argv, char *inputFile);
void cleanUpInputData(int argc, char **argv);


#ifdef __cplusplus
}
#endif

#endif
