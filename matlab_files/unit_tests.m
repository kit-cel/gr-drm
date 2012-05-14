clear all
clc

fprintf('START OF UNIT TESTS \n \n')
fprintf('The tests have to be executed with ''testfile.aac'' (contains one superframe).\n\n')
fprintf('VARIOUS TESTS \n')

%% Scrambler
n_total = 1;
n_failed = 0;

fprintf('Test Scrambler...');

ref_prbs = [0 0 0 0 0 1 1 1 1 0 1 1 1 1 1 0]; % see DRM standard ('energy dispersal')

bits_in = randint(1, 16);

ref_out = mod(bits_in + ref_prbs, 2);

bits_out = drm_scrambler(bits_in);

if isequal(bits_out, ref_out)
    fprintf( ' passed. \n' )
else
    fprintf( ' failed! \n' )
    n_failed = n_failed + 1;
end

clear ref_prbs bits_in ref_out bits_out

%% Interleaver index generator
n_total = n_total + 1;

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
    n_failed = n_failed + 1;
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

clear failed MSC SDC FAC indexes length_indexes not_unique too_big too_small

%% Mapping
n_total = n_total + 1;

fprintf('Test Mapping...');

failed = 1;

% normalization factors
a_4 = 1/sqrt(2);
a_16 = 1/sqrt(10);

% example MSC struct
MSC = struct('N_MUX', 16);
ref_in = [0 0 0 0 0 1 0 1 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 1 1 0 1 0 1 1 1 1 ; ...
          0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 ];
ref_out = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
                 1 + 3i, 1 - 1i, 1 + 1i, 1 - 3i, -3 + 3i, -3 - 1i, -3 + 1i, -3 - 3i];

stream_out = drm_mapping(ref_in, 'MSC', MSC);

if isequal(ref_out, stream_out)
    failed = 0;
end

% example FAC struct
FAC = struct('N_FAC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'FAC', FAC);

if isequal(ref_out, stream_out)
    failed = 0;
end

% example SDC struct
SDC = struct('N_SDC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'SDC', SDC);

if isequal(ref_out, stream_out)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' failed! \n\n')
else
    fprintf(' passed. \n\n')
end

clear stream_out ref_in ref_out SDC FAC MSC a_16 a_4

%% Receiver tests start here
fprintf('RECEIVER TESTS\n')

%% OFDM
n_total = n_total + 1;

run drm_transmitter % this realisation is also used for following tests

super_tframe_rx = drm_iofdm(complex_baseband, OFDM);

fprintf('Test OFDM/iOFDM...');

failed = 1;

s = warning('off', 'drm:transmitter');

% this is a very unprecise test because equality can't be exactly tested du
% to little differences after FFT
if isequal(round(super_tframe), round(super_tframe_rx))
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

%% Reference cells

fprintf('Test Reference cells...');

% build empty transmission frame
nchan = OFDM.K_max - OFDM.K_min + 1; % carriers ranging from -103 ... +103 (0 is unused)
nsym = OFDM.N_S; % 15 OFDM symbols per transmission frame
frame = zeros(nchan, nsym); % empty transmission frame
k_off = OFDM.k_off; % k is meant as carrier number, not vector index, so 103 (negative half K) is added; +1 because of matlab indexing

%% frequency pilot cells

freq_pos = [16 48 64] + k_off; % channel position of the cell
freq_phase = [331 651 555]; % phase of the cell
a_freq = sqrt(2); % amplitude of the cell

for i = 1:length(freq_phase)
    freq_phase(i) = exp(1i * 2*pi * freq_phase(i) / 1024);
end

for i = 1:nsym
    for k = 1 : length(freq_pos)
        if frame(freq_pos(k), i) == 0
            frame(freq_pos(k), i) = a_freq * freq_phase(k);
        else
            warning('cell is already occupied')
            disp(frame(freq_pos(k), i))
        end
    end
end

%% time reference cells

time_pos = [14 16 18 20 24 26 32 36 42 44 48 49 50 54 56 62 64 66 68] + k_off;
time_phase = [304 331 108 620 192 704 44 432 588 844 651 651 651 460 460 944 555 940 428];
a_time = sqrt(2);

for i = 1:length(time_phase)
    time_phase(i) = exp(1i * 2*pi * time_phase(i) / 1024);
end

for k = 1 : length(time_pos)
    if frame(time_pos(k), 1) == 0 || frame(time_pos(k), 1) == a_time * time_phase(k)
        frame(time_pos(k), 1) = a_time * time_phase(k);
    else
        error('trying to overwrite cell with different content')
    end
end

%% gain reference cells

% calculate position: k = 1 + 2 * (s mod 3) + 6 * p
gain_pos = cell(nsym,1);
for s = 0 : nsym-1  
    for p = -20 : 1 : 20
        k = 1 + 2 * mod(s, 3) + 6 * p;
        if k >= -103 && k <= 103
            gain_pos{s+1} = [gain_pos{s+1}, k + k_off];
        end
    end

end

% calculate phase index
x = 2;
y = 3;
k_0 = 1;
s = 0:nsym-1;

W_1024 = [512 0 512 0 512; 0 512 0 512 0; 512 0 512 0 512];
Z_256 = [0 57 164 64 12; 168 255 161 106 118; 25 232 132 233 38];
Q_1024 = 12;

gain_phase = cell(nsym,1);

for i = 1 : nsym
    gain_phase{i} = cell(1,length(gain_pos{i}));
    for l = 1 : length(gain_phase{i})
        n = mod(s(i), y) + 1; % + 1 comes from matlab indexing
        m = floor(s(i)/y) + 1;
        p = (gain_pos{i}(l) - k_off - k_0 - n*x)/(x*y);  
        v_1024 = mod(4*Z_256(n,m) + p*W_1024(n,m) + p^2 * (1 + s(i)) * Q_1024, 1024);
        gain_phase{i}{l} = exp(1i * 2*pi * v_1024 / 1024);
    end
end

% map the reference symbols to the transmission frame
a_gain = sqrt(2);
for i = 1 : nsym
    for l = 1 : length(gain_pos{i})
        k = gain_pos{i}(l);
        if frame(gain_pos{i}(l) ,i) == 0
            % check if it's a cell to be over-boosted
            if k == -103 + k_off || k == -101 + k_off || k == 101 + k_off || k == 103 + k_off
                frame(gain_pos{i}(l) ,i) = a_gain^2 * gain_phase{i}{l};
            else
                frame(gain_pos{i}(l) ,i) = a_gain * gain_phase{i}{l};
            end
        else
            if k == -103 + k_off || k == -101 + k_off || k == 101 + k_off || k == 103 + k_off
                % boost time or frequency reference cell but don't change phase
                frame(gain_pos{i}(l) ,i) = a_gain * frame(gain_pos{i}(l), i);
            end
        end
    end
end

% as the pilot structure is repeated in every transmission frame, a super
% transmission frame can be built by appending transmission frames.
pilot_superframe = repmat(frame, 1, 3);

% test gain cells
failed = 0;
n_total = n_total + 1;
for k = 1:nsym
    for n = 1:length(gain_pos{k})
        if abs(pilot_superframe(gain_pos{k}(n), k) - super_tframe_rx(gain_pos{k}(n) + 409, k)) > 0.1
            failed = 1;
        end
    end
end

if ~failed
    fprintf(' Gain pilots passed.')
else
    n_failed = n_failed + 1;
    fprintf(' Gain pilots failed!')
end        

% test time cells
failed = 0;
n_total = n_total + 1;
for k = 1:length(time_pos)
    for n = 1:nsym:nsym*MSC.M_TF
        if abs(pilot_superframe(time_pos(k), n) - super_tframe_rx(time_pos(k) + 409, n)) > 0.01
            failed = 1;
        end
    end
end

if ~failed
    fprintf(' Frequency pilots passed.')
else
    n_failed = n_failed + 1;
    fprintf(' Frequency pilots failed!')
end

% test frequency cells
failed = 0;
n_total = n_total + 1;
for k = 1:length(freq_pos)
    for n = 1:nsym*MSC.M_TF
        if abs(pilot_superframe(freq_pos(k), n) - super_tframe_rx(freq_pos(k) + 409, n)) > 0.01
            failed = 1;
        end
    end
end

if ~failed
    fprintf(' Time pilots passed. \n')
else
    n_failed = n_failed + 1;
    fprintf(' Time pilots failed! \n')
end

%% Cell demapping

fprintf('Test Cell demapping...');

[msc_stream_map_interl_rx sdc_stream_mapped_rx fac_stream_mapped_rx] = drm_cell_demapping(super_tframe, MSC, SDC, FAC, OFDM);

% FAC
n_total = n_total + 1;
failed = 1;

if isequal(fac_stream_mapped_rx, fac_stream_mapped)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' FAC failed! ')
else
    fprintf(' FAC passed. ')
end

% SDC
n_total = n_total + 1;
failed = 1;

if isequal(sdc_stream_mapped_rx, sdc_stream_mapped)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' SDC failed! ')
else
    fprintf(' SDC passed. ')
end

% MSC
n_total = n_total + 1;
failed = 1;

if isequal(msc_stream_map_interl_rx, msc_stream_map_interl)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' MSC failed! \n')
else
    fprintf(' MSC passed. \n')
end

%% MSC cell deinterleaving
fprintf('Test MSC Cell deinterleaving...');

n_total = n_total + 1;
failed = 1;

msc_stream_mapped_rx = drm_mlc_deinterleaver(msc_stream_map_interl_rx, 'MSC_cells', MSC);

%msc_stream_mapped = repmat(msc_stream_mapped, 3, 1);

if isequal(round(msc_stream_mapped_rx), round(msc_stream_mapped))
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

%% Demapping
fprintf('Test Symbol Demapping...');

% MSC
n_total = n_total + 1;
failed = 1;

msc_stream_interl_rx = drm_demapping(msc_stream_mapped_rx, 'MSC', MSC);

if isequal(msc_stream_interl_rx, msc_stream_interleaved)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' MSC failed!')
else
    fprintf(' MSC passed.')
end

% SDC
n_total = n_total + 1;
failed = 1; 

sdc_stream_interl_rx = drm_demapping(sdc_stream_mapped_rx, 'SDC', SDC);

if isequal(sdc_stream_interl_rx, sdc_stream_interleaved)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' SDC failed!')
else
    fprintf(' SDC passed.')
end

% FAC
n_total = n_total + 1;
failed = 1; 

fac_stream_interl_rx = drm_demapping(fac_stream_mapped_rx, 'FAC', FAC);

if isequal(fac_stream_interl_rx, fac_stream_interleaved)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' FAC failed! \n')
else
    fprintf(' FAC passed. \n')
end

%% Deinterleaver
fprintf('Test Bit Deinterleaving...');

% MSC
n_total = n_total + 1;
failed = 1;

msc_stream_deinterl_rx = drm_mlc_deinterleaver(msc_stream_interl_rx, 'MSC', MSC);

if isequal(msc_stream_deinterl_rx, msc_stream_encoded)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' MSC failed!')
else
    fprintf(' MSC passed.')
end

% SDC
n_total = n_total + 1;
failed = 1;

sdc_stream_deinterl_rx = drm_mlc_deinterleaver(sdc_stream_interl_rx, 'SDC', SDC);

if isequal(sdc_stream_deinterl_rx, sdc_stream_encoded)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' SDC failed!')
else
    fprintf(' SDC passed.')
end

% FAC
n_total = n_total + 1;
failed = 1;

fac_stream_deinterl_rx = drm_mlc_deinterleaver(fac_stream_interl_rx, 'FAC', FAC);

if isequal(fac_stream_deinterl_rx, fac_stream_encoded)
    failed = 0;
end

if failed
    n_failed = n_failed + 1;
    fprintf(' FAC failed! \n')
else
    fprintf(' FAC passed. \n')
end

%% Viterbi Decoder


%% End of unit tests
fprintf('\nTOTAL: %d / %d Tests failed. \n', n_failed, n_total);
