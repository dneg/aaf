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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMMSStructuredStorage.h"

#include "OMAssertions.h"
#include "OMDynamicLibrary.h"

#include "OMSingleton.h"

#if defined(OM_OS_WINDOWS)
#define LINKAGE __stdcall
const wchar_t* ssLibraryName = L"ole32.dll";
const wchar_t* coLibraryName = L"ole32.dll";
#elif defined(OM_OS_UNIX)
#define LINKAGE
const wchar_t* ssLibraryName = L"librefstg.so";
const wchar_t* coLibraryName = L"librefstg.so";
#endif

typedef OMInt32 (LINKAGE *pStgCreateDocfile_t)(const SSCHAR*,
                                               OMUInt32,
                                               OMUInt32,
                                               IStorage**);
typedef OMInt32 (LINKAGE *pStgCreateDocfileOnILockBytes_t)(ILockBytes*,
                                                           OMUInt32,
                                                           OMUInt32,
                                                           IStorage**);
typedef OMInt32 (LINKAGE *pStgOpenStorage_t)(SSCHAR*,
                                             IStorage*,
                                             OMUInt32,
                                             SSCHAR**,
                                             OMUInt32,
                                             IStorage**);
typedef OMInt32 (LINKAGE *pStgOpenStorageOnILockBytes_t)(ILockBytes*,
                                                         IStorage*,
                                                         OMUInt32,
                                                         SSCHAR**,
                                                         OMUInt32,
                                                         IStorage**);
typedef OMInt32 (LINKAGE *pStgIsStorageFile_t)(const SSCHAR*);

class MSSSLibrary;

class MSSSLibrary : public OMSingleton<MSSSLibrary> {
public:

  MSSSLibrary(void);

  ~MSSSLibrary(void);

  OMInt32 StgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen);

  OMInt32 StgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen);

  OMInt32 StgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen);

  OMInt32 StgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen);

  OMInt32 StgIsStorageFile(const SSCHAR* pwcsName);

private:

  OMDynamicLibrary* library(void);

  OMDynamicLibrary* _ssLibrary;
  pStgCreateDocfile_t _pStgCreateDocfile;
  pStgCreateDocfileOnILockBytes_t _pStgCreateDocfileOnILockBytes ;
  pStgOpenStorage_t _pStgOpenStorage;
  pStgOpenStorageOnILockBytes_t _pStgOpenStorageOnILockBytes;
  pStgIsStorageFile_t _pStgIsStorageFile;
};

MSSSLibrary::MSSSLibrary(void)
: _ssLibrary(0),
  _pStgCreateDocfile(0),
  _pStgCreateDocfileOnILockBytes(0),
  _pStgOpenStorage(0),
  _pStgOpenStorageOnILockBytes(0),
  _pStgIsStorageFile(0)
{
}

MSSSLibrary::~MSSSLibrary(void)
{
  OMDynamicLibrary::unloadLibrary(_ssLibrary);
  _ssLibrary = 0;
}

OMInt32 MSSSLibrary::StgCreateDocfile(const SSCHAR* pwcsName,
                                      OMUInt32 grfMode,
                                      OMUInt32 reserved,
                                      IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgCreateDocfile");

  if (_pStgCreateDocfile == 0) {
    _pStgCreateDocfile = (pStgCreateDocfile_t)library()->findSymbol(
                                                           "StgCreateDocfile");
  }
  ASSERT("Symbol found", _pStgCreateDocfile != 0);
  OMInt32 result = (*_pStgCreateDocfile)(pwcsName,
                                        grfMode,
                                        reserved,
                                        ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                                  OMUInt32 grfMode,
                                                  OMUInt32 reserved,
                                                  IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgCreateDocfileOnILockBytes");

  if (_pStgCreateDocfileOnILockBytes == 0) {
    _pStgCreateDocfileOnILockBytes =
                       (pStgCreateDocfileOnILockBytes_t)library()->findSymbol(
                                               "StgCreateDocfileOnILockBytes");
  }
  ASSERT("Symbol found", _pStgCreateDocfileOnILockBytes != 0);
  OMInt32 result = (*_pStgCreateDocfileOnILockBytes)(plkbyt,
                                                    grfMode,
                                                    reserved,
                                                    ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgOpenStorage(SSCHAR* pwcsName,
                                    IStorage* pstgPriority,
                                    OMUInt32 grfMode,
                                    SSCHAR** snbExclude,
                                    OMUInt32 reserved,
                                    IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgOpenStorage");

  if (_pStgOpenStorage == 0) {
    _pStgOpenStorage = (pStgOpenStorage_t)library()->findSymbol(
                                                             "StgOpenStorage");
  }
  ASSERT("Symbol found", _pStgOpenStorage!= 0);
  OMInt32 result = (*_pStgOpenStorage)(pwcsName,
                                      pstgPriority,
                                      grfMode,
                                      snbExclude,
                                      reserved,
                                      ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgOpenStorageOnILockBytes");

  if (_pStgOpenStorageOnILockBytes == 0) {
    _pStgOpenStorageOnILockBytes =
                         (pStgOpenStorageOnILockBytes_t)library()->findSymbol(
                                                 "StgOpenStorageOnILockBytes");
  }
  ASSERT("Symbol found", _pStgOpenStorageOnILockBytes != 0);
  OMInt32 result = (*_pStgOpenStorageOnILockBytes)(plkbyt,
                                                  pstgPriority,
                                                  grfMode,
                                                  snbExclude,
                                                  reserved,
                                                  ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgIsStorageFile(const SSCHAR* pwcsName)
{
  TRACE("MSSSLibrary::StgIsStorageFile");

  if (_pStgIsStorageFile == 0) {
    _pStgIsStorageFile = (pStgIsStorageFile_t)library()->findSymbol(
                                                           "StgIsStorageFile");
  }
  ASSERT("Symbol found", _pStgIsStorageFile!= 0);
  OMInt32 result = (*_pStgIsStorageFile)(pwcsName);
  return result;
}

OMDynamicLibrary* MSSSLibrary::library(void)
{
  TRACE("MSSSLibrary::library");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);
  return _ssLibrary;
}

OMInt32 OMStgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfile");
  return MSSSLibrary::instance()->StgCreateDocfile(pwcsName,
                                                   grfMode,
                                                   reserved,
                                                   ppstgOpen);
}

OMInt32 OMStgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfileOnILockBytes");
  return MSSSLibrary::instance()->StgCreateDocfileOnILockBytes(plkbyt,
                                                               grfMode,
                                                               reserved,
                                                               ppstgOpen);
}

OMInt32 OMStgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen)
{
  return MSSSLibrary::instance()->StgOpenStorage(pwcsName,
                                                 pstgPriority,
                                                 grfMode,
                                                 snbExclude,
                                                 reserved,
                                                 ppstgOpen);
}

OMInt32 OMStgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen)
{
  TRACE("OMStgOpenStorageOnILockBytes");
  return MSSSLibrary::instance()->StgOpenStorageOnILockBytes(plkbyt,
                                                             pstgPriority,
                                                             grfMode,
                                                             snbExclude,
                                                             reserved,
                                                             ppstgOpen);
}



#ifdef OM_USE_STORAGE_EX

//Below are the definititions for MS Strucutred Storage Library

#if !defined(OM_OS_WINDOWS)
typedef unsigned char BYTE;     // 1 byte
typedef unsigned short WORD;    // 2 bytes
typedef unsigned short USHORT;  // 2 bytes
typedef unsigned int ULONG;     // 4 bytes
typedef unsigned int DWORD;     // 4 bytes
typedef USHORT WCHAR;           // 2 bytes
typedef short OFFSET;           // 2 bytes
typedef USHORT FSOFFSET;        // 2 bytes
#endif
typedef ULONG SECT;             // 4 bytes
typedef ULONG FSINDEX;          // 4 bytes
typedef ULONG DFSIGNATURE;      // 4 bytes
typedef ULONG StrID;            // 4 bytes

//FAT Entries
const SECT MAXREGSECT = 0xFFFFFFFA; // maximum SECT
const SECT DIFSECT    = 0xFFFFFFFC; // denotes a DIFAT sector in a FAT
const SECT FATSECT    = 0xFFFFFFFD; // denotes a FAT sector in a FAT
const SECT ENDOFCHAIN = 0xFFFFFFFE; // end of a virtual stream chain
const SECT FREESECT   = 0xFFFFFFFF; // unallocated sector

const StrID MAXREGSID   = 0xFFFFFFFA; // maximum directory entry ID
const StrID NOSTREAM    = 0xFFFFFFFF; // unallocated directory entry




struct StructuredStorageHeader { // [offset from start (bytes), length (bytes)]
  BYTE _abSig[8];            // [00H,08] {0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 
                             //   0x1a, 0xe1} for current version
  CLSID _clsid;              // [08H,16] reserved must be zero (WriteClassStg/
                             //   GetClassFile uses root directory class id)
  USHORT _uMinorVersion;     // [18H,02] minor version of the format: 33 is 
                             //   written by reference implementation
  USHORT _uDllVersion;       // [1AH,02] major version of the dll/format: 3 for
                             //   512-byte sectors, 4 for 4 KB sectors
  USHORT _uByteOrder;        // [1CH,02] 0xFFFE: indicates Intel byte-ordering
  USHORT _uSectorShift;      // [1EH,02] size of sectors in power-of-two; 
                             //   typically 9 indicating 512-byte sectors
  USHORT _uMiniSectorShift;  // [20H,02] size of mini-sectors in power-of-two; 
                             //   typically 6 indicating 64-byte mini-sectors
  USHORT _usReserved;        // [22H,02] reserved, must be zero
  ULONG _ulReserved1;        // [24H,04] reserved, must be zero
  FSINDEX _csectDir;         // [28H,04] must be zero for 512-byte sectors,
                             //   number of SECTs in directory chain for 4 KB
                             //   sectors
  FSINDEX _csectFat;         // [2CH,04] number of SECTs in the FAT chain
  SECT _sectDirStart;        // [30H,04] first SECT in the directory chain
  DFSIGNATURE _signature;    // [34H,04] signature used for transactions; must
                             //   be zero. The reference implementation
                             //   does not support transactions
  ULONG _ulMiniSectorCutoff; // [38H,04] maximum size for a mini stream; 
                             //   typically 4096 bytes
  SECT _sectMiniFatStart;    // [3CH,04] first SECT in the MiniFAT chain
  FSINDEX _csectMiniFat;     // [40H,04] number of SECTs in the MiniFAT chain
  SECT _sectDifStart;        // [44H,04] first SECT in the DIFAT chain
  FSINDEX _csectDif;         // [48H,04] number of SECTs in the DIFAT chain
  SECT _sectFat[109];        // [4CH,436] the SECTs of first 109 FAT sectors
} ;

struct StructuredStorageDirectoryEntry { 
  WCHAR _ab[32];      // [00H,64] 64 bytes. The element name in Unicode, padded
                      //   with zeros to fill this byte array.  Terminating 
                      //   Unicode NULL is required.
  WORD _cb;           // [40H,02] Length of the Element name in bytes, 
                      //   including the Unicode NULL
  BYTE _mse;          // [42H,01] Type of object. Value taken from the 
                      //   STGTY enumeration
  BYTE _bflags;       // [43H,01] Value taken from DECOLOR enumeration
  StrID _sidLeftSib;  // [44H,04] SID of the left-sibling of this entry 
                      //   in the directory tree
  StrID _sidRightSib; // [48H,04] SID of the right-sibling of this entry 
                      //   in the directory tree
  StrID _sidChild;    // [4CH,04] SID of the child acting as the root of all 
                      //   the children of this element 
                      //   (if _mse=STGTY_STORAGE or STGTY_ROOT)
  CLSID _clsId;       // [50H,16] CLSID of this storage 
                      //   (if _mse=STGTY_STORAGE or STGTY_ROOT)
  DWORD _dwUserFlags; // [60H,04] User flags of this storage 
                      //   (if _mse=STGTY_STORAGE or STGTY_ROOT)
  FILETIME _time[2];  // [64H,16] Create/Modify time-stamps 
                      //   (if _mse=STGTY_STORAGE)
  SECT _sectStart;    // [74H,04] starting SECT of the stream 
                      //   (if _mse=STGTY_STREAM)
  ULONG _ulSizeLow;   // [78H,04] size of stream in bytes 
                      //   (if _mse=STGTY_STREAM)
  ULONG _ulSizeHigh;  // [7CH,02] must be zero for 512-byte sectors, 
                      //   high part of 64-bit size for 4 KB sectors
};


#define SZ_4K  4*1024
#define SZ_512 512
#define NUMB_DIR_ENTRY	(SZ_4K/sizeof(	StructuredStorageDirectoryEntry))

//this structure produces a minmal 4K secotr structured storage file.
//It has 3 sectors Headere, FAT and Directory.
//20040108 Ian Baker
struct MinSS4KFile   {
	union {
		StructuredStorageHeader  HeaderInfo;
		BYTE Header[ SZ_4K ];
	}  ;

	SECT FAT[SZ_4K/sizeof(SECT)];

	union {
		StructuredStorageDirectoryEntry Entry[NUMB_DIR_ENTRY];
        BYTE Dir[SZ_4K];
	};

	MinSS4KFile()
	{
		int i;

		memset(Header,0,sizeof(Header));
		memset(FAT,0xff,sizeof(FAT));
		memset(Dir,0,sizeof(Dir));
		

		BYTE sig[]={0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1,  0x1a, 0xe1};
		memcpy(HeaderInfo._abSig,sig,sizeof(HeaderInfo._abSig));
		memset(&HeaderInfo._clsid,0,sizeof(HeaderInfo._clsid));
		HeaderInfo._uMinorVersion=0x3E;
		HeaderInfo._uDllVersion=4;		   // 4K sectors
		HeaderInfo._uByteOrder=0xFFFE;
		HeaderInfo._uSectorShift=0x0C;	    // 4K sectors
		HeaderInfo._uMiniSectorShift=0x06;
		HeaderInfo._csectDir=0x01;
		HeaderInfo._csectFat=0x01;
		HeaderInfo._sectDirStart=0x01;
		HeaderInfo._ulMiniSectorCutoff=0x1000;
		HeaderInfo._sectMiniFatStart=ENDOFCHAIN;
		HeaderInfo._sectDifStart=ENDOFCHAIN;
		memset(HeaderInfo._sectFat,0xFF,sizeof(HeaderInfo._sectFat));
		HeaderInfo._sectFat[0]=0;  // first block is in use.

		FAT[0]=FATSECT;
		FAT[1]=ENDOFCHAIN ;


		for(i=0;i<NUMB_DIR_ENTRY;i++)
		{
			Entry[i]._sidLeftSib=NOSTREAM;
			Entry[i]._sidRightSib=NOSTREAM;
			Entry[i]._sidChild=NOSTREAM;
		}

		//Assume here that wide charater is 2 bytes.
		// is 4 under solaris but MS lib does not work under Solaris
		//now initialise root entry
		wcscpy(Entry[0]._ab,(L"Root Entry"));
		Entry[0]._cb=(WORD)(wcslen(Entry[0]._ab)+1)*2;
		Entry[0]._mse=5;
		Entry[0]._sectStart=ENDOFCHAIN;
	}
};


//this function does ot exist in the current MS Structured Storage Library
// but is reqruied to create 4K files through the raw interface.
//Therefore the function is simulated in OMMSStructuredStorage.cpp
//040109 Ian Baker Metaglue Corp.

OMInt32 StgCreateDocfileOnILockBytesEx (
									ILockBytes* piLockBytes,
									DWORD grfMode,
									DWORD stgfmt,
									DWORD grfAttrs,
									OM_STGOPTIONS* pStgOptions,
									void* reserved2,
									REFIID riid,
									void** ppstorage   )

{
	TRACE("StgCreateDocfileOnILockBytesEx");

	if(	pStgOptions->ulSectorSize==512)
	{
		HRESULT status = StgCreateDocfileOnILockBytes(
			piLockBytes,
			grfMode,
			0,
			(IStorage **)ppstorage);
			

		return status;
	}

	//Will only handle 512 secotrs (above) or 4K sectors;
	ASSERT ("Bad Sector Size",pStgOptions->ulSectorSize==SZ_4K);

	HRESULT status;
	MinSS4KFile *MinSSFile;

	// Generate a minimal 4 K file. Code for this is in the Header
	MinSSFile = new MinSS4KFile();


	ULARGE_INTEGER startOfFile;
	startOfFile.HighPart=0;
	startOfFile.LowPart=0;

	ULONG lbytesWritten;

	status=piLockBytes->WriteAt(startOfFile,MinSSFile,sizeof(MinSS4KFile),&lbytesWritten);

	// you might expect STGM_CREATE to the call to StgCreateDocfileOnILockBytesEx
	// but not vaild to StgOpenStorageOnILockBytes
	grfMode= grfMode & ~ STGM_CREATE;	//so remove it - to maintain consistent programming interface
	
	status=StgOpenStorageOnILockBytes (
		piLockBytes,
		NULL,
		grfMode  ,
		NULL,
		0,
		(IStorage **) ppstorage);

	return status;
}


#endif // OM_USE_STORAGE_EX



OMInt32 OMStgIsStorageFile(const SSCHAR* pwcsName)
{
  TRACE("OMStgIsStorageFile");
  return MSSSLibrary::instance()->StgIsStorageFile(pwcsName);
}

typedef OMInt32 (LINKAGE *pCoInitialize_t)(void*);
typedef void (LINKAGE *pCoUninitialize_t)(void);

class MSCOMLibrary;

class MSCOMLibrary : public OMSingleton<MSCOMLibrary> {
public:

  MSCOMLibrary(void);

  ~MSCOMLibrary(void);

  OMInt32 CoInitialize(void* pvReserved);

  void CoUninitialize(void);

private:

  OMDynamicLibrary* library(void);

  OMDynamicLibrary* _coLibrary;
  pCoInitialize_t _pCoInitialize;
  pCoUninitialize_t _pCoUninitialize;
};

MSCOMLibrary::MSCOMLibrary(void)
: _coLibrary(0),
  _pCoInitialize(0),
  _pCoUninitialize(0)
{
}

MSCOMLibrary::~MSCOMLibrary(void)
{
  OMDynamicLibrary::unloadLibrary(_coLibrary);
  _coLibrary = 0;
}

OMInt32 MSCOMLibrary::CoInitialize(void* pvReserved)
{
  TRACE("MSCOMLibrary::CoInitialize");

  if (_pCoInitialize == 0) {
    _pCoInitialize = (pCoInitialize_t)library()->findSymbol("CoInitialize");
  }
  ASSERT("Symbol found", _pCoInitialize!= 0);
  OMInt32 result = (*_pCoInitialize)(pvReserved);
  return result;
}

void MSCOMLibrary::CoUninitialize(void)
{
  TRACE("MSCOMLibrary::OMCoUninitialize");

  if (_pCoUninitialize == 0) {
    _pCoUninitialize = (pCoUninitialize_t)library()->findSymbol(
                                                             "CoUninitialize");
  }
  ASSERT("Symbol found", _pCoUninitialize!= 0);
  (*_pCoUninitialize)();
}

OMDynamicLibrary* MSCOMLibrary::library(void)
{
  TRACE("MSCOMLibrary::library");
  if (_coLibrary == 0) {
    _coLibrary = OMDynamicLibrary::loadLibrary(coLibraryName);
  }
  ASSERT("Library loaded", _coLibrary != 0);
  return _coLibrary;
}

OMInt32 OMCoInitialize(void* pvReserved)
{
  TRACE("OMCoInitialize");
  return MSCOMLibrary::instance()->CoInitialize(pvReserved);
}

void OMCoUninitialize(void)
{
  TRACE("OMCoUninitialize");
  MSCOMLibrary::instance()->CoUninitialize();
}

#if defined(OM_USE_WINDOWS_SS)
static int _COMIsInitialized; // automatically initialized to 0
#endif

void OMMSSInitialize(void)
{
#if defined(OM_USE_WINDOWS_SS)
  if (!_COMIsInitialized) {
    CoInitialize(0);
    _COMIsInitialized = 1;
  }
#endif
}

void OMMSSFinalize(void)
{
#if defined(OM_USE_WINDOWS_SS)
  if (_COMIsInitialized) {
    CoUninitialize();
    _COMIsInitialized = 0;
  }
#endif
  MSSSLibrary::destroy();
  MSCOMLibrary::destroy();
}

#endif // !OM_NO_STRUCTURED_STORAGE
