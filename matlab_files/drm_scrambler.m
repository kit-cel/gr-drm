function [bits_out] = drm_scrambler(bits_in)

L = length(bits_in);

prbs = ones(1, L) - 2;

shift_register = ones(1, 9); % Startbelegung

for i = 1:L
    % mod2 Addition gemaess Polynom G(x) = x^9 + x^5 + 1
    output = mod(shift_register(5) + shift_register(9), 2); 
    prbs(i) = output;
    
    % shift
    shift_tmp = shift_register;
    for k = 2:9
        shift_register(k) = shift_tmp(k-1);
    end
    shift_register(1) = output;
end

bits_out = mod(prbs + bits_in, 2);