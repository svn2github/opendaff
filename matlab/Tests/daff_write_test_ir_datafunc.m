function [ data, samplerate, metadata ] = daff_write_test_ir_datafunc( alpha, beta, basepath )
    data = [0 0 0 0 0 1 alpha/360 beta/360; 0 0 0 0 0 1 beta/360 alpha/360; 0 0 0 0 0 0 0 0];
    samplerate = 44100;
    metadata = [];
end