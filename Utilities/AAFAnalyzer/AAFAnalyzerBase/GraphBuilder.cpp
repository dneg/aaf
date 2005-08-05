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

#include "GraphBuilder.h"

#include "EdgeMap.h"
#include "NodeFactory.h"
#include "AAFContainment.h"
#include "MobNodeMap.h"

/*TEMPORARY SOLUTION UNTIL WEAK REFERENCES ARE IMPLEMENTED**/
#include "TempAllNodeMap.h"

#include <AxBaseObjIter.h>
#include <AxBaseObj.h>
#include <AxObject.h>
#include <AxFile.h>
#include <AxHeader.h>
#include <AxEx.h>
#include <AxMob.h>
#include <AxSmartPointer.h>

#include <memory>
#include <stack>

namespace {

using namespace aafanalyzer;
using namespace boost;


void UpdateNodeMap( IAAFObjectSP spObj, boost::shared_ptr<Node> spNode )
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
  //TEMPORARY SOLUTION UNTIL WEAK REFERENCES ARE IMPLEMENTED
  TempAllNodeMap::GetInstance().AddNode(spNode->GetLID(), spNode);
}

void BuildTree( AxBaseObjRecIter& recIter,
          shared_ptr<Node> spRootParent,
          NodeFactory& nodeFactory,
    EdgeMap& edgeMap )
{
  std::auto_ptr<AxBaseObj> nextPtr;
  std::stack< std::pair<int, shared_ptr<Node> > > parentStack;  
  int level = 0;

  // Initialize the parent stack with the root node passed as an
  // argument to this function.
  parentStack.push(std::pair<int, shared_ptr<Node> >(level, spRootParent));

  // Ignore the first object returned by the recursive iterator because it
  // is the root object (ie. spRootParent).
  recIter.NextOne(nextPtr, level);

  for(bool nextExists = recIter.NextOne(nextPtr, level); nextExists; nextExists = recIter.NextOne(nextPtr, level)) 
  {
    if(dynamic_cast<AxObject*>(nextPtr.get()))
     {
       std::auto_ptr<AxObject> obj(dynamic_cast<AxObject*>(nextPtr.release()));

       // This remains for debug.
       /*std::wcout << L"level = " << level << std::endl;
       std::wcout << L"parentStack.size() = " << parentStack.size() << std::endl ;
       std::wcout << L"parentStack.top().first = " << parentStack.top().first << std::endl;
       std::wcout << L"class name = " << obj->GetClassName() << std::endl << std::endl;*/

       // Coerce the AxObject wrapper into giving us an IAAFObjectSP
       // pointer to the wrapped AAF object.
       IAAFObjectSP spObj = static_cast<IAAFObjectSP>(*obj);

       // Used the supplied factory to create a node for the
       // containment graph we are bulding.
       boost::shared_ptr<Node> spChildNode( nodeFactory.CreateNode( spObj ) );

       // Add to the node map (if necessary)
       UpdateNodeMap( spObj, spChildNode );

       // We now have a parent, and a child. Create a containment edge
       // and add it to the edge map.
       shared_ptr<Edge> spEdge( new AAFContainment( parentStack.top().second, spChildNode ) );
       edgeMap.AddEdge( spEdge );
        
       // The child node becomes the new parent for lower level objects.
       parentStack.push( std::pair<int, shared_ptr<Node> >(level, spChildNode) );
     }

    // These remain for reference.
    // We don't care about the individual properties, and property
    // values. The iterator returns them, but we just ignore them.
    // else if ( dynamic_cast<AxProperty*>( nextPtr.get() ) ) {
    // }
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
      
      std::auto_ptr< AxBaseObjAny<AxExHResult> > ex (
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

TestGraph GraphBuilder::CreateGraph(const std::basic_string<wchar_t>& fileName, boost::shared_ptr<NodeFactory> spFactory)
{
  AxFile axFile;
  axFile.OpenExistingRead(fileName.c_str(), 0);
  
  AxHeader axHeader(axFile.getHeader());
  
  //create the root node
  boost::shared_ptr<Node> spRootNode = spFactory->CreateNode(axHeader);

  // Initialize the recursive iterator.
  AxObject axRootObject( axHeader );
  std::auto_ptr< AxBaseObjIterPrtcl >
    axRootObjectIter( new AxBaseSolitaryObjIter<AxObject>(axRootObject) );
  AxBaseObjRecIter recIter( axRootObjectIter );
  
  shared_ptr<EdgeMap> spEdgeMap( new EdgeMap );
  
  BuildTree( recIter, spRootNode, *spFactory, *spEdgeMap );
  
  axFile.Close();

  return TestGraph( spEdgeMap, spRootNode );
}

}//end of namespace

