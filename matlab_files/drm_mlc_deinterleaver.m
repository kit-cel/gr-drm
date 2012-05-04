function [stream_deinterl] = drm_mlc_deinterleaver(stream_interl, channel_type, channel_params)
% deinterleaver for bit streams and msc cells.

switch channel_type
    case 'MSC'
        % MSC deinterleaving
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC', MSC);
        stream_deinterl = cell(MSC.M_TF,1);
        for i = 1 : MSC.M_TF
            stream_deinterl{i} = zeros(2, 2*MSC.N_MUX);
            for l = 1 : 2*MSC.N_MUX
                stream_deinterl{i}(1, p_ind(1, l)) = stream_interl{i}(1, l);
                stream_deinterl{i}(2, p_ind(2, l)) = stream_interl{i}(2, l);
            end
        end

    case 'SDC'
        % SDC deinterleaving
        SDC = channel_params;
        p_ind = drm_mlc_permutation('SDC', SDC);
        stream_deinterl = zeros(1, 2*SDC.N_SDC);
        for i = 1 : 2*SDC.N_SDC
            stream_deinterl(p_ind(i)) = stream_interl(i);
        end        
   
    case 'FAC'
        % FAC deinterleaving
        FAC = channel_params;
        p_ind = drm_mlc_permutation('FAC', FAC);
        stream_deinterl = zeros(1, 2*FAC.N_FAC);
        for i = 1 : 2*FAC.N_FAC
            stream_deinterl(p_ind(i)) = stream_interl(i);
        end  
        
    case 'MSC_cells'
        % MSC cell deinterleaving (short, N_MUX cells (=400ms) are
        % interleaved)
        MSC = channel_params;
        p_ind = drm_mlc_permutation('MSC_cells', MSC);
        stream_deinterl = zeros(MSC.M_TF, MSC.N_MUX);
        for i = 1 : MSC.M_TF
            for l = 1 : MSC.N_MUX
                stream_deinterl(i, p_ind(l)) = stream_interl(i, l);
            end
        end
                    
end

end