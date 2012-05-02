function [stream_demapped] = drm_demapping(stream_mapped, channel_type, channel_params)
% performs demapping (symbol to bit)

% map_tab_16 = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
%                     1 + 3i, 1 - 1i, 1 + 1i, 1 - 3i, -3 + 3i, -3 - 1i, -3 - 1i, -3 - 3i];
% map_tab_4 = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

map_tab_4 = {[1 0], [1 1], [0 0], [0 1]};
%map_tab_16 = {[],[],[],[],[],[],[],[],...
%              [],[],[],[],[],[],[],[]}

switch channel_type
    case 'MSC'
        % 16-QAM, 4 bit per symbol
        MSC = channel_params;
        stream_demapped = cell(3, 2, 2); %% TODO
        
    case 'SDC'
        % 4-QAM, 2 bit per symbol
        SDC = channel_params;
        stream_demapped = zeros(1, SDC.L_SDC);
        for i = 1 : SDC.N_SDC
            stream_demapped(2*i-1 : 2*i) = map_tab_4{qamdemod(stream_mapped(i), 4) + 1};
        end       
        
    case 'FAC'
        % 4-QAM, 2 bit per symbol
        FAC = channel_params;
        stream_demapped = zeros(1, FAC.L_FAC);
        for i = 1 : FAC.N_FAC
            stream_demapped(2*i-1 : 2*i) = map_tab_4{qamdemod(stream_mapped(i), 4) + 1};
        end
        
end

end