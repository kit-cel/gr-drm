#include "drmrx_conf.h"

drmrx_conf::drmrx_conf()
{
    d_params = new drmrx_params();
    set_RM(p()->RM_NONE());
    set_SO(p()->SO_NONE());

    /********** channel coding parameters ******************/
    set_msc_const_size(0); // default value, invalid
    set_sdc_const_size(0); // default value, invalid
    set_fac_const_size(4); // FAC always uses 4QAM
}
