#ifndef __AAFPropertyIDs_h__
#define __AAFPropertyIDs_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


// AAF property identifiers (PIDs).
//

// A property is identified by a globally unique 16-byte
// identifier. To save space in an AAF file we store a
// 2-byte file unique PID with each property.
// The mapping for a particular file is recorded in the
// dictionary contained in that file.
// For the predefined properties we optimize by using a
// fixed, compiled-in mapping.
// This file defines that mapping.
//
const int PID_InterchangeObject_ObjClass                         = 0x0101;
const int PID_InterchangeObject_Generation                       = 0x0102;
const int PID_Component_DataDefinition                           = 0x0201;
const int PID_Component_Length                                   = 0x0202;
const int PID_Component_KLVData                                  = 0x0203;
const int PID_EdgeCode_Start                                     = 0x0401;
const int PID_EdgeCode_FilmKind                                  = 0x0402;
const int PID_EdgeCode_CodeFormat                                = 0x0403;
const int PID_EdgeCode_Header                                    = 0x0404;
const int PID_EssenceGroup_Choices                               = 0x0501;
const int PID_EssenceGroup_StillFrame                            = 0x0502;
const int PID_Event_Position                                     = 0x0601;
const int PID_Event_Comment                                      = 0x0602;
const int PID_GPITrigger_ActiveState                             = 0x0801;
const int PID_CommentMarker_Annotation                           = 0x0901;
const int PID_OperationGroup_OperationDefinition                 = 0x0B01;
const int PID_OperationGroup_InputSegments                       = 0x0B02;
const int PID_OperationGroup_Parameters                          = 0x0B03;
const int PID_OperationGroup_BypassOverride                      = 0x0B04;
const int PID_OperationGroup_Rendering                           = 0x0B05;
const int PID_NestedScope_Slots                                  = 0x0C01;
const int PID_Pulldown_InputSegment                              = 0x0D01;
const int PID_Pulldown_PulldownKind                              = 0x0D02;
const int PID_Pulldown_PulldownDirection                         = 0x0D03;
const int PID_Pulldown_PhaseFrame                                = 0x0D04;
const int PID_ScopeReference_RelativeScope                       = 0x0E01;
const int PID_ScopeReference_RelativeSlot                        = 0x0E02;
const int PID_Selector_Selected                                  = 0x0F01;
const int PID_Selector_Alternates                                = 0x0F02;
const int PID_Sequence_Components                                = 0x1001;
const int PID_SourceReference_SourceID                           = 0x1101;
const int PID_SourceReference_SourceMobSlotID                    = 0x1102;
const int PID_SourceClip_StartTime                               = 0x1201;
const int PID_SourceClip_FadeInLength                            = 0x1202;
const int PID_SourceClip_FadeInType                              = 0x1203;
const int PID_SourceClip_FadeOutLength                           = 0x1204;
const int PID_SourceClip_FadeOutType                             = 0x1205;
const int PID_HTMLClip_BeginAnchor                               = 0x1401;
const int PID_HTMLClip_EndAnchor                                 = 0x1402;
const int PID_Timecode_Start                                     = 0x1501;
const int PID_Timecode_FPS                                       = 0x1502;
const int PID_Timecode_Drop                                      = 0x1503;
const int PID_TimecodeStream_SampleRate                          = 0x1601;
const int PID_TimecodeStream_Source                              = 0x1602;
const int PID_TimecodeStream_SourceType                          = 0x1603;
const int PID_TimecodeStream12M_IncludeSync                      = 0x1701;
const int PID_Transition_OperationGroup                          = 0x1801;
const int PID_Transition_CutPoint                                = 0x1802;
const int PID_ContentStorage_Mobs                                = 0x1901;
const int PID_ContentStorage_EssenceData                         = 0x1902;
const int PID_ControlPoint_Value                                 = 0x1A02;
const int PID_ControlPoint_Time                                  = 0x1A03;
const int PID_ControlPoint_EditHint                              = 0x1A04;
const int PID_DefinitionObject_Identification                    = 0x1B01;
const int PID_DefinitionObject_Name                              = 0x1B02;
const int PID_DefinitionObject_Description                       = 0x1B03;
const int PID_ClassDefinition_ParentClass                        = 0x0008;
const int PID_ClassDefinition_Properties                         = 0x0009;
const int PID_ClassDefinition_IsConcrete                         = 0x000A;
const int PID_OperationDefinition_DataDefinition                 = 0x1E01;
const int PID_OperationDefinition_IsTimeWarp                     = 0x1E02;
const int PID_OperationDefinition_DegradeTo                      = 0x1E03;
const int PID_OperationDefinition_Category                       = 0x1E06;
const int PID_OperationDefinition_NumberInputs                   = 0x1E07;
const int PID_OperationDefinition_Bypass                         = 0x1E08;
const int PID_OperationDefinition_ParametersDefined              = 0x1E09;
const int PID_ParameterDefinition_Type                           = 0x1F01;
const int PID_ParameterDefinition_DisplayUnits                   = 0x1F03;
const int PID_PropertyDefinition_Type                            = 0x000B;
const int PID_PropertyDefinition_IsOptional                      = 0x000C;
const int PID_PropertyDefinition_LocalIdentification             = 0x000D;
const int PID_PropertyDefinition_IsUniqueIdentifier              = 0x000E;
const int PID_PluginDefinition_CategoryClass                     = 0x2203;
const int PID_PluginDefinition_VersionNumber                     = 0x2204;
const int PID_PluginDefinition_VersionString                     = 0x2205;
const int PID_PluginDefinition_Manufacturer                      = 0x2206;
const int PID_PluginDefinition_ManufacturerInfo                  = 0x2207;
const int PID_PluginDefinition_ManufacturerID                    = 0x2208;
const int PID_PluginDefinition_Platform                          = 0x2209;
const int PID_PluginDefinition_MinPlatformVersion                = 0x220A;
const int PID_PluginDefinition_MaxPlatformVersion                = 0x220B;
const int PID_PluginDefinition_Engine                            = 0x220C;
const int PID_PluginDefinition_MinEngineVersion                  = 0x220D;
const int PID_PluginDefinition_MaxEngineVersion                  = 0x220E;
const int PID_PluginDefinition_PluginAPI                         = 0x220F;
const int PID_PluginDefinition_MinPluginAPI                      = 0x2210;
const int PID_PluginDefinition_MaxPluginAPI                      = 0x2211;
const int PID_PluginDefinition_SoftwareOnly                      = 0x2212;
const int PID_PluginDefinition_Accelerator                       = 0x2213;
const int PID_PluginDefinition_Locators                          = 0x2214;
const int PID_PluginDefinition_Authentication                    = 0x2215;
const int PID_PluginDefinition_DefinitionObject                  = 0x2216;
const int PID_CodecDefinition_FileDescriptorClass                = 0x2301;
const int PID_CodecDefinition_DataDefinitions                    = 0x2302;
const int PID_ContainerDefinition_EssenceIsIdentified            = 0x2401;
const int PID_Dictionary_OperationDefinitions                    = 0x2603;
const int PID_Dictionary_ParameterDefinitions                    = 0x2604;
const int PID_Dictionary_DataDefinitions                         = 0x2605;
const int PID_Dictionary_PluginDefinitions                       = 0x2606;
const int PID_Dictionary_CodecDefinitions                        = 0x2607;
const int PID_Dictionary_ContainerDefinitions                    = 0x2608;
const int PID_Dictionary_InterpolationDefinitions                = 0x2609;
const int PID_EssenceData_MobID                                  = 0x2701;
const int PID_EssenceData_Data                                   = 0x2702;
const int PID_EssenceData_SampleIndex                            = 0x2B01;
const int PID_EssenceDescriptor_Locator                          = 0x2F01;
const int PID_FileDescriptor_SampleRate                          = 0x3001;
const int PID_FileDescriptor_Length                              = 0x3002;
const int PID_FileDescriptor_ContainerFormat                     = 0x3004;
const int PID_FileDescriptor_CodecDefinition                     = 0x3005;
const int PID_AIFCDescriptor_Summary                             = 0x3101;
const int PID_DigitalImageDescriptor_Compression                 = 0x3201;
const int PID_DigitalImageDescriptor_StoredHeight                = 0x3202;
const int PID_DigitalImageDescriptor_StoredWidth                 = 0x3203;
const int PID_DigitalImageDescriptor_SampledHeight               = 0x3204;
const int PID_DigitalImageDescriptor_SampledWidth                = 0x3205;
const int PID_DigitalImageDescriptor_SampledXOffset              = 0x3206;
const int PID_DigitalImageDescriptor_SampledYOffset              = 0x3207;
const int PID_DigitalImageDescriptor_DisplayHeight               = 0x3208;
const int PID_DigitalImageDescriptor_DisplayWidth                = 0x3209;
const int PID_DigitalImageDescriptor_DisplayXOffset              = 0x320A;
const int PID_DigitalImageDescriptor_DisplayYOffset              = 0x320B;
const int PID_DigitalImageDescriptor_FrameLayout                 = 0x320C;
const int PID_DigitalImageDescriptor_VideoLineMap                = 0x320D;
const int PID_DigitalImageDescriptor_ImageAspectRatio            = 0x320E;
const int PID_DigitalImageDescriptor_AlphaTransparency           = 0x320F;
const int PID_DigitalImageDescriptor_Gamma                       = 0x3210;
const int PID_DigitalImageDescriptor_ImageAlignmentFactor        = 0x3211;
const int PID_DigitalImageDescriptor_FieldDominance              = 0x3212;
const int PID_DigitalImageDescriptor_FieldStartOffset            = 0x3213;
const int PID_DigitalImageDescriptor_FieldEndOffset              = 0x3214;
const int PID_CDCIDescriptor_ComponentWidth                      = 0x3301;
const int PID_CDCIDescriptor_HorizontalSubsampling               = 0x3302;
const int PID_CDCIDescriptor_ColorSiting                         = 0x3303;
const int PID_CDCIDescriptor_BlackReferenceLevel                 = 0x3304;
const int PID_CDCIDescriptor_WhiteReferenceLevel                 = 0x3305;
const int PID_CDCIDescriptor_ColorRange                          = 0x3306;
const int PID_CDCIDescriptor_PaddingBits                         = 0x3307;
const int PID_CDCIDescriptor_VerticalSubsampling                 = 0x3308;
const int PID_CDCIDescriptor_AlphaSamplingWidth                  = 0x3309;
const int PID_RGBADescriptor_PixelLayout                         = 0x3401;
const int PID_RGBADescriptor_Palette                             = 0x3403;
const int PID_RGBADescriptor_PaletteLayout                       = 0x3404;
const int PID_TIFFDescriptor_IsUniform                           = 0x3701;
const int PID_TIFFDescriptor_IsContiguous                        = 0x3702;
const int PID_TIFFDescriptor_LeadingLines                        = 0x3703;
const int PID_TIFFDescriptor_TrailingLines                       = 0x3704;
const int PID_TIFFDescriptor_JPEGTableID                         = 0x3705;
const int PID_TIFFDescriptor_Summary                             = 0x3706;
const int PID_WAVEDescriptor_Summary                             = 0x3801;
const int PID_FilmDescriptor_FilmFormat                          = 0x3901;
const int PID_FilmDescriptor_FrameRate                           = 0x3902;
const int PID_FilmDescriptor_PerforationsPerFrame                = 0x3903;
const int PID_FilmDescriptor_FilmAspectRatio                     = 0x3904;
const int PID_FilmDescriptor_Manufacturer                        = 0x3905;
const int PID_FilmDescriptor_Model                               = 0x3906;
const int PID_FilmDescriptor_FilmGaugeFormat                     = 0x3907;
const int PID_FilmDescriptor_FilmBatchNumber                     = 0x3908;
const int PID_TapeDescriptor_FormFactor                          = 0x3A01;
const int PID_TapeDescriptor_VideoSignal                         = 0x3A02;
const int PID_TapeDescriptor_TapeFormat                          = 0x3A03;
const int PID_TapeDescriptor_Length                              = 0x3A04;
const int PID_TapeDescriptor_ManufacturerID                      = 0x3A05;
const int PID_TapeDescriptor_Model                               = 0x3A06;
const int PID_TapeDescriptor_TapeBatchNumber                     = 0x3A07;
const int PID_TapeDescriptor_TapeStock                           = 0x3A08;
const int PID_Header_ByteOrder                                   = 0x3B01;
const int PID_Header_LastModified                                = 0x3B02;
const int PID_Header_Content                                     = 0x3B03;
const int PID_Header_Dictionary                                  = 0x3B04;
const int PID_Header_Version                                     = 0x3B05;
const int PID_Header_IdentificationList                          = 0x3B06;
const int PID_Header_ObjectModelVersion                          = 0x3B07;
const int PID_Identification_CompanyName                         = 0x3C01;
const int PID_Identification_ProductName                         = 0x3C02;
const int PID_Identification_ProductVersion                      = 0x3C03;
const int PID_Identification_ProductVersionString                = 0x3C04;
const int PID_Identification_ProductID                           = 0x3C05;
const int PID_Identification_Date                                = 0x3C06;
const int PID_Identification_ToolkitVersion                      = 0x3C07;
const int PID_Identification_Platform                            = 0x3C08;
const int PID_Identification_GenerationAUID                      = 0x3C09;
const int PID_NetworkLocator_URLString                           = 0x4001;
const int PID_TextLocator_Name                                   = 0x4101;
const int PID_Mob_MobID                                          = 0x4401;
const int PID_Mob_Name                                           = 0x4402;
const int PID_Mob_Slots                                          = 0x4403;
const int PID_Mob_LastModified                                   = 0x4404;
const int PID_Mob_CreationTime                                   = 0x4405;
const int PID_Mob_UserComments                                   = 0x4406;
const int PID_Mob_KLVData                                        = 0x4407;
const int PID_CompositionMob_DefaultFadeLength                   = 0x4501;
const int PID_CompositionMob_DefFadeType                         = 0x4502;
const int PID_CompositionMob_DefFadeEditUnit                     = 0x4503;
const int PID_SourceMob_EssenceDescription                       = 0x4701;
const int PID_MobSlot_SlotID                                     = 0x4801;
const int PID_MobSlot_SlotName                                   = 0x4802;
const int PID_MobSlot_Segment                                    = 0x4803;
const int PID_MobSlot_PhysicalTrackNumber                        = 0x4804;
const int PID_EventMobSlot_EditRate                              = 0x4901;
const int PID_TimelineMobSlot_EditRate                           = 0x4B01;
const int PID_TimelineMobSlot_Origin                             = 0x4B02;
const int PID_Parameter_Definition                               = 0x4C01;
const int PID_ConstantValue_Value                                = 0x4D01;
const int PID_VaryingValue_Interpolation                         = 0x4E01;
const int PID_VaryingValue_PointList                             = 0x4E02;
const int PID_TaggedValue_Name                                   = 0x5001;
const int PID_TaggedValue_Value                                  = 0x5003;
const int PID_KLVData_Value                                      = 0x5101;
const int PID_TypeDefinitionInteger_Size                         = 0x000F;
const int PID_TypeDefinitionInteger_IsSigned                     = 0x0010;
const int PID_TypeDefinitionStrongObjectReference_ReferencedType = 0x0011;
const int PID_TypeDefinitionWeakObjectReference_ReferencedType   = 0x0012;
const int PID_TypeDefinitionWeakObjectReference_TargetSet        = 0x0013;
const int PID_TypeDefinitionEnumeration_ElementType              = 0x0014;
const int PID_TypeDefinitionEnumeration_ElementNames             = 0x0015;
const int PID_TypeDefinitionEnumeration_ElementValues            = 0x0016;
const int PID_TypeDefinitionFixedArray_ElementType               = 0x0017;
const int PID_TypeDefinitionFixedArray_ElementCount              = 0x0018;
const int PID_TypeDefinitionVariableArray_ElementType            = 0x0019;
const int PID_TypeDefinitionSet_ElementType                      = 0x001A;
const int PID_TypeDefinitionString_ElementType                   = 0x001B;
const int PID_TypeDefinitionRecord_MemberTypes                   = 0x001C;
const int PID_TypeDefinitionRecord_MemberNames                   = 0x001D;
const int PID_TypeDefinitionRename_RenamedType                   = 0x001E;
const int PID_TypeDefinitionExtendibleEnumeration_ElementNames   = 0x001F;
const int PID_TypeDefinitionExtendibleEnumeration_ElementValues  = 0x0020;
const int PID_MetaDefinition_Identification                      = 0x0005;
const int PID_MetaDefinition_Name                                = 0x0006;
const int PID_MetaDefinition_Description                         = 0x0007;
const int PID_MetaDictionary_ClassDefinitions                    = 0x0003;
const int PID_MetaDictionary_TypeDefinitions                     = 0x0004;

#endif // ! __AAFPropertyIDs_h__
