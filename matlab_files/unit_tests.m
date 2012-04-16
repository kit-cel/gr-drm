%% Test 1: Scrambler

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

