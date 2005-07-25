#!/bin/bash
# This script generates a list of all the interface objects which are to be used in TypedNodeFactoryRegistry and outputs
# the list into a text file (classgen.txt).

OBJECT_LIST=`grep AAF_CLASS ../../ref-impl/include/AAFMetaDictionary.h | grep -v CLASS_ | grep -v def | grep -v // | cut -d\( -f 2 | cut -d\, -f 1 | grep -v InterchangeObject | grep -v TextClip | grep -v HTMLClip | grep -v HTMLDescriptor | grep -v MetaDictionary`

rm -f classgen.txt
echo IAAFObject >> classgen.txt;
for obj in $OBJECT_LIST; do 
 echo $obj
 echo IAAF${obj} >> classgen.txt;
done

#must rename below to match how it is defined in AAFMetaDictionary.h
sed s/EdgeCode/Edgecode/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/DefinitionObject/DefObject/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/ClassDefinition/ClassDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFDataDefinition/IAAFDataDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFOperationDefinition/IAAFOperationDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFParameterDefinition/IAAFParameterDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefinition/IAAFTypeDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFPropertyDefinition/IAAFPropertyDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFPluginDefinition/IAAFPluginDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFCodecDefinition/IAAFCodecDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFContainerDefinition/IAAFContainerDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFInterpolationDefinition/IAAFInterpolationDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefinition/IAAFTypeDef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefInteger/IAAFTypeDefInt/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefStrongObjectReference/IAAFTypeDefStrongObjRef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefWeakObjectReference/IAAFTypeDefWeakObjRef/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefEnumeration/IAAFTypeDefEnum/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt

sed s/IAAFTypeDefExtendibleEnumeration/IAAFTypeDefExtEnum/g < classgen.txt > /tmp/classgen.txt
mv /tmp/classgen.txt classgen.txt











