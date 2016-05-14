function [ data, samplerate, metadata ] = dfDiracIR( ~, ~, ~ )
% Omnidirectional unit impulse response

    channels = 1;
    filter_length = 32;
    samplerate = 44100.0;
    
    metadata = [];
    
    data = zeros( channels, filter_length );
    for c=1:size(data, 1)
        for f=1:size(data, 2)
            data(c,f,1) = 1.0; % set first value to 1
        end
    end   
end
