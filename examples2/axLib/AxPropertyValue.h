#ifndef __AxPropertyValue_h__
#define __AxPropertyValue_h__

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
#include "AxBaseObj.h"

#include <iosfwd>

class AxPropertyValuePrtcl {
public:

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefCharacterSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIntSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefEnumSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefExtEnumSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRecordSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStreamSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStringSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefWeakObjRefSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefObjectRefSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& ) = 0;
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefVariableArraySP& ) = 0;
};

class AxPropertyValueNoopPrtcl : public AxPropertyValuePrtcl {
public:

	AxPropertyValueNoopPrtcl();
	virtual ~AxPropertyValueNoopPrtcl();

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefCharacterSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIntSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefEnumSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefExtEnumSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRecordSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStreamSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStringSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefWeakObjRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefObjectRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefVariableArraySP& );
};


//=---------------------------------------------------------------------=

class AxPropertyValue : public AxBaseObj {
public:

	AxPropertyValue( IAAFPropertyValueSP spIaafPropertyValue );
	AxPropertyValue( const AxPropertyValue& );
	virtual ~AxPropertyValue();

	IAAFTypeDefSP GetType();
	
	void Process( AxPropertyValuePrtcl& prtcl );

private:
	AxPropertyValue& operator=( const AxPropertyValue& );
	
	mutable IAAFPropertyValueSP _spIaafPropertyValue;
};

//=---------------------------------------------------------------------=

#endif
