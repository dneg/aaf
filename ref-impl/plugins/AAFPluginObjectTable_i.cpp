////////////////////////////////////////////////
//
// Advanced Authoring Format
//
// Copyright (c) 1998 Avid Technology, Inc.
// Copyright (c) 1998 Microsoft Corporation
//
////////////////////////////////////////////////

//
// Include the appropriate header files:
//
#include "CAAFObjectStream.h"
#include "CAAFWaveCodec.h"
//
// Include all objects in the following table:
//
AAF_BEGIN_OBJECT_MAP(AAFPluginObjectMap)
	AAF_OBJECT_ENTRY(AAFDefaultCodec)
	AAF_OBJECT_ENTRY(AAFDefaultStream)
	AAF_OBJECT_ENTRY(AAFEssenceDataStream)
	AAF_OBJECT_ENTRY(AAFWaveCodec)
AAF_END_OBJECT_MAP()
