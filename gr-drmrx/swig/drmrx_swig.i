/* -*- c++ -*- */

#define DRMRX_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "drmrx_swig_doc.i"

namespace std {
    %template()     vector< std::vector< std::vector< std::vector<int> > > >;
    %template()     vector< std::vector< std::vector< std::vector<gr_complex > > > >;
};


%{
#include "drmrx_conf.h"
#include "drmrx_params.h"
%}

%include "drmrx_conf.h"
%include "drmrx_params.h"

