//=---------------------------------------------------------------------=
//
// $Id$
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
//=---------------------------------------------------------------------=

#ifndef __AAFTYPEDOBJNODE_h__
#define __AAFTYPEDOBJNODE_h__

//AAF Analyzer Base files
#include "AAFObjNode.h"

//Ax files
#include <AxSmartPointer.h>

namespace aafanalyzer {

using namespace std;
using namespace boost;

template<typename AAFObjType>
class AAFTypedObjNode : public AAFObjNode
{
 public:
  ~AAFTypedObjNode();
  
  virtual IAAFSmartPointer<AAFObjType> GetAAFObjectOfType() const =0;;

 protected:
  AAFTypedObjNode(IAAFSmartPointer<AAFObjType> ObjectType,
          const basic_string<wchar_t>& name );
  AAFTypedObjNode<AAFObjType>( shared_ptr<AAFTypedObjNode<AAFObjType> > spExistingNode );

 private:

  //prohibited
  AAFTypedObjNode();
  AAFTypedObjNode( const AAFTypedObjNode& );
  AAFTypedObjNode& operator=( const AAFTypedObjNode& );

};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
