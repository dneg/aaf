#ifndef __ImplAAFBuiltinDefs_h__
#define __ImplAAFBuiltinDefs_h__
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#include "ImplAAFDictionary.h"
#include "AAFDataDefs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFResult.h"

//
// This class provides a facility to allow clients to access builtin
// definition objects easily.  When set up, the client simply has to
// call a method on this function, which returns a pointer to the
// requested definition object.
//
// The returned object is *not* reference counted beyond that
// maintained by the this class; furthermore, this class maintains no
// reference count of any object beyond that of the associated
// dictionary.  Therefore the client must promise that his use of the
// returned definition object will last no longer than the scope of
// the dictionary with which this ImplAAFBuiltinDefs object is
// created, unless he explicitly maintains a reference count of the
// use of that object beyond that scope.
//
// Usage example:
//
// void MyFunc (ImplAAFDictionary * pDict)
// {
//   // Use of CAAFBuiltinDefs to obtain a class definition
//   CAAFBuiltinDefs defs (pDict);	// create builtin defs object.
//   IAAFFiller * pFiller = 0;
//   defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IAAFObject**) &pFiller);
//   // Note that ImplAAFClassDef returned by cdFiller() is only used
//   // for the duration of the CreateInstance() function call.
// 
//   // Use of CAAFBuiltinDefs to obtain a data definition
//   pFiller->Initialize(defs.ddPicture(), 10);
//   // Note that ImplAAFDataDef returned by ddPicture() is only used
//   // for the duration of the Initialize() function call.
// }
//

// To avoid including assert.h in a header file (this one)
#ifndef AAF_BUILTIN_DEFS_ASSERT
#define AAF_BUILTIN_DEFS_ASSERT(condition) \
  if (! (condition)) throw #condition
#endif // ! AAF_BUILTIN_DEFS_ASSERT


#define CLASS_DEF_METHOD_DECL(meth_name, cdef_constant) \
public:	\
  ImplAAFClassDef * meth_name () \
  { \
	if (! _pcd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupClassDef(cdef_constant, \
											  &_pcd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
		aafUInt32 refCnt = _pcd##meth_name->ReleaseReference (); \
		AAF_BUILTIN_DEFS_ASSERT (refCnt); \
	  } \
	return _pcd##meth_name; \
  } \
private: \
  ImplAAFDumbPointer<ImplAAFClassDef> _pcd##meth_name

#define CLASS_DEF_METHOD(name) \
   CLASS_DEF_METHOD_DECL(cd##name, AUID_AAF##name)


#define DATA_DEF_METHOD_DECL(meth_name, ddef_constant) \
public:	\
  ImplAAFDataDef * meth_name () \
  { \
	if (! _pdd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupDataDef(ddef_constant, \
											 &_pdd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
		aafUInt32 refCnt = _pdd##meth_name->ReleaseReference (); \
		AAF_BUILTIN_DEFS_ASSERT (refCnt); \
	  } \
	return _pdd##meth_name; \
  } \
private: \
  ImplAAFDumbPointer<ImplAAFDataDef> _pdd##meth_name

#define DATA_DEF_METHOD(name) \
   DATA_DEF_METHOD_DECL(dd##name, DDEF_##name)


#define TYPE_DEF_METHOD_DECL(meth_name, tdef_constant) \
public:	\
  ImplAAFTypeDef * meth_name () \
  { \
	if (! _ptd##meth_name) \
	  { \
		AAFRESULT hr = _pDict->LookupTypeDef(tdef_constant, \
											 &_ptd##meth_name); \
		AAF_BUILTIN_DEFS_ASSERT (AAFRESULT_SUCCEEDED (hr)); \
		aafUInt32 refCnt = _ptd##meth_name->ReleaseReference (); \
		AAF_BUILTIN_DEFS_ASSERT (refCnt); \
	  } \
	return _ptd##meth_name; \
  } \
private: \
  ImplAAFDumbPointer<ImplAAFTypeDef> _ptd##meth_name

#define TYPE_DEF_METHOD(name) \
   TYPE_DEF_METHOD_DECL(td##name, kAAFTypeID_##name)



//
// Private dumb pointer template to make default pointer
// declarations be initialized to zero.  No reference counting done
// here.
//
template <typename T>
class ImplAAFDumbPointer
{
public:
  ImplAAFDumbPointer (T * init = 0) : _rep (init) {}
  operator T * () { return _rep; }
  T ** operator & () { return &_rep; }
  T * operator -> () { return _rep; }
private:
  T * _rep;
};



class ImplAAFBuiltinDefs
{
public:
  ImplAAFBuiltinDefs(ImplAAFDictionary * pDict)
  {
	// Don't reference count the dictionary!
	AAF_BUILTIN_DEFS_ASSERT (pDict);
	_pDict = pDict;
  }


  //
  // Class Def 'get' methods
  //
  CLASS_DEF_METHOD(AIFCDescriptor);
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
  CLASS_DEF_METHOD(HTMLClip);
  CLASS_DEF_METHOD(Identification);
  CLASS_DEF_METHOD(InterpolationDefinition);
  CLASS_DEF_METHOD(Locator);
  CLASS_DEF_METHOD(MasterMob);
  CLASS_DEF_METHOD(Mob);
  CLASS_DEF_METHOD(MobSlot);
  CLASS_DEF_METHOD(NestedScope);
  CLASS_DEF_METHOD(NetworkLocator);
  CLASS_DEF_METHOD(OperationDef);
  CLASS_DEF_METHOD(OperationGroup);
  CLASS_DEF_METHOD(Parameter);
  CLASS_DEF_METHOD(ParameterDef);
  CLASS_DEF_METHOD(PluginDescriptor);
  CLASS_DEF_METHOD(PropertyDef);
  CLASS_DEF_METHOD(Pulldown);
  CLASS_DEF_METHOD(ScopeReference);
  CLASS_DEF_METHOD(Selector);
  CLASS_DEF_METHOD(Sequence);
  CLASS_DEF_METHOD(SourceClip);
  CLASS_DEF_METHOD(SourceMob);
  CLASS_DEF_METHOD(SourceReference);
  CLASS_DEF_METHOD(TIFFDescriptor);
  CLASS_DEF_METHOD(TaggedValue);
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
  CLASS_DEF_METHOD(TypeDefString);
  CLASS_DEF_METHOD(TypeDefStrongObjRef);
  CLASS_DEF_METHOD(TypeDefVariableArray);
  CLASS_DEF_METHOD(TypeDefWeakObjRef);
  CLASS_DEF_METHOD(VaryingValue);
  CLASS_DEF_METHOD(WAVEDescriptor);


  //
  // Data Def 'get' methods
  //
  DATA_DEF_METHOD(Edgecode);
  DATA_DEF_METHOD(Matte);
  DATA_DEF_METHOD(Picture);
  DATA_DEF_METHOD(PictureWithMatte);
  DATA_DEF_METHOD(Sound);
  DATA_DEF_METHOD(Timecode);


  //
  // Type Def 'get' methods
  //
  TYPE_DEF_METHOD(Indirect);
  TYPE_DEF_METHOD(Character);
  TYPE_DEF_METHOD(String);


private:
  ImplAAFDictionary * _pDict;
};


#endif // ! __ImplAAFBuiltinDefs_h__
