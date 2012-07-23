# Copyright 2011 Free Software Foundation, Inc.
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
add_library(gnuradio-drm SHARED drm_audio_encoder_svb.cc drm_scrambler_vbvb.cc 
			drm_tables_mapping.cc drm_tables_params.cc drm_config.cc drm_util.cc
			drm_params.cc drm_transm_params.cc drm_generate_sdc_vb.cc drm_generate_fac_vb.cc drm_partitioning_vbvb.cc drm_punct_vbvb.cc drm_add_tailbits_vbvb.cc drm_interleaver_vbvb.cc drm_qam_map_vbvb.cc drm_cell_mapping_vbvb.cc drm_interleaver_vcvc.cc )
target_link_libraries(gnuradio-drm ${Boost_LIBRARIES} ${GRUEL_LIBRARIES} ${GNURADIO_CORE_LIBRARIES} ${Faac_LIBRARIES})
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
#find_package(Boost COMPONENTS unit_test_framework)

#include(GrTest)
#set(GR_TEST_TARGET_DEPS gnuradio-drm)
#turn each test cpp file into an executable with an int main() function
#add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

#add_executable(qa_drm_audio_encoder_svb qa_drm_audio_encoder_svb.cc)
#target_link_libraries(qa_drm_audio_encoder_svb gnuradio-drm ${Boost_LIBRARIES})
#GR_ADD_TEST(qa_drm_audio_encoder_svb qa_drm_audio_encoder_svb)
