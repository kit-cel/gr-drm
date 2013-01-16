/* -*- c++ -*- */

#define DRMRX_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drmrx_swig_doc.i"


%{
#include "drmrx_constants.h"
#include "drmrx_conf.h"
#include "drmrx_params.h"
#include "drmrx_freq_sync_cc.h"
#include "drmrx_time_sync_cc.h"
%}

%include "drmrx_constants.h"
%include "drmrx_conf.h"
%include "drmrx_params.h"

GR_SWIG_BLOCK_MAGIC(drmrx,freq_sync_cc);
%include "drmrx_freq_sync_cc.h"

GR_SWIG_BLOCK_MAGIC(drmrx,time_sync_cc);
%include "drmrx_time_sync_cc.h"
