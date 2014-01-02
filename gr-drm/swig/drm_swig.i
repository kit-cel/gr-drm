/* -*- c++ -*- */

#define DRM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drm_swig_doc.i"

%{
#include "drm/scrambler_vbvb.h"
%}


%include "drm/scrambler_vbvb.h"
GR_SWIG_BLOCK_MAGIC2(drm, scrambler_vbvb);
