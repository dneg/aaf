//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFPlatform.h"


#if defined(OS_DARWIN)
// Do not declare the following functions when building with
// CodeWarrior 9 or later.
#if defined(COMPILER_MWERKS) && (__MWERKS__ < 0x3200)
int wcslen(const wchar_t*aString);
int wcscmp(const wchar_t*string1, const wchar_t*strign2);
wchar_t *wcscpy(wchar_t*stringDest, const wchar_t*stringSrc);
wchar_t *wcscat(wchar_t*stringDest, const wchar_t*stringSrc);
int wcsncmp(const wchar_t*stringDest, const wchar_t*stringSrc, const int length);
//#include <stdlib.h>
//int wcstombs(char *s, wchar_t *pwcs, unsigned long n);
//int mbstowcs(wchar_t *s, char *pwcs, unsigned long n);
#else
#include <wchar.h>
#endif
#else
#include <wchar.h>
//#undef _MSL_NO_WCHART_C_SUPPORT
#endif

