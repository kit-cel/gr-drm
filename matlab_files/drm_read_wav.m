function [raw_pcm_stream] = drm_read_wav(filename)
% read wav file and reshape it. '.wav' may be omitted.
% wav file sampling frequency must be 24 kHz.

% read wav file
samples = wavread(filename, 'native');
samples = samples(:, 1); % take only one channel if it's stereo

% reshape vector
transf_length = 960; % see DRM standard (AAC transform length)

n = floor(length(samples)/transf_length); % number of whole input blocks
if n<1
    error('wav file too short');
end

samples = samples(1: n*transf_length);

samples = transpose(samples); % transpose for reshape
samples = reshape(samples, transf_length, n); % reshape
raw_pcm_stream = transpose(samples); % n rows, 960 columns

end