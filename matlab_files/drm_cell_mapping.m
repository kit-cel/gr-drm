function [stream_out] = drm_cell_mapping(msc_stream, sdc_stream, fac_stream)

fac_pos = cell(12, 2); % 1st column: symbol index; 2nd column: carrier indexes k
for i = 1:12
    fac_pos{i,1} = i + 1; % symbol index (zero-based)
end

% carrier indexes k
fac_pos{1,2} = [13 25 43 55 67];
fac_pos{2,2} = [15 27 45 57 69];
fac_pos{3,2} = [17 29 47 59 71];
fac_pos{4,2} = [19 31 49 61 73];
fac_pos{5,2} = [9 21 33 51 63 75];
fac_pos{6,2} = [11 23 35 53 65 77];
fac_pos{7,2} = [13 25 37 55 67 79];
fac_pos{8,2} = [15 27 39 57 69 81];
fac_pos{9,2} = [17 29 41 59 71 83];
fac_pos{10,2} = [19 31 43 61 73];
fac_pos{11,2} = [21 33 45 63 75];
fac_pos{12,2} = [23 35 47 65 77];

end