#ifndef __AAFFileMode_h__
#define __AAFFileMode_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

//
// Mode flags for use in AAFFileOpenXXX() functions and
// IAAFFile::SaveAs() method.
//
// Combinations of flags may be bitwise OR'd together.  If any
// specific combinations are illegal, those will be explicitly noted.
//
// All bits not specified here are reserved.
//
// All reserved bits must be set to zero.
//

// Control of lazy/eager loading
//
// - When set, the bit signifies eager loading.
// - When clear (the default), lazy loading is specified.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_EAGER_LOADING        (1 << 0)

// Control of reversion
//
// - When set, the bit signifies that the file will be revertable.
// - When clear (the default), the file will not be revertable.
//
// The following applies to:
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_REVERTABLE           (1 << 1)

// Control of OS buffering
//
// - When set, the bit signifies that the file is to be accessed in
//   unbuffered mode.
// - When clear (the default), the file will be accessed in buffered
//   mode.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_UNBUFFERED           (1 << 2)

// Control of reclamation of memory used by clean objects
//
// - When set, the bit signifies that memory for clean objects may be
//   reclaimed.
// - When clear (the default), the memory for clean objects will not
//   be reclaimed.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_RECLAIMABLE          (1 << 3)

// ...

// Unpublished mode flags

// Control of use of large sector size mode in Structured Storage
// The following applies to:
//
// - When set, the bit signifies that large sector size mode will be
//   used in Structured Storage.
// - When clear (the default), normal-sized sectors will be used in
//   Structured Storage.
//
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_USE_LARGE_SS_SECTORS    (1 << 28)

// Indicates if Close() should fail when there are dirty objects
//
// - When set, the bit signifies that Close() should fail if any
//   objects are dirty (modified and unsaved).
// - When clear (the default), Close() will succeed even if any
//   objects are dirty.
//
// The following applies to:
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_CLOSE_FAIL_DIRTY     (1 << 29)


// Reserved bits

#define AAF_FILE_MODE_DEBUG0_ON            (1 << 30)
#define AAF_FILE_MODE_DEBUG1_ON            (1 << 31)


#endif // __AAFFileMode_h__
