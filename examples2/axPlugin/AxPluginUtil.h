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

#include <AxEx.h>

#include <AxTypes.h>

#include <map>

//
// Miscellaneous items.
//

// FIXME - This is similar to AxPluginSpecifierMap.  If a generic means
// to implement GetMaxValBufSize() is determined, then a generic implementation
// is possible.

class AxPluginNameMap
	: public std::map<aafUID_t, AxString>
{
public:

	typedef std::map<aafUID_t, AxString>::iterator IterType;
	typedef std::pair<aafUID_t, AxString>		   PairType;
	typedef AxCharTraits                           CharTraits;

	// Use this to double check if a uid is in the map before
	// using operator[]().
	bool IsFound( const aafUID_t& uid );

	// Returns largest string size in characters.
	int GetMaxStringSize();

	// Return size of buffer (in bytes) required to store largest string.
	// Including null terminator required by C string.
	int GetMaxCBufferSize();

};

// Call object OBJ method NAME using args ARGS.
// Catch hresult exception and return error value, or
// any other exception and return error code, or return
// S_OK on success.
// Use to implement COM interfaces that wrap an underlying
// implementation that, generally, has identical method
// name and arguments but does not return an error code.

#define AX_PLUGIN_TRY( OBJ, NAME, ARGS )		\
	try {										\
		(OBJ)->NAME ARGS ;						\
	}											\
	catch( const AxExHResult& ex ) {			\
		return ex.getHResult();					\
	}											\
	catch( ... ) {								\
		return AAFRESULT_UNEXPECTED_EXCEPTION;	\
	}											\
	return S_OK;

#endif
