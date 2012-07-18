%module drm_init
%{
#include "drm_global_constants.h"
#include "drm_tables.h"
#include "drm_config.h"
#include "drm_params.h"
#include "drm_transm_params.h" 
#include "drm_util.h"
%}

%include "typemaps.i"
%include "std_vector.i"

%template(IntVector) std::vector<int>;
%template(UCharVector) std::vector<unsigned char>;
%template(VecIntVector) std::vector< std::vector<int> >;

%include "drm_global_constants.h"
%include "drm_tables.h"
%include "drm_config.h"
%include "drm_params.h"
%include "drm_transm_params.h"
%include "drm_util.h"

