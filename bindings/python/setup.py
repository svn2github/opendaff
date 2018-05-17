from distutils.core import setup, Extension

daffmodule = Extension('daff',
                    include_dirs = [ '../../include' ],
					sources = [ 'pydaff.cpp', 
								'../../src/DAFFMetadataImpl.cpp', 
								'../../src/DAFFReader.cpp', 
								'../../src/DAFFReaderImpl.cpp', 
								'../../src/DAFFSCTransform.cpp', 
								'../../src/DAFFUtils.cpp', 
								'../../src/Utils.cpp' ] )

setup ( name = 'pydaff',
        version = '17',
        description = 'OpenDAFF Python 3 binding',
        author = 'Institute of Technical Acoustics (ITA), RWTH Aachen University',
        author_email = 'post@akustik.rwth-aachen.de',
        url = 'https://www.opendaff.org',
        long_description = '''The OpenDAFF project is free, open source software package for directional audio data.''',
		license = "Copyright 2016-2018. Apache License, Version 2.0",
        ext_modules = [ daffmodule ]
		)
