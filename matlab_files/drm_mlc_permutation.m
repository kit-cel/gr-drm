function [indexes] = drm_mlc_permutation(channel_type, channel_params)

switch channel_type
    case 'MSC'
        MSC = channel_params;
        x_in = 2 * (MSC.N_1 + MSC.N_2); % number of input bits
        P_0 = zeros(1, x_in) - 1; % all indexes are set to -1 at first to detect errors
        P_1 = zeros(1, x_in) - 1;
        
        % calculate variables
        t_0 = 13;
        t_1 = 21;
        s = 2^(ceil(log2(x_in)));
        q = s/4 - 1;
        
        % calculate indexes for the stream coming from C_0
        P_0(1) = 1;
        for i = 2 : x_in
            P_0(i) = mod(t_0 * P_0(i-1) + q, s);
            
            while P_0(i) > x_in
                P_0(i) = mod(t_0 * P_0(i) + q, s);
            end
        end
        
        % calculate indexes for the stream coming from C_1
        P_1(1) = 1;
        for i = 2 : x_in
            P_1(i) = mod(t_1 * P_1(i-1) + q, s);
            
            while P_1(i) > x_in
                P_1(i) = mod(t_1 * P_1(i) + q, s);
            end
        end
        
        indexes = [P_0 ; P_1];
        
    case 'SDC'
        SDC = channel_params;
        x_in = 2 * SDC.N_SDC; % number of input bits
        P_0 = zeros(1, x_in) - 1; % all indexes are set to -1 at first to detect errors
        
        % calculate variables
        t_0 = 21;
        s = 2^(ceil(log2(x_in)));
        q = s/4 - 1;
        
        % calculate indexes
        P_0(1) = 1;
        for i = 2 : x_in
            P_0(i) = mod(t_0 * P_0(i-1) + q, s);
            
            while P_0(i) > x_in
                P_0(i) = mod(t_0 * P_0(i) + q, s);
            end
        end
        
        indexes = P_0;
        
    case 'FAC'
        FAC = channel_params;
        x_in = 2 * FAC.N_FAC; % number of input bits
        P_0 = zeros(1, x_in) - 1; % all indexes are set to -1 at first to detect errors
        
        % calculate variables
        t_0 = 21;
        s = 2^(ceil(log2(x_in)));
        q = s/4 - 1;
        
        % calculate indexes
        P_0(1) = 1;
        for i = 2 : x_in
            P_0(i) = mod(t_0 * P_0(i-1) + q, s);
            
            while P_0(i) > x_in
                P_0(i) = mod(t_0 * P_0(i) + q, s);
            end
        end
        
        indexes = P_0;
end

end