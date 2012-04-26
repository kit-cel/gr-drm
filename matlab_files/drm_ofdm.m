function [stream_out] = drm_ofdm(super_tframe, MSC)
% calculates the baseband transmission signal

%% declare variables
N_S = 15;
K_min = -103;
K_max = 103;
r_max = MSC.M_TF; % number of transmission frames in a super transmission frame

f_R = 6000000; % arbitrary RF
fs = 24000; % arbitrary sample rate

T_f = 0.4; % duration of one transmission frame
T_SF = r_max * T_f; 
T_u = 21.33 * 10^(-3);
T_g = 5.33 * 10^(-3);
T_S = T_u + T_g;

k_off = - K_min + 1;

t = linspace(0, T_SF, fs * T_SF); % time vector

%% OFDM operation TODO: check for correctness!!!

nfft = 128;
nguard = nfft/4;
OFDM_signal = zeros(r_max * N_S, nfft + nguard);

for i = 1 : r_max * N_S
    OFDM_signal(i, nguard + 1 : end) = ifft(super_tframe(:, i), nfft);
    OFDM_signal(i, 1 : nguard) = OFDM_signal(i, end - nguard + 1 : end);   
end

%% insert guard interval

stream_out = OFDM_signal;
end