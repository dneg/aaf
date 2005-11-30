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

//AAF Analyzer Base files
#include "GraphBuilder.h"
#include "NodeFactory.h"
#include "AAFContainment.h"
#include "MobNodeMap.h"
#include "AAFGraphInfo.h"

//Analyzer Base files
#include <TestGraph.h>
#include <EdgeMap.h>

//Ax files
#include <AxBaseObjIter.h>
#include <AxBaseObj.h>
#include <AxObject.h>
#include <AxFile.h>
#include <AxHeader.h>
#include <AxEx.h>
#include <AxMob.h>
#include <AxSmartPointer.h>
#include <AxProperty.h>
#include <AxMetaDef.h>

//AAF files
#include <AAFTypes.h>

//STL files
#include <memory>
#include <stack>

namespace {

using namespace aafanalyzer;
using namespace std;

void UpdateNodeMap( IAAFObjectSP spObj, shared_ptr<Node> spNode )
{
  IAAFMobSP spMob;
  AxObject axObj(spObj);

  //add the Mob ID to the map for future reference resolving
  if(AxIsA(axObj, spMob))
  {
    AxMob axMob(spMob);
    aafMobID_t mobid = axMob.GetMobID();
    MobNodeMap::GetInstance().AddMobNode(mobid, spNode);
  }
}

void BuildTree( AxBaseObjRecIter& recIter,
          shared_ptr<Node> spRootParent,
          NodeFactory& nodeFactory,
    EdgeMap& edgeMap )
{
  auto_ptr<AxBaseObj> nextPtr;
  stack< pair<int, shared_ptr<Node> > > parentStack;  
  int level = 0;

  // Initialize the parent stack with the root node passed as an
  // argument to this function.
  parentStack.push(pair<int, shared_ptr<Node> >(level, spRootParent));

  // Ignore the first object returned by the recursive iterator because it
  // is the root object (ie. spRootParent).
  recIter.NextOne(nextPtr, level);

  for(bool nextExists = recIter.NextOne(nextPtr, level); nextExists; nextExists = recIter.NextOne(nextPtr, level)) 
  {
    if(dynamic_cast<AxObject*>(nextPtr.get()))
     {
       auto_ptr<AxObject> obj(dynamic_cast<AxObject*>(nextPtr.release()));

       // This remains for debug.
       /*wcout << L"level = " << level << endl;
       wcout << L"parentStack.size() = " << parentStack.size() << endl ;
       wcout << L"parentStack.top().first = " << parentStack.top().first << endl;
       wcout << L"class name = " << obj->GetClassName() << endl << endl;*/

       // Coerce the AxObject wrapper into giving us an IAAFObjectSP
       // pointer to the wrapped AAF object.
       IAAFObjectSP spObj = static_cast<IAAFObjectSP>(*obj);

       // Used the supplied factory to create a node for the
       // containment graph we are bulding.
       shared_ptr<Node> spChildNode( nodeFactory.CreateNode( spObj ) );

       // Add to the node map (if necessary)
       UpdateNodeMap( spObj, spChildNode );

       // We now have a parent, and a child. Create a containment edge
       // and add it to the edge map.
       shared_ptr<Edge> spEdge( new AAFContainment( parentStack.top().second, spChildNode ) );
       edgeMap.AddEdge( spEdge );
        
       // The child node becomes the new parent for lower level objects.
       parentStack.push( pair<int, shared_ptr<Node> >(level, spChildNode) );
     }

     else if ( dynamic_cast<AxProperty*>( nextPtr.get() ) ) {
        
        //We cannot let the traversal continue over an properties with types
        //that can not be determined until runtime.  The iterator will return
        //StrongObjectReferences, Sets, VarriableArrays, Records, FixedArrays,
        //Indirects, Opaques and Renames.  Of these, FixedArrays and Indirects
        //have types that must be determined at runtime.  Therefore, stop
        //traversal on these properties.
        
        auto_ptr<AxProperty> prop(dynamic_cast<AxProperty*>(nextPtr.release()));
        AxPropertyDef axPropDef( prop->GetDefinition() );
        AxTypeDef axTypeDef( axPropDef.GetTypeDef() );
        eAAFTypeCategory_t typeCategory = axTypeDef.GetTypeCategory();

        if(  typeCategory == kAAFTypeCatOpaque || typeCategory == kAAFTypeCatIndirect )
        {
            recIter.PopStack();
        }

        
     }
    // This remains for reference.
    // We don't care about the individual property values. The iterator returns
    // them, but we just ignore them.
    // else if ( dynamic_cast<AxPropertyValue*>( nextPtr.get() ) ) {
    // }

    // A new object never exists in a record subtree, so just stop the
    // traveral.
    else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( nextPtr.get() ) ) {
      recIter.PopStack();
    }

    // If the iterator returns and exception object, then pull extract
    // the underlying exception and rethrow it.
    else if ( dynamic_cast<AxBaseObjAny<AxExHResult>*>( nextPtr.get() ) ) {
      
      auto_ptr< AxBaseObjAny<AxExHResult> > ex (
       dynamic_cast<AxBaseObjAny<AxExHResult>*>( nextPtr.release() ) );
      
      // Copy it, and throw the copy.
      AxExHResult excopy( ex->get() );

      throw excopy;
    }

    // When we climb back up the tree, after decensing to a leaf
    // object, we need to pop objects off the stack.
    while(level < parentStack.top().first) 
    {
      parentStack.pop();
    }
  }
}

} // end of namespace

//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{

GraphBuilder::GraphBuilder()
{
}

GraphBuilder::~GraphBuilder()
{
}

const shared_ptr<const AAFGraphInfo> GraphBuilder::CreateGraph(const AxString& fileName, shared_ptr<NodeFactory> spFactory)
{
  
  shared_ptr<AxFile> axFile( new AxFile );

  axFile->OpenExistingRead(fileName.c_str(), 0);
  
  AxHeader axHeader(axFile->getHeader());
  
  //create the root node
  shared_ptr<Node> spRootNode = spFactory->CreateNode(axHeader);

  // Initialize the recursive iterator.
  AxObject axRootObject( axHeader );
  auto_ptr< AxBaseObjIterPrtcl >
    axRootObjectIter( new AxBaseSolitaryObjIter<AxObject>(axRootObject) );
  AxBaseObjRecIter recIter( axRootObjectIter );
  
  shared_ptr<EdgeMap> spEdgeMap( new EdgeMap );
  
  BuildTree( recIter, spRootNode, *spFactory, *spEdgeMap );

  shared_ptr<const TestGraph> spGraph( new TestGraph( spEdgeMap, spRootNode ) );
  shared_ptr<const AAFGraphInfo> spInfo( new AAFGraphInfo( spGraph, axFile ) );

  return spInfo;
}

}//end of namespace

