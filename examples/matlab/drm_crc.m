function [ data_crc ] = drm_crc(data, channel_type)
% appends CRC word to FAC/SDC data

switch channel_type
    case 'FAC' 
        % CRC polynomial x^8 + x^4 + x^3 + x^2 + 1;
        shift_reg = ones(1, 8); % shift register cells initialized with ones
        for i = 1 : length(data)
            shift_reg_old = shift_reg;
            bit_add = mod(data(i) + shift_reg_old(1), 2);
            shift_reg(8) = bit_add;
            shift_reg(7) = shift_reg_old(8);
            shift_reg(6) = mod(shift_reg_old(7) + bit_add, 2);
            shift_reg(5) = mod(shift_reg_old(6) + bit_add, 2);
            shift_reg(4) = mod(shift_reg_old(5) + bit_add, 2);
            shift_reg(3) = shift_reg_old(4);
            shift_reg(2) = shift_reg_old(3);
            shift_reg(1) = shift_reg_old(2);
        end
        % read out crc word
        %crc_word = [0 1 1 0 0 1 0 0];
        crc_word = mod(shift_reg + 1, 2); % crc word shall be inverted prior to transmission
        
    case 'SDC'
        % CRC polynomial x^16 + x^12 + x^5 + 1
        shift_reg = ones(1, 16); % shift register cells initialized with ones      
        for i = 1 : length(data)
            shift_reg_old = shift_reg;
            bit_add = mod(data(i) + shift_reg_old(1), 2);
            shift_reg(16) = bit_add;
            shift_reg(12:15) = shift_reg_old(13:16);
            shift_reg(11) = mod(shift_reg_old(12) + bit_add, 2);
            shift_reg(5:10) = shift_reg_old(6:11);
            shift_reg(4) = mod(shift_reg_old(5) + bit_add, 2);
            shift_reg(1:3) = shift_reg_old(2:4);                 
        end
        % read out crc word
        crc_word = mod(shift_reg + 1, 2); % crc word shall be inverted prior to transmission
end

% append CRC word to the data (both with MSb first)
data_crc = [data crc_word];

end