function [ freqs, mags, metadata ] = dfUnityMS( ~, ~, unity_ms_config )
% Omnidirectional magnitude spectrum
           
    if ~isempty( fieldnames( unity_ms_config ) )
        freqs = unity_ms_config.freqs;
        channels = unity_ms_config.channels;
    else
        % Third-octave resolution
        freqs = [20 25 31.5 40 50 63 80 100 125 160 ...
                 200 250 315 400 500 630 800 1000 1250 1600 ...
                 2000 2500 3150 4000 5000 6300 8000 10000 12500 16000 20000];

        channels = 1;
    end
    
    mags = zeros( channels, numel( freqs ) );
    metadata = [];
    
    for c=1:channels
        for f=1:length(freqs)
            mags(c,f) = 1.0;
        end
    end   
end
