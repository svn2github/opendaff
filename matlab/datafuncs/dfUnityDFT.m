function [ data, sampleRate, isSymetric, metadata ] = dfUnityDFT( ~, ~, ~ )
% Omnidirectional discrete Fourier transform
    
    channels = 1;
    bins = 128;
    data = zeros( channels, length( bins ) );
    metadata = [];
    isSymetric = true;
    sampleRate = 44100;
    
    for c=1:channels
        for f=1:length( bins )
            data( c, f ) = 1.0 + 1i*0.0;
        end
    end   
end
