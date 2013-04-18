#include "drmrx_conf.h"

drmrx_conf::drmrx_conf()
{
    d_params = new drmrx_params();
    set_RM(p()->RM_NONE());
    set_SO(p()->SO_NONE());
}
