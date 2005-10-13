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
//=---------------------------------------------------------------------=

#ifndef __AAFOBJNODE_h__
#define __AAFOBJNODE_h__

//Analyzer Base files
#include <Node.h>

//Ax files
#include <AxSmartPointer.h>

namespace aafanalyzer {

using namespace std;

class AAFObjNode : public Node
{
 public:
  AAFObjNode(IAAFSmartPointer<IAAFObject> spIAAFObj);
  AAFObjNode(IAAFSmartPointer<IAAFObject> spIAAFObj, const basic_string<wchar_t>& name);
  virtual ~AAFObjNode();

  IAAFSmartPointer<IAAFObject> GetAAFObject() const;

 protected:
  AAFObjNode(IAAFSmartPointer<IAAFObject> spIAAFObj, const basic_string<wchar_t>& name, Node::LID lid);

 private:
  IAAFSmartPointer<IAAFObject> _spIAAFObj;  

  // prohibited
  AAFObjNode();
  AAFObjNode( const AAFObjNode& );
  AAFObjNode& operator=( const AAFObjNode& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
