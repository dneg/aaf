// @doc INTERNAL
// @com This file implements the module test for CAAFHeader.
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
// Licensor of the AAF Association is Schema Software, Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=



/**
* 
*  SsrwInputSource.h
*  
*  Provides an abstraction from the C file I/O functions.
*
*  External clients can implement their own input sources.  This is the method
*  by which the structured storage library can be directed to read and write 
*  from third-party I/O sources.
*
*  The SsrwInputSource structure consists of several function pointers as well
*  as a void* pointer, m_pSrc.  
*
*  Each of the function pointers corresponds to a C standard library API 
*  function.  A client that wishes to implement an external input source must
*  implement a version of each of these functions.  When the SsrwInputSource
*  is constructed by the client, the function pointers should each point to the
*  client's corresponding I/O function.
*
*  m_pSrc is a cookie that can point to any type of data, including a C++ 
*  object.  This pointer is a anchor that is used to hold any client-specific
*  data that the input source implementation may need.  For instance, the 
*  file input source stores a structure that contains both a FILE* pointer, 
*  and a flag indicating whether the input source "owns" the file pointer.
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*/

#ifndef SSRWINPUTSOURCE_H_INCLUDED
#define SSRWINPUTSOURCE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SSRWIS_SEEK_SET 0
#define SSRWIS_SEEK_CUR 1
#define SSRWIS_SEEK_END 2

typedef struct _SsrwInputSource SSRWIS;

/**
 * Corresponds to a pointer to the function fread in the standard C library
 */
typedef size_t (*Ssrw_Func_Fread)(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS);

/**
 * Corresponds to a pointer to the function fwrite in the standard C library
 */
typedef size_t (*Ssrw_Func_Fwrite)(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS);

/**
 * Corresponds to a pointer to the function fclose in the standard C library
 */
typedef int (*Ssrw_Func_Fclose)(
    SSRWIS*     in_pv);


/**
 * Corresponds to a pointer to the function fseek in the standard C library
 */
typedef int (*Ssrw_Func_Fseek)(
    SSRWIS*     in_pIS,
    SSRW_INT64  in_lOffset,
    int         in_nWhence);

/**
 * Corresponds to a pointer to the function ftell in the standard C library
 */
typedef SSRW_INT64 (*Ssrw_Func_Ftell)(
    SSRWIS*     in_pIS);

/**
 * Truncates the file to a given size if possible
 */
typedef long (*Ssrw_Func_Ftruncate)(
    SSRWIS*     in_pIS,
    SSRW_INT64  in_llNewSIze);

/**
 * This structure corresponds to a FILE structure in the standard C library
 */
typedef struct _SsrwInputSource
{
    void* m_pSrc;

    Ssrw_Func_Fread     m_pFread;
    Ssrw_Func_Fwrite    m_pFwrite;
    Ssrw_Func_Fclose    m_pFclose;
    Ssrw_Func_Fseek     m_pFseek;
    Ssrw_Func_Ftell     m_pFtell;
    Ssrw_Func_Ftruncate m_pFtruncate;
} SsrwInputSource;

/**
 * Works just like fread, but accepts an SSRWIS* instead of a FILE*.
 *
 * @param out_pv -  buffer into which to read data
 * @param in_size - size, in bytes, of each item to read
 * @param in_num -  number of items to read
 * @param in_pIS -  pointer to an input source to read from 
 * @return number of items read
 */
size_t SsrwFread(
    void*   out_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS);

/**
 * Works just like fwrite, but accepts an SSRWIS* instead of a FILE*.
 *
 * @param in_pv -   buffer of data to write
 * @param in_size - size, in bytes, of each item to write
 * @param in_num -  number of items to write
 * @param in_pIS -  pointer to an input source to write to
 * @return number of items write
 */
size_t SsrwFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS);

/**
 * Works just like fclose, but accepts an SSRWIS* instead of a FILE*.
 *
 * @param in_pIS -  pointer to an input source to close
 * @return 0 on success, nonzero on failure
 */
int SsrwFclose(
    SSRWIS* in_pv);

/**
 * Works just like fseek, but accepts an SSRWIS* instead of a FILE*.
 *
 * @param in_pIS -      pointer to an input stream whose seek location should be
 *                      moved
 * @param in_llOffset - offset to which to move pointer
 * @param in_whence -   SEEK_SET, etc. origin from which to move pointer
 * @return 0 on success, nonzero on failure
 */
int SsrwFseek(
    SSRWIS* in_pIS,
    SSRW_INT64   in_llOffset,
    int     in_whence);

/**
 * Works just like ftell, but accepts an SSRWIS* instead of a FILE*.
 *
 * @param in_pIS - input stream from which to get seek location
 * @return offset from beginning of input source, in bytes
 */
SSRW_INT64 SsrwFtell(
    SSRWIS* in_pIS);

/**
 * Truncates the file to a given size if possible
 *
 * @param in_pIS - input stream
 * @param in_llNewSize - new size of the stream
 *
 * @return 0 on success, nonzero on error 
 */
long SsrwFtruncate(
    SSRWIS* in_pIS,
    SSRW_INT64 in_llNewSIze);

/**
 * Disconnects an SSRWIS* from whatever it was connected to.
 *
 * @param in_pIS - non-NULL, open SSRWIS* that was created using 
 *                 SsrwConnectToExternalPalmFileHand.
 * @return 0 on success, nonzero on failure.
 */
int SsrwDisconnect(SSRWIS* in_pIS);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* SSRWINPUTSOURCE_H_INCLUDED */
