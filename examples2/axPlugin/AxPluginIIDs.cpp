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


// Include the definitions for all of the interface IID's.
// This defines the IID's for the AAF plugin interfaces.  This
// should only be included in one of the library's files.

#include <AAFPlugin.h>

#if defined(OS_MACOS)
// FIXME - Why is this required on mac, but not on Windows?!?!
#define __IID_DEFINED__
#endif

#include <AAFPlugin_i.c>
