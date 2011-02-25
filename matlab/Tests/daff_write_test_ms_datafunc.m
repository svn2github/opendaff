function [ freqs, magnitudes, metadata ] = daff_write_test_ms_datafunc( alpha, beta, basepath )
    freqs = [20 25 30 60 90 120 144];
    magnitudes = [0 0 0 0 1 alpha/360 beta/360];
    metadata = [];
end