/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"

%{
#include "drm/drm_global_constants.h"
#include "drm/drm_tables.h"
#include "drm/drm_config.h"
#include "drm/drm_params.h"
#include "drm/drm_transm_params.h" 
#include "drm/drm_util.h"
#include "drm/scrambler_bb.h"
#include "drm/audio_encoder_sb.h"
#include "drm/generate_fac_b.h"
#include "drm/generate_sdc_b.h"
#include "drm/add_tailbits_bb.h"
#include "drm/punct_bb.h"
#include "drm/interleaver_bb.h"
#include "drm/interleaver_cc.h"
#include "drm/partitioning_bb.h"
#include "drm/cell_mapping_cc.h"
#include "drm/qam_map_bc.h"
#include "drm/m3ufile_source_f.h"
%}

%include "drm/drm_global_constants.h"
%include "drm/drm_tables.h"
%include "drm/drm_config.h"
%include "drm/drm_params.h"
%include "drm/drm_transm_params.h"
%include "drm/drm_util.h"

%include "drm/scrambler_bb.h"
GR_SWIG_BLOCK_MAGIC2(drm, scrambler_bb);
%include "drm/audio_encoder_sb.h"
GR_SWIG_BLOCK_MAGIC2(drm, audio_encoder_sb);
%include "drm/generate_fac_b.h"
GR_SWIG_BLOCK_MAGIC2(drm, generate_fac_b);
%include "drm/generate_sdc_b.h"
GR_SWIG_BLOCK_MAGIC2(drm, generate_sdc_b);
%include "drm/add_tailbits_bb.h"
GR_SWIG_BLOCK_MAGIC2(drm, add_tailbits_bb);
%include "drm/punct_bb.h"
GR_SWIG_BLOCK_MAGIC2(drm, punct_bb);
%include "drm/interleaver_bb.h"
GR_SWIG_BLOCK_MAGIC2(drm, interleaver_bb);
%include "drm/interleaver_cc.h"
GR_SWIG_BLOCK_MAGIC2(drm, interleaver_cc);
%include "drm/partitioning_bb.h"
GR_SWIG_BLOCK_MAGIC2(drm, partitioning_bb);
%include "drm/cell_mapping_cc.h"
GR_SWIG_BLOCK_MAGIC2(drm, cell_mapping_cc);
%include "drm/qam_map_bc.h"
GR_SWIG_BLOCK_MAGIC2(drm, qam_map_bc);
%include "drm/m3ufile_source_f.h"
GR_SWIG_BLOCK_MAGIC2(drm, m3ufile_source_f);
