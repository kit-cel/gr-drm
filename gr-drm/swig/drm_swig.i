/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"

%include "typemaps.i"

%template(IntVector) std::vector<int>;
%template(UCharVector) std::vector<unsigned char>;
%template(VecIntVector) std::vector< std::vector<int> >;

%{
#include "drm_global_constants.h"
#include "drm_tables.h"
#include "drm_config.h"
#include "drm_params.h"
#include "drm_transm_params.h" 
#include "drm_util.h"
#include "drm_audio_encoder_svb.h"
#include "drm_scrambler_vbvb.h"
#include "drm_generate_sdc_vb.h"
#include "drm_generate_fac_vb.h"
#include "drm_partitioning_vbvb.h"
#include "drm_punct_vbvb.h"
#include "drm_add_tailbits_vbvb.h"
#include "drm_interleaver_vbvb.h"
#include "drm_qam_map_vbvb.h"
#include "drm_cell_mapping_vbvb.h"
#include "drm_audio_decoder_vbs.h"
#include "drm_cell_interleaver_vcvc.h"
%}

%include "drm_global_constants.h"
%include "drm_tables.h"
%include "drm_config.h"
%include "drm_params.h"
%include "drm_transm_params.h"
%include "drm_util.h"

GR_SWIG_BLOCK_MAGIC(drm,audio_encoder_svb);

drm_audio_encoder_svb_sptr drm_make_audio_encoder_svb (transm_params* tp);
  
class drm_audio_encoder_svb : public gr_block
{
private:
    drm_audio_encoder_svb (transm_params* tp);
};

GR_SWIG_BLOCK_MAGIC(drm,scrambler_vbvb);
%include "drm_scrambler_vbvb.h"
GR_SWIG_BLOCK_MAGIC(drm,generate_sdc_vb);
%include "drm_generate_sdc_vb.h"

GR_SWIG_BLOCK_MAGIC(drm,generate_fac_vb);
%include "drm_generate_fac_vb.h"

GR_SWIG_BLOCK_MAGIC(drm,partitioning_vbvb);
%include "drm_partitioning_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,punct_vbvb);
%include "drm_punct_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,add_tailbits_vbvb);
%include "drm_add_tailbits_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,interleaver_vbvb);
%include "drm_interleaver_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,qam_map_vbvb);
%include "drm_qam_map_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,cell_mapping_vbvb);
%include "drm_cell_mapping_vbvb.h"

GR_SWIG_BLOCK_MAGIC(drm,audio_decoder_vbs);
%include "drm_audio_decoder_vbs.h"

GR_SWIG_BLOCK_MAGIC(drm,cell_interleaver_vcvc);
%include "drm_cell_interleaver_vcvc.h"
