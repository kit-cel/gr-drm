/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"


%{
#include "drm_source_encoder_sb.h"
%}


GR_SWIG_BLOCK_MAGIC(drm,source_encoder_sb);
%include "drm_source_encoder_sb.h"
