#
AUTODUCK="c:/program files/autoduck/autoduck.exe"
#
ADFLAGS="OMEXTERNAL | OMINTERNAL"
#
SRC="src/*.cpp src/*.h"
#
OUT=docs/OM
HFMT="c:/program files/autoduck/html.fmt"
CONT="docs/OM.d"
#
# html output
#
"${AUTODUCK}" /x "${ADFLAGS}" /o ${OUT}.htm /r html /f"${HFMT}" ${SRC} "${CONT}"
#
