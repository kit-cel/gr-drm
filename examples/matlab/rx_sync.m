%% Frequenz- und Zeitsynchronisation eines DRM30-Empfaengers

clear all
close all
clc

%% Vorbereitung der komplexen Basebandsamples

load('signal.mat'); % Sendesignal und weitere Parameter aus Datei laden (RM B, SO 3)

s = complex_baseband; % reines Basisbandsignal ohne Kanaleinfluss

% Sendesignal auf Leistung 1 normieren zur besseren Darstellung
p_s = var(s);
s = s./sqrt(p_s); 

% TODO: Kanal simulieren (Daempfung, Phasenverschiebung, Frequenzverschiebung,
% Doppler, Mehrwege...)

% AWGN addieren
r = awgn(s, 100, 'measured');

%% Frequenzsynchronisation

% Schaetzung durch FFT. FFT-Laenge entsprechend der maximal moeglichen
% Untertraegeranzahl.

nfft = 2^13; % 460 Traeger maximal moeglich (RM A, SO 5)
nsamp_sym = 48e3 * 0.4; % 48e3*400ms
r_w = r(1:nsamp_sym); % gefenstertes Empfangssignal (evtl. Fensterfkt. verwenden)
%f = 0:fs/nfft:fs - fs/nfft; % Frequenzachse
R_w = abs(fft(r_w, nfft));
plot(R_w); %xlabel('Frequenz in Hz'); ylabel('|R(f)|');

% Extrahieren der 3 groessten Spektrallinien
freq_pil_index = [0 0 0];
for i = 1 : 3
    freq_pil_index(i) = find(R_w == max(R_w));
    R_w(freq_pil_index(i)) = 0;
end
freq_pil_index = sort(freq_pil_index);
freq_pil_f = freq_pil_index*fs/nfft;
freq_pil_f(3) - freq_pil_f(2)
freq_pil_f(2) - freq_pil_f(1)


