clear all
clc

%% Scrambler

i = 1;
fprintf('Test %d...',i);

ref_prbs = [0 0 0 0 0 1 1 1 1 0 1 1 1 1 1 0]; % see DRM standard ('energy dispersal')

bits_in = randint(1, 16);

ref_out = mod(bits_in + ref_prbs, 2);

bits_out = drm_scrambler(bits_in);

if bits_out == ref_out
    fprintf( ' passed. \n' )
else
    fprintf( ' failed! \n' )
end

i = i + 1;

%% Interleaver index generator

fprintf('Test %d...',i);

failed = 0;

% check MSC
MSC = struct('N_1', 396, 'N_2', 1941); % example MSC struct
indexes = drm_mlc_permutation('MSC', MSC);

length_indexes = length(indexes(1, :));

if length_indexes ~= 2 * (MSC.N_1 + MSC.N_2)
    warning('index vector length mismatch')
    failed = 1;
end
    

for k = 1 : 2
    for m = 1 : length_indexes
        % all indexes have to be unique and between 1 and 2 * N
        if length(find(indexes(k, :) == m)) ~= 1 || indexes(k, m) > 2 * (MSC.N_1 + MSC.N_2) || indexes(k, m) <= 0
            disp(indexes(k, m)); disp(m);
            failed = 1;
        end
    end
end

% check SDC
SDC = struct('N_SDC', 322);
indexes = drm_mlc_permutation('SDC', SDC);
length_indexes = length(indexes);

if length_indexes ~= 2 * SDC.N_SDC
    warning('index vector length mismatch')
    failed = 1;
end

for k = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == k)) > 1 || indexes(k) > 2 * SDC.N_SDC || indexes(k) <= 0
        disp(indexes(k))
        failed = 1;
    end
end

% check FAC
FAC = struct('N_FAC', 65);
indexes = drm_mlc_permutation('FAC', FAC);
length_indexes = length(indexes);

if length_indexes ~= 2 * FAC.N_FAC
    warning('index vector length mismatch')
    failed = 1;
end

not_unique = 0;
too_big = 0;
too_small = 0;

for k = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == k)) > 1
        warning('index not unique') 
        disp(k); disp(find(indexes == k));
        failed = 1;
        not_unique = 1;
    end
    if indexes(k) > 2 * FAC.N_FAC
        warning('index exceeds vector range')   
        too_big = 1;
        disp(indexes(k))
    end
    if indexes(k) <= 0
        warning('index is <= 0')
        too_small = 1;
        disp(k); disp(indexes(k))    
    end
    if length(find(indexes == k)) == 0
        fprintf('missing index'); disp(k);
    end
                
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

clear failed MSC SDC FAC indexes length_indexes

i = i + 1;

%% Mapping

fprintf('Test %d...',i);

failed = 0;

% normalization factors
a_4 = 1/sqrt(2);
a_16 = 1/sqrt(10);

% example MSC struct
MSC = struct('N_MUX', 16);
ref_in = [0 0 0 0 0 1 0 1 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 1 1 0 1 0 1 1 1 1 ; ...
          0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 0 0 0 1 0 0 0 1 1 0 1 1 1 0 1 1 ];
ref_out = a_16.*[3 + 3i, 3 - 1i, 3 + 1i, 3 - 3i, -1 + 3i, -1 - 1i, -1 + 1i, -1 - 3i, ...
                 1 + 3i, 1 - 1i, 3 + 1i, 3 - 3i, -3 + 3i, -3 - 1i, -3 + 1i, -3 - 3i];

stream_out = drm_mapping(ref_in, 'MSC', MSC);

if ref_out ~= stream_out
    failed = 1;
end

% example FAC struct
FAC = struct('N_FAC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'FAC', FAC);

% example SDC struct
SDC = struct('N_SDC', 4);
ref_in = [0 0 0 1 1 0 1 1];
ref_out = a_4.*[1 + 1i, 1 - 1i, -1 + 1i, -1 - 1i];

stream_out = drm_mapping(ref_in, 'SDC', SDC);

if ref_out ~= stream_out
    failed = 1;
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

i = i + 1;
