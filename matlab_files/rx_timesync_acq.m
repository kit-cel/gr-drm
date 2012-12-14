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

Ts = .4/15; % OFDM Symboldauer RM B
nsamp_Ts = ceil(fs*Ts); % Anzahl Samples pro OFDM Symbol
Tg = 5.33/1000; % CP Dauer RM B
nsamp_Tg = ceil(fs*Tg); % Anzahl Samples im CP

% Guard Time Correlation

avg_sym = 10; % Anzahl der Symbole, ueber die gemittelt wird
decim_factor = 5; % Dezimationsfaktor fuer die Korrelation
lag = 300 % Verschiebung vom Anfang des Symbols in Samples (unabh. von decim_factor)

s = awgn(s, 5, 'measured'); % AWGN addieren

corrvec = zeros(1, ceil(nsamp_Ts/decim_factor)); % Vektor fuer die Korrelationswerte
corrlen = length(corrvec);
for start = 0 : decim_factor : avg_sym*nsamp_Ts-1
    corrvec(mod(ceil(start/decim_factor),length(corrvec))+1) = ... % Korrelation berechnen und zum vorhandenen Werte addieren
        corrvec(mod(ceil(start/decim_factor),corrlen)+1) + ...
        corr(s(start+lag+1:start+lag+nsamp_Tg+1), s(start+lag+1+nsamp_Ts-nsamp_Tg:start+lag+nsamp_Ts+1));
end
corrvec = corrvec/avg_sym; % Mittelung anwenden

lag_est = (corrlen - find(corrvec == max(corrvec)))*decim_factor

plot(abs(corrvec)); title('Guard Intervall Korrelation'); 
xlabel('Samples'); ylabel('Korrelationskoeffizient');
