# Copyright (c) 2009, Whispersoft s.r.l.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
# * Neither the name of Whispersoft s.r.l. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Finds Faac library
#
#  Faac_INCLUDE_DIR - where to find faac.h, etc.
#  Faac_LIBRARIES   - List of libraries when using Faac.
#  Faac_FOUND       - True if Faac found.
#

if (Faac_INCLUDE_DIR)
  # Already in cache, be silent
  set(Faac_FIND_QUIETLY TRUE)
endif (Faac_INCLUDE_DIR)

find_path(Faac_INCLUDE_DIR faac.h
  /opt/local/include
  /usr/local/include
  /usr/include
)

set(Faac_NAMES faac)
find_library(Faac_LIBRARY
  NAMES ${Faac_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib
)

if (Faac_INCLUDE_DIR AND Faac_LIBRARY)
   set(Faac_FOUND TRUE)
   set( Faac_LIBRARIES ${Faac_LIBRARY} )
else (Faac_INCLUDE_DIR AND Faac_LIBRARY)
   set(Faac_FOUND FALSE)
   set(Faac_LIBRARIES)
endif (Faac_INCLUDE_DIR AND Faac_LIBRARY)

if (Faac_FOUND)
   if (NOT Faac_FIND_QUIETLY)
      message(STATUS "Found Faac: ${Faac_LIBRARY}")
   endif (NOT Faac_FIND_QUIETLY)
else (Faac_FOUND)
   if (Faac_FIND_REQUIRED)
      message(STATUS "Looked for Faac libraries named ${Faac_NAMES}.")
      message(STATUS "Include file detected: [${Faac_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${Faac_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find Faac library")
   endif (Faac_FIND_REQUIRED)
endif (Faac_FOUND)

mark_as_advanced(
  Faac_LIBRARY
  Faac_INCLUDE_DIR
  )
