clear all
clc

fprintf('VARIOUS TESTS... \n')
%% Scrambler

fprintf('Test Scrambler...');

ref_prbs = [0 0 0 0 0 1 1 1 1 0 1 1 1 1 1 0]; % see DRM standard ('energy dispersal')

bits_in = randint(1, 16);

ref_out = mod(bits_in + ref_prbs, 2);

bits_out = drm_scrambler(bits_in);

if bits_out == ref_out
    fprintf( ' passed. \n' )
else
    fprintf( ' failed! \n' )
end

clear ref_prbs bits_in ref_out bits_out

%% Interleaver index generator

fprintf('Test Index Generator...');

failed = 0;

% check MSC
MSC = struct('N_1', 396, 'N_2', 1941, 'N_MUX', 2337); % example MSC struct
indexes = drm_mlc_permutation('MSC', MSC);

length_indexes = length(indexes(1, :));

if length_indexes ~= 2 * (MSC.N_1 + MSC.N_2)
    warning('index vector length mismatch')
    failed = 1;
end
    

for k = 1 : 2
    for m = 1 : length_indexes
        % all indexes have to be unique and between 1 and 2 * N
        if length(find(indexes(k, :) == m)) ~= 1 || indexes(k, m) > 2 * (MSC.N_1 + MSC.N_2) || indexes(k, m) <= 0
            disp(indexes(k, m)); disp(m);
            failed = 1;
        end
    end
end

% check SDC
SDC = struct('N_SDC', 322);
indexes = drm_mlc_permutation('SDC', SDC);
length_indexes = length(indexes);

if length_indexes ~= 2 * SDC.N_SDC
    warning('index vector length mismatch')
    failed = 1;
end

for k = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == k)) > 1 || indexes(k) > 2 * SDC.N_SDC || indexes(k) <= 0
        disp(indexes(k))
        failed = 1;
    end
end

% check FAC
FAC = struct('N_FAC', 65);
indexes = drm_mlc_permutation('FAC', FAC);
length_indexes = length(indexes);

if length_indexes ~= 2 * FAC.N_FAC
    warning('index vector length mismatch')
    failed = 1;
end

not_unique = 0;
too_big = 0;
too_small = 0;

for k = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == k)) > 1
        warning('index not unique') 
        disp(k); disp(find(indexes == k));
        failed = 1;
        not_unique = 1;
    end
    if indexes(k) > 2 * FAC.N_FAC
        warning('index exceeds vector range')   
        too_big = 1;
        disp(indexes(k))
    end
    if indexes(k) <= 0
        warning('index is <= 0')
        too_small = 1;
        disp(k); disp(indexes(k))    
    end
    if length(find(indexes == k)) == 0
        fprintf('missing index'); disp(k);
    end
                
end

% check MSC cell interleaving
indexes = drm_mlc_permutation('MSC_cells', MSC);
length_indexes = length(indexes);

if length_indexes ~= MSC.N_MUX
    warning('index vector length mismatch')
    failed = 1;
end

for k = 1 : length_indexes
    % all indexes have to be unique and between 1 and N
    if length(find(indexes == k)) > 1 || indexes(k) > MSC.N_MUX || indexes(k) <= 0
        disp(indexes(k))
        failed = 1;
    end
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

clear failed MSC SDC FAC indexes length_indexes not_unique too_big too_small

%% Mapping

fprintf('Test Mapping...');

failed = 0;

% normalization factors
a_4 = 1/sqrt(2);
a_16 = 1/sqrt(10);

% example MSC struct
MSC = struct('N_MUX', 16);
ref_in = [0 0 0 0 0 1 0 1 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 1 1 0 1 0 1 1 1 1 ; ...
          0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 ];
ref_out = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
                 1 + 3i, 1 - 1i, 3 + 1i, 3 - 3i, -3 + 3i, -3 - 1i, -3 + 1i, -3 - 3i];

stream_out = drm_mapping(ref_in, 'MSC', MSC);

if ref_out ~= stream_out
    failed = 1;
end

% example FAC struct
FAC = struct('N_FAC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'FAC', FAC);

% example SDC struct
SDC = struct('N_SDC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'SDC', SDC);

if ref_out ~= stream_out
    failed = 1;
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

clear stream_out ref_in ref_out SDC FAC MSC a_16 a_4

%% Receiver tests start here
fprintf('RECEIVER TESTS...\n')

%% OFDM

run drm_transmitter % this realisation is also used for following tests

super_tframe_recv = drm_iofdm(complex_baseband, OFDM);

fprintf('Test OFDM/iOFDM...');

failed = 0;

s = warning('off', 'drm:transmitter');



if super_tframe ~= super_tframe_recv
    failed = 1;
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

%% Cell demapping

fprintf('Test Cell demapping...');

[msc_stream_map_interl_rx sdc_stream_mapped_rx fac_stream_mapped_rx] = drm_cell_demapping(super_tframe, MSC, SDC, FAC, OFDM);

% FAC
failed = 0;

if fac_stream_mapped_rx ~= fac_stream_mapped
    failed = 1;
end

if failed
    fprintf(' FAC failed! ')
else
    fprintf(' FAC passed. ')
end

% SDC
failed = 0;

if sdc_stream_mapped_rx ~= sdc_stream_mapped
    failed = 1;
end

if failed
    fprintf(' SDC failed! ')
else
    fprintf(' SDC passed. ')
end

% MSC
failed = 0;

if msc_stream_map_interl_rx ~= repmat(msc_stream_map_interl, 3, 1)
    failed = 1;
end

if failed
    fprintf(' MSC failed! \n')
else
    fprintf(' MSC passed. \n')
end

%% MSC cell deinterleaving
fprintf('Test MSC Cell deinterleaving...');

failed = 0;

msc_stream_mapped_rx = drm_mlc_deinterleaver(repmat(msc_stream_map_interl, 3, 1), 'MSC_cells', MSC);

msc_stream_mapped = repmat(msc_stream_mapped, 3, 1);

if msc_stream_mapped_rx ~= msc_stream_mapped
    failed = 1;
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

%% Demapping
fprintf('Test Symbol Demapping...');

% MSC
failed = 0;

% msc_stream_interl_rx = drm_demapping(msc_stream_mapped, 'MSC', MSC);
% 
% if msc_stream_interl_rx ~= msc_stream_interl
%     failed = 1;
% end
% 
% if failed
%     fprintf(' MSC failed!')
% else
%     fprintf(' MSC passed.')
% end

% SDC
failed = 0; 

sdc_stream_interl_rx = drm_demapping(sdc_stream_mapped, 'SDC', SDC);

if sdc_stream_interl_rx ~= sdc_stream_interleaved
    failed = 1;
end

if failed
    fprintf(' SDC failed!')
else
    fprintf(' SDC passed.')
end

% FAC
failed = 0; 

fac_stream_interl_rx = drm_demapping(fac_stream_mapped, 'FAC', FAC);

if fac_stream_interl_rx ~= fac_stream_interleaved
    failed = 1;
end

if failed
    fprintf(' FAC failed! \n')
else
    fprintf(' FAC passed. \n')
end