/*****************************************************************************\
*                                                                             *
* macapi.h - 	MAC specific API's. All grouped here for reference.			  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__MACAPI__) && !defined(_MACAPI_H_)
#define __MACAPI__
#define _MACAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

STDAPI OleSetClipboardEx(LPDATAOBJECT pDataObj);
STDAPI OleSetClipboardLate(LPDATAOBJECT pDataObj, 
	unsigned long dwSig, unsigned long dwreserved);

// handlers for AE's 
STDAPI_(OSErr)	OleProcessClipboardAE(AppleEvent *, AppleEvent *, long);
STDAPI_(OSErr)	OleProcessLrpcAE	 (AppleEvent *, AppleEvent *, long);
STDAPI_(OSErr)	OleProcessDdeAE		 (AppleEvent *, AppleEvent *, long);

STDAPI_(unsigned long) OLEInitDBCSCountry(int iCntry);

// Debugging API support.
#if defined(_DEBUG) && !defined(MAC_STATIC)
STDAPI_(void) FnAssertOn(Boolean);
#endif // _DEBUG

STDAPI_(unsigned long)  OleQueryCreateAll (LPDATAOBJECT lpSrcDataObj, unsigned long *pdwResult);
#define OLE_CREATE_EMBED	0x0001
#define OLE_CREATE_STATIC	0x0002
#define OLE_CREATE_LINK 	0x0004

STDAPI_(OLEREG_HKEY)	OleregOpenRegistration (void);
STDAPI_(OLEREGSTATUS)	OleregCloseRegistration (OLEREG_HKEY);
STDAPI_(OLEREGSTATUS)	OleregGetValue (const char *, OLELONG, OLELONG, OLEREG_VALUE, OLELONG *);
STDAPI_(OLEREGSTATUS)	OleregSetValue (const char *, OLELONG, OLEREG_ORDER, OLEREG_VALUE, OLELONG);
STDAPI_(OLEREGSTATUS)	OleregRemoveKey(const char *, OLELONG, OLELONG);


STDAPI_(OSErr)	OleMakeFSSpec(short vRefNum,
							  long dirID,
							  const char *szFileName,
							  FSSpecPtr pFSSpec);

STDAPI_(OSErr)	OleFullPathFromFSSpec(FSSpecPtr pFSSpec,
									  char *szFileName,
									  unsigned long *pcb);

STDAPI_(OSErr) OleGetFSSpecInfo(FSSpecPtr pFSSpec,
								unsigned long *pfIsDir,
								long *pDirID);


STDAPI OleCreateLinkToFSp(const FSSpec *pFSSpec, REFIID riid, 
            unsigned long renderopt, LPFORMATETC lpFormatEtc, 
            LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void **ppvObj);

STDAPI OleCreateFromFSp (REFCLSID rclsid, const FSSpec *pFSSpec,
			REFIID iid, unsigned long renderopt, LPFORMATETC lpFormatEtc,
			IOleClientSite *pClientSite, IStorage *pStg, void **ppvObj);


STDAPI GetClassFSp(const FSSpec *pFSSpec, CLSID *pcid);
STDAPI CreateFileMonikerFSp (const FSSpec *pFSSpec, LPLPMONIKER ppmk);
STDAPI MkParseDisplayNameMac(LPBC pbc, char * szUserName, short vRefNum, long dirID, 
                unsigned long  * pchEaten, LPLPMONIKER ppmk);

STDAPI MkGetMacNetInfo(IMoniker *pmk, char **ppszZoneName, char **ppszServerName);
STDAPI MkGetFileMonWinPath(IMoniker *pmk, char **ppszWinPath, OLEBOOL *pfGeneratedFromMacPath);

STDAPI_(Handle)	OleMetaFileToPict(Handle hMf, Rect *prect, short x, short y);

/* Icon extraction Helper APIs */
STDAPI_(PicHandle) OleGetIconOfFSp(FSSpec *pFSSpec, OLEBOOL fUseFileAsLabel);
STDAPI_(PicHandle) OlePictFromIconAndLabel(PicHandle hIcon, char * lpszLabel, OleIconSource * pIconSource);

STDAPI_(PicHandle) OleGetIconFromIconSuite(Handle hSuite, char * lpszLabel, OleIconSource * pIconSource);

STDAPI_(void) OleUIPictIconFree(PicHandle hPict);
STDAPI_(unsigned long) OleUIPictExtractLabel(PicHandle hPict, char *pszLabel, unsigned long cchLabel);
STDAPI_(PicHandle) OleUIPictExtractIcon(PicHandle hPict);
STDAPI_(unsigned long) OleUIPictIconDraw(GrafPtr pGrafPort, Rect *pRect, PicHandle hPict, OLEBOOL fIconOnly);
STDAPI_(unsigned long) OleUIPictExtractIconSource(PicHandle hPict, OleIconSource *pIconSource);

/* the following two apis are used to read/write Ole1 ProgIDs corresponding to the
   underlying format of the data from/to stgs containing Ole1 objects.
*/   
STDAPI ReadOle1FmtProgIDStgMac(IStorage *pstg, char **ppszOle1ProgID);
STDAPI WriteOle1FmtProgIDStgMac(IStorage *pstg, const char *pszOle1ProgID);




#ifndef HKEY
#define HKEY unsigned long
#endif

#define HKEY_CLASSES_ROOT           (( HKEY ) 0x80000000 )

#define REG_SZ                      ( 1 )
#define REG_BINARY                  ( 3 )


/* Registration Database Prototypes. */


STDAPI_(long)  RegCloseKey(HKEY hkey);
STDAPI_(long)  _RegCloseKey(HKEY hkey);
STDAPI_(long)  StaticRegCloseKey(HKEY hkey);

STDAPI_(long)  RegCreateKey(HKEY hkey, const char * lpszSubKey, HKEY * lphkResult);
STDAPI_(long)  _RegCreateKey(HKEY hkey, const char * lpszSubKey, HKEY * lphkResult);
STDAPI_(long)  StaticRegCreateKey(HKEY hkey, const char * lpszSubKey, HKEY * lphkResult);
 
STDAPI_(long)  RegDeleteKey(HKEY hkey, const char * lpszSubKey);
STDAPI_(long)  _RegDeleteKey(HKEY hkey, const char * lpszSubKey);
STDAPI_(long)  StaticRegDeleteKey(HKEY hkey, const char * lpszSubKey);

STDAPI_(long)  RegEnumKey(HKEY hkey, unsigned long iSubkey, char * lpszBuffer, unsigned long cbBuffer);
STDAPI_(long)  _RegEnumKey(HKEY hkey, unsigned long iSubkey, char *lpszBuffer, unsigned long cbBuffer);
STDAPI_(long)  StaticRegEnumKey(HKEY hkey, unsigned long iSubkey, char *lpszBuffer, unsigned long cbBuffer);

STDAPI_(long) RegEnumProgID(HKEY hkey, unsigned long iSubkey, 
							char *lpszBuffer, unsigned long cbBuffer, 
							char *lpszMainUserType, unsigned long cbMainUserTypeBuf, 
							unsigned long * lpgrfFlags);
STDAPI_(long) _RegEnumProgID(HKEY hkey, unsigned long iSubkey, 
							char *lpszBuffer, unsigned long cbBuffer, 
							char *lpszMainUserType, unsigned long cbMainUserTypeBuf, 
							unsigned long * lpgrfFlags);
STDAPI_(long) StaticRegEnumProgID(HKEY hkey, unsigned long iSubkey, 
							char *lpszBuffer, unsigned long cbBuffer, 
							char *lpszMainUserType, unsigned long cbMainUserTypeBuf, 
							unsigned long * lpgrfFlags);

STDAPI_(long)  RegOpenKey(HKEY hkey, const char *lpszSubKey, HKEY * lphkResult);
STDAPI_(long)  _RegOpenKey(HKEY hkey, const char *lpszSubKey, HKEY * lphkResult);
STDAPI_(long)  StaticRegOpenKey(HKEY hkey, const char *lpszSubKey, HKEY * lphkResult);

STDAPI_(long)  RegQueryValue(HKEY hkey, const char *lpszSubKey, char *lpszValue, long * lpcb);
STDAPI_(long)  _RegQueryValue(HKEY hkey, const char *lpszSubKey, char *lpszValue, long * lpcb);
STDAPI_(long)  StaticRegQueryValue(HKEY hkey, const char *lpszSubKey, char *lpszValue, long * lpcb);

STDAPI_(long)  RegQueryValueEx( HKEY hkey,
								const char *lpszValueName,
								unsigned long * dwReserved, 
								unsigned long * lpdwType,
								unsigned char *lpbData,
								long * lpcbData);

STDAPI_(long)  _RegQueryValueEx( HKEY hkey,
								const char *lpszValueName,
								unsigned long * dwReserved, 
								unsigned long * lpdwType,
								unsigned char *lpbData,
								long * lpcbData);

STDAPI_(long)  StaticRegQueryValueEx( HKEY hkey,
								const char *lpszValueName,
								unsigned long * dwReserved, 
								unsigned long * lpdwType,
								unsigned char *lpbData,
								long * lpcbData);
								

STDAPI_(long)  RegSetValue(HKEY hkey, const char *lpszSubKey, unsigned long fdwType, const char *lpszValue, unsigned long cb);
STDAPI_(long)  _RegSetValue(HKEY hkey, const char *lpszSubKey, unsigned long fdwType, const char *lpszValue, unsigned long cb);
STDAPI_(long)  StaticRegSetValue(HKEY hkey, const char *lpszSubKey, unsigned long fdwType, const char *lpszValue, unsigned long cb);

STDAPI_(long)  _CloseDB( void );
STDAPI_(long)  StaticCloseDB( void );

STDAPI_(long) RegInitialize(void);
STDAPI_(long) _RegInitialize(void);
STDAPI_(long) StaticRegInitialize(void);

STDAPI_(long)  RegFlush( void );
STDAPI_(long)  _RegFlush( void );
STDAPI_(long)  StaticRegFlush( void );

STDAPI_(long) RegDeleteValue(HKEY hKey, const char * lpszValue);
STDAPI_(long) _RegDeleteValue(HKEY hKey, const char * lpszValue);
STDAPI_(long) StaticRegDeleteValue(HKEY hKey, const char * lpszValue);

STDAPI_(long) RegSetValueEx(HKEY hKey,
							const char * lpszValueName,
							unsigned long dwReserved,
							unsigned long dwType,
							const char * lpszValue,
							unsigned long dwValSize);
STDAPI_(long) _RegSetValueEx(HKEY hKey,
							const char * lpszValueName,
							unsigned long dwReserved,
							unsigned long dwType,
							const char * lpszValue,
							unsigned long dwValSize);
STDAPI_(long) StaticRegSetValueEx(HKEY hKey,
								const char * lpszValueName,
								unsigned long dwReserved,
								unsigned long dwType,
								const char * lpszValue,
								unsigned long dwValSize);


STDAPI_(long) RegEnumValue(HKEY hKey,
						unsigned long dwIdx,
						char * lpszValue,
						unsigned long *lpcbValue,
						unsigned long *lpdwReserved,
						unsigned long *lpdwType,
						unsigned char * lpbData,
						unsigned long *lpcbData);
STDAPI_(long) _RegEnumValue(HKEY hKey,
						unsigned long dwIdx,
						char * lpszValue,
						unsigned long *lpcbValue,
						unsigned long *lpdwReserved,
						unsigned long *lpdwType,
						unsigned char * lpbData,
						unsigned long *lpcbData);
STDAPI_(long) StaticRegEnumValue(HKEY hKey,
							unsigned long dwIdx,
							char * lpszValue,
							unsigned long *lpcbValue,
							unsigned long *lpdwReserved,
							unsigned long *lpdwType,
							unsigned char * lpbData,
							unsigned long *lpcbData); 

STDAPI_(long) RegImportFromText(char* lpTextBuffer, unsigned long cbSize,
								char* lpszVerifyKey, OLEBOOL fDelete);
STDAPI_(long) RegImportFromFile(FSSpec fsp, ResType rtResource, 
								OLEBOOL fVerify, OLEBOOL fDelete);
STDAPI_(long) RegImportFromFolder(FSSpec fsp, ResType rtResource, char* lpTypeList, 
								  OLEBOOL fVerify, OLEBOOL fDelete);

STDAPI_(long) RegSetFileLocationToReg   (HKEY hKey, const char * lpSubkeyName, const FSSpec *pfspec, Handle hReserved);
STDAPI_(long) RegGetFileLocationFromReg (HKEY hKey, const char * lpSubkeyName,       FSSpec *pfspec);


// MAC Static API's.

STDAPI_(OSErr)	_OleMakeFSSpec(short vRefNum,
							  long dirID,
							  const char *szFileName,
							  FSSpecPtr pFSSpec);

STDAPI_(OSErr)	StaticOleMakeFSSpec(short vRefNum,
							  long dirID,
							  const char *szFileName,
							  FSSpecPtr pFSSpec);
						

STDAPI_(OSErr)	_OleFullPathFromFSSpec(FSSpecPtr pFSSpec,
									  char *szFileName,
									  unsigned long *pcb);

STDAPI_(OSErr)	StaticOleFullPathFromFSSpec(FSSpecPtr pFSSpec,
									  char *szFileName,
									  unsigned long *pcb);


STDAPI_(OSErr) _OleGetFSSpecInfo(FSSpecPtr pFSSpec,
								unsigned long *pfIsDir,
								long *pDirID);

STDAPI_(OSErr) StaticOleGetFSSpecInfo(FSSpecPtr pFSSpec,
								unsigned long *pfIsDir,
								long *pDirID);

STDAPI StaticStgCreateDocfile(const char * pwcsName,
            unsigned long grfMode,
            unsigned long reserved,
            IStorage  *  *ppstgOpen);
STDAPI _StgCreateDocfile(const char * pwcsName,
            unsigned long grfMode,
            unsigned long reserved,
            IStorage  *  *ppstgOpen);
STDAPI _StgCreateDocfileOnILockBytes(ILockBytes  *plkbyt,
                    unsigned long grfMode,
                    unsigned long reserved,
                    IStorage  *  *ppstgOpen);

STDAPI StaticStgCreateDocfileOnILockBytes(ILockBytes  *plkbyt,
                    unsigned long grfMode,
                    unsigned long reserved,
                    IStorage  *  *ppstgOpen);
					
STDAPI _StgOpenStorage(const char * pwcsName,
              IStorage  *pstgPriority,
              unsigned long grfMode,
              SNB snbExclude,
              unsigned long reserved,
              IStorage  *  *ppstgOpen);
			  
STDAPI StaticStgOpenStorage(const char * pwcsName,
              IStorage  *pstgPriority,
              unsigned long grfMode,
              SNB snbExclude,
              unsigned long reserved,
              IStorage  *  *ppstgOpen);
STDAPI StaticStgOpenStorageOnILockBytes(ILockBytes  *plkbyt,
                  IStorage  *pstgPriority,
                  unsigned long grfMode,
                  SNB snbExclude,
                  unsigned long reserved,
                  IStorage  *  *ppstgOpen);

STDAPI _StgOpenStorageOnILockBytes(ILockBytes  *plkbyt,
                  IStorage  *pstgPriority,
                  unsigned long grfMode,
                  SNB snbExclude,
                  unsigned long reserved,
                  IStorage  *  *ppstgOpen);

STDAPI _StgSetTimes( const char * lpszName,
          			const FILETIME * pctime,
                  	const FILETIME * patime,
                  	const FILETIME * pmtime);

STDAPI StaticStgSetTimes( const char * lpszName,
          			const FILETIME * pctime,
                  	const FILETIME * patime,
                  	const FILETIME * pmtime);
				  
STDAPI _StgIsStorageFile(const char * pwcsName);
STDAPI StaticStgIsStorageFile(const char * pwcsName);
STDAPI _StgIsStorageILockBytes(ILockBytes * plkbyt);
STDAPI StaticStgIsStorageILockBytes(ILockBytes * plkbyt);


STDAPI _StgCreateDocfileMac(const char *szFileName,
							short vRefNum,
							long  dirID,
							OSType creator,
							OSType fileType,
            				unsigned long grfMode,
							unsigned long reserved, 
							IStorage **ppstgOpen);

STDAPI StgCreateDocfileMac(const char *szFileName,
							short vRefNum,
							long  dirID,
							OSType creator,
							OSType fileType,
							unsigned long grfMode,
							unsigned long reserved,
							IStorage **ppstgOpen);
			
STDAPI StaticStgCreateDocfileMac(const char *szFileName,
							short vRefNum,
							long  dirID,
							OSType creator,
							OSType fileType,
							unsigned long grfMode,
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI StgCreateDocfileFSp(const FSSpec *pFSSpec,
							OSType creator, 
							OSType fileType,
							ScriptCode scriptTag,
							unsigned long grfMode,
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI _StgCreateDocfileFSp(const FSSpec *pFSSpec,
							OSType creator,
							OSType fileType, 
							ScriptCode scriptTag,
 							unsigned long grfMode,
							unsigned long reserved,
							IStorage **ppstgOpen);
			
STDAPI StaticStgCreateDocfileFSp(const FSSpec *pFSSpec,
							OSType creator,
							OSType fileType,
							ScriptCode scriptTag,
							unsigned long grfMode,
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI StgOpenStorageMac(const char *szFileName, 
							short vRefNum,
							long  dirID,
							IStorage *pstgPriority,
							const unsigned long grfMode,
							SNB snbExclude,
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI _StgOpenStorageMac(const char *szFileName,
							short vRefNum,
							long  dirID,
							IStorage *pstgPriority,
							const unsigned long grfMode,
							SNB snbExclude,
							unsigned long reserved, 
							IStorage **ppstgOpen);
			  
STDAPI StaticStgOpenStorageMac(const char *szFileName,
							short vRefNum,
							long  dirID,
							IStorage *pstgPriority,
							const unsigned long grfMode,
							SNB snbExclude,
							unsigned long reserved, 
							IStorage **ppstgOpen);
			  

STDAPI StgOpenStorageFSp(const FSSpec *pFSSpec,
							IStorage *pstgPriority,
							unsigned long grfMode,
							SNB snbExclude, 
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI _StgOpenStorageFSp(const FSSpec *pFSSpec,
							IStorage *pstgPriority,
							unsigned long grfMode,
							SNB snbExclude,
							unsigned long reserved,
							IStorage **ppstgOpen);
			  
STDAPI StaticStgOpenStorageFSp(const FSSpec *pFSSpec,
							IStorage *pstgPriority, 
							unsigned long grfMode,
							SNB snbExclude,
							unsigned long reserved,
							IStorage **ppstgOpen);

STDAPI StgIsStorageFileMac(const char *szFileName,
							short vRefNum,
							long dirID);

STDAPI _StgIsStorageFileMac(const char *szFileName,
							short vRefNum,
							long dirID);

STDAPI StaticStgIsStorageFileMac(const char *szFileName,
							short vRefNum,
							long dirID);

STDAPI StgIsStorageFileFSp(const FSSpec *pFSSpec);
STDAPI _StgIsStorageFileFSp(const FSSpec *pFSSpec);
STDAPI StaticStgIsStorageFileFSp(const FSSpec *pFSSpec);


STDAPI StgSetTimesMac( const char * lpszName,
						short vRefNum,
						long  dirID,
          				const FILETIME * pctime,
                  		const FILETIME * patime,
                  		const FILETIME * pmtime);

STDAPI _StgSetTimesMac( const char * lpszName,
						short vRefNum,
						long  dirID,
          				const FILETIME * pctime,
                  		const FILETIME * patime,
                  		const FILETIME * pmtime);

STDAPI StaticStgSetTimesMac( const char * lpszName,
						short vRefNum,
						long  dirID,
          				const FILETIME * pctime,
                  		const FILETIME * patime,
                  		const FILETIME * pmtime);


STDAPI StgSetTimesFSp( const FSSpec *pFSSpec,
          				const FILETIME * pctime,
                  		const FILETIME * patime,
                  		const FILETIME * pmtime);
STDAPI _StgSetTimesFSp( const FSSpec *pFSSpec,
          				const FILETIME * pctime,
                  		const FILETIME * patime,
                  		const FILETIME * pmtime);

STDAPI StaticStgSetTimesFSp( const FSSpec *pFSSpec,
          					const FILETIME * pctime,
                  			const FILETIME * patime,
                  			const FILETIME * pmtime);


// given a stg, these give back the fsspec or fref of underlying file

STDAPI StgGetFSpFromIStorage(const IStorage *pStg, FSSpec *pFSSpec);
STDAPI _StgGetFSpFromIStorage(const IStorage *pStg, FSSpec *pFSSpec);
STDAPI StaticStgGetFSpFromIStorage(const IStorage *pStg, FSSpec *pFSSpec);

STDAPI StgGetFRefFromIStorage(const IStorage *pStg, short *pFRefNum);
STDAPI _StgGetFRefFromIStorage(const IStorage *pStg, short *pFRefNum);
STDAPI StaticStgGetFRefFromIStorage(const IStorage *pStg, short *pFRefNum);


// static OLE helper API

STDAPI_(unsigned long) _IsEqualGUID(REFGUID guid1, REFGUID guid2);
STDAPI_(unsigned long) StaticIsEqualGUID(REFGUID guid1, REFGUID guid2);

STDAPI_(int) _StringFromGUID2(REFGUID rguid, char * lpsz, int cbMax);
STDAPI_(int) StaticStringFromGUID2(REFGUID rguid, char * lpsz, int cbMax);

STDAPI_(unsigned long) _GUIDFromString(char * lpsz, LPGUID pguid);
STDAPI_(unsigned long) StaticGUIDFromString(char * lpsz, LPGUID pguid);

STDAPI _StringFromCLSID(REFCLSID rclsid, char ** lplpsz);
STDAPI StaticStringFromCLSID(REFCLSID rclsid, char ** lplpsz);

STDAPI _CLSIDFromString(char * lpsz, LPCLSID lpclsid);
STDAPI StaticCLSIDFromString(char * lpsz, LPCLSID lpclsid);

STDAPI _IIDFromString(char * lpsz, LPIID lpiid);
STDAPI StaticIIDFromString(char * lpsz, LPIID lpiid);

STDAPI _StringFromIID(REFIID rclsid, char ** lplpsz);
STDAPI StaticStringFromIID(REFIID rclsid, char ** lplpsz);

STDAPI _ReadClassStg( IStorage  * pstg, LPCLSID pclsid);
STDAPI StaticReadClassStg( IStorage  * pstg, LPCLSID pclsid);

STDAPI _WriteClassStg( IStorage  * pstg, REFCLSID clsid);
STDAPI StaticWriteClassStg( IStorage  * pstg, REFCLSID clsid);

STDAPI_(unsigned long)  _CoIsOle1Class(REFCLSID rclsid);
STDAPI_(unsigned long)  StaticCoIsOle1Class(REFCLSID rclsid);

STDAPI _CoGetTreatAsClass(REFCLSID clsidOld, LPCLSID lpClsidNew);
STDAPI StaticCoGetTreatAsClass(REFCLSID clsidOld, LPCLSID lpClsidNew);

STDAPI  _CoTreatAsClass(REFCLSID clsidOld, REFCLSID clsidNewIn);
STDAPI  StaticCoTreatAsClass(REFCLSID clsidOld, REFCLSID clsidNewIn);

STDAPI_(unsigned long) _CoFileTimeToMacDateTime(FILETIME * lpFileTime, unsigned long *psecs);
STDAPI_(unsigned long) StaticCoFileTimeToMacDateTime(FILETIME * lpFileTime, unsigned long *psecs);

STDAPI_(unsigned long) _CoMacDateTimeToFileTime(unsigned long secs, FILETIME * lpFileTime);
STDAPI_(unsigned long) StaticCoMacDateTimeToFileTime(unsigned long secs, FILETIME * lpFileTime);

STDAPI _CoFileTimeNow( FILETIME * lpFileTime );
STDAPI StaticCoFileTimeNow( FILETIME * lpFileTime );

STDAPI  _CoInitialize(LPMALLOC pMalloc);
STDAPI  StaticCoInitialize(LPMALLOC pMalloc);

STDAPI_(void)   _CoUninitialize(void);
STDAPI_(void)   StaticCoUninitialize(void);

STDAPI  _OleInitialize(LPMALLOC pMalloc);
STDAPI  StaticOleInitialize(LPMALLOC pMalloc);

STDAPI_(void)   _OleUninitialize(void);
STDAPI_(void)   StaticOleUninitialize(void);

STDAPI  _CoGetMalloc(unsigned long dwMemContext, LPLPMALLOC ppMalloc);
STDAPI  StaticCoGetMalloc(unsigned long dwMemContext, LPLPMALLOC ppMalloc);

STDAPI  _CoCreateStandardMalloc(unsigned long memctx, IMalloc **ppMalloc);
STDAPI  StaticCoCreateStandardMalloc(unsigned long memctx, IMalloc **ppMalloc);

STDAPI_(unsigned long)  _CoBuildVersion( void );
STDAPI_(unsigned long)  StaticCoBuildVersion( void );

STDAPI_(unsigned long) _OleBuildVersion( void );
STDAPI_(unsigned long) StaticOleBuildVersion( void );

STDAPI _ReadClassStm(LPSTREAM pStm, LPCLSID pclsid);
STDAPI StaticReadClassStm(LPSTREAM pStm, LPCLSID pclsid);

STDAPI _WriteClassStm(LPSTREAM pStm, REFCLSID clsid);
STDAPI StaticWriteClassStm(LPSTREAM pStm, REFCLSID clsid);

STDAPI _GetDocumentBitStg(LPSTORAGE pStg);
STDAPI StaticGetDocumentBitStg(LPSTORAGE pStg);

STDAPI _SetDocumentBitStg(LPSTORAGE pStg, OLEBOOL fDocument);
STDAPI StaticSetDocumentBitStg(LPSTORAGE pStg, OLEBOOL fDocument);

STDAPI _GetConvertStg(LPSTORAGE pStg);
STDAPI StaticGetConvertStg(LPSTORAGE pStg);

STDAPI _SetConvertStg(LPSTORAGE pStg, OLEBOOL fConvert);
STDAPI StaticSetConvertStg(LPSTORAGE pStg, OLEBOOL fConvert);

STDAPI _OleRegGetUserType (REFCLSID clsid,unsigned long dwFormOfType, char**  pszUserType);
STDAPI StaticOleRegGetUserType (REFCLSID clsid,unsigned long dwFormOfType, char**  pszUserType);

STDAPI _OleRegGetMiscStatus(REFCLSID clsid,unsigned long dwAspect, unsigned long *pdwStatus);
STDAPI StaticOleRegGetMiscStatus(REFCLSID clsid,unsigned long dwAspect, unsigned long *pdwStatus);

STDAPI _OleGetAutoConvert(REFCLSID clsidOld, LPCLSID pClsidNew);
STDAPI StaticOleGetAutoConvert(REFCLSID clsidOld, LPCLSID pClsidNew);

STDAPI _OleSetAutoConvert(REFCLSID clsidOld, REFCLSID clsidNew);
STDAPI StaticOleSetAutoConvert(REFCLSID clsidOld, REFCLSID clsidNew);

STDAPI _ProgIDFromCLSID (REFCLSID clsid,char ** pszProgID);
STDAPI StaticProgIDFromCLSID (REFCLSID clsid,char ** pszProgID);

STDAPI _CLSIDFromProgID(const char *  szProgID,	LPCLSID pclsid);
STDAPI StaticCLSIDFromProgID(const char *  szProgID,	LPCLSID pclsid);

STDAPI _WriteFmtUserTypeStg(LPSTORAGE	pstg, unsigned long cf, char * szUserType);
STDAPI StaticWriteFmtUserTypeStg(LPSTORAGE	pstg, unsigned long cf, char * szUserType);

STDAPI _ReadFmtUserTypeStg(IStorage  * pstg, unsigned long * pcf, char ** pszUserType);
STDAPI StaticReadFmtUserTypeStg(IStorage  * pstg, unsigned long * pcf,char ** pszUserType);


/* Storage Utility APIs */
STDAPI _GetHGlobalFromILockBytes (LPLOCKBYTES plkbyt, Handle * phglobal);
STDAPI StaticGetHGlobalFromILockBytes (LPLOCKBYTES plkbyt, Handle * phglobal);

STDAPI _CreateILockBytesOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease,LPLPLOCKBYTES pplkbyt);
STDAPI StaticCreateILockBytesOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease,LPLPLOCKBYTES pplkbyt);

STDAPI _GetHGlobalFromStream (LPSTREAM pstm, Handle * phglobal);
STDAPI StaticGetHGlobalFromStream (LPSTREAM pstm, Handle * phglobal);

STDAPI _CreateStreamOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease, LPLPSTREAM ppstm);
STDAPI StaticCreateStreamOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease, LPLPSTREAM ppstm);


STDAPI_(OLEREG_HKEY)	_OleregOpenRegistration (void);
STDAPI_(OLEREG_HKEY)	StaticOleregOpenRegistration (void);
						
STDAPI_(OLEREGSTATUS)	_OleregCloseRegistration (OLEREG_HKEY);
STDAPI_(OLEREGSTATUS)	StaticOleregCloseRegistration (OLEREG_HKEY);

STDAPI_(OLEREGSTATUS)	_OleregGetValue (const char *, OLELONG, OLELONG, OLEREG_VALUE, OLELONG *);
STDAPI_(OLEREGSTATUS)	StaticOleregGetValue (const char *, OLELONG, OLELONG, OLEREG_VALUE, OLELONG *);

STDAPI_(OLEREGSTATUS)	_OleregSetValue (const char *, OLELONG, OLEREG_ORDER, OLEREG_VALUE, OLELONG);
STDAPI_(OLEREGSTATUS)	StaticOleregSetValue (const char *, OLELONG, OLEREG_ORDER, OLEREG_VALUE, OLELONG);

STDAPI_(OLEREGSTATUS)	_OleregRemoveKey(const char *, OLELONG, OLELONG);
STDAPI_(OLEREGSTATUS)	StaticOleregRemoveKey(const char *, OLELONG, OLELONG);

STDAPI_(unsigned long) _CoGetCurrentProcess(void);
STDAPI_(unsigned long) StaticCoGetCurrentProcess(void);



// API's that are NOT statically linked. The _XXXX API either thunks to the DLL, or calls OleNYI API
// which in the default implementation simply asserts. Apps can link in their own implementation
// of OleNYI.

STDAPI _InitOleManager(long dwFlags);
STDAPI_(unsigned long) _UninitOleManager(void);

STDAPI_(unsigned long) _OLEInitDBCSCountry(int iCntry);
STDAPI  _CoRegisterClassObject(REFCLSID rclsid, LPUNKNOWN pUnk,unsigned long dwClsContext, unsigned long flags, unsigned long * lpdwRegister);
STDAPI  _CoRevokeClassObject(unsigned long dwRegister);
STDAPI  _CoGetClassObject(REFCLSID rclsid, unsigned long dwClsContext, void * pvReserved,REFIID riid, void * * ppv);
STDAPI  _CoMarshalInterface(LPSTREAM pStm, REFIID riid, LPUNKNOWN pUnk,unsigned long dwDestContext, void * pvDestContext, unsigned long mshlflags);
STDAPI  _CoUnmarshalInterface(LPSTREAM pStm, REFIID riid, void * * ppv);

STDAPI _CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, unsigned long dwClsContext, REFIID riid, void * * ppv);
STDAPI _CoDisconnectObject(LPUNKNOWN pUnk, unsigned long dwReserved);
STDAPI _CoReleaseMarshalData(LPSTREAM pStm);
STDAPI _CoRegisterMessageFilter( LPMESSAGEFILTER lpMessageFilter,LPMESSAGEFILTER * lplpMessageFilter );
STDAPI_(unsigned long) _CoIsHandlerConnected(LPUNKNOWN pUnk);
STDAPI _CoMarshalHresult(LPSTREAM pstm, HRESULT hresult);
STDAPI _CoUnmarshalHresult(LPSTREAM pstm, HRESULT  * phresult);
STDAPI _CoLockObjectExternal(LPUNKNOWN pUnk, OLEBOOL fLock, OLEBOOL fLastUnlockReleases);
STDAPI _CoGetStandardMarshal(REFIID riid, LPUNKNOWN pUnk, unsigned long dwDestContext, void * pvDestContext, unsigned long mshlflags,	LPMARSHAL * ppMarshal);
STDAPI _CoCreateGuid(GUID  *pguid);

STDAPI_(OSErr)	_OleProcessLrpcAE(AppleEvent * ae, AppleEvent * ae1, long l);
STDAPI  _OleQueryLinkFromData(LPDATAOBJECT pSrcDataObject);
STDAPI  _OleQueryCreateFromData(LPDATAOBJECT pSrcDataObject);
STDAPI  _OleCreate(REFCLSID rclsid, REFIID riid, unsigned long renderopt,LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);
STDAPI  _OleCreateFromData(LPDATAOBJECT pSrcDataObj, REFIID riid,unsigned long renderopt, LPFORMATETC pFormatEtc,LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,void * * ppvObj);
STDAPI  _OleCreateLinkFromData(LPDATAOBJECT pSrcDataObj, REFIID riid,unsigned long renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,void * * ppvObj);
STDAPI  _OleCreateStaticFromData(LPDATAOBJECT pSrcDataObj, REFIID iid,unsigned long renderopt, LPFORMATETC pFormatEtc,LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,void * * ppvObj);
STDAPI  _OleCreateLink(LPMONIKER pmkLinkSrc, REFIID riid,unsigned long renderopt, LPFORMATETC lpFormatEtc,LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);
STDAPI  _OleLoad(LPSTORAGE pStg, REFIID riid, LPOLECLIENTSITE pClientSite, void * * ppvObj);
STDAPI  _OleSave(LPPERSISTSTORAGE pPS, LPSTORAGE pStg, OLEBOOL fSameAsLoad);
STDAPI  _OleRun(LPUNKNOWN pUnknown);
STDAPI_(OLEBOOL)   _OleIsRunning(LPOLEOBJECT pObject);
STDAPI _OleLockRunning(LPUNKNOWN pUnknown, OLEBOOL fLock, OLEBOOL fLastUnlockCloses);
STDAPI  _BindMoniker(LPMONIKER pmk, unsigned long grfOpt, REFIID iidResult, void * * ppvResult);
STDAPI  _MkParseDisplayName(LPBC pbc, char * szUserName, unsigned long  * pchEaten, LPMONIKER  * ppmk);
STDAPI  _OleSaveToStream( LPPERSISTSTREAM pPStm, LPSTREAM pStm );
STDAPI  _OleLoadFromStream( LPSTREAM pStm, REFIID iidInterface, void * * ppvObj);
STDAPI  _CreateBindCtx(unsigned long reserved, LPBC * ppbc);
STDAPI  _CreateItemMoniker(char * lpszDelim, char * lpszItem,LPMONIKER * ppmk);
STDAPI  _CreateFileMoniker(char * lpszPathName, LPMONIKER * ppmk);
STDAPI  _CreateGenericComposite(LPMONIKER pmkFirst, LPMONIKER pmkRest,LPMONIKER * ppmkComposite);
STDAPI  _CreateAntiMoniker(LPMONIKER * ppmk);
STDAPI  _CreatePointerMoniker(LPUNKNOWN punk, LPMONIKER * ppmk);
STDAPI  _GetRunningObjectTable( unsigned long reserved, LPRUNNINGOBJECTTABLE * pprot);
STDAPI_(void)   _ReleaseStgMedium(LPSTGMEDIUM lpStg);
STDAPI  _RegisterDragDrop(WindowPtr pWnd, LPDROPTARGET pDropTarget);
STDAPI  _RevokeDragDrop(WindowPtr pWnd);
STDAPI  _DoDragDrop(LPDATAOBJECT pDataObj, LPDROPSOURCE pDropSource,	unsigned long dwOKEffects, unsigned long * pdwEffect);
STDAPI  _CreateOleAdviseHolder(LPOLEADVISEHOLDER * ppOAHolder);
STDAPI  _CreateDataAdviseHolder(LPDATAADVISEHOLDER * ppDAHolder);
STDAPI  _MonikerRelativePathTo(LPMONIKER pmkSrc, LPMONIKER pmkDest, LPMONIKER * ppmkRelPath, OLEBOOL fCalledFromMethod);
STDAPI  _MonikerCommonPrefixWith(LPMONIKER pmkThis, LPMONIKER pmkOther, LPMONIKER * ppmkCommon);
STDAPI  _OleSetClipboard(LPDATAOBJECT pDataObj);
STDAPI  _OleGetClipboard(LPDATAOBJECT * ppDataObj);
STDAPI  _OleFlushClipboard(void);
STDAPI_(Handle) _OleDuplicateData(Handle hSrc, unsigned long cfFormat,unsigned short uiFlags);
STDAPI  _GetClassFile(const char * szFilename, CLSID * pclsid);
STDAPI  _OleDraw(LPUNKNOWN pUnknown, unsigned long dwAspect, GrafPtr pGrafDraw,const Rect* lprcBounds);
STDAPI  _OleCreateDefaultHandler(REFCLSID clsid, LPUNKNOWN pUnkOuter,REFIID riid, void * * lplpObj);
STDAPI  _OleCreateEmbeddingHelper(REFCLSID clsid, LPUNKNOWN pUnkOuter, unsigned long flags, LPCLASSFACTORY pCF,REFIID riid, void * * lplpObj);
STDAPI  _OleConvertIStorageToOLESTREAMEx(LPSTORAGE	pstg, unsigned long cfFormat, long	lWidth, long lHeight, unsigned long	dwSize, LPSTGMEDIUM	pmedium,LPOLESTREAM	polestm);
STDAPI  _OleConvertOLESTREAMToIStorageEx(LPOLESTREAM polestm,LPSTORAGE pstg, unsigned long *	pcfFormat, long  *plwWidth, long *plHeight, unsigned long  *pdwSize, LPSTGMEDIUM pmedium);

STDAPI  _OleCreateFromFile(REFCLSID rclsid, const char * lpszFileName, REFIID riid, unsigned long renderopt, LPFORMATETC lpFormatEtc,LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);
STDAPI  _OleCreateLinkToFile(const char * lpszFileName, REFIID riid, unsigned long renderopt, LPFORMATETC lpFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);
STDAPI  _CreateDataCache(LPUNKNOWN pUnkOuter, REFCLSID rclsid,	REFIID iid, void * * ppv);
STDAPI  _OleConvertIStorageToOLESTREAM(LPSTORAGE pstg, LPOLESTREAM polestm);
STDAPI  _OleConvertOLESTREAMToIStorage(LPOLESTREAM polestm,LPSTORAGE pstg, const DVTARGETDEVICE *   ptd);
STDAPI  _OleIsCurrentClipboard(LPDATAOBJECT pDataObj);
STDAPI  _OleTranslateAccelerator(LPOLEINPLACEFRAME lpFrame, LPOLEINPLACEFRAMEINFO lpFrameInfo, EventRecord * pEvt);
STDAPI  _OleDoAutoConvert(LPSTORAGE pStg, LPCLSID pClsidNew);
STDAPI  _OleSetContainedObject(LPUNKNOWN pUnknown, OLEBOOL fContained);
STDAPI  _OleNoteObjectVisible(LPUNKNOWN pUnknown, OLEBOOL fVisible);
STDAPI  _OleRegEnumFormatEtc(REFCLSID clsid, unsigned long dwDirection,	LPENUMFORMATETC * ppenum);
STDAPI  _OleRegEnumVerbs(REFCLSID clsid, LPENUMOLEVERB * ppenum);
STDAPI  _OleSendLLE(OSType sig, EventRecord *pevt);
STDAPI  _OleSetInPlaceWindow(WindowPtr pWndObject, LPOLEINPLACEFRAMEINFO lpFrameInfo);
STDAPI  _OleUnSetInPlaceWindow(OLEBOOL fInvalidate);
STDAPI  _OleClipWindows(WindowPtr pWndClip);
STDAPI  _OleClipWindow(WindowPtr clipwp);
STDAPI  _OleUnclipWindow(WindowPtr clipwp, Boolean fPaint);

STDAPI  _OleInsertMenus(OleMBarHandle hOleMBar, short beforeID1, short beforeID3, short beforeID5);
STDAPI  _OleHashMenuID(short *pMenuID);
STDAPI  _OleUnhashMenuID(short *pMenuID);
STDAPI  _OlePatchGetMHandle(OleMBarHandle hOleMBar);
STDAPI  _OleUnpatchGetMHandle(OleMBarHandle hOleMBar);
STDAPI  _OleAddMBarMenu(OleMBarHandle hOleMBar, MenuHandle hMenu, short group);
STDAPI  _OleSetInPlaceRects(RgnHandle posRgn, RgnHandle clipRgn, RgnHandle frameRgn, RgnHandle cliRgn, RgnHandle clickRgn);
STDAPI  _OleMaskMouse(Boolean bEnable);
STDAPI_(void) _OleMoveWindow(WindowPtr w, short a, short b, Boolean c);
STDAPI_(void) _OleSizeParentWindow(WindowPtr w, short h, short v, Boolean fUpdate);
STDAPI_(void) _OleSizeObjectWindow(WindowPtr w, const Rect *prcNewBounds, Boolean fUpdate);
STDAPI_(long) _OleZoomParentWindow(WindowPtr w, short a, Boolean b);
STDAPI_(long) _OleGrowParentWindow(WindowPtr w, Point p, const Rect *prcMinMax);
STDAPI_(long) _OleGrowObjectWindow(WindowPtr w, Point p, const Rect *prcMinMax, Rect *prcNewBounds);
STDAPI_(short) _OleWhichGrowHandle(WindowPtr w, Point pt);
STDAPI  _OleGetCursor(CursPtr pCursor, WindowPtr pWndObject);
STDAPI  _OleSetCursor(CursPtr pCursor, WindowPtr pWndParent);
STDAPI  _OleUpdateCursor(WindowPtr pWndObject);
STDAPI_(void) _OleDragParentWindow(WindowPtr pWnd,	Point startPt, const Rect *pLimitRect);
STDAPI_(long) _OleDragObjectWindow(WindowPtr pWnd,	Point startPt, const Rect *pLimitRect, const Rect *pSlopRect,	short constrain, ProcPtr actionProc);
STDAPI  _OleNewMBar(OleMBarHandle *phOleMBar);
STDAPI  _OleDisposeMBar(OleMBarHandle hOleMBar);
STDAPI_(OSErr)	_OleProcessDdeAE(AppleEvent * ae, AppleEvent * ae1, long l);
STDAPI_(OSErr)	_OleProcessClipboardAE(AppleEvent *ae, AppleEvent *ae1, long l);
STDAPI  _OleSetClipboardEx(LPDATAOBJECT pDataObj);
STDAPI  _OleSetParentRgns(RgnHandle clipRgn, RgnHandle frameRgn, RgnHandle clientRgn);
STDAPI  _CreateFileMonikerFSp(const FSSpec *pFSSpec, LPMONIKER * ppmk);
STDAPI  _OleCreateFromFSp(REFCLSID rclsid, const FSSpec *pFSSpec,REFIID iid, unsigned long renderopt, LPFORMATETC lpFormatEtc,IOleClientSite *pClientSite, IStorage *pStg, void **ppvObj);
STDAPI  _OleCreateLinkToFSp(const FSSpec *pFSSpec, REFIID riid, unsigned long renderopt, LPFORMATETC lpFormatEtc,LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void **ppvObj);
STDAPI  _GetClassFSp(const FSSpec *pFSSpec, CLSID *pcid);
STDAPI  _MkParseDisplayNameMac(LPBC pbc, char * szUserName, short vRefNum, long dirID, unsigned long  * pchEaten, LPLPMONIKER ppmk);
STDAPI  _ReadOle1FmtProgIDStgMac(IStorage *pstg, char **ppszOle1ProgID);
STDAPI  _WriteOle1FmtProgIDStgMac(IStorage *pstg, const char *pszOle1ProgID);
STDAPI_(PicHandle) _OleGetIconOfFile(char * lpszPath, OLEBOOL fUseFileAsLabel);
STDAPI_(PicHandle) _OleGetIconOfClass(REFCLSID rclsid, char * lpszLabel, OLEBOOL fUseTypeAsLabel);
STDAPI_(PicHandle) _OleGetIconOfFSp(FSSpec *pFSSpec, OLEBOOL fUseFileAsLabel);
STDAPI_(PicHandle) _OlePictFromIconAndLabel(PicHandle hIcon, char *pszLabel, OleIconSource *pIconSource);
STDAPI_(PicHandle) _OleGetIconFromIconSuite(Handle hSuite, char * lpszLabel, OleIconSource *pIconSource);
STDAPI_(void) _OleUIPictIconFree(PicHandle hPict);
STDAPI_(unsigned long) _OleUIPictExtractLabel(PicHandle hPict, char *pszLabel, unsigned long cchLabel);
STDAPI_(PicHandle) _OleUIPictExtractIcon(PicHandle hPict);
STDAPI_(unsigned long) _OleUIPictIconDraw(GrafPtr pGrafPort, Rect *pRect, PicHandle hPict, OLEBOOL fIconOnly);
STDAPI_(unsigned long) _OleUIPictExtractIconSource(PicHandle hPict, OleIconSource *pIconSource);
STDAPI_(unsigned long)  _OleQueryCreateAll(LPDATAOBJECT lpSrcDataObj, unsigned long *pdwResult);
STDAPI  _MkGetMacNetInfo(IMoniker *pmk, char **ppszZoneName, char **ppszServerName);

STDAPI _OleSetInFrontOf(ProcessSerialNumber *psn);

#ifdef __cplusplus
}
#endif

#endif /* !__MACAPI__ */
