function [msc_source_enc] = drm_source_enc(MSC, SDC)
% AAC source encoding (see 5.3.1 in the standard)
% currently, only random bits are inserted as payload!

%% Declare configuration dependant variables
%  NOTE: b stands for bit, B for byte
n_frames = 10; % number of AAC audio frames (-> one audio super frame)
n_borders = n_frames - 1; % number of frame borders
num_total_b = MSC.L_MUX; % total number of MSC bits in one transmission frame (which contains one super audio frame + overhead)
num_higher_prot_B = SDC.L_A; % number of higher protected bytes (-> SDC)
num_padding_b = 4; % padding bits for byte alignment at the end of the header
num_lower_prot_B = (num_total_b - num_higher_prot_B*8 - num_padding_b - n_borders*12)/8; % number of lower protected bytes (-> SDC)
audio_payl_len_b = (num_higher_prot_B + num_lower_prot_B)*8 ...
                    - num_padding_b - n_borders*12;
                
%% build Audio Super Frame with UEP

% Header - contains information to recover the frame lengths of n_frames
% AAC frames stored in the audio super frame

% Distances are coded as 12 bit unsigned int with MSB first (measured in
% bytes from the start of the AAC bitstream sequence.

header = zeros(1, n_borders * 12);
header(1 : (n_borders-1) * 12) = repmat([0 0 0 0 0 1 0 1 0 1 0 0], 1, n_borders-1); % 84 bytes FIXME
header((n_borders-1)*12 + 1 : end) = [0 0 0 0 0 1 0 1 0 1 0 0]; % 84 bytes FIXME
header = [header, zeros(1, num_padding_b)]; % add padding bits because n_frames == 10

% successively append payload and CRC to the header
audio_superframe = header;

audio_superframe = [audio_superframe, zeros(1, MSC.L_MUX - length(audio_superframe))];
msc_source_enc = repmat(audio_superframe, 1, MSC.M_TF);
    










