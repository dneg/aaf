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

#include <MStructuredStorage.h>
#include <assert.h>


HRESULT 
MStructuredStorage::StgOpenStorage(
                                const TCHAR FAR* pwcsName,
                                IStorage* pstgPriority,
                                DWORD grfMode,
                                SNB snbExclude,
                                DWORD reserved,
                                IStorage** ppstgOpen)
{
    return ::StgOpenStorage(pwcsName, pstgPriority, 
          grfMode, snbExclude, reserved, ppstgOpen);
}                                

HRESULT 
MStructuredStorage::StgIsStorageFile(
                                const TCHAR FAR* pwcsName)
{
    return ::StgIsStorageFile(pwcsName);
}                                

void 
MStructuredStorage::wcsFree(TCHAR FAR*  wcs)
{
    CoTaskMemFree(wcs);
}                                    
                                
MStructuredStorage::MStructuredStorage(void)
{
    CoInitialize(0);
}

MStructuredStorage::~MStructuredStorage(void)
{
    CoUninitialize();
}    

