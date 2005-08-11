//=---------------------------------------------------------------------=
//
// $Test.cpp$,v 1.2 2005/07/11 13:29:14 dks Exp $
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


//project files
#include <AAFTypedObjNode.h>
#include <DepthFirstTraversal.h>
#include <EdgeMap.h>
#include <Edge.h>
#include <TypedVisitor.h>
#include <TestVisitor.h>
#include <ResolveRefVisitor.h>
#include <AcyclicVisitor.h>
#include <GraphBuilder.h>
#include <NodeFactory.h>
#include <NodeFactoryImpl.h>

#include <AxEx.h>

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>

using namespace aafanalyzer;

//a test harness: a second main method, commented out below, is for testing
void BasicDataStructureTest()
{
  //boost::shared_ptr<Node> parentOne(new AAFTypedObjNode<IAAFObject>(L"Header"));
  boost::shared_ptr<Node> parentOne(new Node());
  boost::shared_ptr<Node> parentTwo(new Node());
  boost::shared_ptr<Node> childOne(new Node());
  boost::shared_ptr<Node> childTwo(new Node());
  boost::shared_ptr<Edge> edgeOne(new Edge(parentOne, childOne));
  boost::shared_ptr<Edge> edgeTwo(new Edge(parentOne, childTwo));
  boost::shared_ptr<Edge> edgeThree(new Edge(parentTwo, childTwo));

  //test the EdgeMap class
  boost::shared_ptr<EdgeMap> spEdgeMap(new EdgeMap());

  spEdgeMap->AddEdge(edgeOne);
  spEdgeMap->AddEdge(edgeTwo);
  spEdgeMap->AddEdge(edgeThree);
  EdgeMap::EdgeVectorSP theParentsOne = spEdgeMap->GetParents(childOne);
  EdgeMap::EdgeVectorSP theParentsTwo = spEdgeMap->GetParents(childTwo);
  EdgeMap::EdgeVectorSP theChildrenOne = spEdgeMap->GetChildren(parentOne);
  EdgeMap::EdgeVectorSP theChildrenTwo = spEdgeMap->GetChildren(parentTwo);

  //ensure the correct edges were returned by checking Node LIDs
  assert(theParentsOne->front()->GetParentNode()->GetLID() == parentOne->GetLID());//parent of childOne
  assert(theParentsTwo->front()->GetParentNode()->GetLID() == parentOne->GetLID());//parent of childTwo
  assert(theParentsTwo->back()->GetParentNode()->GetLID() == parentTwo->GetLID());//parent of childTwo
  assert(theChildrenOne->front()->GetChildNode()->GetLID() == childOne->GetLID());//child of parentOne
  assert(theChildrenOne->back()->GetChildNode()->GetLID() == childTwo->GetLID());//child of parentOne
  assert(theChildrenTwo->front()->GetChildNode()->GetLID() == childTwo->GetLID());//child of parentTwo

  //test the DepthFirstTraversal class
  boost::shared_ptr<Visitor> spVisitor(new TestVisitor());  
  boost::shared_ptr<Node> fooOne(new Node());
  boost::shared_ptr<Node> fooTwo(new Node());  
  spEdgeMap->AddEdge(boost::shared_ptr<Edge>(new Edge(childTwo, fooOne)));
  spEdgeMap->AddEdge(boost::shared_ptr<Edge>(new Edge(childTwo, fooTwo)));

  DepthFirstTraversal dfs(spEdgeMap, parentOne);
  dfs.TraverseDown(spVisitor, parentOne);
  std::cout << "********************" << std::endl;
  dfs.TraverseUp(spVisitor, fooOne);

  //test the Acyclic Visitor
  std::cout << "***CYCLE TESTER (no cycles)***" << std::endl;
  //spVisitor.reset(new AcyclicVisitor());
  //dfs.TraverseDown(spVisitor, parentOne);
  
  std::cout << "***CYCLE TESTER (w/ cycle present)***" << std::endl;
  spEdgeMap->AddEdge(boost::shared_ptr<Edge>(new Edge(fooTwo, parentTwo)));//adds a cycle
  dfs.TraverseDown(spVisitor, parentOne);

  std::cout << "Completed Basic Testing Successfully!" << std::endl << std::endl;
}


int blah()
{
  GraphBuilder graphBuild;
  boost::shared_ptr<Visitor> spVisitor;
  boost::shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());
  const std::basic_string<wchar_t> file = L"test.aaf";  

  try
  {
    BasicDataStructureTest();

    if(AAFLoad(getenv("AX_AAF_COMAPI")) == 0)
    {
      TestGraph testGraph(graphBuild.CreateGraph(file, spFactory));
      DepthFirstTraversal dfs(testGraph.GetEdgeMap(), testGraph.GetRootNode()); 
      
      //resolve the references
      //spVisitor.reset(new ResolveRefVisitor(testGraph.GetEdgeMap())); 
      //dfs.TraverseDown(spVisitor, testGraph.GetRootNode());

      //check for cycles
      //spVisitor.reset(new AcyclicVisitor());
      //dfs.TraverseDown(spVisitor, testGraph.GetRootNode());
      
      //now dump the objects to the console
      spVisitor.reset(new TestVisitor());
      dfs.TraverseDown(spVisitor, testGraph.GetRootNode());

      AAFUnload();
    }
    else
      std::cout << "Failed to load library!" << std::endl;
  }
  catch ( const AxExHResult& ex )
  {
    std::wcout << L"Error: " << ex.what() << std::endl;
  }
  catch ( ... )
  {
    std::cout << "Error: unhandled exeption" << std::endl;
  }

  std::cout << "Completed Graph Testing!" << std::endl << std::endl;
  return 0;
}
