//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFTYPEDOBJNODE_h__
#define __AAFTYPEDOBJNODE_h__

//AAF Analyzer Base files
#include "AAFObjNode.h"
#include "TypedNodeFactoryRegistry.h"
#include "TypedNodeFactory.h"

//Ax files
#include <AxSmartPointer.h>
#include <AxDictionary.h>
#include <AxMetaDef.h>

namespace aafanalyzer {

using namespace std;
using namespace boost;

template<typename AAFObjType>
class AAFTypedObjNode : public AAFObjNode
{
 public:

  // Category is initialized to AAFNODE_UNDEFINED at construct time.
  // During reference resolutin it s changed to end of chain, or out
  // of file.  other's remain undefined.
  enum Kind_e { AAFNODE_KIND_END_OF_CHAIN_REF,
		AAFNODE_KIND_IN_FILE_REF,
		AAFNODE_KIND_OUT_OF_FILE_REF,
		AAFNODE_KIND_UNDEFINED };

  ~AAFTypedObjNode();
  
  virtual IAAFSmartPointer<AAFObjType> GetAAFObjectOfType() const =0;

  Kind_e GetKind() const
  {
    return _kind;
  }

  void SetKind( Kind_e kind )
  {
    // Normal usage of this routing would not require that the result
    // be changed once it has been set to something other than
    // undefined.
    assert( AAFNODE_KIND_UNDEFINED == _kind );

    _kind = kind;
  }

  //This function will return a new node and all type information about the
  //actual type of the templated parameter will be lost.  Therefore, this node
  //should not be stored anywhere unless it is acceptable to use the node
  //without complete type information.
  template<typename To>
  boost::shared_ptr<AAFTypedObjNode<To> > DownCastToAAF()
  {

    //Get the dictionary.
    AxObject axObj( this->GetAAFObject() );
    AxDictionary axDictionary( axObj.GetDictionary() );

    //Find the class definition of the class to cast to.
    To* pTo = NULL;
    AxClassDef clsDef( axDictionary.LookupClassDef( AxAUID(pTo) ) );

    //Find the TypedNodeFactory for the class to cast to.
    shared_ptr<TypedNodeFactory> spNodeFactory;
    spNodeFactory = TypedNodeFactoryRegistry::GetInstance().LookUp(clsDef);
    
    //Get a shared pointer to this node.
    shared_ptr<AAFObjNode> spThis = 
        dynamic_pointer_cast<AAFObjNode>( this->GetSharedPointerToNode() );

    //Return the casted node.  If the cast is not legal, the node factory will
    //throw an exception.
    return dynamic_pointer_cast<AAFTypedObjNode<To> > ( spNodeFactory->CreateNodeFrom( spThis ) );

  }

 protected:
  AAFTypedObjNode(IAAFSmartPointer<AAFObjType> ObjectType,
          const basic_string<wchar_t>& name );
  AAFTypedObjNode( IAAFSmartPointer<AAFObjType> ObjectType,
          boost::shared_ptr<Node> spExistingNode );

 private:

  //prohibited
  AAFTypedObjNode();
  AAFTypedObjNode( const AAFTypedObjNode& );
  AAFTypedObjNode& operator=( const AAFTypedObjNode& );

  Kind_e _kind;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
