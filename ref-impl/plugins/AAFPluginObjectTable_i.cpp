//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

//
// Include the appropriate header files:
//
#include "CAAFEssenceDataStream.h"
#include "CAAFEssenceFileContainer.h"
#include "CAAFAIFCCodec.h"
#include "CAAFWAVECodec.h"
#include "CAAFJPEGCodec.h"
#include "CAAFCDCICodec.h"

//
// Include all objects in the following table:
//
AAF_BEGIN_OBJECT_MAP(AAFPluginObjectMap)
	AAF_OBJECT_ENTRY(AAFEssenceDataStream)
	AAF_OBJECT_ENTRY(AAFEssenceFileContainer)
	AAF_OBJECT_ENTRY(AAFAIFCCodec)
	AAF_OBJECT_ENTRY(AAFWaveCodec)
	AAF_OBJECT_ENTRY(AAFJPEGCodec)
	AAF_OBJECT_ENTRY(AAFCDCICodec)
AAF_END_OBJECT_MAP()
