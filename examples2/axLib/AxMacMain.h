#ifndef __AxMacMain_h__
#define __AxMacMain_h__

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

#if defined( OS_MACOS )

// This will get the IID_IUnknown definition (amoung others).
// FIXME - There is probably a better place for this than here.
#include <initguid.h>
#include <coguid.h>

// Include this to avoid polluting code with the conditional include
// below.  Application is still responsible for linking with
// AAFMacSDK/DevUils/DataInput.cpp and add the include path for DataInput.h
// 
// **** DataInput.h redefines main() !!! ****

#include <DataInput.h>

#endif

#endif
