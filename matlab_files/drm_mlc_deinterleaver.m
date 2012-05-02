function [stream_deinterl] = drm_mlc_deinterleaver(stream_interl, channel_type, channel_params)
% deinterleaver for bit streams and msc cells.

switch channel_type
    case 'MSC'
        % MSC interleaving
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC', MSC);
       

    case 'SDC'
        % SDC interleaving
        SDC = channel_params;
        p_ind = drm_mlc_permutation('SDC', SDC);
        
   
    case 'FAC'
        % FAC interleaving
        FAC = channel_params;
        p_ind = drm_mlc_permutation('FAC', FAC);
       
        
    case 'MSC_cells'
        % MSC cell interleaving (short, N_MUX cells (=400ms) are
        % interleaved)
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC_cells', MSC);
        stream_deinterl = zeros(MSC.M_TF, MSC.N_MUX);
        for i = 1 : MSC.M_TF
            for l = 1 : MSC.N_MUX
                stream_deinterl(i, l) = stream_interl(i, p_ind(l));
            end
        end
                    
end

end