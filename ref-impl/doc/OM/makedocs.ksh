#
ZIP="c:/program files/winzip/winzip32.exe"
HCW="C:/program files/microsoft visual studio/common/tools/hcw.exe"
AUTODUCK="c:/program files/autoduck/autoduck.exe"
#ZIP=c:/pkzip/pkzip.exe
ZIPFLAGS="-min -a"
#ZIPFLAGS=""
#
#ADFLAGS="OMEXTERNAL"
ADFLAGS="OMEXTERNAL | OMINTERNAL"
#
# SRC="OMPropertySet.h OMPropertySet.cpp OMProperty.h OMProperty.cpp OMStoredObject.h OMStoredObject.cpp"
#SRC="OMPropertySet.h OMPropertySet.cpp OMProperty.h OMProperty.cpp OMStoredObject.h OMStoredObject.cpp OMPropertyT.h OMDataStreamProperty.cpp OMDataStreamProperty.h OMStorable.h OMStorable.cpp"
#SRC="../src/OMPropertySet.h ../src/OMPropertySet.cpp ../src/OMProperty.h ../src/OMProperty.cpp ../src/OMStoredObject.h ../src/OMStoredObject.cpp ../src/OMPropertyT.h ../src/OMDataStreamProperty.cpp ../src/OMDataStreamProperty.h ../src/OMStorable.h ../src/OMStorable.cpp"
#SRC="../src/*.cpp ../src/*.h"
SRC="src/*.cpp src/*.h"
#
# print SRC="$SRC"
#
#OUT=OM
OUT=docs/OM
#
PRODS="${OUT}.htm ${OUT}.hlp ${OUT}.doc"
#
HFMT="c:/program files/autoduck/html.fmt"
#CONT="OM.d"
CONT="docs/OM.d"
#
# Windows help file output
#
##"${AUTODUCK}" /x "${ADFLAGS}" /o ${OUT}.rtf /r help ${SRC} "${CONT}"
##"${HCW}" /a /e /c ${OUT}.hpj
##rm -f ${OUT}.rtf ${OUT}.hpj
#
# html output
#
"${AUTODUCK}" /x "${ADFLAGS}" /o ${OUT}.htm /r html /f"${HFMT}" ${SRC} "${CONT}"
#
# Word file output
#
##"${AUTODUCK}" /x "${ADFLAGS}" /o ${OUT}.doc /r doc ${SRC} "${CONT}"
#
##"${ZIP}" ${ZIPFLAGS} ${OUT}.zip ${PRODS}
## rm -f ${PRODS}
#
