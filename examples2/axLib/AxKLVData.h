#ifndef __AxKLV_h__
#define __AxKLV_h__

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


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxKLVData : public AxObject {

public:
	AxKLVData( IAAFKLVDataSP& spIaafKLVData );
	virtual ~AxKLVData();

	void Initialize( aafUID_t  key,
					 aafUInt32 length,
					 const aafDataBuffer_t pValue );
	
	operator IAAFKLVDataSP ()
	{ return _spIaafKLVData; }

private:
	AxKLVData();
	AxKLVData( const AxKLVData& );
	AxKLVData& operator=( const AxKLVData& );

	IAAFKLVDataSP _spIaafKLVData;
};




#endif
