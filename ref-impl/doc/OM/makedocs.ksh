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
ADROOT="../bin"
#
# The autoduck program
#
ADEXEC="${ADROOT}/autoduck.exe"
#
# The standard autoduck html format
#
ADHTML="${ADROOT}/html.fmt"
#
# The Object Manager autoduck directory
#
OMDOC=doc/OM
#
# The Object Manager sources
#
SRC="src/OM/*.cpp src/OM/*.h include/OM/*.h"
#
# Extract both the internal and external documentation
#
# OMEXTERNAL = documentation of interest to Object Manager clients
# OMINTERNAL = documentation of interest to Object Manager developers
#
FLAGS="OMEXTERNAL | OMINTERNAL"
#
# The generated html file
#
OUTPUT=${OMDOC}/ObjectManager.html
#
# Customized files
#
OMHTML=${OMDOC}/html.fmt
OMCONT=${OMDOC}/contents.d
#
"${ADEXEC}" /x "${FLAGS}" /s ${OMHTML} /o ${OUTPUT} /r html /f "${ADHTML}" ${SRC} ${OMCONT}
#
