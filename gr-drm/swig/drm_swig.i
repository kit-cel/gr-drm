/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"


%{
#include "drm_audio_encoder_sb.h"
#include "drm_scrambler_vbvb.h"
/*#include "drm_global_constants.h"
#include "drm_tables.h"
#include "drm_config.h"
#include "drm_params.h"
#include "drm_transm_params.h"*/
%}

/*%include "drm_global_constants.h"
%include "drm_tables.h"
%include "drm_config.h"
%include "drm_params.h"
%include "drm_transm_params.h"*/

GR_SWIG_BLOCK_MAGIC(drm,audio_encoder_sb);

drm_audio_encoder_sb_sptr drm_make_audio_encoder_sb (transm_params* tp);
  
class drm_audio_encoder_sb : public gr_block
{
private:
    drm_audio_encoder_sb (transm_params* tp);
};

GR_SWIG_BLOCK_MAGIC(drm,scrambler_vbvb);
%include "drm_scrambler_vbvb.h"
