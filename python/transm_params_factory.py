import drm


def make_transm_params(RM=1, SO=3, qam_order_msc=64, qam_order_sdc=16, long_interl=True,
                       audio_samp_rate=24000, station_label="station label", text_message="text message"):
    # This simplified constructor disables UEP and chooses a code rate of R=0.5 for MSC and SDC
    msc_mapping = None
    if qam_order_msc == 16:
        msc_mapping = 1
    elif qam_order_msc == 64:
        msc_mapping = 2
    else:
        raise ValueError("Invalid MSC modulation order, choose 16 or 64.")

    sdc_mapping = None
    if qam_order_sdc == 4:
        sdc_mapping = 1
    elif qam_order_sdc == 16:
        sdc_mapping = 0
    else:
        raise ValueError("Invalid SDC modulation order, choose 4 or 16.")

    # allow giving the sample rate in kHz
    if audio_samp_rate == 12 or audio_samp_rate == 24:
        audio_samp_rate *= 1000

    return drm.transm_params(RM=RM, SO=SO, UEP=False, n_bytes_A=0, msc_mapping=msc_mapping, msc_prot_level_1=0,
                            msc_prot_level_2=0, sdc_mapping=sdc_mapping, sdc_prot_level=0, long_interl=long_interl,
                            audio_samp_rate=audio_samp_rate, station_label=station_label, text_message=text_message)

