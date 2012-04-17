function [streams_out] = drm_mlc_partitioning(streams_in, M, mapping, error_protection)

% TODO: throw error if input is invalid, create global variables before

N_mux = 2337; % assuming robustness mode B and spectrum occupancy 3 (10 kHz)
N_1 = -1;
N_2 = -1;

switch  error_protection
    case 'EEP'
        N_2 = N_mux;
        N_1 = 0;
    case 'UEP'
        % calculate N_1 for UEP(p. 112)
        N_2 = N_mux - N_1;
end



if M == 4 || M == 16
    % SM is used
    switch M
        case 4
            streams_out = streams_in;
        case 16
            % blub
    end

else
    % SM, HMsym or HMmix is possible
    switch mapping
        case 'SM'
            % SM
        case 'HMsym'
            % HMsym
        case 'HMmix'
            % HMmix
    end
    
end
    
