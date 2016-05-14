% 
%  OpenDAFF - A free, open-source software package for directional audio data,
%             distributed under the terms of the GNU Lesser Public License (LGPL)
%              
%  (c) Copyright Institute of Technical Acoustics, RWTH Aachen University
%   
%  File:	generate%Directivities.m
%  Purpose:	Matlab script that generates a set of example directivities
%           for testing, debugging and playing around
%  Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
%

addpath( '..' )

basefolder = '..';

% 5x5 degree grid resolution
alpha_res = 5; 
beta_res = 5;

channels = 1;
orient = [0 0 0];

metadata = [];
metadata = daff_metadata_addKey( metadata, 'Description', 'String', 'Generated OpenDAFF example directivity' );
metadata = daff_metadata_addKey( metadata, 'Creation date', 'String', date );
metadata = daff_metadata_addKey( metadata, 'License', 'String', 'CC BY' );


% omnidirectional      
daff_write('filename', fullfile( basefolder, 'ExampleOmni.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfOmniMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient, ...
           'metadata', metadata);

% frontal dirac
daff_write('filename', fullfile( basefolder, 'ExampleFrontalDirac.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfFrontalDiracMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient, ...
           'metadata', metadata);
       
daff_write('filename', fullfile( basefolder, 'ExampleFrontalHemisphere.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfFrontalHemisphereMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);
       
daff_write('filename', fullfile( basefolder, 'ExampleUpperHemisphere.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfUpperHemisphereMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);
       
daff_write('filename', fullfile( basefolder, 'ExampleOmnidirectional.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfOmnidirectionalMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);
       
daff_write('filename', fullfile( basefolder, 'ExampleDipole.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfDipoleMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);
       
daff_write('filename', fullfile( basefolder, 'ExampleQuadrupole.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfQuadrupoleMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);
       
daff_write('filename', fullfile( basefolder, 'ExampleStar.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfStarMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient); 
       
daff_write('filename', fullfile( basefolder, 'ExampleDisc.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfDiscMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient); 

daff_write('filename', fullfile( basefolder, 'ExampleCube.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfCubeMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient);   

daff_write('filename', fullfile( basefolder, 'ExampleBulb.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfBulbMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient); 
       
daff_write('filename', fullfile( basefolder, 'ExampleThinBelt.MS.daff' ), ...
           'content', 'ms', ...
           'datafunc', @dfOmnidirectionalMS, ...
           'channels', channels, ...
           'alphares', alpha_res, ...
           'betares', beta_res, ...
           'orient', orient, ...
           'betarange', [80 100]);
