function [data_dec] = drm_mlc_decoder(data_enc, channel_type, channel_params, OFDM_params)

trellis =  poly2trellis(7, [133 171 145 133 171 145]);

switch channel_type
    case 'MSC'
    case 'SDC'
    case 'FAC'
        % FAC decoding
        FAC = channel_params;
        OFDM = OFDM_params;
        data_dec = zeros(OFDM.M_TF, FAC.L_FAC);
        
        puncturing_pattern = [1 1 0 0 0 0 1 0 0 0 0 0 1 1 0 0 0 0 ]; % code rate 3/5
        n_ones = length(find(puncturing_pattern)); % number of ones in the puncturing pattern
        for i = 1:OFDM.M_TF
%             [data_dec_1, finalmetric, finalstates, finalinputs] = ...
%                 vitdec(data_enc(i, 1:end-n_ones), trellis, 10, 'cont', 'hard', puncturing_pattern);
%             data_dec_2 = vitdec( data_enc(i, end - n_ones + 1 : end), trellis, 10, 'cont', 'hard', puncturing_pattern, finalmetric, finalstates, finalinputs);
%             data_dec(i, :) = [data_dec_1, data_dec_2];
            data_dec = vitdec(data_enc(i,:), trellis, 10, 'term', 'hard', puncturing_pattern);
        end
end

end