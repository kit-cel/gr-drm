function [fac] = drm_generate_fac(FAC)
% generates a valid fac block (RM B, SO 3) containing of first,
% intermediate and last block. First block has valid AFS flag.

% preallocate empty fac data
fac = zeros(3, FAC.L_FAC);

% channel parameters (20 bit)
for i = 1:3
    fac(i, 1) = 0; % base/enhancement layer (base)
    switch i 
        case 1 % first fac block of super transmission frame, SDC AFS index is valid
            fac(i, 2:3) = [0 0];
        case 2 % intermediate fac block
            fac(i, 2:3) = [0 1];
        case 3 % last fac block
            fac(i, 2:3) = [1 0];
    end
    fac(i, 4) = 0; % robustness mode (A-D)
    fac(i, 5:7) = transpose([0 1 1]); % spectrum occupancy (3 -> 10 kHz)
    fac(i, 8) = 1; % time interleaver depth (400 ms)
    fac(i, 9:10) = [1 1]; % MSC Mode (16-QAM, no hierachical)
    fac(i, 11) = 1; % SDC Mode (4-QAM, R=0.5)
    fac(i, 12:15) = [0 1 0 0]; % Number of services (1 audio service)
    fac(i, 16:18) = [0 0 0]; % Reconfiguration index (zero means no reconfiguration)
    fac(i, 19) = 0; % Toggle flag (set to zero for robustness mode A-D)
    fac(i, 20) = 0; % rfu (set to zero until defined)

    % service parameters (44 bit)
    fac(i, 21:44) = [1 0 1 0 1 0 1 0 1 0 1 0 ...
                     1 0 1 0 1 0 1 0 1 0 1 0]; % unique service identifier (arbitrarily chosen)
    fac(i, 45:46) = [0 0]; % short ID (arbitrarily chosen)
    fac(i, 47) = 0; % audio CA indication (no)
    fac(i, 48:51) = [0 1 1 1]; % language (german)
    fac(i, 52) = 0; % Audio/Data flag (audio)
    fac(i, 53:57) = [0 1 0 1 0]; % service descriptor (Pop music)
    fac(i, 58) = 0; % data CA indication (no data stream present)
    fac(i, 59:64) = [0 0 0 0 0 0]; % rfu (set to zero until defined)

    % add crc word (G(x) = x^8 + x^4 + x^3 + x^2 + 1)
    crc_gen = crc.generator('Polynomial', [1 0 0 0 1 1 1 0 1],...
                            'InitialState', [1 1 1 1 1 1 1 1],...
                            'FinalXOR', [1 1 1 1 1 1 1 1]);
    generate(crc_gen, fac(i, 1:64));
    fac(i, :) = generate(crc_gen, transpose(fac(i, 1:64)));
end

end