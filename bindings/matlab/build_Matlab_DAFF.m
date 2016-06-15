%
% -------------------------------------------------------------------------------------
%
%  OpenDAFF - A free, open source software package for directional audio data
%  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
%
%  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
%
%  ------------------------------------------------------------------------------------
%
%  Visit the OpenDAFF homepage: http://www.opendaff.org
%
%  ------------------------------------------------------------------------------------
%
%  License and warranty notes
%
%  OpenDAFF is free software, distributed under the terms of the
%  GNU Lesser General Public License (LGPL) version 3.
%  You can redistribute it and/or modify it under the terms of the
%  GNU Lesser General Public License (LGPL) version 3,
%  as published by the Free Software Foundation.
%
%  This program is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
%  See the GNU General Public License for more details.
%
%  You should have received a copy of the GNU General Public License
%  along with this program.  If not, see <http://www.gnu.org/licenses/>.
%
%  ------------------------------------------------------------------------------------
%
%

clear all;
clc;

% Determine the C++ compiler configuration
cxx = mex.getCompilerConfigurations('C++', 'Selected');
if isempty(cxx)
    error('No C++ compiler installed or selected for Matlab. Please run ''mex -setup''.');
end

% --= Options =--

% Output directory
outdir = '../../matlab';

% Output file
outfile = fullfile(outdir, ['DAFF.' mexext]);

% Source files
srcs = {'DAFFMEX.cpp', ...
        'Helpers.cpp', ...
        '../../src/DAFFReader.cpp', ...
        '../../src/DAFFReaderImpl.cpp', ...
        '../../src/DAFFMetadataImpl.cpp', ...
        '../../src/DAFFSCTransform.cpp', ...
        '../../src/DAFFUtils.cpp', ...
        '../../src/Utils.cpp'};

% ---------------

fprintf('Building OpenDAFF Matlab executable using ''%s''...\n', cxx.Name)

% Create the destination directory if it does not exist
if ~exist(outdir, 'dir'), mkdir(outdir); end

% Compile and link
srcs = fullfile(srcs);
srcs = sprintf('%s ', srcs{:});

% Note: We need to call mex via 'system' here, because we used the symbol
% 'mex' with '.' above and Matlab would complain when doing 'mex ...'
cmd = sprintf('mex -O -I../../include -I../../src %s -output %s', ...
              srcs, outfile);
% Debug: disp(cmd);
[errorcode, result] = system(cmd, '-echo');
if (errorcode ~= 0), error('Building OpenDAFF Matlab executable failed'); end;

fprintf('\nOpenDAFF Matlab executable successfully built!\n\nOutput MEX file: ''%s''\n\n', outfile)
