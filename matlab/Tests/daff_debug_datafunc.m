function [ data, samplerate, metadata ] = daff_debug_datafunc( alpha, beta, basepath )
    data = [ beta*1000+alpha 0 0 0 ];
    samplerate = 44100;
    metadata = [];
end