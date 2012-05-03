function [stream_demapped] = drm_demapping(stream_mapped, channel_type, channel_params)
% performs demapping (symbol to bit)

% map_tab_16 = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
%                     1 + 3i, 1 - 1i, 1 + 1i, 1 - 3i, -3 + 3i, -3 - 1i, -3 + 1i, -3 - 3i];
% map_tab_4 = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

map_tab_4 = {[1 0], [1 1], [0 0], [0 1]}; % these orderings come from matlab qamdemod...
map_tab_16 = {[1 1 0 0],[1 1 1 0],[1 1 0 1],[1 1 1 1],...
              [0 1 0 0],[0 1 1 0],[0 1 0 1],[0 1 1 1],...
              [1 0 0 0],[1 0 1 0],[1 0 0 1],[1 0 1 1],...
              [0 0 0 0],[0 0 1 0],[0 0 0 1],[0 0 1 1]};

switch channel_type
    case 'MSC'
        % 16-QAM, 4 bit per symbol
        MSC = channel_params;
        stream_demapped = zeros(3, 4*MSC.N_MUX);
        for i = 1 : MSC.M_TF
            for l = 1 : MSC.N_MUX
                stream_demapped(i, 4*l-3 : 4*l) = map_tab_16{qamdemod(sqrt(10)*stream_mapped(i, l), 16) + 1};
            end
        end
        
        % split stream up (MLC)
        stream_splitted = cell(MSC.M_TF, 1);
        for i = 1 : MSC.M_TF
            stream_splitted{i} = zeros(2, 2*MSC.N_MUX);
            for l = 1 : MSC.N_MUX
                stream_splitted{i}(1, 2*l-1) = stream_demapped(i, 4*l-3);
                stream_splitted{i}(1, 2*l) = stream_demapped(i, 4*l-1);
                stream_splitted{i}(2, 2*l-1) = stream_demapped(i, 4*l-2);
                stream_splitted{i}(2, 2*l) = stream_demapped(i, 4*l);
            end
        end
        
        % rename for output
        stream_demapped = stream_splitted;
        
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