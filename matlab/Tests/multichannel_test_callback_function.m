function [ freqs, mags, metadata ] = multichannel_test_callback_function( alpha, beta, basepath )
% Example magnitude spectrum of bulb shape for higher frequencies
    
    % This is just an example directivity.
    % For low frequencies it is omnidirectional
    % For higher frequencies it focusses in the frontal direction

    % Third-octave resolution
    freqs = [20 25 31.5 40 50 63 80 100 125 160 ...
             200 250 315 400 500 630 800 1000 1250 1600 ...
             2000 3150 4000 5000 6300 8000 10000 12500 16000 20000];
    
    channels = 3;
    mags = zeros(channels, length(freqs));
    metadata = [];
    
    for c=1:channels
        for f=1:length(freqs)
            % Great circle angle - Spherical law of cosines
            phi = acos( cos(alpha*pi/180) * cos( (beta-90) *pi/180) ) * 180/pi;

            % Exponential weighting lamba
            % 20 Hz -> 0, 20 kHz -> 0.002
            lambda = 0.002 * freqs(f) / 10000;
            
            % Frequency weighting
            w = log(freqs(f)) / log(20000) / 2;
            if c==1
                mags(c,f) = (1-w) + w*exp(-lambda*phi^2);
            elseif c==2
                mags(c,f) = c*f/100;
            else
                mags(c,f) = freqs(f)/20000;
            end
        end
    end   
end