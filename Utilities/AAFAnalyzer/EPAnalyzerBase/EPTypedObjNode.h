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

#ifndef __EPTYPEDOBJNODE_h__
#define __EPTYPEDOBJNODE_h__

//Edit Protocol Analyzer Base files
#include "EPObjects.h"
#include "EPDerivationChainObject.h"
#include "EPTrack.h"
#include "EPEffect.h"
#include "EPCastException.h"

//AAF Analyzer Base files
#include <AAFTypedObjNodeDecorator.h>

namespace aafanalyzer {

using namespace boost;

template<typename AAFObjType, typename EPObjType>
class EPTypedObjNode : public AAFTypedObjNodeDecorator<AAFObjType>
{
   
 public:
  EPTypedObjNode( const boost::shared_ptr<AAFTypedObjNode<AAFObjType> > spNode,
                  boost::shared_ptr<EPObjType> spEPObject );
  ~EPTypedObjNode();
  bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  boost::shared_ptr<EPObjType> GetEPObject();

  //This function will return a new node and all type information about the
  //actual type of the templated parameter will be lost.  Therefore, this node
  //should not be stored anywhere unless it is acceptable to use the node
  //without complete type information.
  template<typename AAFTo, typename EPTo>
  boost::shared_ptr<EPTypedObjNode<AAFTo, EPTo> > DownCast()
  {

    //First, get a parent node of the correct base type:
    
    //Get the dictionary.
    AxObject axObj( this->GetAAFObject() );
    AxDictionary axDictionary( axObj.GetDictionary() );

    //Find the class definition of the class to cast to.
    AAFTo* pAAFTo = NULL;
    AxClassDef clsDef( axDictionary.LookupClassDef( AxAUID(pAAFTo) ) );

    //Find the TypedNodeFactory for the class to cast to.
    boost::shared_ptr<TypedNodeFactory> spNodeFactory;
    spNodeFactory = TypedNodeFactoryRegistry::GetInstance().LookUp(clsDef);
    
    //Get a shared pointer to this node.
    boost::shared_ptr<AAFObjNode> spThis = 
        dynamic_pointer_cast<AAFObjNode>( this->GetSharedPointerToNode() );

    //If the cast is not legal, the node factory will throw an exception.
    boost::shared_ptr<AAFTypedObjNode<AAFTo> >spNewNode = 
      dynamic_pointer_cast<AAFTypedObjNode<AAFTo> > ( 
        spNodeFactory->CreateNodeFrom( spThis ) );
    
    //Now, make sure the object can be cast to the appropriate type:
    
    boost::shared_ptr<EPTo> spEPTo( dynamic_pointer_cast<EPTo>( _spEPObject ) );
    if ( !spEPTo )
    {
      wstring msg = L"Cannot cast from \"" + EPObjType::GetName() + L"\" to \"" + EPTo::GetName() + L".\"";
      throw EPCastException( msg.c_str() );
    }
    
    boost::shared_ptr<EPTypedObjNode<AAFTo, EPTo> > 
        spDecoratedNewNode( new EPTypedObjNode<AAFTo, EPTo>( spNewNode, spEPTo ) );
    
    return spDecoratedNewNode;

  }


 private:
 
  boost::shared_ptr<EPObjType> _spEPObject;

  //prohibited
  EPTypedObjNode();
  EPTypedObjNode( const EPTypedObjNode& );
  EPTypedObjNode& operator=( const EPTypedObjNode& );

};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
