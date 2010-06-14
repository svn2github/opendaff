%
%  OpenDAFF
%
%  File:    make.m
%  Purpose: Make script for building the OpenDAFF Matlab extension
%  Author:  Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
%
%  $Id: make.m,v 1.2 2010/02/05 08:45:19 fwefers Exp $
%

clear all;

% Fix me: Error "'mex' is used here as the name of a function or command"
% is this is uncommented
%cxx = mex.getCompilerConfigurations('C++', 'Selected');

outname = 'DAFF';
outsuffix = mexext;
outdir = '../../build/Matlab';
outfile = fullfile(outdir, [outname '.' outsuffix]);

% Later: fprintf('Building OpenDAFF Matlab extension using ''%s''...\n', cxx.Name)
fprintf('Building OpenDAFF Matlab extension ...\n')

% Create the destination directory if it does not exist
if ~exist(outdir, 'dir'), mkdir(outdir); end

% Compile and link
% TODO: Matlab variables in MEX-Call: 
% mex -O -Isrc -I../../include -I../../src src/DAFFMEX.cpp src/Helpers.cpp ../../src/DAFFReader.cpp ../../src/DAFFReaderImpl.cpp ../../src/DAFFMetadataImpl.cpp ../../src/DAFFSCTransform.cpp ../../src/DAFFUtils.cpp ../../src/Utils.cpp -outdir ''outdir'' -output ''outname''
mex -O -Isrc -I../../include -I../../src src/DAFFMEX.cpp src/Helpers.cpp ../../src/DAFFReader.cpp ../../src/DAFFReaderImpl.cpp ../../src/DAFFMetadataImpl.cpp ../../src/DAFFSCTransform.cpp ../../src/DAFFUtils.cpp ../../src/Utils.cpp -outdir ../../build/Matlab -output DAFF

fprintf('\nOpenDAFF Matlab extension successfully built!\n\nOutput file: ''%s''\n', outfile)
