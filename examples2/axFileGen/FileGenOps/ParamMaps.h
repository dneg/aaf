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

#include <AxTypes.h>

#include <map>

class AxFGOp;

// Yet another singleton class that stores a map of any data types.
// Data is always returned by value.  No iterator.  It may be use full
// to extend this to generate some kind of use message.

// Tag exists to permit different instances of the template for the
// same Type.  This is required becaues explicit constructor instances
// build the parameter maps for different parameters... but, sometimes
// those parameters have the same type.  So, for example, to ensure all
// parameters of type aafUID_t don't have to lumped into the same table,
// we just add a distict Tag.  The tag is normally just a forward class
// declaration of a non-existant class.

template <class Type, class Tag>
class ParamMap {
public:

	typedef std::map<AxString, Type> MapType;
	typedef std::map<AxString, Type>::iterator MapIterType;

	static ParamMap& GetInstance();
		
	// The AxFGOp& is used to build a meaning message in 
	// event an exception is thrown.
	// FIXME - Perhaps AxFGOps& not such a good idea. 
	// These maps could be used outside of axFileGen.
	Type Find( AxFGOp& op, const AxString& name );

	AxString GenerateNameList();

private:

	ParamMap();
	~ParamMap();

	MapType _map;

	static ParamMap* _singleInstance;
};

//=---------------------------------------------------------------------=

// Create unique type tags for untyped parameters that use uid's.
class _ContainerDefsTag;
class _CodecIDTag;
class _DDefTag;
class _OperationDefTag;

typedef ParamMap< aafUID_t,             _OperationDefTag     >  OperationDefParams;
typedef ParamMap< aafUID_t,             _ContainerDefsTag    >	ContainerDefParams;
typedef ParamMap< aafUID_t,             _CodecIDTag          >	CodecIDParams;
typedef ParamMap< aafUID_t,             _DDefTag             >	DDefParams;

typedef ParamMap< aafCompressEnable_t,  aafCompressEnable_e  >	CompressionEnableParams;
typedef ParamMap< aafTapeCaseType_t,    aafTapeCaseType_e    >	TapeCaseParams;
typedef ParamMap< aafVideoSignalType_t, aafVideoSignalType_e >	SignalTypeParams;
typedef ParamMap< aafTapeFormatType_t,  aafTapeFormatType_e  >	TapeFormatParams;
typedef ParamMap< aafFrameLayout_t,     aafFrameLayout_e     >  FrameLayoutParams;
typedef ParamMap< aafColorSpace_t,      aafColorSpace_e      >  ColorSpaceParams;
typedef ParamMap< aafPixelFormat_t,     aafPixelFormat_e     >  PixelFormatParams;
typedef ParamMap< aafDropType_t,        aafDropType_e        >  DropTypeParams;
typedef ParamMap< aafBoolean_t,         aafBoolean_e         >  BooleanParams;
