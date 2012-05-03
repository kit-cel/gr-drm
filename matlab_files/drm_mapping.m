function [stream_out] = drm_mapping(stream_in, channel_type, channel_params)

% normalization factors
a_16 = 1/sqrt(10);
a_4 = 1/sqrt(2);

% mapping tables are ordered by the binary index of the symbols + 1
% example: 11 -> -1 - i = map_tab_4(4); 00 -> 1 + i = map_tab_4(1)
map_tab_16 = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
                    1 + 3i, 1 - 1i, 1 + 1i, 1 - 3i, -3 + 3i, -3 - 1i, -3 + 1i, -3 - 3i];
map_tab_4 = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

switch channel_type
    case 'MSC'
        % 16-QAM, 4 bit per symbol
        MSC = channel_params;
        stream_out = zeros(1, MSC.N_MUX);
        for i = 1 : MSC.N_MUX
            map_ind = 2^3 * stream_in(1, 2*i-1) + 2^2 * stream_in(2,2*i-1) ...
                         + 2^1 * stream_in(1, 2*i) + 2^0 * stream_in(2, 2*i);
            stream_out(i) = map_tab_16(map_ind + 1); % matlab indexing
        end
    case 'SDC'
        % 4-QAM, 2 bit per symbol
        SDC = channel_params;
        stream_out = zeros(1, SDC.N_SDC);
        for i = 1 : SDC.N_SDC
            map_ind = 2^1 * stream_in(2*i - 1) + 2^0 * stream_in(2*i);
            stream_out(i) = map_tab_4(map_ind + 1); % matlab indexing
        end
    case 'FAC'
        % 4-QAM, 2 bit per symbol
        FAC = channel_params;
        stream_out = zeros(1, FAC.N_FAC);
        for i = 1 : FAC.N_FAC
            map_ind = 2^1 * stream_in(2*i - 1) + 2^0 * stream_in(2*i);
            stream_out(i) = map_tab_4(map_ind + 1); % matlab indexing
        end
end

end