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
