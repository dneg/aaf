//Auto generated file for Typed Visitor methods

#ifndef __TYPEDVISITOR_h__
#define __TYPEDVISITOR_h__

//project files
#include "Visitor.h"
#include "AAFTypedObjNode.h"

//AAF files
#include <AAF.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class AAFContainment;
class AAFComponentReference;
class AAFMobReference;
class AAFSlotReference;


class TypedVisitor : public Visitor
{
  public:
    TypedVisitor();
    ~TypedVisitor();

    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFAIFCDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFAIFCDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFClassDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFClassDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCodecDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCodecDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCommentMarker> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCommentMarker> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFConstantValue> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFConstantValue> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContainerDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContainerDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContentStorage> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContentStorage> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFControlPoint> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFControlPoint> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDefObject> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDefObject> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPluginDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPluginDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDictionary> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDictionary> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEdgecode> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEdgecode> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFOperationDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFOperationDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceAccess> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceAccess> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceData> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceData> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEvent> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEvent> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEventMobSlot> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEventMobSlot> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFile> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFile> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFileDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFileDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFilmDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFilmDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDigitalImageDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDigitalImageDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCDCIDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCDCIDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceFormat> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceFormat> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceGroup> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceGroup> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFiller> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFiller> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFindSourceInfo> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFFindSourceInfo> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFOperationGroup> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFOperationGroup> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFGPITrigger> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFGPITrigger> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFIdentification> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFIdentification> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFImportDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFImportDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFInterpolationDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFInterpolationDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFKLVData> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFKLVData> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFLocator> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFLocator> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMetaDefinition> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMetaDefinition> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMob> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMob> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFNestedScope> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFNestedScope> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFNetworkLocator> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFNetworkLocator> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFObject> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFObject> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPCMDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPCMDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFParameter> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFParameter> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFParameterDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFParameterDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPhysicalDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPhysicalDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFProperty> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFProperty> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPropertyDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPropertyDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPropertyValue> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPropertyValue> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPluginManager> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPluginManager> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPulldown> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFPulldown> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRGBADescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRGBADescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFScopeReference> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFScopeReference> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSegment> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSegment> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSelector> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSelector> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSequence> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSequence> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSoundDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSoundDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceClip> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceClip> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceReference> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceReference> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFStaticMobSlot> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFStaticMobSlot> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTapeDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTapeDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTaggedValue> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTaggedValue> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTextLocator> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTextLocator> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecode> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecode> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTransition> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTransition> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTIFFDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTIFFDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecodeStream> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecodeStream> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecodeStream12M> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimecodeStream12M> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefCharacter> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefCharacter> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefIndirect> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefIndirect> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefInt> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefInt> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefRename> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefRename> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefFixedArray> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefFixedArray> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefRecord> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefRecord> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefSet> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefSet> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefStream> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefStream> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefString> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefString> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefStrongObjRef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefStrongObjRef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefWeakObjRef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefWeakObjRef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefObjectRef> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefObjectRef> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefOpaque> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefOpaque> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefVariableArray> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefVariableArray> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFVaryingValue> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFVaryingValue> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFWAVEDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFWAVEDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRawStorage> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRawStorage> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRandomRawStorage> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRandomRawStorage> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFGetFileBits> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFGetFileBits> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSetFileBits> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSetFileBits> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRandomFile> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRandomFile> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTaggedValueDefinition> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTaggedValueDefinition> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFKLVDataDefinition> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFKLVDataDefinition> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRecordingDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRecordingDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFAuxiliaryDescriptor> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFAuxiliaryDescriptor> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDescriptiveFramework> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDescriptiveFramework> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDescriptiveMarker> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDescriptiveMarker> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCDCIDescriptor2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCDCIDescriptor2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFComponent2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFComponent2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef3> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDataDef3> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDictionary2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDictionary2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDigitalImageDescriptor2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFDigitalImageDescriptor2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEndian> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEndian> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceDataEx> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceDataEx> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceMultiAccess> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFEssenceMultiAccess> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMobEx> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMobEx> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMob2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMob2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRGBADescriptor2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFRGBADescriptor2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSearchSource> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSearchSource> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceReference2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceReference2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot2> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot2> > spNode);
    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefVariableArrayEx> > spNode);
    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFTypeDefVariableArrayEx> > spNode);

    bool EdgeVisit(boost::shared_ptr<AAFContainment> spEdge);
    bool EdgeVisit(boost::shared_ptr<AAFComponentReference> spEdge);
    bool EdgeVisit(boost::shared_ptr<AAFMobReference> spEdge);
    bool EdgeVisit(boost::shared_ptr<AAFSlotReference> spEdge); 
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
