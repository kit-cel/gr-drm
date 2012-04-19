function [stream_out] = drm_mlc_encoder(stream_in, channel_type, channel_params)

trellis =  poly2trellis(7, [133 171 145 133 171 145]);
tail_bits = zeros(1,6);

switch channel_type
    case 'MSC'
        % MSC encoding with tailbits
        MSC = channel_params;
        
        stream_in_0 = stream_in{1,1}; % stream encoded by C0 (R_0)
        puncturing_pattern_0 = [ 1 1 0 0 0 0 ];
        % r_0 = 2*MSC.N_MUX - 12 - MSC.R_Y0 * floor((2*MSC.N_MUX - 12) / MSC.R_Y0);
        % r_0 = 0
        punct_pat_tailbits_0 = [ 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        [stream_out_0_p1 final_state] = convenc(stream_in_0, trellis, puncturing_pattern_0);
        stream_out_0_p2 = convenc(tail_bits, trellis, punct_pat_tailbits_0, final_state);
        stream_out_0 = [stream_out_0_p1 stream_out_0_p2];
        
        stream_in_1 = stream_in{2,1}; % stream encoded by C1 (R_1)
        % r_1 = 2*MSC.N_MUX - 12 - MSC.R_Y1 * floor((2*MSC.N_MUX - 12) / MSC.R_Y1);
        % r_1 = 2
        puncturing_pattern_1 = [ 1 1 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 ];
        punct_pat_tailbits_1 = [ 1 1 1 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 1 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        [stream_out_1_p1 final_state] = convenc(stream_in_1, trellis, puncturing_pattern_1);
        stream_out_1_p2 = convenc(tail_bits, trellis, punct_pat_tailbits_1, final_state);
        stream_out_1 = [ stream_out_1_p1 stream_out_1_p2];
        
        stream_out = [stream_out_0 ; stream_out_1];     
        
    case 'SDC'
        % SDC encoding with tailbits
        SDC = channel_params;
        
        stream_in = stream_in{1,1};
        puncturing_pattern = [1 1 0 0 0 0];
        % r_0 = 2*SDC.N_SDC - 12 - SDC.R_Y0 * floor((2*SDC.N_SDC - 12) / SDC.R_Y0);
        % r_0 = 0
        punct_pat_tailbits = [ 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 0 0 ];
        [stream_out_p1 final_state] = convenc(stream_in, trellis, puncturing_pattern);       
        stream_out_p2 = convenc(tail_bits, trellis, punct_pat_tailbits, final_state);
        stream_out = [stream_out_p1 stream_out_p2];
        
        if length(stream_out) ~= SDC.N_SDC * 2; warning('encoded SDC length mismatch'); end
   
    case 'FAC'
        % FAC encoding (no tailbits)
        FAC = channel_params;
        
        stream_in = stream_in{1,1};       
        puncturing_pattern = [1 1 0 0 0 0 1 0 0 0 0 0 1 1 0 0 0 0 ]; % code rate 3/5
        [stream_out_p1, final_state] = convenc(stream_in, trellis, puncturing_pattern);
        stream_out_p2 = convenc(tail_bits, trellis, puncturing_pattern);
        stream_out = [stream_out_p1 stream_out_p2];
        
        if length(stream_out) ~= FAC.N_FAC * 2; warning('encoded FAC length mismatch'); end
            
end