#
AUTODUCK="c:/program files/autoduck/autoduck.exe"
#
FLAGS="OMEXTERNAL | OMINTERNAL"
#
SRC="src/*.cpp src/*.h"
#
OUT=docs/ObjectManager
HFMT="c:/program files/autoduck/html.fmt"
OMHTML="docs/html.fmt"
CONTENTS="docs/contents.d"
#
# html output
#
"${AUTODUCK}" /x "${FLAGS}" /s"${OMHTML}" /o ${OUT}.htm /r html /f"${HFMT}" ${SRC} "${CONTENTS}"
#
