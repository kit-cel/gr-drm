function [msc_stream_map_interl, sdc_stream_mapped, fac_stream_mapped] = drm_cell_demapping(super_tframe, MSC, SDC, FAC, OFDM)
% performs the demapping of the qam cells from the super transmission frame
% for each channel

% load indexes for MSC and SDC from file, brings in pos_SDC and pos_MSC
load pos_cells

%% remove empty channels at both sides
nchan = OFDM.K_max - OFDM.K_min + 1;
nfft = OFDM.nfft;
zero_chan_down = ceil((nfft - nchan)/2);

super_tframe = super_tframe(zero_chan_down + 1 : nchan + zero_chan_down, :);

%% FAC cell demapping

% 1st column: symbol index; 2nd column: carrier indexes k
fac_pos = cell(12, 2); 

% symbol index (zero-based)
for i = 1:12
    fac_pos{i,1} = i + 2; 
end

% carrier indexes k
k_off = OFDM.k_off;
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

n_fac = 0;
for i = 1 : 12
    n_fac = n_fac + length(fac_pos{i, 2});
end

if n_fac ~= FAC.N_FAC
    [n_fac FAC.N_FAC]
    error('FAC cells mismatch')
end

fac_stream_mapped = zeros(3, FAC.N_FAC);
for l = 0:2
    sym_off = l*OFDM.N_S;
    n = 0;
    for i = 1 : 12
        for k = 1 : length(fac_pos{i, 2})
            n = n + 1;
            fac_stream_mapped(l+1, n) = super_tframe(fac_pos{i, 2}(k), fac_pos{i, 1} + sym_off);
        end
    end
end


%% SDC cell demapping
% the receiver knows the structure and positions of the pilots. All cells
% in the first 2 OFDM symbols but the reference cells belong to SDC.

% indexes of free cells are loaded from mat-file

sdc_stream_mapped = zeros(1, SDC.N_SDC);
n = 0;
for i = 1 : 2
    for l = 1 : length(pos_SDC{i})
        if pos_SDC{i}(l) ~= k_off
            n = n + 1;
            sdc_stream_mapped(n) = super_tframe(pos_SDC{i}(l), i);
        end
    end
end


%% MSC cell demapping

msc_stream_map_interl = zeros(1, MSC.N_SFA);

n = 0;
for i = 1 : length(pos_MSC)
    for l = 1 : length(pos_MSC{i})
        if pos_MSC{i}(l) ~= k_off % the DC carrier is omitted
            n = n + 1;
            msc_stream_map_interl(n) = super_tframe(pos_MSC{i}(l), i);
        end
    end
end

% cut away the dummy symbols
msc_stream_map_interl = msc_stream_map_interl(1: end - MSC.N_L);

% break down into a matrix of M_TF rows and N_MUX cells
% msc_cells = cell(MSC.M_TF, 1);
% for i = 1 : MSC.M_TF
%     msc_cells{i} = msc_stream_map_interl((i-1)*MSC.N_MUX + 1 : i*MSC.N_MUX);
% end
% 
% msc_stream_map_interl = msc_cells; % rename for output


msc_stream_map_interl = [msc_stream_map_interl(1 : MSC.N_MUX);...
                         msc_stream_map_interl(MSC.N_MUX+1 : 2*MSC.N_MUX);...
                         msc_stream_map_interl(2*MSC.N_MUX+1 : end)];


end