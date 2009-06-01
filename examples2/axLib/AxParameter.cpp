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

#include "AxParameter.h"

//=---------------------------------------------------------------------=

AxParameter::AxParameter( IAAFParameterSP spIaafParameter )
  : AxObject( AxQueryInterface<IAAFParameter, IAAFObject>( spIaafParameter ) ),
    _spIaafParameter( spIaafParameter )
{}

AxParameter::~AxParameter()
{}

IAAFParameterDefSP AxParameter::GetParameterDefinition()
{
  IAAFParameterDefSP spParamDef;
  CHECK_HRESULT( _spIaafParameter->GetParameterDefinition( &spParamDef ) );
  return spParamDef;
}

//=---------------------------------------------------------------------=

AxConstantValue::AxConstantValue( IAAFConstantValueSP spIaafConstantValue )
  : AxParameter( AxQueryInterface<IAAFConstantValue, IAAFParameter>( spIaafConstantValue ) ),
    _spIaafConstantValue( spIaafConstantValue )
{}

AxConstantValue::~AxConstantValue()
{}

void AxConstantValue::Initialize( IAAFParameterDefSP spParameterDef,
				  aafUInt32 valueSize,
				  aafDataBuffer_t pValue )
{
  CHECK_HRESULT( _spIaafConstantValue->Initialize( spParameterDef, valueSize, pValue ) );
}

void AxConstantValue::GetValue( aafUInt32 valueSize,
				aafDataBuffer_t pValue,
				aafUInt32* bytesRead )
{
  CHECK_HRESULT( _spIaafConstantValue->GetValue( valueSize, pValue, bytesRead ) );
}

aafUInt32 AxConstantValue::GetValueBufLen()
{
  aafUInt32 valueBufLen;
  CHECK_HRESULT( _spIaafConstantValue->GetValueBufLen( &valueBufLen ) );
  return valueBufLen;
}

IAAFTypeDefSP AxConstantValue::GetTypeDefinition()
{
  IAAFTypeDefSP typeDefinition;
  CHECK_HRESULT( _spIaafConstantValue->GetTypeDefinition( &typeDefinition ) );
  return typeDefinition;
}

void AxConstantValue::SetValue( aafUInt32 valueSize, aafDataBuffer_t pValue )
{
  CHECK_HRESULT( _spIaafConstantValue->SetValue( valueSize, pValue ) );
}

//=---------------------------------------------------------------------=

AxVaryingValue::AxVaryingValue( IAAFVaryingValueSP spIaafVaryingValue )
  : AxParameter( AxQueryInterface<IAAFVaryingValue, IAAFParameter>( spIaafVaryingValue ) ),
    _spIaafVaryingValue( spIaafVaryingValue )
{}

AxVaryingValue::~AxVaryingValue()
{}

void AxVaryingValue::Initialize( IAAFParameterDefSP spParameterDef, IAAFInterpolationDefSP spInterpolation )
{
  CHECK_HRESULT( _spIaafVaryingValue->Initialize( spParameterDef, spInterpolation ) );
}

void AxVaryingValue::AddControlPoint( IAAFControlPointSP spControlPoint )
{
  CHECK_HRESULT( _spIaafVaryingValue->AddControlPoint( spControlPoint ) );
}

IEnumAAFControlPointsSP AxVaryingValue::GetControlPoints()
{
  IEnumAAFControlPointsSP controlPoints;

  CHECK_HRESULT( _spIaafVaryingValue->GetControlPoints( &controlPoints ) );

  return controlPoints;
}

aafUInt32 AxVaryingValue::CountControlPoints()
{
  aafUInt32 numControlPoints;

  CHECK_HRESULT( _spIaafVaryingValue->CountControlPoints( &numControlPoints ) );

  return numControlPoints;
}

IAAFControlPointSP AxVaryingValue::GetControlPointAt( aafUInt32 index )
{
  IAAFControlPointSP controlPointAt;

  CHECK_HRESULT( _spIaafVaryingValue->GetControlPointAt( index, &controlPointAt ) );

  return controlPointAt;
}

void AxVaryingValue::RemoveControlPointAt( aafUInt32 index )
{
  CHECK_HRESULT( _spIaafVaryingValue->RemoveControlPointAt( index ) );
}

IAAFInterpolationDefSP AxVaryingValue::GetInterpolationDefinition()
{
  IAAFInterpolationDefSP interpolationDef;

  CHECK_HRESULT( _spIaafVaryingValue->GetInterpolationDefinition( &interpolationDef ) );

  return interpolationDef;
}

aafInt32 AxVaryingValue::GetValueBufLen()
{
  aafInt32 length;

  CHECK_HRESULT( _spIaafVaryingValue->GetValueBufLen( &length ) );

  return length;
}

void AxVaryingValue::GetInterpolatedValue( aafRational_t inputValue, aafInt32 valueSize, aafDataBuffer_t pValue, aafInt32* bytesRead )
{
  CHECK_HRESULT( _spIaafVaryingValue->GetInterpolatedValue( inputValue, valueSize, pValue, bytesRead ) );
}

//=---------------------------------------------------------------------=

AxControlPoint::AxControlPoint( IAAFControlPointSP spIaafControlPoint )
  : AxObject( AxQueryInterface<IAAFControlPoint, IAAFObject>( spIaafControlPoint ) ),
    _spIaafControlPoint( spIaafControlPoint )
{}

AxControlPoint::~AxControlPoint()
{}

void AxControlPoint::Initialize( IAAFVaryingValueSP spIaafVaryingValue, aafRational_constref time, aafUInt32 valueSize, aafDataBuffer_t buffer )
{
  CHECK_HRESULT( _spIaafControlPoint->Initialize( spIaafVaryingValue, time, valueSize, buffer ) );
}

aafRational_t AxControlPoint::GetTime()
{
  aafRational_t time;
  CHECK_HRESULT( _spIaafControlPoint->GetTime( &time ) );
  return time;
}

aafEditHint_t AxControlPoint::GetEditHint()
{
  aafEditHint_t editHint;
  CHECK_HRESULT( _spIaafControlPoint->GetEditHint( &editHint ) );
  return editHint;
}

aafUInt32 AxControlPoint::GetValueBufLen()
{
  aafUInt32 valueBufLen;
  CHECK_HRESULT( _spIaafControlPoint->GetValueBufLen( &valueBufLen ) );
  return valueBufLen;
}

aafUInt32 AxControlPoint::GetValue( aafUInt32 valueSize, aafDataBuffer_t buffer )
{
  aafUInt32 bytesRead = 0;
  CHECK_HRESULT( _spIaafControlPoint->GetValue( valueSize, buffer, &bytesRead ) );
  return bytesRead;
}

void AxControlPoint::SetTime( aafRational_t time )
{
  CHECK_HRESULT( _spIaafControlPoint->SetTime( time ) );
}

void AxControlPoint::SetEditHint( aafEditHint_t editHint )
{
  CHECK_HRESULT( _spIaafControlPoint->SetEditHint( editHint ) );
}

IAAFTypeDefSP AxControlPoint::GetTypeDefinition()
{
  IAAFTypeDefSP typeDef;
  CHECK_HRESULT( _spIaafControlPoint->GetTypeDefinition( &typeDef ) );
  return typeDef;
}

void AxControlPoint::SetValue( aafUInt32 valueSize, aafDataBuffer_t buffer )
{
  CHECK_HRESULT( _spIaafControlPoint->SetValue( valueSize, buffer ) );
}

//=---------------------------------------------------------------------=


