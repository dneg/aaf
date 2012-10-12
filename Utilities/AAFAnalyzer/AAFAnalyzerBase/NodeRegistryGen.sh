#!/bin/bash
# This script generates the RegisterAAFObjects() method in TypedNodeFactoryRegistry.cpp
# The implementation is placed into the file RegistrationCode.cpp.gen

INPUTFILE=classgen.txt
CLASSLIST=''

#Call the generator script to produce a class list
. generator.sh
CLASSLIST=`cat $INPUTFILE`

rm -f RegistrationCode.cpp.gen
count=0
for aafobj in $CLASSLIST
do
    count=`expr $count + 1`
    echo "${aafobj}* p${aafobj} = NULL;" >> RegistrationCode.cpp.gen
    echo "Register(AxAUID(p${aafobj}), boost::shared_ptr<TypedNodeFactory>(new TypedNodeFactoryImpl<${aafobj}>()));" >> RegistrationCode.cpp.gen
    echo >> RegistrationCode.cpp.gen
done

rm classgen.txt



