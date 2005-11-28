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
