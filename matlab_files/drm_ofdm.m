function [stream_out] = drm_ofdm(super_tframe, MSC)
% calculates the complex baseband transmission signal

%% declare variables
N_S = 15;
r_max = MSC.M_TF; % number of transmission frames in a super transmission frame

%% OFDM operation TODO: check for correctness!!!

nfft = 256;
nguard = nfft/4;
OFDM_signal = zeros(r_max * N_S, nfft + nguard);

for i = 1 : r_max * N_S
    OFDM_signal(i, nguard + 1 : end) = ifft(super_tframe(:, i), nfft);
    OFDM_signal(i, 1 : nguard) = OFDM_signal(i, end - nguard + 1 : end);   
end

%% insert guard interval

stream_out = OFDM_signal;
end