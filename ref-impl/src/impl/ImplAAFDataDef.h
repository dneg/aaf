//@doc
//@class    AAFDataDef | Implementation class for AAFDataDef
#ifndef __ImplAAFDataDef_h__
#define __ImplAAFDataDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFDataDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDataDef ();

protected:
  virtual ~ImplAAFDataDef ();

public:



  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        // @parm [retval,out] Pointer to name
        (aafString_t *  pstrName);

  //****************
  // IsPictureKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsPictureKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsPictureKind);

  //****************
  // IsMatteKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsMatteKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsMatteKind);

  //****************
  // IsPictureWithMatteKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsPictureWithMatteKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsPictureWithMatteKind);

  //****************
  // IsSoundKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsSoundKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsSoundKind);

  //****************
  // DoesDataDefConvertTo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    DoesDataDefConvertTo
        (// @parm [in,ref] Auid to compare against
         aafUID_t *  pAuid,

         // @parm [retval,out] pointer to result
         aafBool *  bDoesConvertTo);
		   
  //****************
  // IsDataDefOf()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsDataDefOf
        (// @parm [in,ref] Auid to compare against
         aafUID_t *  pAuid,

         // @parm [retval,out] pointer to result
         aafBool *  bIsDataDefOf);

  //****************
  // DoesDataDefConvertFrom()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    DoesDataDefConvertFrom
        (// @parm [in,ref] Auid to compare against
         aafUID_t *  pAuid,

         // @parm [retval,out] pointer to result
         aafBool *  bDoesConvertFrom);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFDataDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFDataDefTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFDataDef_h__


