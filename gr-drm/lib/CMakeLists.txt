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

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND drm_sources    
    drm_util.cc 
    drm_transm_params.cc 
    drm_params.cc 
    drm_config.cc
	  drm_tables_mapping.cc 
	  drm_tables_params.cc
	  scrambler_bb_impl.cc
    audio_encoder_sb_impl.cc
    generate_fac_b_impl.cc
    generate_sdc_b_impl.cc
    add_tailbits_bb_impl.cc
    punct_bb_impl.cc
    interleaver_bb_impl.cc
    interleaver_cc_impl.cc
    partitioning_bb_impl.cc
    cell_mapping_cc_impl.cc
    qam_map_bc_impl.cc
    m3ufile_source_f_impl.cc
    wavfile_drm.cc )

add_library(gnuradio-drm SHARED ${drm_sources})
target_link_libraries(gnuradio-drm ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${Faac_LIBRARIES})
set_target_properties(gnuradio-drm PROPERTIES DEFINE_SYMBOL "gnuradio_drm_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-drm
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_drm_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_drm.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_drm.cc
)

add_executable(test-drm ${test_drm_sources})

target_link_libraries(
  test-drm
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-drm
)

GR_ADD_TEST(test_drm test-drm)
