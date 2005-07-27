AAFTestProgram.xml.zip can be viewed using MagicDraw.  However, a pdf file exists for viewing.

The scripts in the AAFAnalyzerBase dir (where all the code is) used to auto-generate code:  

- NodeRegistryGen.sh generates the code which registers AAFObjects in TypedNodeFactoryRegistry.cpp -> RegistrationCode.cpp.gen 

- TypedVisitorGen.sh creates the TypedVisitor signature methods for TypedVisitor.h -> TypedVisitor.h.gen

- TypedNodeGen.sh generates the templated classes in AAFTypedNodeObj.cpp -> TypedNodeGen.sh

- generator.sh uses the MetaDictionary.h file to populate a list of AAFObjects.  This list is used by the above scripts.

