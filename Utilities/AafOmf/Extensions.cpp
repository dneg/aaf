// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

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
 *  prior written permission of Avid Technology, Inc.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// OMF Includes

#include "AafOmf.h"
#include "AAFDomainUtils.h"
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#include "EffectTranslate.h"
#include "Extensions.h"
#include "AAFClassDefUIDs.h" 
#include "AAFException.h"
#include "AAFDomainUtils.h"
#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"

extern AafOmfGlobals* gpGlobals;

#define CHECKAAF
static void     LogError(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define checkAAF(a)  \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}

#define checkOMF(a)  \
{ moduleErrorTmp = a; \
	if (moduleErrorTmp != OMF2::OM_ERR_NONE) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}
#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     exit(1);\
}



// ============================================================================
// Constructor
// ============================================================================
ExtendedOmf2Aaf::ExtendedOmf2Aaf()	// : pFile(NULL), pHeader(NULL), pDictionary(NULL)
{
}
// ============================================================================
// Destructor
// ============================================================================
ExtendedOmf2Aaf::~ExtendedOmf2Aaf()
{
}

// ============================================================================
// Constructor
// ============================================================================
ExtendedAaf2Omf::ExtendedAaf2Omf() //	: pFile(NULL), pHeader(NULL), pDictionary(NULL)
{
}
// ============================================================================
// Destructor
// ============================================================================
ExtendedAaf2Omf::~ExtendedAaf2Omf()
{
}
