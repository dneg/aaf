//@doc
//@class    AAFFile | Implementation class for AAFFile
#ifndef __ImplAAFFile_h__
#define __ImplAAFFile_h__

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




//
// Forward declaration
//
struct IAAFFile;
class AAFFile;


class ImplAAFFile
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFile ();
  virtual ~ImplAAFFile ();

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFileTest.cpp.
  static AAFRESULT test();

  void InitContainer (void * pContainer);

  void * GetContainer ();

private:

  void * _pParent;

};

#endif // ! __ImplAAFFile_h__

