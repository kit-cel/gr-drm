/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
 
#include <drm/transm_params.h>
#include <drm/tables.h>
#include <drm/params.h>

namespace gr {
  namespace drm {
    transm_params::transm_params(unsigned short RM,
        unsigned short SO,
        bool UEP,
        unsigned int n_bytes_A,
        bool text,
        unsigned short msc_mapping,
        unsigned short msc_prot_level_1,
        unsigned short msc_prot_level_2,
        unsigned short sdc_mapping,
        unsigned short sdc_prot_level,
        bool long_interl,
        unsigned int audio_samp_rate,
        std::string station_label,
        std::string text_message)
    {
            /* init tables */
            tables* t = new tables;

            /* init user defined configuration (currently partly hardcoded)*/
            config* c = new config;
            c->init(t, RM, SO, UEP, n_bytes_A, text, msc_mapping, msc_prot_level_1, msc_prot_level_2,
                            sdc_mapping, sdc_prot_level, long_interl, audio_samp_rate, station_label, text_message);

            /* init transmission parameters matching the user defined configuration */
            ofdm_params* o = new ofdm_params;
            o->init(c);

            msc_params* m = new msc_params;
            m->init(c);

            sdc_params* s = new sdc_params;
            s->init(c);

            fac_params* f = new fac_params;
            f->init(c);

            t->calc_gain_cell_params(c->RM(), c->SO(), o->N_S(), o->K_min(), o->K_max());

            /* set member variables (pointers to param instances) */
            d_cfg = c;
            d_ofdm = o;
            d_msc = m;
            d_sdc = s;
            d_fac = f;
    }

    config
    transm_params::cfg()
    {
            return *d_cfg;
    }

    ofdm_params
    transm_params::ofdm()
    {
            return *d_ofdm;
    }

    msc_params
    transm_params::msc()
    {
            return *d_msc;
    }

    sdc_params
    transm_params::sdc()
    {
            return *d_sdc;
    }

    fac_params
    transm_params::fac()
    {
            return *d_fac;
    }
  }
}
