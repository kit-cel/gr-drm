# Finds Fdkaac library
#
#  Fdkaac_INCLUDE_DIR - where to find Fdkaac.h, etc.
#  Fdkaac_LIBRARIES   - List of libraries when using Fdkaac.
#  Fdkaac_FOUND       - True if Fdkaac found.
#

if (Fdkaac_INCLUDE_DIR)
  # Already in cache, be silent
  set(Fdkaac_FIND_QUIETLY TRUE)
endif (Fdkaac_INCLUDE_DIR)

find_path(Fdkaac_INCLUDE_DIR aacenc_lib.h aacdecoder_lib.h FDK_audio.h genercStds.h machine_type.h
  /opt/local/include
  /usr/local/include
  /usr/include
  /usr/local/include/fdk-aac
)

set(Fdkaac_NAMES fdk-aac)
find_library(Fdkaac_LIBRARY
  NAMES ${Fdkaac_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib
)

if (Fdkaac_INCLUDE_DIR AND Fdkaac_LIBRARY)
   set(Fdkaac_FOUND TRUE)
   set( Fdkaac_LIBRARIES ${Fdkaac_LIBRARY} )
else (Fdkaac_INCLUDE_DIR AND Fdkaac_LIBRARY)
   set(Fdkaac_FOUND FALSE)
   set(Fdkaac_LIBRARIES)
endif (Fdkaac_INCLUDE_DIR AND Fdkaac_LIBRARY)

if (Fdkaac_FOUND)
   if (NOT Fdkaac_FIND_QUIETLY)
      message(STATUS "Found Fdkaac: ${Fdkaac_LIBRARY}")
   endif (NOT Fdkaac_FIND_QUIETLY)
else (Fdkaac_FOUND)
   if (Fdkaac_FIND_REQUIRED)
      message(STATUS "Looked for Fdkaac libraries named ${Fdkaac_NAMES}.")
      message(STATUS "Include file detected: [${Fdkaac_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${Fdkaac_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find Fdkaac library")
   endif (Fdkaac_FIND_REQUIRED)
endif (Fdkaac_FOUND)

mark_as_advanced(
  Fdkaac_LIBRARY
  Fdkaac_INCLUDE_DIR
  )

message(STATUS "fdk results: ${Fdkaac_LIBRARY}  ${Fdkaac_INCLUDE_DIR}")
