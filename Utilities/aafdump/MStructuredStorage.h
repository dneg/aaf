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
//
//=---------------------------------------------------------------------=

#ifndef __MSTRUCTUREDSTORAGE_H__
#define __MSTRUCTUREDSTORAGE_H__

#include <StructuredStorage.h>

class MStructuredStorage : public StructuredStorage
{
public:
    MStructuredStorage();
    virtual ~MStructuredStorage();

public:
	virtual HRESULT STDMETHODCALLTYPE StgOpenStorage(
          const TCHAR FAR* pwcsName,
          IStorage* pstgPriority,
          DWORD grfMode,
          SNB snbExclude,
          DWORD reserved,
          IStorage** ppstgOpen);

    virtual HRESULT STDMETHODCALLTYPE StgIsStorageFile(
          const TCHAR FAR* pwcsName);

public:                                
    virtual void wcsFree(TCHAR FAR* wcs);
                                                            
};

#endif // __MSTRUCTUREDSTORAGE_H__
