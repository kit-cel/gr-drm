function [raw_pcm_stream fs] = drm_read_wav(filename)
% read wav file and truncate it, if needed. '.wav' may be omitted.
% wav file sampling frequency must be 12 kHz. output is also written to
% binary file.

% read wav file
[samples fs] = wavread(filename, 'native');
samples = samples(:, 1); % take only one channel if it's stereo

% reshape vector
transf_length = 960; % see DRM standard (AAC transform length)

n = floor(length(samples)/transf_length); % number of whole input blocks
if n<1
    error('wav file too short');
end

% truncate samples to an integer multiple of transform length
raw_pcm_stream = samples(1: n*transf_length);

% write raw PCM stream to binary file
fid = fopen('raw_pcm.dat', 'w', 'native');
fwrite(fid, raw_pcm_stream, 'uint8', 'native');

end