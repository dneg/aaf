#
ADROOT="c:/program files/autoduck"
AUTODUCK="${ADROOT}/autoduck.exe"
ADHTML="${ADROOT}/html.fmt"
#
SRC="src/*.cpp src/*.h"
#
FLAGS="OMEXTERNAL | OMINTERNAL"
#
OUTPUT=docs/ObjectManager.html
OMHTML="docs/html.fmt"
OMCONT="docs/contents.d"
#
"${AUTODUCK}" /x "${FLAGS}" /s"${OMHTML}" /o ${OUTPUT} /r html /f"${ADHTML}" ${SRC} "${OMCONT}"
#
