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

#include <AAF.h>
#include <AAFSmartPointer.h>

#include <AAFCodecDefs.h>

#include <sstream>

class CmdState;

const int MULTI_GEN_AUDIO_SAMPLE_BPP  =      16;
const int MULTI_GEN_VIDEO_WIDTH       = (720/4);
const int MULTI_GEN_VIDEO_HEIGHT      = (486/4);

const int MULTI_GEN_VIDEO_RATE_NUM = 30000;
const int MULTI_GEN_VIDEO_RATE_DEN =  1001;
const int MULTI_GEN_AUDIO_RATE_NUM = 22500;
const int MULTI_GEN_AUDIO_RATE_DEN =     1;
const int MULTI_GEN_EDIT_RATE_NUM  =   100;
const int MULTI_GEN_EDIT_RATE_DEN  =     1;

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      aafSearchCrit_t* criteria,
		      IAAFSmartPointer<IAAFMob>& mob );

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMob>& mob );

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMasterMob>& mob );

bool find_comment( IAAFSmartPointer<IAAFMob> mob,
		   const wchar_t* category,
		   const wchar_t* comment,
		   IAAFSmartPointer<IAAFTaggedValue>& taggedValue );

// Throws UsageEx if conversion fails.
unsigned long int ToULong( const char* str );

// Caller takes ownership of returned pointer.
wchar_t* ToWideString( const char* str );

// wide string compare
bool wstrcmp( const wchar_t* a, const wchar_t* b );

template <class T>
inline IUnknown** ToIUnknown( T** p )
{
  return reinterpret_cast<IUnknown**>(p);
}

template <class T>
inline void** ToVoid( T** p )
{
  return reinterpret_cast<void**>(p);
}

template <class MobType>
void get_mob_throw_if_not_found( CmdState& state,
				 const char* name,
				 IAAFSmartPointer<MobType>& mob )
{
  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );
    
  if ( !get_mob_by_name( header, name, mob ) ) {
    stringstream anError;
    anError << name << " not found";
    throw TestFailedEx( anError.str() );
  }
}
