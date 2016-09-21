function [] = daffv17_convert_from_miro( miro_obj, target_file_path, additional_metadata )
%
% daffv17_convert_from_miro Converts a MIRO object into DAFF
%
% miro_obj                      MIRO object containing IR data sets
% target_file_path  [optional]  target file path (uses name otherwise)
%
% This file is part of OpenDAFF, http://www.opendaff.org
% To obtain MIRO content, see http://audiogroup.web.th-koeln.de
%

if nargin < 1
    error( 'No MIRO object given' )
elseif nargin < 2
    target_file_path = [ miro_obj.name '.ir.v17.daff' ];
    additional_metadata = [];
elseif nargin < 3
    additional_metadata = [];
end


%% Auto-add props to metadata

metadata = daffv17_add_metadata( additional_metadata, 'Converter Script', 'String', 'daffv17_convert_from_miro.m' );

metadata_field_names = fieldnames( miro_obj );
for i = 1:size( metadata_field_names, 1 )
    key = metadata_field_names{i};
    switch( key )
        case { 'irChOne', 'irChTwo', 'headPhone', 'headPhoneComp', ...
                'shutUp', 'irCenter', 'azimuth', 'elevation', ...
                'quadWeight' }
            continue;
    end
    
    val = miro_obj.( key );
    if isempty( val ) || ( size( val, 1 ) > 1 ) || ( size( val, 2 ) > 1000 )
        continue
    end
    
    if isnumeric( val )
        metadata = daffv17_add_metadata( metadata, key, 'Float', val );
    else
        metadata = daffv17_add_metadata( metadata, key, 'String', val );
    end
end


%% Convert angles

if ~strcmpi( miro_obj.quadGrid, 'Gauss-Leg. 16020SP (89E/180A)' )
    error( [ 'Incompatible grid ''' miro_obj.quadGrid '''' ] );
end

quadrature = miro_obj.getQuadrature;

alphanums = size( unique( quadrature( :, 1 ) ), 1 );
alpharange = rad2deg( [ min( miro_obj.azimuth ) max( miro_obj.azimuth ) ] );
alphares = diff( alpharange ) / ( alphanums - 1 );
if abs( alpharange( end ) + alphares ) - 360 < eps
    alpharange( end ) = 360.0;
end

betanums = size( unique( quadrature( :, 2 ) ), 1 );
betarange = rad2deg( [ min( miro_obj.elevation ) max( miro_obj.elevation ) ] );
betares = diff( betarange ) / ( betanums - 1 );

%$ Execute using data function 'dfMIRO'
daffv17_write( 'filename', target_file_path, ...
                'datafunc', @dfMIRO, ...
                'userdata', miro_obj, ...
                'metadata', metadata, ...
                'content', 'ir', ...
                'alphares', alphares, ...
                'alpharange', alpharange, ...
                'betares', betares, ...
                'betarange', betarange, ...
                'orient', [ 0 0 0 ], ...
                'channels', 2, ...
                'quantization', 'float32' );
            