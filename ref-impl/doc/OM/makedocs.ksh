#
# Generate the Object Manager documentation
#
# These commands generate html, other formats are possible
#
# Autoduck works by extracting specially tagged comments from
# the source files.
#
# Where autoduck was installed
#
ADROOT="c:/program files/autoduck"
#
# The autoduck program
#
ADEXEC="${ADROOT}/autoduck.exe"
#
# The standard autoduck html format
#
ADHTML="${ADROOT}/html.fmt"
#
# The Object Manager sources
#
SRC="src/*.cpp src/*.h"
#
# Extract both the internal and external documentation
#
FLAGS="OMEXTERNAL | OMINTERNAL"
#
# The generated html file
#
OUTPUT=docs/ObjectManager.html
#
# Customized files
#
OMHTML="docs/html.fmt"
OMCONT="docs/contents.d"
#
"${ADEXEC}" /x "${FLAGS}" /s ${OMHTML} /o ${OUTPUT} /r html /f "${ADHTML}" ${SRC} ${OMCONT}
#
