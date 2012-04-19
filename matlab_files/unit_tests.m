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

MSC = struct('N_1', 396, 'N_2', 1941); % fake MSC struct
indexes = drm_mlc_permutation('MSC', MSC);

length_indexes = length(indexes(1, :));

for k = 1 : 2
    for i = 1 : length_indexes
        % all indexes have to be unique and between 1 and 2 * N
        if length(find(indexes(k, :) == i)) > 1 || indexes(k, i) > 2 * (MSC.N_1 + MSC.N_2) || indexes(k, i) < 0
            disp(indexes(k, i))
            failed = 1;
        end
    end
end

% check SDC
SDC = struct('N_SDC', 322);
indexes = drm_mlc_permutation('SDC', SDC);
length_indexes = length(indexes);

for i = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == i)) > 1 || indexes(i) > 2 * SDC.N_SDC || indexes(i) < 0
        disp(indexes(i))
        failed = 1;
    end
end

% check FAC
FAC = struct('N_FAC', 65);
indexes = drm_mlc_permutation('FAC', FAC);
length_indexes = length(indexes);

for i = 1 : length_indexes
    % all indexes have to be unique and between 1 and 2 * N
    if length(find(indexes == i)) > 1 || indexes(i) > 2 * FAC.N_FAC || indexes(i) < 0
        disp(indexes(i))
        failed = 1;
    end
end

if failed
    fprintf(' failed! \n')
else
    fprintf(' passed. \n')
end

clear failed MSC SDC FAC indexes length_indexes

i = i + 1;