AAFTestProgram.xml.zip is a UML for the base components of the
analyzer and can be viewed using MagicDraw.  A communitry edition of
MagicDraw is available at: http://www.magicdraw.com.

AAFAnalyzerBaseFramework.pdf - a pdf of the above UML diagram.

The scripts in the AAFAnalyzerBase dir are used to auto-generate code
that depends on the full set of AAF objects:

- NodeRegistryGen.sh generates the code which registers AAFObjects in
  TypedNodeFactoryRegistry.cpp -> RegistrationCode.cpp.gen

- TypedVisitorGen.sh creates the TypedVisitor signature methods for
  TypedVisitor.h -> TypedVisitor.h.gen

- TypedNodeGen.sh generates the templated classes in
  AAFTypedNodeObj.cpp -> TypedNodeGen.sh

- generator.sh uses the MetaDictionary.h file to populate a list of
  AAFObjects.  This list is used by the above scripts.

