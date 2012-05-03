%% DRM transmitter

% disable clear for unit_tests and receiver
% clear all
% clc

%% calculate global variables, for the list of assumptions see drm_global_variables.m
run drm_global_variables

%% create dummy bit streams TODO: replace msc stream by a 3,1-matrix
msc_stream = randint(1, MSC.L_MUX); % one MSC multiplex frame
sdc_stream = randint(1, SDC.L_SDC); % one SDC block
fac_stream = randint(1, FAC.L_FAC); % one FAC block

%% energy dispersal
msc_stream_scrambled = drm_scrambler(msc_stream);
sdc_stream_scrambled = drm_scrambler(sdc_stream);
fac_stream_scrambled = drm_scrambler(fac_stream);

%% partitioning
msc_stream_partitioned = drm_mlc_partitioning(msc_stream_scrambled, 'MSC', MSC);
sdc_stream_partitioned = drm_mlc_partitioning(sdc_stream_scrambled, 'SDC', SDC);
fac_stream_partitioned = drm_mlc_partitioning(fac_stream_scrambled, 'FAC', FAC);

%% encoding
msc_stream_encoded = drm_mlc_encoder(msc_stream_partitioned, 'MSC', MSC);
sdc_stream_encoded = drm_mlc_encoder(sdc_stream_partitioned, 'SDC', SDC);
fac_stream_encoded = drm_mlc_encoder(fac_stream_partitioned, 'FAC', FAC);

%% interleaving
msc_stream_interleaved = drm_mlc_interleaver(msc_stream_encoded, 'MSC', MSC);
sdc_stream_interleaved = drm_mlc_interleaver(sdc_stream_encoded, 'SDC', SDC);
fac_stream_interleaved = drm_mlc_interleaver(fac_stream_encoded, 'FAC', FAC);

%% bit to symbol mapping
msc_stream_mapped = drm_mapping(msc_stream_interleaved, 'MSC', MSC);
sdc_stream_mapped = drm_mapping(sdc_stream_interleaved, 'SDC', SDC);
fac_stream_mapped = drm_mapping(fac_stream_interleaved, 'FAC', FAC);

%% MSC cell interleaving
msc_stream_map_interl = drm_mlc_interleaver(msc_stream_mapped, 'MSC_cells', MSC);

%% build super transmission frame
super_tframe = drm_cell_mapping(msc_stream_map_interl, sdc_stream_mapped, fac_stream_mapped, MSC, SDC, FAC, OFDM);

%% OFDM (complex baseband output)
complex_baseband = drm_ofdm(super_tframe, MSC);

%% clear data
clear msc_stream sdc_stream fac_stream
clear msc_stream_scrambled sdc_stream_scrambled fac_stream_scrambled
clear msc_stream_partitioned sdc_stream_partitioned fac_stream_partitioned
%clear msc_stream_encoded sdc_stream_encoded fac_stream_encoded
%clear msc_stream_interleaved sdc_stream_interleaved fac_stream_interleaved
%clear msc_stream_mapped
%clear msc_stream_map_interl sdc_stream_mapped fac_stream_mapped