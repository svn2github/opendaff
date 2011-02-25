function [ data, sampleRate, isSymetric, metadata ] = dft_test_callback_function( alpha, beta, basepath )
    isSymetric = true;
    
    numDFTCoeffs = 1024;
    sampleRate = 44100;
    data = zeros(1, numDFTCoeffs);
    metadata = [];
    
    data(1,:) = complex(hamming(numDFTCoeffs), (0:1/numDFTCoeffs:1-1/numDFTCoeffs)');
end