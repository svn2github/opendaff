function [ freqs, mags ] = dfDiscMS( alpha, beta, basepath )
% Example magnitude spectrum in disc shape weighted by frequency
           
    % Third-octave resolution
    freqs = [20 25 31.5 40 50 63 80 100 125 160 ...
             200 250 315 400 500 630 800 1000 1250 1600 ...
             2000 3150 4000 5000 6300 8000 10000 12500 16000 20000];
    
    channels = 1;
    mags = zeros(channels, length(freqs));
    
    for c=1:channels
        for f=1:length(freqs)
            %mags(c,f) = sin(beta*pi/180)^3/f;
            mags(c,f) = exp(-abs(90-beta)/30)/f;
        end
    end   
end


