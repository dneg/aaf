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

// AAFShellExtDebug.h

#include <PidlMgr.h>

void _AAFShellExtLog( const char* msg );
void _AAFShellExtLog( const wchar_t* msg );
void _AAFShellExtLog( int line );
void _AAFShellExtPidlLog( LPCITEMIDLIST pidl );

#if 0
#define AAFShellExtLog( msg ) \
{_AAFShellExtLog( __FILE__ );  _AAFShellExtLog( __LINE__); _AAFShellExtLog(msg);}

#define AAFShellExtPidlLog( pidl) \
{_AAFShellExtLog( __FILE__ ); _AAFShellExtLog( __LINE__); _AAFShellExtPidlLog( pidl );}
#elif 0
#define AAFShellExtLog( msg ) _AAFShellExtLog( msg )
#define AAFShellExtPidlLog( pidl) _AAFShellExtPidlLog( pidl )
#else
#define AAFShellExtLog( msg ) 
#define AAFShellExtPidlLog( pidl) 
#endif

