#ifndef __AxMobs_h__
#define __AxMobs_h__

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

class AxMob : public AxObject {

public:
	AxMob( IAAFMobSP& spIaafMob );
	virtual ~AxMob();

	aafMobID_t GetMobID();
	
	AxString GetName();
	void SetName( const AxString& name );
	
	aafNumSlots_t CountSlots();

	void AppendComment( const AxString& category,
						const AxString& comment );

	void AppendKLVData( const IAAFKLVDataSP& );

	IAAFTimelineMobSlotSP AppendNewTimelineSlot(
		aafRational_t  editRate,
		IAAFSegmentSP spIaafSegment,
		aafSlotID_t  slotID,
		const AxString& slotName,
		aafPosition_t  origin );
	
	IEnumAAFMobSlotsSP GetSlots();

	inline operator IAAFMobSP ()
	{ return _spIaafMob; }

private:
	AxMob();
	AxMob( const AxMob& );
	AxMob& operator=( const AxMob& );

	IAAFMobSP _spIaafMob;
};

//=---------------------------------------------------------------------=

// No parent class!  See the UML.
class AxSearchSource {

public:
	AxSearchSource( IAAFSearchSourceSP& spIaafSearchSource );
	virtual ~AxSearchSource();

private:
	AxSearchSource();
	AxSearchSource( const AxSearchSource& );
	AxSearchSource& operator=( const AxSearchSource& );

	IAAFSearchSourceSP _spIaafSearchSource;
};


//=---------------------------------------------------------------------=

class AxMasterMob : public AxMob, public AxSearchSource {

public:
	AxMasterMob( IAAFMasterMobSP& spIaafMasterMob );
	
	virtual ~AxMasterMob();

	void Initialize();

	// FIXME - Use references.
	IAAFEssenceAccessSP CreateEssence( 
		aafSlotID_t			slotID,
		IAAFDataDefSP		pMediaKind,		// create essence of this type
		aafUID_constref		codecID,		// using this codec
		aafRational_t		editRate,		// with this edit rate
		aafRational_t		samplerate,		// with this sample rate
		aafCompressEnable_t	Enable,			// optionally compressing it
		IAAFLocatorSP		destination,	// Optionally create the file here.
		aafUID_constref		fileFormat );	// with this format
	
	IAAFEssenceAccessSP OpenEssence( 
		aafSlotID_t			slotId,
		aafMediaCriteria_t*	mediaCrit,		// using this essence criteria
		aafMediaOpenMode_t	openMode,		// ReadOnly or Append
		aafCompressEnable_t	compEnable );	// optionally decompressing
	
	inline operator IAAFMasterMobSP ()
	{ return _spIaafMasterMob; }

private:
	AxMasterMob();
	AxMasterMob( const AxMasterMob& );
	AxMasterMob& operator=( const AxMasterMob& );

	// As soon as you attempt to implement a copy constructor,
	// and/or operator=,  you have to deal with a const AxMasterMob&.
	// That leads to the need to declare the smart pointer mutable.
	mutable IAAFMasterMobSP _spIaafMasterMob;
};

//=---------------------------------------------------------------------=

class AxMasterMobEx : public AxMasterMob {

public:
	AxMasterMobEx( IAAFMasterMobExSP& spIaafMasterMobEx );

	virtual ~AxMasterMobEx();

	IAAFEssenceAccessSP ExtendEssence( 
		aafSlotID_t			slotID,
		IAAFDataDefSP		pMediaKind,		// create essence of this type
		aafUID_constref		codecID,		// using this codec
		aafRational_t		editRate,		// with this edit rate
		aafRational_t		samplerate,		// with this sample rate
		aafCompressEnable_t	Enable,			// optionally compressing it
		IAAFLocatorSP		destination,	// Optionally create the file here.
		aafUID_constref		fileFormat );	// with this format

	inline operator IAAFMasterMobExSP ()
		{ return _spIaafMasterMobEx; }

private:
	AxMasterMobEx();
	AxMasterMobEx( const AxMasterMobEx& );
	AxMasterMobEx& operator=( const AxMasterMobEx& );

	// As soon as you attempt to implement a copy constructor,
	// and/or operator=,  you have to deal with a const AxMasterMob&.
	// That leads to the need to declare the smart pointer mutable.
	mutable IAAFMasterMobExSP _spIaafMasterMobEx;
};

//=---------------------------------------------------------------------=

class AxCompositionMob : public AxMob {

public:
	AxCompositionMob( IAAFCompositionMobSP& spIaafCompositionMob );
	virtual ~AxCompositionMob();

private:
	AxCompositionMob();
	AxCompositionMob( const AxCompositionMob& );
	AxCompositionMob& operator=( const AxCompositionMob& );

	IAAFCompositionMobSP _spIaafCompositionMob;
};

//=---------------------------------------------------------------------=

class AxSourceMob : public AxMob, public AxSearchSource {

public:
	AxSourceMob( IAAFSourceMobSP& spIaafSourceMob );
	virtual ~AxSourceMob();

	IAAFEssenceDescriptorSP GetEssenceDescriptor();

private:
	AxSourceMob();
	AxSourceMob( const AxSourceMob& );
	AxSourceMob& operator=( const AxSourceMob& );

	IAAFSourceMobSP _spIaafSourceMob;
};


//=---------------------------------------------------------------------=

#endif
