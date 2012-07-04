/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"


%{
#include "drm_audio_encoder_sb.h"
#include "drm_scrambler_vbvb.h"
#include "drm_generate_sdc_vb.h"
#include "drm_generate_fac_vb.h"
#include "drm_partitioning_vbvb.h"
%}

GR_SWIG_BLOCK_MAGIC(drm,audio_encoder_sb);

drm_audio_encoder_sb_sptr drm_make_audio_encoder_sb (transm_params* tp);
  
class drm_audio_encoder_sb : public gr_block
{
private:
    drm_audio_encoder_sb (transm_params* tp);
};

GR_SWIG_BLOCK_MAGIC(drm,scrambler_vbvb);
%include "drm_scrambler_vbvb.h"
GR_SWIG_BLOCK_MAGIC(drm,generate_sdc_vb);
%include "drm_generate_sdc_vb.h"

GR_SWIG_BLOCK_MAGIC(drm,generate_fac_vb);
%include "drm_generate_fac_vb.h"

GR_SWIG_BLOCK_MAGIC(drm,partitioning_vbvb);
%include "drm_partitioning_vbvb.h"
