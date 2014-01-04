/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"

%{
#include "drm_global_constants.h"
#include "drm_tables.h"
#include "drm_config.h"
#include "drm_params.h"
#include "drm_transm_params.h" 
#include "drm_util.h"
#include "drm/scrambler_vbvb.h"
#include "drm/audio_encoder_svb.h"
#include "drm/generate_fac_vb.h"
#include "drm/generate_sdc_vb.h"
#include "drm/add_tailbits_vbvb.h"
#include "drm/punct_vbvb.h"
#include "drm/interleaver_vbvb.h"
%}

%include "drm_global_constants.h"
%include "drm_tables.h"
%include "drm_config.h"
%include "drm_params.h"
%include "drm_transm_params.h"
%include "drm_util.h"

%include "drm/scrambler_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm, scrambler_vbvb);

%include "drm/audio_encoder_svb.h"
GR_SWIG_BLOCK_MAGIC2(drm, audio_encoder_svb);

%include "drm/generate_fac_vb.h"
GR_SWIG_BLOCK_MAGIC2(drm, generate_fac_vb);
%include "drm/generate_sdc_vb.h"
GR_SWIG_BLOCK_MAGIC2(drm, generate_sdc_vb);
%include "drm/add_tailbits_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm, add_tailbits_vbvb);
%include "drm/punct_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm, punct_vbvb);
%include "drm/interleaver_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm, interleaver_vbvb);
