function [stream_out] = drm_mlc_partitioning(stream_in, channel_type, channel_params)
% the assumptions made for this calculation are listed in
% drm_global_variables.m

switch channel_type
    case 'MSC'
        MSC = channel_params;
        M_01 = 2 * MSC.N_1 * MSC.R_0;
        M_11 = 2 * MSC.N_1 * MSC.R_1;
        M_02 = MSC.R_X0 * floor((2 * MSC.N_2 - 12) / MSC.R_Y0);
        M_12 = MSC.R_X1 * floor((2 * MSC.N_2 - 12) / MSC.R_Y1);
        stream_part = cell(1,4);
        stream_part{1,1} = stream_in( 1 : M_01 ); % 1st higher protected part
        stream_part{1,2} = stream_in( M_01 + 1 : M_01 + M_11 ); % 2nd higher protected part
        stream_part{1,3} = stream_in( M_01 + M_11 + 1 : M_01 + M_11 + M_02 ); % 1st lower protected part
        stream_part{1,4} = stream_in( M_01 + M_11 + M_02 + 1 :  M_01 + M_11 + M_02 + M_12); % 2nd lower protected part
        stream_out = cell(2, 1);
        stream_out{1,1} = [ stream_part{1,1} stream_part{1,3} ]; % stream going to encoder C_0
        stream_out{2,1} = [ stream_part{1,2} stream_part{1,4} ]; % stream going to encoder C_1
        if  M_01 + M_11 + M_02 + M_12 ~= length(stream_in); error('msc length does not fit'); end
    case 'SDC' 
        SDC = channel_params;
        M_02 = SDC.R_X0 * floor((2 * SDC.N_SDC - 12) / SDC.R_Y0); % M_01 = 0 because of N_1 = 0 (N_2 = N_SDC)
        stream_out = cell(1,1);
        stream_out{1,1} = stream_in( 1 : M_02 );
        if M_02 ~= length(stream_in); error('sdc length does not fit'); end       
    case 'FAC'
        FAC = channel_params; 
        M_02 = FAC.R_X0 * floor((2 * FAC.N_FAC - 12) / FAC.R_Y0); % M_01 = 0, same as for SDC
        stream_out = cell(1,1);
        stream_out{1,1} = stream_in; % FIXME: why does this fail? M_02 is smaller than L_FAC, cropping does not seem to make sense.
        % if M_02 ~= length(stream_in); disp(FAC.L_FAC); disp(M_02); error('fac length does not fit'); end       
end

% TODO: throw error if input is invalid

