function [ data, samplerate, metadata ] = dfDiracIR( alpha, beta, ~ )
% Omnidirectional unit impulse response

    channels = 1;
    filter_length = 4;
    samplerate = 48000.0;
    
    metadata = [];
    metadata = daff_metadata_addKey( metadata, 'Alpha', 'String', num2str( alpha ) );
    metadata = daff_metadata_addKey( metadata, 'Beta', 'String', num2str( beta ) );
    
    data = zeros( channels, filter_length );
    for c=1:size(data, 1)
        data( c, 1 ) = 1.0; % set first value to 1
    end
end
