#
AUTODUCK="c:/program files/autoduck/autoduck.exe"
#
ADFLAGS="OMEXTERNAL | OMINTERNAL"
#
SRC="src/*.cpp src/*.h"
#
OUT=docs/ObjectManager
HFMT="c:/program files/autoduck/html.fmt"
OMFMT="docs/html.fmt"
CONTENTS="docs/contents.d"
#
# html output
#
"${AUTODUCK}" /x "${ADFLAGS}" /s"${OMFMT}" /o ${OUT}.htm /r html /f"${HFMT}" ${SRC} "${CONTENTS}"
#
