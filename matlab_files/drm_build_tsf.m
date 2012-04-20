function [stream_out] = drm_build_tsf(stream_in, channel_params)

% builds the transmission super frame and appends the N_L dummy cells
MSC = channel_params;
a_16 = 1/sqrt(10);
stream_out = [ repmat(stream_in, 1, MSC.M_TF), a_16.*[1 + 1i, 1 - 1i] ];

end