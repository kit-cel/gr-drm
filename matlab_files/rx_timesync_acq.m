%% Zeitsynchronisation (Akquisition) eines DRM30-Empfaengers

% Frequenzsynchronisation --> DC bekannt

% Frequenztracking moeglich durch Guard Time Correlation, allerdings ist
% die Tracking Frequency Range um den Dezimationsfaktor kleiner als bei
% einem Frequenzbereichsalgorithmus.

% Guard Time Correlation ermoeglicht auch eine Detektion des RMj

% Signal auf interessanten Bereich filtern (z. B. 10 KHz)


% Guard Time Correlation Experiment
load('signal.mat'); % Sendesignal und weitere Parameter aus Datei laden (RM B, SO 3)

s = complex_baseband; % reines Basisbandsignal ohne Kanaleinfluss

% Sendesignal auf Leistung 1 normieren zur besseren Darstellung
p_s = var(s);
s = s./sqrt(p_s);

max_Ts = .4/15; % maximale OFDM Symboldauer
nsamp_Ts = ceil(fs*max_Ts) % maximale Anzahl Samples pro OFDM Symbol

% Guard Time Correlation

slagged = s(100000:100000+1280); % Ausschnitt aus dem Signal, enthaelt min. 1 Symbol
scorr = xcorr(s(100000:200000), slagged);

Tg_B = 5.33/1000;
nsamp_Tg = ceil(fs*Tg_B)

find(scorr == max(scorr))

plot(abs(scorr));

