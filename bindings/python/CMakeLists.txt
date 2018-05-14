cmake_minimum_required( VERSION 2.8 )

project( OpenDAFF )

find_package( Python REQUIRED )

if( BUILD_SHARED_LIBS )
	message( WARNING "You are building shared libs, but it is _highly_ recommended to use static linking of OpenDAFF libraries for bindings." )
endif( )

if( PYTHON_INCLUDE_DIR )
	include_directories( "${PYTHON_INCLUDE_DIR}" )
endif( )

add_definitions( -DPy_NO_ENABLE_SHARED )

add_library( pydaff STATIC "pydaff.cpp" "pydaffdoc.hpp" "MANIFEST.in" "setup.py" )# Only add library here to check if it compiles in your preferred build env. CMake will run Python distutils to actually compile the extension, see below. 

target_link_libraries( pydaff DAFF )

set_property( TARGET pydaff PROPERTY FOLDER "Bindings/Python" )

# Run distutils after (useless) build in your dev env to compile the Python extension into 'dist' folder
configure_file( "setup.py.proto" "${CMAKE_CURRENT_SOURCE_DIR}/setup.py" @ONLY )
set( BATCH_SCRIPT_EXTENSION "bat" )
if( NOT WIN32 )
	set( BATCH_SCRIPT_EXTENSION "sh" )
endif( )
set( BATCH_SCRIPT_TYPE distutils_build_python_daff )
add_custom_command( TARGET pydaff POST_BUILD COMMAND "${BATCH_SCRIPT_TYPE}.${BATCH_SCRIPT_EXTENSION}" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" COMMENT "Running distutils" VERBATIM )


# install by copying DLL/SO next to python extension
set( SHARED_LIBRARY_EXTENSION "dll" )
if( NOT WIN32 )
	set( SHARED_LIBRARY_EXTENSION "so" )
endif( )

if( BUILD_SHARED_LIBS )
	# OpenDAFF shared libs
	install( FILES "${DAFF_LIBRARY_DIRS}/DAFF.${SHARED_LIBRARY_EXTENSION}" DESTINATION "python/Lib/site-packages" )
endif( )

# Tests
#install( DIRECTORY "tests" DESTINATION "python" )

# Examples
#install( DIRECTORY "examples" DESTINATION "python" )
