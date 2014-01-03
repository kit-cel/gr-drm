/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"

//%include "typemaps.i"

//%template(IntVector) std::vector<int>;
//%template(UCharVector) std::vector<unsigned char>;
//%template(VecIntVector) std::vector< std::vector<int> >;

%{
#include "drm/global_constants.h"
#include "drm/tables.h"
#include "drm/config.h"
#include "drm/params.h"
#include "drm/transm_params.h" 
//#include "drm_util.h"
#include "drm/audio_encoder_svb.h"
#include "drm/scrambler_vbvb.h"
#include "drm/generate_sdc_vb.h"
#include "drm/generate_fac_vb.h"
#include "drm/partitioning_vbvb.h"
#include "drm/punct_vbvb.h"
#include "drm/add_tailbits_vbvb.h"
#include "drm/interleaver_vbvb.h"
#include "drm/qam_map_vbvc.h"
#include "drm/cell_mapping_vcvc.h"
#include "drm/audio_decoder_vbs.h"
#include "drm/cell_interleaver_vcvc.h"
%}

%include "drm/global_constants.h"
%include "drm/tables.h"
%include "drm/config.h"
%include "drm/params.h"
%include "drm/transm_params.h"
//%include "drm_util.h"

//drm_audio_encoder_svb_sptr drm_make_audio_encoder_svb (transm_params* tp);
//  
//class drm_audio_encoder_svb : public gr::block
//{
//private:
//    drm_audio_encoder_svb (transm_params* tp);
//};

%include "drm/audio_encoder_svb.h"
GR_SWIG_BLOCK_MAGIC2(drm,audio_encoder_svb);

%include "drm/scrambler_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm,scrambler_vbvb);

%include "drm/generate_sdc_vb.h"
GR_SWIG_BLOCK_MAGIC2(drm,generate_sdc_vb);

%include "drm/generate_fac_vb.h"
GR_SWIG_BLOCK_MAGIC2(drm,generate_fac_vb);

%include "drm/partitioning_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm,partitioning_vbvb);

%include "drm/punct_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm,punct_vbvb);

%include "drm/add_tailbits_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm,add_tailbits_vbvb);

%include "drm/interleaver_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm,interleaver_vbvb);

%include "drm/qam_map_vbvc.h"
GR_SWIG_BLOCK_MAGIC2(drm,qam_map_vbvc);

%include "drm/cell_mapping_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(drm,cell_mapping_vcvc);

%include "drm/audio_decoder_vbs.h"
GR_SWIG_BLOCK_MAGIC2(drm,audio_decoder_vbs);

%include "drm/cell_interleaver_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(drm,cell_interleaver_vcvc);
