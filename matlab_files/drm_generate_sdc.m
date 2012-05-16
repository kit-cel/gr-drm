function [sdc] = drm_generate_sdc(SDC)
% generates a valid SDC block.

% preallocate empty SDC data
sdc = zeros(1, SDC.L_SDC - 16);
%sdc = drm_crc([zeros(1, 4) sdc], 'SDC'); % AFS is preceded by 4 zeros

%% AFS index (next identical SDC block in 1 STF)
sdc(1:4) = [0 0 0 1]; 

%% Data fields (RM B, SO 3, SDC mode 1 => total of 37 byte / 296 bit)

% type 0 - Multiplex description data entity (mandatory)

% header
sdc(5:11) = [0 0 0 0 1 1 1]; % length of body in bytes, 7 bit (52 bit -> 7 byte)
sdc(12) = 0; % version flag (data for current super transmission frame)
sdc(13:16) = [0 0 0 0]; % type of data entity
 
% body
sdc(17:18) = [0 1]; % protection level part A FIXME
sdc(19:20) = [0 1]; % protection level part B FIXME
sdc(21:44) = randint(1, 24); % stream description for stream 0 FIXME
sdc(45:56) = [0 0 0 0 0 1 1 1 1 0 1 1]; % data length for part A FIXME
sdc(57:68) = [0 0 0 0 0 0 0 0 0 0 0 0]; % data length for part B FIXME

% type 1 - Label data entity

% header
sdc(69:75) = [0 0 0 0 1 1 0]; % length of body
sdc(76) = 0; % version flag (no meaning, shall be set to zero)
sdc(77:80) = [0 0 0 1]; % type of data entity

% body
sdc(81:82) = [0 0]; % Short ID (relates it to the Service ID given in the FAC)
sdc(83:84) = [0 0]; % rfu
% label (UTF-8 coded)
sdc(85:124) = [0 1 1 0 0 1 1 1 0 1 1 1 0 0 1 0 0 1 0 0 0 1 0 0 ...
               0 1 0 1 0 0 1 0 0 1 0 0 1 1 0 1]; 

%% CRC calculation
sdc = drm_crc([zeros(1, 4) sdc], 'SDC'); % AFS is preceded by 4 zeros for CRC calculation
sdc = sdc(5:end); % Crop the leading zeros which were inserted for CRC calculation
sdc = [sdc, zeros(1, SDC.L_SDC - length(sdc))];


end