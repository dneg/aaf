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

#include "stdafx.h"
#include "AAFShellExtDebug.h"

#include <stdio.h>
#include <stdlib.h>

namespace {
const char* _logFile = "c:\\tmp\\aafShellLog.txt";
}

void _AAFShellExtLog( const char* msg )
{
	FILE *fp = fopen( _logFile, "a+" );

	if ( fp ) {
		fprintf( fp, " %s ", msg );
	}

	fclose( fp );
}

void _AAFShellExtLog( int i )
{
	FILE *fp = fopen( _logFile, "a+" );

	if ( fp ) {
		fprintf( fp, " %d ", i);
	}

	fclose( fp );
}

void _AAFShellExtLog( const wchar_t* msg )
{
	FILE *fp = fopen( _logFile, "a+" );

	if ( fp ) {
		fwprintf( fp, L" %s ", msg );
	}

	fclose( fp );
}

void _AAFShellExtPidlLog( LPCITEMIDLIST pidl  )
{

	TCHAR szPath[MAX_PATH];

	if( SHGetPathFromIDList(pidl, szPath) ) {
		AAFShellExtLog( szPath );
	}

}
