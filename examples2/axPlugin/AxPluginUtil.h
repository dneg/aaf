#ifndef __AxPluginUtil_h__
#define __AxPluginUtil_h__

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
//=---------------------------------------------------------------------=

//
// Miscellaneous items.
//

#include <AAFTypes.h>

bool operator==( const aafUID_t& uidL, const aafUID_t& uidR );

inline bool operator!=( const aafUID_t& uidL, const aafUID_t& uidR )
{
	return !(uidL == uidR);
}

#if !defined(OS_WINDOWS)
bool operator==( const tagGUID& uidL, const tagGUID& uidR );

inline bool operator!=( const tagGUID& uidL, const tagGUID& uidR )
{
	return !(uidL == uidR);
}
#endif

#endif
