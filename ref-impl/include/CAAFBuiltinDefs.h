#ifndef __CAAFBuiltinDefs_h__
#define __CAAFBuiltinDefs_h__
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

//
// This class provides a facility to allow clients to access builtin
// definition objects easily.  When set up, the client simply has to
// call a method on this function, which returns a pointer to the
// primary interface of the requested definition object.
//
// The returned object is *not* reference counted beyond that
// maintained by the this class.  Therefore the client must promise
// that his use of the returned interface will last no longer than the
// scope of the CAAFBuiltinDefs object from which it was obtained
// unless he explicitly maintains a reference count of the use of the
// interface beyond that scope.
//
// Usage example:
//
// // Creates and returns a Filler object, using the given dictionary.
// void MyFunc (IAAFDictionary * pDict)
// {
//   // Use of ImplAAFBuiltinDefs to obtain a class definition
//   ImplAAFBuiltinDefs defs (pDict);	// create builtin defs object.
//   ImplAAFObject * pFiller = 0;
//   defs.cdFiller()->CreateInstance(&pFiller);
//   // Note that IAAFClassDef interface returned by cdFiller() is
//   // only used for the duration of the CreateInstance() function
//   // call.
// 
//   // Use of ImplAAFBuiltinDefs to obtain a data definition
//   pFiller->Initialize(defs.ddkAAFPicture(), 10);
//   // Note that IAAFDataDef interface returned by ddkAAFPicture() is
//   // only used for the duration of the Initialize() function call.
// }
//

#include "AAF.h"  // for IAAFDictionary
#include "AAFDataDefs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFClassDef>   IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFDataDef>    IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFDictionary> IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFTypeDef>    IAAFTypeDefSP;

// To avoid including assert.h in a header file (this one)
#ifndef AAF_BUILTIN_DEFS_ASSERT
#define AAF_BUILTIN_DEFS_ASSERT(condition) \
  if (! (condition)) throw #condition
#endif // ! AAF_BUILTIN_DEFS_ASSERT


#define CLASS_DEF_METHOD_DECL(meth_name, cdef_constant) \
public:	\
  IAAFClassDef * meth_name () \
  { \
	if (! _pcd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupClassDef(cdef_constant, \
                                              &_pcd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
	  } \
	return _pcd##meth_name; \
  } \
private: \
  IAAFClassDefSP _pcd##meth_name

#define CLASS_DEF_METHOD(name) \
   CLASS_DEF_METHOD_DECL(cd##name, AUID_AAF##name)


#define DATA_DEF_METHOD_DECL(meth_name, ddef_constant) \
public:	\
  IAAFDataDef * meth_name () \
  { \
	if (! _pdd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupDataDef(ddef_constant, \
                                             &_pdd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
	  } \
	return _pdd##meth_name; \
  } \
private: \
  IAAFDataDefSP _pdd##meth_name

#define DATA_DEF_METHOD_LEGACY(name) \
   DATA_DEF_METHOD_DECL(dd##name, DDEF_##name)

#define DATA_DEF_METHOD(name) \
   DATA_DEF_METHOD_DECL(ddkAAF##name, kAAFDataDef_##name)


#define TYPE_DEF_METHOD_DECL(meth_name, tdef_constant) \
public:	\
  IAAFTypeDef * meth_name () \
  { \
	if (! _ptd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupTypeDef(tdef_constant, \
                                             &_ptd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
	  } \
	return _ptd##meth_name; \
  } \
private: \
  IAAFTypeDefSP _ptd##meth_name

#define TYPE_DEF_METHOD(name) \
   TYPE_DEF_METHOD_DECL(td##name, kAAFTypeID_##name)


class CAAFBuiltinDefs
{
public:
  CAAFBuiltinDefs(IAAFDictionary * pDict)
  {
	// Don't reference count the dictionary!
	AAF_BUILTIN_DEFS_ASSERT (pDict);
	_pDict = pDict;
  }


  //
  // Class Def 'get' methods
  //
  CLASS_DEF_METHOD(AES3PCMDescriptor);
  CLASS_DEF_METHOD(AIFCDescriptor);
  CLASS_DEF_METHOD(AuxiliaryDescriptor);
  CLASS_DEF_METHOD(BWFImportDescriptor);
  CLASS_DEF_METHOD(CDCIDescriptor);
  CLASS_DEF_METHOD(ClassDef);
  CLASS_DEF_METHOD(CodecDef);
  CLASS_DEF_METHOD(CommentMarker);
  CLASS_DEF_METHOD(Component);
  CLASS_DEF_METHOD(CompositionMob);
  CLASS_DEF_METHOD(ConstantValue);
  CLASS_DEF_METHOD(ContainerDef);
  CLASS_DEF_METHOD(ContentStorage);
  CLASS_DEF_METHOD(ControlPoint);
  CLASS_DEF_METHOD(DataDef);
  CLASS_DEF_METHOD(DataEssenceDescriptor);
  CLASS_DEF_METHOD(DescriptiveClip);
  CLASS_DEF_METHOD(DescriptiveFramework);
  CLASS_DEF_METHOD(DescriptiveMarker);
  CLASS_DEF_METHOD(DescriptiveObject);
  CLASS_DEF_METHOD(Dictionary);
  CLASS_DEF_METHOD(DigitalImageDescriptor);
  CLASS_DEF_METHOD(Edgecode);
  CLASS_DEF_METHOD(EssenceData);
  CLASS_DEF_METHOD(EssenceDescriptor);
  CLASS_DEF_METHOD(EssenceGroup);
  CLASS_DEF_METHOD(Event);
  CLASS_DEF_METHOD(EventMobSlot);
  CLASS_DEF_METHOD(FileDescriptor);
  CLASS_DEF_METHOD(Filler);
  CLASS_DEF_METHOD(FilmDescriptor);
  CLASS_DEF_METHOD(GPITrigger);
  CLASS_DEF_METHOD(HTMLClip);
  CLASS_DEF_METHOD(HTMLDescriptor);
  CLASS_DEF_METHOD(Identification);
  CLASS_DEF_METHOD(ImportDescriptor);
  CLASS_DEF_METHOD(InterpolationDefinition);
  CLASS_DEF_METHOD(KLVData);
  CLASS_DEF_METHOD(KLVDataDefinition);
  CLASS_DEF_METHOD(Locator);
  CLASS_DEF_METHOD(MasterMob);
  CLASS_DEF_METHOD(Mob);
  CLASS_DEF_METHOD(MobSlot);
  CLASS_DEF_METHOD(MPEGVideoDescriptor);
  CLASS_DEF_METHOD(MultipleDescriptor);
  CLASS_DEF_METHOD(NestedScope);
  CLASS_DEF_METHOD(NetworkLocator);
  CLASS_DEF_METHOD(OperationDef);
  CLASS_DEF_METHOD(OperationGroup);
  CLASS_DEF_METHOD(Parameter);
  CLASS_DEF_METHOD(ParameterDef);
  CLASS_DEF_METHOD(PCMDescriptor);
  CLASS_DEF_METHOD(PhysicalDescriptor);
  CLASS_DEF_METHOD(PluginDef);
  CLASS_DEF_METHOD(PropertyDef);
  CLASS_DEF_METHOD(Pulldown);
  CLASS_DEF_METHOD(RecordingDescriptor);
  CLASS_DEF_METHOD(RGBADescriptor);
  CLASS_DEF_METHOD(RIFFChunk);
  CLASS_DEF_METHOD(ScopeReference);
  CLASS_DEF_METHOD(Selector);
  CLASS_DEF_METHOD(Sequence);
  CLASS_DEF_METHOD(SoundDescriptor);
  CLASS_DEF_METHOD(SourceClip);
  CLASS_DEF_METHOD(SourceMob);
  CLASS_DEF_METHOD(SourceReference);
  CLASS_DEF_METHOD(StaticMobSlot);
  CLASS_DEF_METHOD(SubDescriptor);
  CLASS_DEF_METHOD(TIFFDescriptor);
  CLASS_DEF_METHOD(TaggedValue);
  CLASS_DEF_METHOD(TaggedValueDefinition);
  CLASS_DEF_METHOD(TapeDescriptor);
  CLASS_DEF_METHOD(TextClip);
  CLASS_DEF_METHOD(TextLocator);
  CLASS_DEF_METHOD(Timecode);
  CLASS_DEF_METHOD(TimecodeStream);
  CLASS_DEF_METHOD(TimecodeStream12M);
  CLASS_DEF_METHOD(TimelineMobSlot);
  CLASS_DEF_METHOD(Transition);
  CLASS_DEF_METHOD(TypeDef);
  CLASS_DEF_METHOD(TypeDefEnum);
  CLASS_DEF_METHOD(TypeDefExtEnum);
  CLASS_DEF_METHOD(TypeDefFixedArray);
  CLASS_DEF_METHOD(TypeDefCharacter);
  CLASS_DEF_METHOD(TypeDefIndirect);
  CLASS_DEF_METHOD(TypeDefOpaque);
  CLASS_DEF_METHOD(TypeDefInt);
  CLASS_DEF_METHOD(TypeDefRecord);
  CLASS_DEF_METHOD(TypeDefRename);
  CLASS_DEF_METHOD(TypeDefStream);
  CLASS_DEF_METHOD(TypeDefString);
  CLASS_DEF_METHOD(TypeDefStrongObjRef);
  CLASS_DEF_METHOD(TypeDefVariableArray);
  CLASS_DEF_METHOD(TypeDefWeakObjRef);
  CLASS_DEF_METHOD(VaryingValue);
  CLASS_DEF_METHOD(WAVEDescriptor);


  //
  // Data Def 'get' methods
  //
  // defines ddXXX() which use AAF v1.0 values
  DATA_DEF_METHOD_LEGACY(Edgecode);
  DATA_DEF_METHOD_LEGACY(Matte);
  DATA_DEF_METHOD_LEGACY(Picture);
  DATA_DEF_METHOD_LEGACY(PictureWithMatte);
  DATA_DEF_METHOD_LEGACY(Sound);
  DATA_DEF_METHOD_LEGACY(Timecode);

  // defines ddkAAFXXX() which use AAF v1.1 values
  DATA_DEF_METHOD(Auxiliary);
  DATA_DEF_METHOD(DescriptiveMetadata);
  DATA_DEF_METHOD(Edgecode);
  DATA_DEF_METHOD(Matte);
  DATA_DEF_METHOD(Picture);
  DATA_DEF_METHOD(PictureWithMatte);
  DATA_DEF_METHOD(Sound);
  DATA_DEF_METHOD(Timecode);


  //
  // Type Def 'get' methods
  //
  TYPE_DEF_METHOD(Rational);
  TYPE_DEF_METHOD(String);
  TYPE_DEF_METHOD(Character);
  TYPE_DEF_METHOD(Indirect);
  TYPE_DEF_METHOD(Opaque);
  TYPE_DEF_METHOD(Stream);

  // Built-in integer types
  TYPE_DEF_METHOD(UInt8);
  TYPE_DEF_METHOD(UInt16);
  TYPE_DEF_METHOD(UInt32);
  TYPE_DEF_METHOD(UInt64);
  TYPE_DEF_METHOD(Int8);
  TYPE_DEF_METHOD(Int16);
  TYPE_DEF_METHOD(Int32);
  TYPE_DEF_METHOD(Int64);

private:
  IAAFDictionary * _pDict;
};


#endif // ! __CAAFBuiltinDefs_h__
