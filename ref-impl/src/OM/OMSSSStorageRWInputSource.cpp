/******************************************************************************\
* 
* OMSSSStorageRWInputSource.cpp
*  
*  Passes calls from the SsrwInputSource abstraction down to the real C FILE* 
*  functions
* 
*  Copyright (C) 2002 - 2003 Schema Software, Inc. All rights reserved.
*  Portions Copyright (C) 2003 Metaglue Corparation. All rights reserved.
*
*  $Revision$
*  $Date$
*  
\******************************************************************************/
#include "OMRawStorage.h"
#include "OMSSSStructuredStorage.h"


//Type definitiosn taken from the schemasoft source

typedef unsigned long UINT4;

#ifdef _MSC_VER /* If this is Visual C++ */
typedef signed __int64 SINT8;
#else /* _MSC_VER */
typedef signed long long SINT8;
#endif /* _MSC_VER */

typedef signed long SINT4;


#ifdef DEBUG
#define ASSERT(x) if ((x)) {} else { AssertProc((char*)"#x", (char*)__FILE__, __LINE__); }
extern void AssertProc(char* cond, char* file, SINT4 line);
#else /* DEBUG */
#define ASSERT(x)
#endif /* DEBUG */

#ifdef SS_DEBUG
//we must use Schemasofts debug malloc routines under schemasoft debug builds or
// the heap corruption tests will fail
//This will only be needed for devleopers who have access to the  schemasoft code
#define ss_malloc dbgMalloc

void * dbgMalloc(size_t cb);


#else   /* SS_DEBUG */
#define ss_malloc  malloc
#endif   /* SS_DEBUG */

extern "C" {

size_t SsrwOMRawFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(in_pIS->m_pSrc != NULL);

		OMUInt32 ret = 0;

    ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->read( (OMByte *)in_pv, in_size*in_num, ret );

		return ret/in_size;
}


size_t SsrwOMRawFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(in_pIS->m_pSrc != NULL);

		OMUInt32 ret = 0;

    ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->write( (OMByte *)in_pv, in_size*in_num, ret);

		return ret/in_size;
}


int SsrwOMRawFclose(
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pSrc != NULL);

    ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->synchronize();

	return 0;
}

int SsrwOMRawFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_nWhence)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pSrc != NULL);

		OMInt64 dest = in_llOffset;

		if( !( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->isPositionable() ) return EOF;

		if( SEEK_CUR==in_nWhence )
			dest += ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->position();
		else if( SEEK_END==in_nWhence )
			dest += ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->size();

		( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->setPosition( dest );
    return 0;
}


SINT8 SsrwOMRawFtell(
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(( static_cast<OMRawStorage *>(in_pIS->m_pSrc)) != NULL);

    return ( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->position();
}


SINT4 SsrwOMRawFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize)
{
    ASSERT(in_pIS != NULL);
    ASSERT(( static_cast<OMRawStorage *>(in_pIS->m_pSrc)) != NULL);

	if( !( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->isExtendible() ) return EOF;
		( static_cast<OMRawStorage *>(in_pIS->m_pSrc))->extend( in_llNewSize );
		return 0;
}


SSRWIS* SsrwConnectToOMRaw(const OMRawStorage* in_pRaw)
{
    ASSERT(in_pRaw != NULL);

    SSRWIS*     pIS = NULL;

    pIS = (struct _SsrwInputSource *) ss_malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));

        pIS->m_pFread = &SsrwOMRawFread;
        pIS->m_pFwrite = &SsrwOMRawFwrite;
        pIS->m_pFclose = &SsrwOMRawFclose;
        pIS->m_pFseek = &SsrwOMRawFseek;
        pIS->m_pFtell = &SsrwOMRawFtell;
        pIS->m_pFtruncate = &SsrwOMRawFtruncate;

		 pIS->m_pSrc = const_cast<OMRawStorage *>( in_pRaw );
    }

    return pIS;
}



/*============================================================================
 Description: (API Call) Creates new structured storage and compound file and
   attaches them one to another, on an existing AAF OMRawStorage
 Arguments:
   in_pRaw -         OMRawStorage to which the library will connect
   in_accessMode -   Must be STG_WRITE or STG_RW.
   out_ppRoot -      on success, will be filled with a pointer to a new
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
 Return:
   Status code
 ==========================================================================*/
int createStructuredStorageInOMRawStorageEx (
      const OMRawStorage* in_pRaw,
      StgMode in_accessMode,
      RootStorage** out_ppRoot,
      unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_pRaw != NULL);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_pRaw == NULL) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToOMRaw(in_pRaw);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }
	 iRet = createStructuredStorageInSource(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           (UINT4)in_ulSectorSize);
/*
    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_FALSE,
                                           (UINT4)in_ulSectorSize);
										   */
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}


/*============================================================================
 Description: (API Call) Opens an existing structured storage and compound 
   file, on an existing AAF OMRawStorage
 Arguments:
   in_pRaw -         OMRawStorage to which the library will connect
   in_accessMode -   Must be STG_WRITE or STG_RW.
   out_ppRoot -      on success, will be filled with a pointer to a new
                     RootStorage structure representing a new structured 
                     storage file.
 Return:
   Status code
 ==========================================================================*/
int openStructuredStorageInOMRawStorage (
      const OMRawStorage* in_pRaw,
      StgMode in_accessMode,
      RootStorage** out_ppRoot)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;
	  
    ASSERT (in_pRaw != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_pRaw == NULL) ||
            (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToOMRaw(in_pRaw);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }
	    iRet = openStructuredStorageInSource(pIS,
			in_accessMode,
                                         out_ppRoot);
	/*

    iRet = openStructuredStorageInternal(pIS,
                                         out_ppRoot,
                                         SSRW_FALSE,
                                         in_accessMode);
										 */
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

} //extern "C"

