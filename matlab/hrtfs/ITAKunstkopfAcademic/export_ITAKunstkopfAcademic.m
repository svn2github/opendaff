%
% OpenDAFF
%

% The ITA Kunstkopf Academic HRIR data set can be obtained from:
% http://www.akustik.rwth-aachen.de/cms/Technische-Akustik/Studium/~edgv/Lehrmaterialien/

addpath( '../..' ) % Find daff_write and daff_add_metadata function

metadata = [];
metadata = daff_add_metadata( metadata, 'Description', 'String', 'Generated OpenDAFF example directivity' );
metadata = daff_add_metadata( metadata, 'Creation date', 'String', date );
metadata = daff_add_metadata( metadata, 'License', 'String', 'CC BY-NC 4.0' );
metadata = daff_add_metadata( metadata, 'CC License Deed', 'String', 'https://creativecommons.org/licenses/by-nc/4.0/' );
metadata = daff_add_metadata( metadata, 'Generation script', 'String', 'Opendaff-v1.7/matlab/hrtfs/ITAKunstkopfAcademic/export_ITAKunstkopfAcademic.m' );
metadata = daff_add_metadata( metadata, 'Web Resource (2016)', 'String', 'http://www.akustik.rwth-aachen.de/cms/Technische-Akustik/Studium/~edgv/Lehrmaterialien/' );

config.basepath = 'ITA_Academic_HRIRs_2008-12-22/Standard';
metadata_standard = daff_add_metadata( metadata, 'Postprocessing', 'String', 'Untouched' );

daff_write( 'filename', 'ITAKunstkopfAcademicStandard.ir.daff', ...
            'content', 'ir', ...
            'datafunc', @dfITAKunstkopfAcademic, ...
			'userdata', config, ...
            'channels', 2, ...
            'alphares', 45, ...
            'betares', 45, ...
            'betarange', [ 90 180 ], ...
            'orient', [ 0 0 0 ], ...
            'quantization', 'float32', ...
            'metadata', metadata_standard );

			
config.basepath = 'ITA_Academic_HRIRs_2008-12-22/Free-field equalized';
metadata_equalized = daff_add_metadata( metadata, 'Postprocessing', 'String', 'Free-field pressure equalized' );

daff_write( 'filename', 'ITAKunstkopfAcademicFreeFieldEqualized.ir.daff', ...
            'content', 'ir', ...
            'datafunc', @dfITAKunstkopfAcademic, ...
			'userdata', config, ...
            'channels', 2, ...
            'alphares', 45, ...
            'betares', 45, ...
            'betarange', [ 90 180 ], ...
            'orient', [ 0 0 0 ], ...
            'quantization', 'float32', ...
            'metadata', metadata_equalized );
