//@doc
//@class    AAFEdgecode | Implementation class for AAFEdgecode
#ifndef __ImplAAFEdgecode_h__
#define __ImplAAFEdgecode_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

class ImplAAFEdgecode : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEdgecode ();

protected:
  virtual ~ImplAAFEdgecode ();

public:


  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] Length Property Value
         aafLength_t    length  ,

         // @parm [in] Edgecode Value
         aafEdgecode_t  edgecode);

  //****************
  // GetEdgecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEdgecode
        // @parm [out] Edgecode
        (aafEdgecode_t *  edgecode);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEdgecode)

private:
	OMFixedSizeProperty<aafPosition_t>			_start;
	OMFixedSizeProperty<aafFilmType_t>			_filmType;
	OMFixedSizeProperty<aafEdgeType_t>			_edgeType;
	OMVariableSizeProperty<aafUInt8>			_header;

};

#endif // ! __ImplAAFEdgecode_h__


