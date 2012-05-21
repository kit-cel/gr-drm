function [sdc] = drm_generate_sdc(SDC)
% generates a valid SDC block.

%% AFS index (next identical SDC block in 1 STF)
AFS = [0 0 0 1]; 

%% Data fields (RM B, SO 3, SDC mode 1 => total of 37 byte / 296 bit)

% type 0 - Multiplex description data entity (mandatory)

% header
n_bytes_0 = 5;
type_0 = zeros(1, n_bytes_0 * 8);
%type_0 = zeros(1, 52);
type_0(1:7) = [0 0 0 0 0 1 1]; % length of body in bytes, 7 bit (52 bit -> 7 byte)
type_0(8) = 0; % version flag (data for current super transmission frame)
type_0(9:12) = [0 0 0 0]; % type of data entity

% body
type_0(13:14) = [0 1]; % protection level part A FIXME
type_0(15:16) = [0 1]; % protection level part B FIXME
type_0(17:28) = [0 0 0 0 0 1 1 1 1 0 1 1]; % data length for part A FIXME
type_0(29:40) = [0 0 1 0 0 1 0 1 1 1 0 1]; % data length for part B FIXME


% type 1 - Label data entity

% header
n_bytes_1 = 7;
type_1 = zeros(1, n_bytes_1 * 8);
type_1(1:7) = [0 0 0 0 1 0 1]; % length of body
type_1(8) = 0; % version flag (no meaning, shall be set to zero)
type_1(9:12) = [0 0 0 1]; % type of data entity

% body
type_1(13:14) = [0 0]; % Short ID (relates it to the Service ID given in the FAC)
type_1(15:16) = [0 0]; % rfu
% label (UTF-8 coded grDRM)
type_1(17:56) = [0 1 1 0 0 1 1 1 0 1 1 1 0 0 1 0 0 1 0 0 0 1 0 0 ...
                 0 1 0 1 0 0 1 0 0 1 0 0 1 1 0 1]; 

% type 9 - Audio information data entity

% header
n_bytes_9 = 4;
type_9 = zeros(1, n_bytes_9 * 8);
type_9(1:7) = [0 0 0 0 0 1 0]; % length of body
type_9(8) =  0; % version flag (for current frame)
type_9(9:12) = [1 0 0 1]; % type of data entity

% body
type_9(13:14) = [0 0]; % Short ID (like FAC and type 0)
type_9(15:16) = [0 0]; % Stream ID (arbitrary?)
type_9(17:18) = [0 0]; % audio coding (AAC) 
type_9(19) = 0; % SBR flag (no SBR) 
type_9(20:21) = [0 0]; % audio mode (mono) 
type_9(22:24) = [1 0 1]; % audio sampling rate (48 kHz)
type_9(25) = 0; % text flag (no text) 
type_9(26) = 0; % enhancement flag (no enhancement information available)
type_9(27:31) = [1 1 1 0 0]; % coder field (no MPEG surround information)
type_9(32) = 0; % rfa


% type 8

% header
n_bytes_8 = 5;
type_8 = zeros(1, n_bytes_8 * 8);
type_8(1:7) = [0 0 0 0 0 1 1]; % length of body
type_8(8) = 0; % version flag
type_8(9:12) = [1 0 0 0]; % type

% body
type_8(13:29) = [0 1 1 0 1 0 1 1 1 0 1 0 0 0 1 1 0]; % MJD
type_8(30:40) = zeros(1, 11); % hours and minutes

%% Append data entities

sdc_data = [AFS type_9 type_0  type_1 type_8   ];
% zeropadding
sdc = [sdc_data zeros(1, SDC.L_SDC - length(sdc_data) - 16)];


%% CRC calculation
sdc = drm_crc([zeros(1, 4) sdc], 'SDC'); % AFS is preceded by 4 zeros for CRC calculation
sdc = sdc(5:end); % Crop the leading zeros which were inserted for CRC calculation
sdc = [sdc, zeros(1, SDC.L_SDC - length(sdc))]; % zeropadding

end