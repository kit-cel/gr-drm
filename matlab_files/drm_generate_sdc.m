function [sdc] = drm_generate_sdc(SDC)
% generates a valid SDC block.

% preallocate empty SDC data
sdc = zeros(1, SDC.L_SDC);

%% AFS index (next identical SDC block in 1 STF)
sdc(1:4) = [0 0 0 1]; 

%% Data fields (RM B, SO 3, SDC mode 1 => total of 37 bytes)

% type 0 - Multiplex description data entity (mandatory)

% header
sdc(5:11) = []; % length of body in bytes, 7 bit
sdc(12) = 0; % version flag (data for current super transmission frame)
sdc(13:16) = [0 0 0 0]; % type of data entity

% body
sdc(17:18) = []; % protection level part A
sdc(19:20) = []; % protection level part B





end