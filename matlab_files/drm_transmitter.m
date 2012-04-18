%% DRM transmitter

clear all

clc


%% calculate global variables, for the list of assumptions see drm_global_variables.m
run drm_global_variables

%% create random bits (TODO: fill them with real data)
msc_stream = randint(1, MSC.L_MUX); % one MSC multiplex frame
sdc_stream = randint(1, SDC.L_SDC); % one SDC block
fac_stream = randint(1, FAC.L_FAC); % one FAC block

%% energy dispersal
msc_stream_scrambled = drm_scrambler(msc_stream);
sdc_stream_scrambled = drm_scrambler(sdc_stream);
fac_stream_scrambled = drm_scrambler(fac_stream);

%% partitioning
msc_stream_partitioned = drm_mlc_partitioning(msc_stream, 'MSC', MSC);
sdc_stream_partitioned = drm_mlc_partitioning(sdc_stream, 'SDC', SDC);
fac_stream_partitioned = drm_mlc_partitioning(fac_stream, 'FAC', FAC);

%% encoding

%% mapping

%% OFDM, guard interval etc.