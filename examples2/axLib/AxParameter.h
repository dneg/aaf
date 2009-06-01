#ifndef __AxParameter_h__
#define __AxParameter_h__

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

  aafUInt32 GetValueBufLen();

  IAAFTypeDefSP GetTypeDefinition();

  void SetValue( aafUInt32 valueSize, aafDataBuffer_t pValue );

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
  
  void Initialize( IAAFParameterDefSP spParameterDef, IAAFInterpolationDefSP spInterpolation );

  void AddControlPoint( IAAFControlPointSP spControlPoint );
  void RemoveControlPointAt( aafUInt32 index );
  IEnumAAFControlPointsSP GetControlPoints();
  aafUInt32 CountControlPoints();
  IAAFControlPointSP GetControlPointAt( aafUInt32 index );

  IAAFInterpolationDefSP GetInterpolationDefinition();
  aafInt32 GetValueBufLen();
  void GetInterpolatedValue( aafRational_t inputValue, aafInt32 valueSize, aafDataBuffer_t pValue, aafInt32* bytesRead );

  template <typename ParamType>
  void GetValue( aafRational_t inputValue, ParamType& val )
  {
    ParamType tmp;
    aafUInt32 bytesRead;
    GetInterpolatedValue( inputValue, sizeof(tmp), reinterpret_cast<aafDataBuffer_t>(&tmp), &bytesRead );
    if ( sizeof(tmp) == bytesRead ) {
      val = tmp;
    }
    else {
      throw AxEx( L"size mismatch in AxVaryingValue::GetValue()" );      
    }
  }

  inline operator IAAFVaryingValueSP ()
    { return _spIaafVaryingValue; }

 private:
  AxVaryingValue();
  AxVaryingValue( const AxVaryingValue& );
  AxVaryingValue& operator=( const AxVaryingValue& );

  IAAFVaryingValueSP _spIaafVaryingValue;
};

//=---------------------------------------------------------------------=

class AxControlPoint : public AxObject {
 public:
  AxControlPoint( IAAFControlPointSP spIaafControlPoint );
  virtual ~AxControlPoint();

  void Initialize( IAAFVaryingValueSP spIaafVaryingValue, aafRational_constref time, aafUInt32 valueSize, aafDataBuffer_t buffer );
  aafRational_t GetTime();
  aafEditHint_t GetEditHint();
  aafUInt32 GetValueBufLen();
  aafUInt32 GetValue( aafUInt32 valueSize, aafDataBuffer_t buffer );
  void SetTime( aafRational_t time );
  void SetEditHint( aafEditHint_t editHint );
  IAAFTypeDefSP GetTypeDefinition();
  void SetValue( aafUInt32 valueSize, aafDataBuffer_t buffer );
  
  inline operator IAAFControlPointSP ()
    { return _spIaafControlPoint; }

 private:
  AxControlPoint();
  AxControlPoint( const AxControlPoint& );
  AxControlPoint& operator=( const AxControlPoint& );

  IAAFControlPointSP _spIaafControlPoint;
};

#endif
