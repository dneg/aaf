#ifndef __AxKLV_h__
#define __AxKLV_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxKLVData : public AxObject {

public:
	AxKLVData( IAAFKLVDataSP spIaafKLVData );
	virtual ~AxKLVData();

	void Initialize( aafUID_t  key,
					 aafUInt32 length,
					 const aafDataBuffer_t pValue );
                     
    aafUID_t GetKey();
    void GetValue( aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead);
    void SetValue( aafUInt32 valueSize, aafDataBuffer_t pValue );

    template <typename ParamType>
    void GetValue( ParamType& val )
    {
        ParamType tmp;
        aafUInt32 bytesRead;
        GetValue( sizeof(tmp), reinterpret_cast<aafDataBuffer_t>(&tmp), &bytesRead );
        if ( sizeof(tmp) == bytesRead ) {
            val = tmp;
        }
        else {
            throw AxEx( L"size mismatch in AxKLVData::GetValue()" );      
        }
      }
      
    template <typename ParamType>
    void SetValue( ParamType val )
    {
        ParamType tmp;
        SetValue( sizeof(tmp), reinterpret_cast<aafDataBuffer_t>(&tmp) );
    }

	operator IAAFKLVDataSP ()
	{ return _spIaafKLVData; }

private:
	AxKLVData();
	AxKLVData( const AxKLVData& );
	AxKLVData& operator=( const AxKLVData& );

	IAAFKLVDataSP _spIaafKLVData;
};




#endif
