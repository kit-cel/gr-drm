/* -*- c++ -*- */

#define DRMRX_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drmrx_swig_doc.i"


%{
#include "drmrx_conf.h"
#include "drmrx_params.h"
%}

%include "drmrx_conf.h"
%include "drmrx_params.h"

