#include <iostream.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

static const replacementSpec_t replacements[] =
{
  { "ClassDefinition", "ClassDef" },
  { "DataDefinition", "DataDef" },
  { "DefinitionObject", "DefObject" },
  { "EdgeCode", "Edgecode" },
  { "OperationDefinition", "OperationDef" },
  { "InterchangeObject", "Object" },
  { "JPEGImageData", "JPEGData" },
  { "MIDIFileData", "MIDIData" },
  { "ParameterDefinition", "ParameterDef" },
  // { "InterpolationDefinition", "InterpolationDef" },
  { "PropertyDefinition", "PropertyDef" },
  { "TypeDefinition", "TypeDef" },
  { "TypeDefinitionCharacter", "TypeDefCharacter" },
  { "TypeDefinitionEnumeration", "TypeDefEnum" },
  { "TypeDefinitionExtendibleEnumeration", "TypeDefExtEnum" },
  { "TypeDefinitionFixedArray", "TypeDefFixedArray" },
  { "TypeDefinitionInteger", "TypeDefInt" },
  { "TypeDefinitionRecord", "TypeDefRecord" },
  { "TypeDefinitionRename", "TypeDefRename" },
  { "TypeDefinitionSet", "TypeDefSet" },
  { "TypeDefinitionStream", "TypeDefStream" },
  { "TypeDefinitionString", "TypeDefString" },
  { "TypeDefinitionIndirect", "TypeDefIndirect" },
  { "TypeDefinitionOpaque", "TypeDefOpaque" },
  { "TypeDefinitionStrongObjectReference", "TypeDefStrongObjRef" },
  { "TypeDefinitionVariableArray", "TypeDefVariableArray" },
  { "TypeDefinitionWeakObjectReference", "TypeDefWeakObjRef" },
  { "UNIXLocator", "UnixLocator" },
  { "CodecDefinition", "CodecDef" },
  { "ContainerDefinition", "ContainerDef" },
  { "PluginDefinition", "PluginDef" },
  { "PluggableDefinition", "PluggableDef" },
  { 0, 0 }
};

#define AAF_CLASS(name, dataid, parent, concrete) \
cout << endl; \
cout << "const aafUID_t AUID_AAF"; \
replaceAndPrint (#name, replacements, cout); \
cout << " =" << endl; \
dataid

#include "literalAuidDef.h"

static void doFile (const char * moduleName)
{
  assert (moduleName);
  cout << "#ifndef __" << moduleName << "_h__" << endl;
  cout << "#define __" << moduleName << "_h__" << endl;
  printCopyright (cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;
#include "AAFMetaDictionary.h"
  cout << endl;
  cout << "#endif // ! __" << moduleName << "_h__" << endl;
}


void main (int argc, char ** argv)
{
  char * moduleName = 0;
  validateArgs (argc, argv, moduleName);
  // If validateArgs() returned (without exit()ing) then moduleName
  // must be properly filled in.
  assert (moduleName);
  doFile (moduleName);
  exit (0);
}
