function [stream_out] = drm_cell_mapping(msc_stream, sdc_stream, fac_stream, MSC, SDC, FAC, OFDM)
% maps MSC, SDC, FAC, control and pilot cells on a transmission super frame

%% build empty transmission frame
nchan = OFDM.K_max - OFDM.K_min + 1; % carriers ranging from -103 ... +103 (0 is unused)
nsym = OFDM.N_S; % 15 OFDM symbols per transmission frame
frame = zeros(nchan, nsym); % empty transmission frame
k_off = OFDM.k_off; % k is meant as carrier number, not vector index, so 103 (negative half K) is added; +1 because of matlab indexing

%% frequency reference cells (channels correspond to 750 Hz, 2250 Hz and 3000 Hz)

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

%% time reference cells (only in the first OFDM symbol of each transmission frame)

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

%% gain references (periodic every 3 symbols)

% calculate position: k = 1 + 2 * (s mod 3) + 6 * p
gain_pos = cell(nsym,1);
i = 1;
for s = 0 : 2  
    for p = -20 : 1 : 20
        k = 1 + 2 * mod(s, 3) + 6 * p;
        if k >= -103 && k <= 103
            gain_pos{i} = [gain_pos{i}, k + k_off];
            % continue periodically
            gain_pos{i+3} = gain_pos{i};
            gain_pos{i+6} = gain_pos{i};
            gain_pos{i+9} = gain_pos{i};
            gain_pos{i+12} = gain_pos{i};
        end
    end
    i = i + 1;
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
        p = (gain_pos{i}(l) - k_0 - n*x)/(x*y);  
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
                frame(gain_pos{i}(l) ,i) = a_gain * frame(gain_pos{i}(l));
            end
        end
    end
end

%% combine 3 transmission frames to a super transmission frame

% Pilot cells are repeated every transmission frame
superframe = repmat(frame, 1, MSC.M_TF);

%% FAC cell mapping

% 1st column: symbol index; 2nd column: carrier indexes k
fac_pos = cell(12, 2); 

% symbol index (zero-based)
for i = 1:12
    fac_pos{i,1} = i + 2; 
end

% carrier indexes k
fac_pos{1,2} = [13 25 43 55 67] + k_off; 
fac_pos{2,2} = [15 27 45 57 69] + k_off;
fac_pos{3,2} = [17 29 47 59 71] + k_off;
fac_pos{4,2} = [19 31 49 61 73] + k_off;
fac_pos{5,2} = [9 21 33 51 63 75] + k_off;
fac_pos{6,2} = [11 23 35 53 65 77] + k_off;
fac_pos{7,2} = [13 25 37 55 67 79] + k_off;
fac_pos{8,2} = [15 27 39 57 69 81] + k_off;
fac_pos{9,2} = [17 29 41 59 71 83] + k_off;
fac_pos{10,2} = [19 31 43 61 73] + k_off; 
fac_pos{11,2} = [21 33 45 63 75] + k_off;
fac_pos{12,2} = [23 35 47 65 77] + k_off;

% first/intermediate/last FAC block is inserted
for l = 0:2 
    sym_off = l*OFDM.N_S;
    n = 0;
    for i = 1:12   
        for k = 1:length(fac_pos{i, 2})
            if superframe(fac_pos{i, 2}(k), fac_pos{i, 1} + sym_off) ~= 0
                error('FAC cell mapping: overwriting non-empty cell');
            else
                n = n + 1;
                superframe(fac_pos{i, 2}(k), fac_pos{i, 1} + sym_off) = fac_stream(l+1, n);
            end
        end
    end
end


%% SDC 
% only at the beginning of each transmission super frame
% all the cells in the first 2 symbols that are not gain, time or frequency references

% find indexes of positions that are still zero, 0 (=> 0 + k_off) has to be
% omitted later on

pos_SDC = cell(1, 2);
pos_SDC{1} = find(frame(:, 1) == 0);
pos_SDC{2} = find(frame(:, 2) == 0);

n = 0;
for i = 1 : 2
    for l = 1 : length(pos_SDC{i})
        if pos_SDC{i}(l) ~= k_off % the DC carrier is omitted
            if superframe(pos_SDC{i}(l), i) ~= 0
                error('SDC cell mapping: overwriting non-empty cell');
            else
                n = n + 1;
                superframe(pos_SDC{i}(l), i) = sdc_stream(n);
            end
        end
    end
end

%% MSC

% add dummy symbols

msc_stream_sf = [];
for i = 1 : MSC.M_TF
    msc_stream_sf = [msc_stream_sf, msc_stream(i, :)];
end
a_16 = 1/sqrt(10);
msc_stream_sf = [msc_stream_sf, a_16.*[1 + 1i, 1 - 1i] ];
msc_stream = msc_stream_sf;

pos_MSC = cell(1, nsym * MSC.M_TF);
for i = 1 : length(pos_MSC)
    pos_MSC{i} = find(superframe(:, i) == 0);
end

n = 0;
for i = 1 : length(pos_MSC)
    for l = 1 : length(pos_MSC{i})
        if pos_MSC{i}(l) ~= k_off % the DC carrier is omitted
            if superframe(pos_MSC{i}(l), i) ~= 0
                error('MSC cell mapping: overwriting non-empty cell');
            else
                n = n + 1;
                superframe(pos_MSC{i}(l), i) = msc_stream(n);
            end
        end
    end
end


%% add zeros for unused channels
% DC is at index nfft/2

nfft = OFDM.nfft;
zero_chan_up = ceil((nfft - nchan)/2);
zero_chan_down = floor((nfft - nchan)/2);
superframe_padded = [zeros(zero_chan_down, nsym*MSC.M_TF); superframe; zeros(zero_chan_up, nsym*MSC.M_TF)];

stream_out = superframe_padded;
end