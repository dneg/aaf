//@doc
//@class    AAFFiller | Implementation class for AAFFiller
#ifndef __ImplAAFFiller_h__
#define __ImplAAFFiller_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

class ImplAAFDataDef;





#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFFiller : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFiller ();
  virtual ~ImplAAFFiller ();



  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
	    (// @parm [in] Data Definition Object
         ImplAAFDataDef * datadef,

		 // @parm [in] Length Property Value
		 aafLength_t  length);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFFiller)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFillerTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFFiller_h__

