function [ freqs, mags, metadata ] = dfOmniLP6dBMS( ~, ~, ~ )
% Omnidirectional magnitude spectrum
           
    % Third-octave resolution
    freqs = [20 25 31.5 40 50 63 80 100 125 160 ...
             200 250 315 400 500 630 800 1000 1250 1600 ...
             2000 2500 3150 4000 5000 6300 8000 10000 12500 16000 20000];
    
    channels = 1;
    mags = zeros(channels, length(freqs));
    metadata = [];
    metadata = daff_metadata_addKey( metadata, 'Generator function', 'String', 'dfOmniLP6dBMS' );
    
    for c=1:channels
        for f=1:length(freqs)
            mags(c,f) = 1/(2*f);
        end
    end   
end


