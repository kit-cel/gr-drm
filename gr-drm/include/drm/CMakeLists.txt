# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

# Get drm_transm_params header files out of the lib/ directory.
message(STATUS "Copying some header files from lib/ to include/")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_global_constants.h ${PROJECT_SOURCE_DIR}/include/drm/drm_global_constants.h
								COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_tables.h ${PROJECT_SOURCE_DIR}/include/drm/drm_tables.h
								COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_config.h ${PROJECT_SOURCE_DIR}/include/drm/drm_config.h
								COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_params.h ${PROJECT_SOURCE_DIR}/include/drm/drm_params.h
								COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_transm_params.h ${PROJECT_SOURCE_DIR}/include/drm/drm_transm_params.h
								COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/drm_util.h ${PROJECT_SOURCE_DIR}/include/drm/drm_util.h
)

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h  
    drm_global_constants.h
    drm_tables.h
    drm_config.h
    drm_params.h
    drm_transm_params.h
    drm_util.h
    scrambler_vbvb.h
    audio_encoder_svb.h
    generate_fac_vb.h
    generate_sdc_vb.h
    add_tailbits_vbvb.h
    punct_vbvb.h
    interleaver_vbvb.h DESTINATION include/drm
)
