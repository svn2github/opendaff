function [ freqs, phases, metadata ] = another_test_callback_function( alpha, beta, basepath )
    freqs = [20 25 30 60 90 120];
    
    channels = 1;
    phases = zeros(channels, length(freqs));
    metadata = [];
    
    for c=1:channels
        for f=1:length(freqs)
            phases(c,f) = alpha/360;
        end
        metadata = daff_metadata_addKey(metadata, 'Description', 'String', sprintf('Channel: %u, Alpha: %u, Beta: %u',c,alpha,beta));
    end   
        
end