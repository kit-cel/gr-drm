import drm

from drm_mlc_64qam_sm_bc import drm_mlc_64qam_sm_bc
from drm_mlc_16qam_bc import drm_mlc_16qam_bc
from drm_mlc_4qam_bc import drm_mlc_4qam_bc

gen_poly = (91, 121, 101, 91, 121, 101)


def make_mlc(channel_type, tp):
    if channel_type is "FAC":  # 4-QAM
        return drm_mlc_4qam_bc(
            bits_per_symbol=tp.fac().mod_order(),
            denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
            gen_poly=gen_poly,
            interl_seq=tp.fac().bit_interl_seq_0_2(),
            map_tab=tp.cfg().ptables().d_QAM4,
            n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
            pp=tp.fac().punct_pat_0(),
            pp_tail=tp.fac().punct_pat_0(),
            vlen_in=tp.fac().L(),
            vlen_out=tp.fac().N()
            )         
    elif channel_type is "SDC":  # 16-QAM
        if tp.cfg().sdc_mapping() == 0:
            return drm_mlc_16qam_bc(
                vlen_in=tp.sdc().L(),
                vlen_out=tp.sdc().N(),
                n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
                denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
                gen_poly=(91, 121, 101, 91, 121, 101),
                bits_per_symbol=tp.sdc().mod_order(),
                map_tab=tp.cfg().ptables().d_QAM16,
                pp_0=tp.sdc().punct_pat_0(),
                interl_seq_0_2=tp.sdc().bit_interl_seq_0_2(),
                interl_seq_1_2=tp.sdc().bit_interl_seq_1_2(),
                pp_1_tail=tp.sdc().punct_pat_tail_1(),
                pp_1=tp.sdc().punct_pat_1(),
                pp_0_tail=tp.sdc().punct_pat_tail_0(),
                M_total=tp.sdc().M_total(),
                part_len_top=tp.sdc().M_total()[0],
                part_len_bot=tp.sdc().M_total()[1],
                )
        elif tp.cfg().sdc_mapping() == 1:
            return drm_mlc_4qam_bc(
                bits_per_symbol=tp.sdc().mod_order(),
                denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
                gen_poly=gen_poly,
                interl_seq=tp.sdc().bit_interl_seq_0_2(),
                map_tab=tp.cfg().ptables().d_QAM4,
                n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
                pp=tp.sdc().punct_pat_0(),
                pp_tail=tp.sdc().punct_pat_0(),
                vlen_in=tp.sdc().L(),
                vlen_out=tp.sdc().N()
                )       
        else:
            raise RuntimeError("Invalid SDC modulation order: " + str(tp.sdc().mod_order()))
    elif channel_type is "MSC": 
        if tp.cfg().msc_mapping() == 0:  # FIXME This is likely to be an errouneous configuration! Only relevant for DRM+, though
            return drm_mlc_4qam_bc(
                bits_per_symbol=tp.msc().mod_order(),
                denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
                gen_poly=gen_poly,
                interl_seq=tp.msc().bit_interl_seq_0_2(),
                map_tab=tp.cfg().ptables().d_QAM4,
                n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
                pp=tp.msc().punct_pat_0_2(),
                pp_tail=tp.msc().punct_pat_0_2(),
                vlen_in=tp.msc().L_MUX(),
                vlen_out=tp.msc().N_MUX()
                )  
        elif tp.cfg().msc_mapping() == 1:
            return drm_mlc_16qam_bc(
                vlen_in=tp.msc().L_MUX(),
                vlen_out=tp.msc().N_MUX(),
                n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
                denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
                gen_poly=(91, 121, 101, 91, 121, 101),
                bits_per_symbol=tp.msc().mod_order(),
                map_tab=tp.cfg().ptables().d_QAM16,
                pp_0=tp.msc().punct_pat_0_2(),
                interl_seq_0_2=tp.msc().bit_interl_seq_0_2(),
                interl_seq_1_2=tp.msc().bit_interl_seq_1_2(),
                pp_1_tail=tp.msc().punct_pat_tail_1_2(),
                pp_1=tp.msc().punct_pat_1_2(),
                pp_0_tail=tp.msc().punct_pat_tail_0_2(),
                M_total=tp.msc().M_total(),
                part_len_top=tp.msc().M_total()[0],
                part_len_bot=tp.msc().M_total()[1],
                )
        elif tp.cfg().msc_mapping() == 2:
            return drm_mlc_64qam_sm_bc(
                vlen_in=tp.msc().L_MUX(),
                n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE,
                denom_mother_code_rate=drm.DENOM_MOTHER_CODE,
                gen_poly=(91, 121, 101, 91, 121, 101),
                vlen_out=tp.msc().N_MUX(),
                bits_per_symbol=tp.msc().mod_order(),
                map_tab=tp.cfg().ptables().d_QAM64SM,
                pp_0=tp.msc().punct_pat_0_2(),
                pp_0_tail=tp.msc().punct_pat_tail_0_2(),
                pp_1=tp.msc().punct_pat_1_2(),
                pp_1_tail=tp.msc().punct_pat_tail_1_2(),
                pp_2=tp.msc().punct_pat_2_2(),
                pp_2_tail=tp.msc().punct_pat_tail_2_2(),
                interl_seq_0_2=tp.msc().bit_interl_seq_0_2(),
                interl_seq_2_2=tp.msc().bit_interl_seq_2_2(),
                interl_seq_1_2=tp.msc().bit_interl_seq_1_2(),
                M_total=tp.msc().M_total(),
                part_len_bot=tp.msc().M_total()[2],
                part_len_top=tp.msc().M_total()[0],
                part_len_mid=tp.msc().M_total()[1],
                )
        else:
            print("MSC Mapping:", tp.cfg().msc_mapping())
            raise RuntimeError("Invalid MSC modulation order: " + str(tp.msc().mod_order()))
    else:
        raise RuntimeError("Invalid channel type:", channel_type)
