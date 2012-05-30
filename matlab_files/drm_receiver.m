%%%%%%%%%%%%%%%%%%
%% DRM Receiver %%
%%%%%%%%%%%%%%%%%%

% perfect synchronisation and no channel influences are assumed
% OFDM and channel parameters are treated as given!

clear all
clc

%% Declare global variables, create input stream
run drm_global_variables
run drm_transmitter

%% preallocate memory for output, calculate number of frames

n_stf = size(complex_baseband, 1); % number of super transmission frames
msc_data = zeros(n_stf, MSC.M_TF*MSC.L_MUX); % always M_TF multiplex frames per row
sdc_data = zeros(1, SDC.L_SDC);
fac_data = zeros(1, FAC.L_FAC);

for n = 1 : n_stf
    %% OFDM Demod
    super_tframe_rx = drm_iofdm(complex_baseband(n, :), OFDM);

    %% Cell Demapping
    [msc_stream_map_interl sdc_stream_map, fac_stream_map] = drm_cell_demapping(super_tframe_rx, MSC, SDC, FAC, OFDM);

    %% MSC cell deinterleaving
    msc_stream_mapped = drm_mlc_deinterleaver(msc_stream_map_interl, 'MSC_cells', MSC);

    %% Symbol demapping
    msc_stream_interl = drm_demapping(msc_stream_mapped, 'MSC', MSC);
    sdc_stream_interl = drm_demapping(sdc_stream_mapped, 'SDC', SDC);
    fac_stream_interl = drm_demapping(fac_stream_mapped, 'FAC', FAC);

    %% Bit deinterleaving
    msc_stream_encoded = drm_mlc_deinterleaver(msc_stream_interl, 'MSC', MSC);
    sdc_stream_encoded = drm_mlc_deinterleaver(sdc_stream_interl, 'SDC', SDC);
    fac_stream_encoded = drm_mlc_deinterleaver(fac_stream_interl, 'FAC', FAC);

    %% Decoding
    msc_stream_decoded = drm_mlc_decoder(msc_stream_encoded, 'MSC', MSC, OFDM);
    sdc_stream_decoded = drm_mlc_decoder(sdc_stream_encoded, 'SDC', SDC, OFDM);
    fac_stream_decoded = drm_mlc_decoder(fac_stream_encoded, 'FAC', FAC, OFDM);   
    
    %% Departitioning

    %% Unscrambling

    %% Output (append to previous output)
end