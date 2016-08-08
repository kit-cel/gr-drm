function [stream_out] = drm_iofdm(complex_baseband, OFDM)

%% reshape baseband signal ordered by the symbol number

complex_baseband = transpose(reshape(complex_baseband, OFDM.nfft + OFDM.nguard, OFDM.N_S*OFDM.M_TF));

%% remove guard interval

time_domain_noprefix = zeros(OFDM.N_S * OFDM.M_TF, OFDM.nfft);

for i = 1 : OFDM.N_S * OFDM.M_TF
    time_domain_noprefix(i, :) = complex_baseband(i, OFDM.nguard + 1 : end);
end    

%% transform to frequency domain

freq_domain = zeros(OFDM.nfft, OFDM.N_S * OFDM.M_TF);
for i = 1 : OFDM.N_S * OFDM.M_TF
    freq_domain(:, i) = fft(time_domain_noprefix(i, :), OFDM.nfft);
end

stream_out = freq_domain;

end
