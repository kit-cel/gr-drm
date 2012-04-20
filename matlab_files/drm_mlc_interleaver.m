function [stream_out] = drm_mlc_interleaver(stream_in, channel_type, channel_params)

switch channel_type
    case 'MSC'
        % MSC interleaving
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC', MSC);
        stream_out = zeros(2, 2 * (MSC.N_1 + MSC.N_2) ) - 1; % minus one for error checking
        for k = 1 : 2
            for i = 1 : 2 * (MSC.N_1 + MSC.N_2)
                stream_out(k, i) = stream_in(k, p_ind(k, i));
            end
        end

    case 'SDC'
        % SDC interleaving
        SDC = channel_params;
        p_ind = drm_mlc_permutation('SDC', SDC);
        stream_out = zeros(1, 2 * SDC.N_SDC) - 1; % minus one for error checking
        for i = 1 : 2 * SDC.N_SDC
            stream_out(i) = stream_in(p_ind(i));
        end 
   
    case 'FAC'
        % FAC interleaving
        FAC = channel_params;
        p_ind = drm_mlc_permutation('FAC', FAC);
        stream_out = zeros(1, 2 * FAC.N_FAC) - 1; % minus one for error checking
        for i = 1 : 2 * FAC.N_FAC
            stream_out(i) = stream_in(p_ind(i));
        end 
        
    case 'MSC_cells'
        % MSC cell interleaving
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC_cells', MSC);
        stream_out = zeros(1, MSC.N_MUX) - 1; % minus one for error checking
        for i = 1 : MSC.N_MUX
            stream_out(i) = stream_in(p_ind(i));
        end
end

end

