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

#ifndef __FILELOAD_h__
#define __FILELOAD_h__

//project files
#include <Test.h>

namespace aafanalyzer {

class FileLoad : public Test
{
 public:
  FileLoad(std::ostream& os, const std::basic_string<wchar_t> AAFFile);
  ~FileLoad();

  TestResult Execute();
  std::string GetName();
  std::string GetDescription();

 private:
  const std::basic_string<wchar_t> _File;

  // prohibited
  FileLoad(const FileLoad&);
  FileLoad& operator=( const FileLoad& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
