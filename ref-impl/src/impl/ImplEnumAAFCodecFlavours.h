//@doc
//@class    EnumAAFCodecFlavours | Implementation class for EnumAAFCodecFlavours
#ifndef __ImplEnumAAFCodecFlavours_h__
#define __ImplEnumAAFCodecFlavours_h__


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "AAFPlugin.h"

class ImplEnumAAFCodecFlavours : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFCodecFlavours ();

protected:
  virtual ~ImplEnumAAFCodecFlavours ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next flavour code
        (aafUID_t *  pAAFCodecFlavour);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFPluggableDefs requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive flavour codes
         aafUID_t *  pAAFCodecFlavours,

         // @parm [out,ref] number of actual flavour IDs fetched into pAAFCodecFlavours array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFCodecFlavours ** ppEnum);

public:
	// SDK-Internal
	AAFRESULT STDMETHODCALLTYPE
		SetEnumCodec( IAAFEssenceCodec *pCodec);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFCodecFlavoursTest.cpp.
  static AAFRESULT test();
private:
	aafInt32			_current;
	IAAFEssenceCodec	*_codec;
};

#endif // ! __ImplEnumAAFCodecFlavours_h__


