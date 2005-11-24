#!/bin/bash
# This script generates a file used by InputParser, using EPDerivationChain.dtd

OPERATION_GROUP_PARAMS=`grep "<\!ENTITY % OPERATION_GROUP" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g'`
SRC_REF_PARAMS=`grep "<\!ENTITY % SOURCE_REFERENCE" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g'`
SEGMENT_PARAMS=`grep "<\!ENTITY % SEGMENT" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g' | sed 's/%OPERATION_GROUP;/'"$OPERATION_GROUP_PARAMS"'/' | sed 's/%SOURCE_REFERENCE;/'"$SRC_REF_PARAMS"'/'`
NON_OG_SEGMENT_PARAMS=`grep "<\!ENTITY % SEGMENT" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g' | sed 's/%OPERATION_GROUP;//'`
NON_OG_COMPONENT_PARAMS=`grep "<\!ENTITY % COMPONENT" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g' | sed 's/%SEGMENT;/'"$NON_OG_SEGMENT_PARAMS"'/'`
MOB_PARAMS=`grep "<\!ENTITY % MOB" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g'`
ESSENCE_PARAMS=`grep "<\!ENTITY % ESSENCE_TYPE" EPDerivationChain.dtd | sed 's/.* \"//'  | sed 's/\">//' | sed 's/ | / /g'`
OUTHEADERFILE=InputParser.cpp.gen

rm -f InputParser.cpp.gen

####GENERATE THE HEADER FILE METHODS
exec 4>$OUTHEADERFILE

# Populate create segment map ( for operation groups )
for ogparams in $OPERATION_GROUP_PARAMS
do
    echo '    _createSegmentMap[L"'$ogparams'"] = &TestFileBuilder::CreateOperationGroup;' >&4
done

echo >&4

# Populate fill segment map (for operation groups)
for ogparams in $OPERATION_GROUP_PARAMS
do
    echo '    _fillSegmentMapD[L"'$ogparams'"] = &TestFileBuilder::AddToOperationGroup;' >&4
done

echo >&4

# Populate type D property map (for operation groups)
for ogparams in $OPERATION_GROUP_PARAMS
do
	for segparams in $SEGMENT_PARAMS
	do
	    echo '    _typeDPropertyMap[TypeDPair( L"'$ogparams'", L"'$segparams'" )] = TestFileBuilder::INPUT_SEGMENT;' >&4
	done
	echo >&4
done

# Populate effect map (for non-operation group components)
for compparams in $NON_OG_COMPONENT_PARAMS
do
    echo '    _effectMap[L"'$compparams'"] = kAAFOperationDef_Unknown;' >&4
done

echo >&4

# Populate the rational parameter map with default (blank) values
for mobparams in $MOB_PARAMS
do
    echo '    _optRationalParam[L"'$mobparams'"] = L"";' >&4
done

echo >&4

# Populate the essence map
for essenceparams in $ESSENCE_PARAMS
do
	upper=`exec echo $essenceparams | tr '[a-z]' '[A-Z]'`
    echo '    _essenceMap[L"'$essenceparams'"] = TestFileBuilder::'$upper';' >&4
done

# Populate the essence map

echo >&4
