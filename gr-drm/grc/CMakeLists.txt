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
install(FILES
    drm_scrambler_vbvb.xml
    drm_audio_encoder_svb.xml
    drm_generate_fac_vb.xml
    drm_generate_sdc_vb.xml
    drm_add_tailbits_vbvb.xml
    drm_punct_vbvb.xml
    drm_interleaver_vbvb.xml
    drm_interleaver_vcvc.xml
    drm_partitioning_4_vbvb.xml
    drm_partitioning_16_vbvb.xml
    drm_partitioning_64sm_vbvb.xml
    drm_cell_mapping_vcvc.xml
    drm_qam_map_vbvc.xml
    drm_m3ufile_source_f.xml DESTINATION share/gnuradio/grc/blocks
)
