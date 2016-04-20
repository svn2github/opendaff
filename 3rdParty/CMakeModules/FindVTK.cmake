# OpenDAFF
#
# VTK_LIBRARIES library names
# VTK_INCLUDE_DIR, where to find the headers
# VTK_LIBRARY_DIRS folder where libraries can be found
# VTK_FOUND TRUE if found
#

if( NOT DEFINED VTK_ROOT_DIR )
	set( VTK_ROOT_DIR "${OPENDAFF_THIRD_PARTY_DIR}/vtk" CACHE PATH "Path to VTK folder with bin, include, lib folder structure" )
endif( NOT DEFINED VTK_ROOT_DIR )

find_path( VTK_INCLUDE_DIR "vtkAssembly.h"
    "$ENV{VTK_DIR}/include"
    "/usr/local/include"
    "/opt/local/include"
    "${OPENDAFF_THIRD_PARTY_DIR}/vtk/include"
	"${VTK_ROOT_DIR}/include"
	"${VTK_ROOT_DIR}/include/vtk-6.1"
)

find_path( VTK_LIBRARY_DIRS "vtkCommonCore-6.1.lib"
    "${OPENDAFF_THIRD_PARTY_DIR}/vtk/lib"
	"${VTK_ROOT_DIR}/lib"
    "${OPENDAFF_THIRD_PARTY_DIR}/vtk/bin"
	"${VTK_ROOT_DIR}/bin"
)

set( VTK_FOUND "" )
if( VTK_LIBRARY_DIRS AND VTK_INCLUDE_DIR )
    set( VTK_FOUND "YES" )
	set( VTK_LIBRARIES 
		optimized vtkalglib-6.1 debug vtkalglib-6.1D
		optimized vtkCommonCore-6.1 debug vtkCommonCore-6.1D
		optimized vtkexpat-6.1 debug vtkexpat-6.1D
		optimized vtkFiltersCore-6.1 debug vtkFiltersCore-6.1D
		optimized vtkfreetype-6.1 debug vtkfreetype-6.1D
		optimized vtkftgl-6.1 debug vtkftgl-6.1D
		#optimized vtkGraphics-6.1 debug vtkGraphics-6.1
		#optimized vtkHybrid-6.1 debug vtkGraphics-6.1
		optimized vtkIOCore-6.1 debug vtkIOCore-6.1D
		optimized vtkjpeg-6.1 debug vtkjpeg-6.1D
		optimized vtkpng-6.1 debug vtkpng-6.1D
		optimized vtkRenderingCore-6.1 debug vtkRenderingCore-6.1D
		optimized vtksys-6.1 debug vtksys-6.1D
		optimized vtktiff-6.1 debug vtktiff-6.1D
		optimized vtkzlib-6.1 debug vtkzlib-6.1D
		)
    message( STATUS "Found VTK includes: ${VTK_INCLUDE_DIR}" )
    message( STATUS "Found VTK libraries: ${VTK_LIBRARIES}" )
    message( STATUS "Found VTK library directories: ${VTK_LIBRARY_DIRS}" )
endif( VTK_LIBRARY_DIRS AND VTK_INCLUDE_DIR )
