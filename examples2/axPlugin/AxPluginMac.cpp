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
//=---------------------------------------------------------------------=

#include <AAFPlatform.h>

#if defined(OS_MACOS)

#include <MacTypes.h> 

// The NullEssenceCodecRegister global constructor is not called
// when the library loads on Mac (as it is on Irix, Linux, and Windows).
// When that issue is resolved these init/fini routines can be removed.
// Note, the project's "PPC Link" settings with also have to change when
// these init/fini routines are removed.

void CAxNullEssenceCodecInit();
void CAxNullEssenceCodecFini();

OSErr pascal AxMacLibInit()
{
	CAxNullEssenceCodecInit();
	return noErr;
}

void pascal AxMacLibFini()
{
	CAxNullEssenceCodecFini();
}

#endif
