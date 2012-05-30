function [data_dec] = drm_mlc_decoder(data_enc, channel_type, channel_params, OFDM_params)
% decodes data using the Viterbi algorithm (hard decision)

OFDM = OFDM_params;
trellis =  poly2trellis(7, [133 171 145 133 171 145]);

switch channel_type
    case 'MSC'
        % MSC decoding
        MSC = channel_params;
        data_dec = cell(MSC.M_TF, 1);
        
        % Puncturing patterns for encoders C_0 and C_1
        puncturing_pattern_0 = [ 1 1 0 0 0 0 ];
        punct_pat_tailbits_0 = [ 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        puncturing_pattern_1 = [ 1 1 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 ];
        punct_pat_tailbits_1 = [ 1 1 1 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 1 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        
        % length of output streams
        M_02 = MSC.R_X0 * floor((2 * MSC.N_2 - 12) / MSC.R_Y0);
        M_12 = MSC.R_X1 * floor((2 * MSC.N_2 - 12) / MSC.R_Y1);
        
        % Decode data using Viterbi algorithm (hard decision)
        for i = 1:MSC.M_TF
            data_dec{i} = cell(2, 1);
            
            % Decoder C_0 
            [data_dec_0_p1, finalmetric,finalstates,finalinputs] = ...
                vitdec(data_enc{i}(1, 1:end-12), trellis, 10, 'cont', 'hard', puncturing_pattern_0);
            data_dec_0_p1 = data_dec_0_p1(11:end); % truncate leading zeros
            % invoke again for tail bit puncturing pattern
            data_dec_0_p2 = vitdec([data_enc{i}(1, end-11:end) zeros(1, 12)], trellis, 10, 'cont', 'hard', punct_pat_tailbits_0, finalmetric, finalstates, finalinputs);
            data_dec_0 = [data_dec_0_p1 data_dec_0_p2];
            data_dec{i}{1} = data_dec_0(1:M_02); % TODO: Find the right size by calculating M00, M01 etc instead of hardcoding
            
            % Decoder C_1
            [data_dec_1_p1, finalmetric,finalstates,finalinputs] = ...
                vitdec(data_enc{i}(2, 1:end-14), trellis, 10, 'cont', 'hard', puncturing_pattern_1);
            data_dec_1_p1 = data_dec_1_p1(11:end); % truncate leading zeros
            % invoke again for tail bit puncturing pattern
            data_dec_1_p2 = vitdec([data_enc{i}(2, end-13:end) zeros(1, 14)], trellis, 10, 'cont', 'hard', punct_pat_tailbits_1, finalmetric, finalstates, finalinputs);
            data_dec_1 = [data_dec_1_p1 data_dec_1_p2];
            data_dec{i}{2} = data_dec_1(1:M_12); % TODO: Find the right size by calculating M00, M01 etc instead of hardcoding 
        end
            
    case 'SDC'
        % SDC decoding
        SDC = channel_params;
        puncturing_pattern = [1 1 0 0 0 0];
        punct_pat_tailbits = [ 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        [data_dec_p1, finalmetric,finalstates,finalinputs] = ...
            vitdec(data_enc(1:end-12), trellis, 10, 'cont', 'hard', puncturing_pattern);
        data_dec_p1 = data_dec_p1(11:end); % output is delayed by 10 (see vitdec)
        data_dec_p2 = vitdec([data_enc(end-11:end) zeros(1, 12)], trellis, 10, 'cont', 'hard', punct_pat_tailbits, finalmetric, finalstates, finalinputs);
        data_dec_p2 = data_dec_p2(1:10); % cut to the right length (TODO: find more elegant way)
        data_dec = [data_dec_p1 data_dec_p2];
    case 'FAC'
        % FAC decoding
        FAC = channel_params;        
        data_dec = zeros(OFDM.M_TF, FAC.L_FAC + 6); % +6 because of tail bits (to be cut off)        
        puncturing_pattern = [1 1 0 0 0 0 1 0 0 0 0 0 1 1 0 0 0 0 ]; % code rate 3/5
        n_ones = length(find(puncturing_pattern)); % number of ones in the puncturing pattern
        for i = 1:OFDM.M_TF
            data_dec(i, :) = vitdec(data_enc(i,:), trellis, 10, 'term', 'hard', puncturing_pattern);
        end
        data_dec = data_dec(:, 1:FAC.L_FAC);
end

end