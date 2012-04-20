function [indexes] = drm_mlc_permutation(channel_type, channel_params)

switch channel_type
    case 'MSC'
        MSC = channel_params;
        
        % calculate variables
        x_in_1 = 2 * MSC.N_1;
        x_in_2 = 2 * MSC.N_2;
        t_0 = 13;
        t_1 = 21;
        s_1 = 2^(ceil(log2(x_in_1))); 
        s_2 = 2^(ceil(log2(x_in_2)));
        q_1 = s_1/4 - 1;        
        q_2 = s_2/4 - 1;

        
        % calculate indexes for the stream coming from C_0
        % higher protected part
        P_0_1 = zeros(1, x_in_1) - 1;
        
        P_0_1(1) = 0;
        for i = 1 : x_in_1 - 1
            P_0_1(i+1) = mod(t_0 * P_0_1(i) + q_1, s_1);
            
            while P_0_1(i+1) >= x_in_1
                P_0_1(i+1) = mod(t_0 * P_0_1(i+1) + q_1, s_1);
            end
        end
        P_0_1 = P_0_1 + 1; % matlab based indexing
        
        % lower protected part
        P_0_2 = zeros(1, x_in_2) - 1;
        
        P_0_2(1) = 0;
        for i = 1 : x_in_2 - 1
            P_0_2(i+1) = mod(t_0 * P_0_2(i) + q_2, s_2);
            
            while P_0_2(i+1) >= x_in_2
                P_0_2(i+1) = mod(t_0 * P_0_2(i+1) + q_2, s_2);
            end
        end
        P_0_2 = P_0_2 + x_in_1 + 1; % matlab based indexing and shift
      
        % calculate indexes for the stream coming from C_1
        % higher protected part
       P_1_1 = zeros(1, x_in_1) - 1;
        
        P_1_1(1) = 0;
        for i = 1 : x_in_1 - 1
            P_1_1(i+1) = mod(t_1 * P_1_1(i) + q_1, s_1);
            
            while P_1_1(i+1) >= x_in_1
                P_1_1(i+1) = mod(t_1 * P_1_1(i+1) + q_1, s_1);
            end
        end
        P_1_1 = P_1_1 + 1; % matlab based indexing
        
        % lower protected part
        P_1_2 = zeros(1, x_in_2) - 1;
        
        P_1_2(1) = 0;
        for i = 1 : x_in_2 - 1
            P_1_2(i+1) = mod(t_1 * P_1_2(i) + q_2, s_2);
            
            while P_1_2(i+1) >= x_in_2
                P_1_2(i+1) = mod(t_1 * P_1_2(i+1) + q_2, s_2);
            end
        end
        P_1_2 = P_1_2 + x_in_1 + 1; % matlab based indexing and shift
        
        % concatenate index vectors
        
        indexes = [P_0_1 P_0_2 ; P_1_1 P_1_2];
        
    case 'SDC'
        SDC = channel_params;
        x_in = 2 * SDC.N_SDC; % number of input bits
        P_0 = zeros(1, x_in) - 1; % all indexes are set to -1 at first to detect errors
        
        % calculate variables
        t_0 = 21;
        s = 2^(ceil(log2(x_in)));
        q = s/4 - 1;
        
        % calculate indexes
        P_0(1) = 0;
        for i = 1 : x_in - 1
            P_0(i+1) = mod(t_0 * P_0(i) + q, s);
            
            while P_0(i+1) >= x_in
                P_0(i+1) = mod(t_0 * P_0(i+1) + q, s);
            end
        end
        
        indexes = P_0 + 1; % matlab based indexing
        
    case 'FAC'
        FAC = channel_params;
        x_in = 2 * FAC.N_FAC; % number of input bits
        P_0 = zeros(1, x_in) - 1; % all indexes are set to -1 at first to detect errors
        
        % calculate variables
        t_0 = 21;
        s = 2^(ceil(log2(x_in)));
        q = s/4 - 1;
        
        % calculate indexes
        P_0(1) = 0;
        for i = 2 : x_in
            P_0(i) = mod(t_0 * P_0(i-1) + q, s);
            while P_0(i) >= x_in
                P_0(i) = mod(t_0 * P_0(i) + q, s);
            end
        end
        
        indexes = P_0 + 1; % matlab based indexing
end

end