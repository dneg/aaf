#ifndef __AxParameter_h__
#define __AxParameter_h__

//=---------------------------------------------------------------------=
//
// $Id$
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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxBaseObj.h"
#include "AxObject.h"


class AxParameter : public AxObject {
 public:
  AxParameter( IAAFParameterSP spIaafParameter );
  virtual ~AxParameter();

  IAAFParameterDefSP GetParameterDefinition();
  
  inline operator IAAFParameterSP ()
    { return _spIaafParameter; }

 private:
  AxParameter();
  AxParameter( const AxParameter& );
  AxParameter& operator=( const AxParameter& );

  IAAFParameterSP _spIaafParameter;
};

//=---------------------------------------------------------------------=

class AxConstantValue : public AxParameter {
 public:
  AxConstantValue( IAAFConstantValueSP spIaafConstantValue );
  virtual ~AxConstantValue();

  void Initialize( IAAFParameterDefSP spParameterDef,
		   aafUInt32 valueSize,
		   aafDataBuffer_t pValue );

  void GetValue( aafUInt32 valueSize,
		 aafDataBuffer_t pValue,
		 aafUInt32* bytesRead );

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
      throw AxEx( L"size mismatch in AxConstantValue::GetValue()" );      
    }
  }


  inline operator IAAFConstantValueSP ()
    { return _spIaafConstantValue; }

 private:
  AxConstantValue();
  AxConstantValue( const AxConstantValue& );
  AxConstantValue& operator=( const AxConstantValue& );

  IAAFConstantValueSP _spIaafConstantValue;
};

//=---------------------------------------------------------------------=

class AxVaryingValue : public AxParameter {
 public:
  AxVaryingValue( IAAFVaryingValueSP spIaafVaryingValue );
  virtual ~AxVaryingValue();

  inline operator IAAFVaryingValueSP ()
    { return _spIaafVaryingValue; }

 private:
  AxVaryingValue();
  AxVaryingValue( const AxVaryingValue& );
  AxVaryingValue& operator=( const AxVaryingValue& );

  IAAFVaryingValueSP _spIaafVaryingValue;
};

#endif
