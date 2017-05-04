%
% -------------------------------------------------------------------------------------
%
%  OpenDAFF - A free, open source software package for directional audio data
%
%  ------------------------------------------------------------------------------------
%
%  Visit the OpenDAFF website: http://www.opendaff.org
%
%  ------------------------------------------------------------------------------------
%
%  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
%
%  Licensed under the Apache License, Version 2.0 (the "License");
%  you may not use the OpenDAFF software package except in compliance with the License.
%  You may obtain a copy of the License at
%
%    http://www.apache.org/licenses/LICENSE-2.0
%
%  Unless required by applicable law or agreed to in writing, software
%  distributed under the License is distributed on an "AS IS" BASIS,
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
%  See the License for the specific language governing permissions and
%  limitations under the License.
%
%  ------------------------------------------------------------------------------------
%


%% Library settings

% Determine the C++ compiler configuration
cxx = mex.getCompilerConfigurations( 'C++', 'Selected' );
if isempty( cxx )
    error( 'No C++ compiler installed or selected for Matlab. Please run ''mex -setup''.' )
end

% Output file
outfile = fullfile( [ 'DAFFv17.' mexext ] );

% Source files
srcs = {'DAFFMexMain.cpp', ...
        'DAFFMexHelpers.cpp', ...
        '../../src/DAFFReader.cpp', ...
        '../../src/DAFFReaderImpl.cpp', ...
        '../../src/DAFFMetadataImpl.cpp', ...
        '../../src/DAFFSCTransform.cpp', ...
        '../../src/DAFFUtils.cpp', ...
        '../../src/Utils.cpp'};

    
%% Build

fprintf( 'Building OpenDAFF Matlab executable\n\tCompiler: ''%s''\n\tOutput: ''%s''\n', cxx.Name, outfile )

% Compile and link
srcs = fullfile( srcs );
srcs = sprintf( '%s ', srcs{:} );

% Note: We need to call mex via 'system' here, because we used the symbol
% 'mex' with '.' above and Matlab would complain when doing 'mex ...'
cmd = sprintf( 'mex -O -I../../include -I../../src %s -output %s', srcs, outfile );
% Debug: disp(cmd);

[ errorcode, result] = system( cmd, '-echo' );
if errorcode ~= 0
    error( 'Building OpenDAFF Matlab executable failed' )
end

fprintf( 'OpenDAFF Matlab executable successfully built to ''%s''\n', outfile )
