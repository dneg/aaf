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

#include <AxHrMap.h>
#include <AxUtil.h>
#include <AxEx.h>

#include <iostream>

int main( int argc, char** argv )
{
	using namespace std;

	if ( argc != 2 ) {
		cout << "axHrMap hresult_value" << endl;
		return -1;
	}

	try {
		long hresult = AxStringUtil::strtoul( argv[1] );
	
		wcout << AxHrMap::getInstance().getStr( hresult );
	}

	catch( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}
	catch( ... ) {
		cout << "unexpected exception" << endl;
	}

	return 0;
}
