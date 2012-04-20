%% global variables corresponding to the following assumptions:
% - robustness mode B
% - spectrum occupancy 3 (10 kHz)
% - protection level 1 for A & B in MSC, SDC uses 4-QAM
% - MSC using 16-QAM, SM and UEP
% - short interleaving of MSC cells

%% MSC: 16-QAM, SM, UEP

M_TF = 3;

N_MUX = 2337;

N_SFU = 7011;

N_SFA = 7013;

N_L = N_SFA - N_SFU;

X = 123; % number of bytes in higher protected part A, arbitraryly chosen

R_all = 0.62; R_0 = 1/2; R_1 = 3/4; R_Ylcm = 4;
R_X0 = 1; R_Y0 = 2; R_X1 = 3; R_Y1 = 4;

N_1 = ceil(8*X/(2*R_Ylcm*(R_0 + R_1)))*R_Ylcm;

N_2 = N_MUX - N_1;

L_1 = 2*N_1*(R_0 + R_1);

L_2 = R_X0*floor((2*N_2 - 12)/R_Y0) + R_X1*floor((2*N_2 - 12)/R_Y1);
     
L_MUX = L_1 + L_2;

if N_1 < 0 || N_1 > N_MUX - 20
    error('N_1 out of bounds')
end

if N_2 < 20 || N_2 > N_MUX
    error('N_2 out of bounds')
end

MSC = struct('M_TF', M_TF, 'N_SFU', N_SFU, 'N_SFA', N_SFA, 'N_L', N_L, 'N_MUX', N_MUX, ...
                'N_1',N_1, 'N_2', N_2, 'L_MUX', L_MUX, 'R_0', R_0, 'R_1', R_1, 'R_X0', R_X0, ...
                'R_Y0', R_Y0, 'R_X1', R_X1, 'R_Y1', R_Y1, 'L_1', L_1, 'L_2', L_2);

%% SDC: 4-QAM, SM, EEP

R_all = 0.5; R_0 = 1/2; R_X0 = 1; R_Y0 = 2;
N_SDC = 322;

L_SDC = R_X0*floor((2*N_SDC - 12)/R_Y0);

SDC = struct('N_SDC', N_SDC, 'L_SDC', L_SDC, 'R_0', R_0, 'R_X0', R_X0, 'R_Y0', R_Y0);

%% FAC: 4-QAM, SM, EEP

R_all = 0.6; R_0 = 3/5; R_X0 = 3; R_Y0 = 5;

FAC = struct('N_FAC', 65, 'L_FAC', 72, 'R_0', R_0, 'R_X0', R_X0, 'R_Y0', R_Y0);

clear L_1 L_2 L_SDC L_MUX N_1 N_2 N_MUX N_SDC R_0 R_1 ...
        R_X0 R_X1 R_Y0 R_Y1 R_Ylcm R_all X