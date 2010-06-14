%
%  OpenDAFF
%
%  File:    exampleReadImpulseResponse.m
%  Purpose: Example for reading impulse response content
%  Author:  Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
%
%  $Id: exampleReadImpulseResponse.m,v 1.1 2010/02/08 14:02:19 fwefers Exp $
%

close all;
clear all;

% Try to open your file (result is a handle)
% (any failure will result in a Matlab error and halt the execution)

h = DAFF('open', 'ir.daff');

% Now you can get your hands on the properties (returned as a struct)
% Note: All important information on the file is contained in the properties

props = DAFF('getProperties', h)

% If you like, you can get the metadata as well, like this...
% The result is returned also as a struct

metadata = DAFF('getMetadata', h)

% Now we fetch the frontal impulse response record,
% in object coordinates (P0�,T0�)

data1 = DAFF('getNearestNeighbourRecord', h, 'object', 0, 0);

% Lets plot the impulse response

[nchannels, filterlength] = size(data1);
figure;
hold on
for i=1:nchannels
    plot(data1(i,:));
end

% Close your opened DAFF file when you finished reading

DAFF('close', h);

% Free the OpenDAFF Matlab extension
% Note: This will close all still opened files automatically

clear DAFF;
