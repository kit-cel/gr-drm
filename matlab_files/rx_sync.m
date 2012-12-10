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
nsamp_sym = fs * 0.4; % Samples pro Symbol (400 ms)
delta_t = 1/fs; % Abtastzeit
delta_f = fs/nsamp_sym; % Abstand der Frequenzpunkte der FFT
n_mp = 5; % Anzahl Mehrwege

n_runs = 50; % Anzahl Test-Durchlaeufe
n_failed = 0; % Anzahl fehlerhafter Synchronisationsvorgaenge
for n = 1 : n_runs
    t = 0:delta_t:delta_t*(length(s)-1);
    f_off = randi([-80, 80], [n_mp 1]); % zufaellige Frequenzverschiebung (Vielfache von delta_f)
    t_off = randi([0 200], [n_mp 1]); % zufaellige Zeitverschiebung (Samples, Vielfache von delta_t)
    a = randn([n_mp 1]) + 1i * randn([n_mp 1]);  % komplexe Kanalkoeffizienten (Rayleigh)
    %abs(a);

    r = zeros(length(s), 1); % Speicher vorbelegen
    for i = 1 : n_mp % Zeit-, phasen- und frequenzverschobene, gedaempfte Kopie vom Sendesignal addieren
        r = r + [zeros(t_off(i), 1); a(i) * s(1:end-t_off(i)) .* exp(1i*2*pi*delta_f*f_off(i)*t(1:end-t_off(i))')];  
    end

    % AWGN addieren
    r = awgn(r, 20, 'measured');

    %% Frequenzsynchronisation DRM30

    % Ansatz aus DREAM: Schaetzung der PSD aus FFT ueber ein Symbol, dann
    % Korrelation mit den bekannten Positionen der Frequenzpiloten

    % Energieschaetzung des Eingangssignals

    psd_est = fftshift(abs(fft(r(1000:1000+nsamp_sym), nsamp_sym).^2));
    psd_est = psd_est/max(psd_est); % Normierung auf Maximum
    f_axis = [0 : fs/nsamp_sym : fs - fs/nsamp_sym];

    % Korrelation mit bekannten Pilotpositionen
    freq_pil_index = round([750/delta_f 2250/delta_f 3000/delta_f]);
    freq_pil = zeros(1, ceil(3000/delta_f));
    freq_pil(freq_pil_index) = 1; % entsprechende Bins auf 1 setzen
    [corr, index] = xcorr(psd_est, freq_pil); % Kreuzkorrelation berechnen und fftshift
    dc_index = index(find(corr == max(abs(corr)))); % DC has to be at the maximum of the cross-correlation function

    % Ergebnis ueberpruefen - Der Algorithmus sollte den Pfad mit dem
    % groessten abs(a) gefunden haben

    max_a = find(a == max(a));
    f_max = f_off(max_a) + 1;

    if dc_index == f_max
        disp('freq sync OK');
    else
        disp('freq sync FAILED');
        n_failed = n_failed + 1;
        
        abs(a)
        f_off
        dc_index    
    end
end

success_rate = 1 - n_failed/n_runs % Erfolgsquote


% Ergebnisse plotten
plot_res = 0;

if plot_res == 1
    figure(); 

    subplot(2,1,1); 
    plot(f_axis, psd_est); 
    xlabel('Frequenz in Hz'); ylabel('|R(f)|^2'); title('PSD Schaetzung');


    subplot(2,1,2);
    plot(index, abs(corr));
    xlabel('Index'); ylabel('xcorr'); title('Korrelation der Frequenzpiloten');
end


