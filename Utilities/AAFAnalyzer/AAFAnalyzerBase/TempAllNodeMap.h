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

#ifndef __TEMPALLNODEMAP_h__
#define __TEMPALLNODEMAP_h__

//Ax files
//#include <AxTypes.h>

//boost files
#include <boost/shared_ptr.hpp>

//stl files
#include <map>

namespace aafanalyzer {

class Node;

class TempAllNodeMap
{
 public:

  typedef std::map<unsigned int, boost::shared_ptr<Node> > Map;

  static TempAllNodeMap& GetInstance();
  void AddNode(unsigned int id, boost::shared_ptr<Node> spNode);
  boost::shared_ptr<Node> GetNode(unsigned int id);

 private:

  TempAllNodeMap();
  virtual ~TempAllNodeMap();

  // prohibited
  TempAllNodeMap( const TempAllNodeMap& );
  TempAllNodeMap& operator=( const TempAllNodeMap& );

  static TempAllNodeMap* _pTempAllNodeMap;
  Map _Map;
};

} // end of namespace diskstream

#endif
