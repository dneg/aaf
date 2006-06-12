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


#include <string.h>
#include <wchar.h>
#include "aafErr.h"

#include "OMSet.h"

struct errorTableTag {
  aafUInt16 _code;
  wchar_t* _name;
  wchar_t* _desc;
} errorTable[] = {
#define AAF_DEFINE_ERROR(name, val, desc) \
  {val, L ## "AAFRESULT_" L ## #name, L ## desc},
#include "AAFMetaResult.h"
};

// Map low 16-bits of error code to table index
OMSet<aafUInt16, aafUInt16> errors;

size_t errorTableEntryCount = sizeof(errorTable) / sizeof(errorTable[0]);

static bool isAAFError(AAFRESULT code)
{
  bool result = false;

  aafUInt16 facility = (aafUInt16)((code >> 16) & 0x1fff);
  if (facility == 0x0012) {
    result = true;
  }
  return result;
}

static bool findEntry(size_t& index, aafUInt16 code)
{
  if (errors.count() == 0) {
    // map is empty - initialize it
    for (aafUInt16 i = 0; i < errorTableEntryCount; i++) {
      errors.insert(errorTable[i]._code, i);
    }
  }

  // lookup
  bool result = false;
  aafUInt16 k;
  if (errors.find(code, k)) {
    index = k;
    result = true;
  }
  return result;
}

size_t length(size_t index)
{
  size_t result;
  if (wcscmp(errorTable[index]._desc, L"") == 0) {
    result  = (wcslen(errorTable[index]._name) + 1) * sizeof(wchar_t);
  } else {
    result  = (wcslen(errorTable[index]._name) +
               wcslen(errorTable[index]._desc) + 
               wcslen(L" - ") + 1) * sizeof(wchar_t);
  }
  return result;
}


HRESULT findEntry(AAFRESULT result, size_t& index)
{
  HRESULT hr;

  if (isAAFError(result)) {
    aafUInt16 x = (aafUInt16)result;

    if (findEntry(index, x)) {
      hr = AAFRESULT_SUCCESS;
    } else {
      hr = AAFRESULT_RESULT_NOT_RECOGNIZED;
    }
  } else {
    hr = AAFRESULT_RESULT_NOT_AAF;
  }
  return hr;
}

AAFRESULT ResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize)
{
  size_t i;
  HRESULT hr = findEntry(result, i);

  if (AAFRESULT_SUCCEEDED(hr)) {
    *pResultTextSize = length(i);
  }
  return hr;
}

AAFRESULT ResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize)
{
  size_t i;
  HRESULT hr = findEntry(result, i);

  if (AAFRESULT_SUCCEEDED(hr)) {
    size_t len = length(i);
    if (resultTextSize >= len) {
      if (wcscmp(errorTable[i]._desc, L"") == 0) {
        wcscpy(pResultText, errorTable[i]._name);
      } else {
        wcscpy(pResultText, errorTable[i]._name);
        wcscat(pResultText, L" - ");
        wcscat(pResultText, errorTable[i]._desc);
      }
    } else {
      hr = AAFRESULT_SMALLBUF;
    }
  }
  return hr;
}

static char    *localErrorStrings[300];
/* A 75-column ruler
         111111111122222222223333333333444444444455555555556666666666777777
123456789012345678901234567890123456789012345678901234567890123456789012345
*/

/************************
 * Function: aafGetErrorString
 *
 * 		Given an error code, returns an error string.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Always returns an error string of some sort, although
 *		it may just be "unknown error code".
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
char *aafGetErrorString(
		aafErr_t code)
{
	if (code < 300 && code >= 0 && localErrorStrings[0] != '\0')
		return (localErrorStrings[code]);
	else
		return ("AAF_ERR: Unknown error code");
}


/************************
 * Function: aafRegErr			(CALLED BY MACROS)
 *
 * 	Internal routine to log an error.  Should not used anymore.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void aafRegErr(
			aafErr_t		/* msgcode */)
{
}

#ifdef AAF_ERROR_TRACE
/************************
 * Function: aafRegErrDebug		(CALLED BY MACROS)
 *
 * 	Debug version of omfRefErr.  This routine is used ONLY if AAF_ERROR_TRACE
 *		is enabled, and prints information about the error raised to a stack trace
 *		buffer, which may be printed.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		<none>.
 *
 * Possible Errors:
 *		<none>.
 */
void aafRegErrDebug(
			aafErr_t		ec,
			char			*filename,
			aafInt16		line)
{
	if (file->isValid())
	{
		if(file->_stackTrace != NULL)
			sprintf(file->_stackTrace, "Error Raised <%1d>%s @ %d in '%s'\n", ec, aafGetErrorString(ec), line, filename);
	}
}

/************************
 * Function: aafReregErrDebug	(CALLED BY MACROS)
 *
 * 	Called when an error is reraised (as opposed to being
 *		initially raised).  This is to allow errors being propagated
 *		up the stack looking different in the stack trace from
 *		Errors initially raised.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		<none>.
 */
void aafReregErrDebug(
			aafErr_t msgcode,
			char *filename,
			aafInt16 line)
{
	char	buf[256];
	aafInt32	left;
		
	if (file->isValid())
	{
		sprintf(buf, "   Error <%1d>%s propagated  @ %d in '%s'\n", msgcode,
					aafGetErrorString(msgcode), line, filename);
		left = file->_stackTraceSize - (strlen(file->_stackTrace) + 1);
		strncat(file->_stackTrace, buf, left);
	}
}


/************************
 * Function: aafPrintStackTrace
 *
 * 	Put this routine in the top-level exception block of the test
 *		application to print out the trace buffer when an error occurs.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		<none>.
 *
 * Possible Errors:
 *		<none>.
 */
//!!!void aafPrintStackTrace(
//			AAFFile * file)
//{
  // if (file != NULL)
	//  {
//		 printf("***Stack trace from last error****\n");
//		 printf("%s\n", file->_stackTrace);
//	  }
//}
#endif
