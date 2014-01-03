# Copyright 2011 Free Software Foundation, Inc.
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
# Install public header files
########################################################################
install(FILES
    api.h
    audio_encoder_svb.h
    scrambler_vbvb.h
    global_constants.h
    tables.h
    config.h
    params.h
    transm_params.h
    transdes.h
    generate_sdc_vb.h
    generate_fac_vb.h 
#   util.h
    partitioning_vbvb.h
    punct_vbvb.h
    add_tailbits_vbvb.h
    interleaver_vbvb.h
    qam_map_vbvc.h
    cell_mapping_vcvc.h
    audio_decoder_vbs.h
    cell_interleaver_vcvc.h DESTINATION include/drm
)
