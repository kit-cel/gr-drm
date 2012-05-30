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
                
%% build Audio Super Frame with EEP

% Header - contains information to recover the frame lengths of n_frames
% AAC frames stored in the audio super frame

% Distances are coded as 12 bit unsigned int with MSB first (measured in
% bytes from the start of the AAC bitstream sequence.

% test header
header1 = zeros(1, n_borders * 12 + num_padding_b); % padding is implicitly added
header1(1:12) = [0 0 0 0 0 0 0 0 0 0 0 1];
header1(13:24) = [0 0 0 0 0 0 0 0 0 0 1 0];
header1(25:36) = [0 0 0 0 0 0 0 0 0 0 1 1];
header1(37:48) = [0 0 0 0 0 0 0 0 0 1 0 0];
header1(49:60) = [0 0 0 0 0 0 0 0 0 1 0 1];
header1(61:72) = [0 0 0 0 0 0 0 0 0 1 1 0];
header1(73:84) = [0 0 0 0 0 0 0 0 0 1 1 1];
header1(85:96) = [0 0 0 0 0 0 0 0 1 0 0 0];
header1(97:108) = [0 0 0 0 0 0 0 0 1 0 0 1];

header2 = zeros(1, n_borders * 12 + num_padding_b);
header2(1:12) = [0 0 0 0 0 0 0 0 0 0 0 1];
header2(13:24) = [0 0 0 0 0 0 0 0 0 0 1 1];
header2(25:36) = [0 0 0 0 0 0 0 0 0 1 0 1];
header2(37:48) = [0 0 0 0 0 0 0 0 0 1 1 1];
header2(49:60) = [0 0 0 0 0 0 0 0 1 0 0 1];
header2(61:72) = [0 0 0 0 0 0 0 0 1 0 1 1];
header2(73:84) = [0 0 0 0 0 0 0 0 1 1 0 1];
header2(85:96) = [0 0 0 0 0 0 0 0 1 1 1 1];
header2(97:108) = [0 0 0 0 0 0 0 1 0 0 0 1];

header3 = zeros(1, n_borders * 12 + num_padding_b);
header3(1:12) = [0 0 0 0 0 0 0 0 0 0 0 1];
header3(13:24) = [0 0 0 0 0 0 0 0 0 1 0 0];
header3(25:36) = [0 0 0 0 0 0 0 0 0 1 1 1];
header3(37:48) = [0 0 0 0 0 0 0 0 1 0 1 0];
header3(49:60) = [0 0 0 0 0 0 0 0 1 1 0 1];
header3(61:72) = [0 0 0 0 0 0 0 1 0 0 0 0];
header3(73:84) = [0 0 0 0 0 0 0 1 0 0 1 1];
header3(85:96) = [0 0 0 0 0 0 0 1 0 1 1 0];
header3(97:108) = [0 0 0 0 0 0 0 1 1 0 0 1];

% successively append payload and CRC to the header
audio_superframe = [header1; header2; header3];

audio_superframe = [audio_superframe, zeros(3, MSC.L_MUX - length(header1))];
% msc_source_enc = repmat(audio_superframe, MSC.M_TF, 1);
msc_source_enc = audio_superframe;
    










