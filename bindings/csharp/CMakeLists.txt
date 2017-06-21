cmake_minimum_required( VERSION 2.8 )

if( OPENDAFF_BUILD_DAFFLIBS_SHARED )
	add_definitions( -DDAFF_DLL )
endif()

add_definitions( -DDAFFCS_EXPORTS )
add_library( DAFFCSWrapper SHARED "DAFFCSWrapper.h" "DAFFCSWrapper.cpp" )
set_property( TARGET DAFFCSWrapper PROPERTY FOLDER "DAFFLibs" )

target_link_libraries( DAFFCSWrapper DAFF )
if( ${FFTW_FOUND} )
	target_link_libraries( DAFFCSWrapper ${FFTW_LIBRARIES} )
endif( )
if( ${SNDFILE_FOUND} )
	target_link_libraries( DAFFCSWrapper ${SNDFILE_LIBRARIES} )
endif( )

install( TARGETS DAFFCSWrapper RUNTIME DESTINATION "bin" LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib" )
