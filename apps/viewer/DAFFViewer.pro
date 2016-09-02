#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T17:54:37
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DAFFViewer
TEMPLATE = app


SOURCES += \
    src/DAFFViewer.cpp \
    src/QDAFFViewerWindow.cpp \
    src/QDAFF2DPlot.cpp

HEADERS  += \
    src/QDAFF2DPlot.h \
    src/QDAFFViewerWindow.h \
    src/QDAFFMetadataTable.h \
    src/QDAFFPropertiesTable.h \
    src/QDAFFHeaderGroupBox.h\
    src/QDAFFVTKWidget.h \
    src/QDAFF2DPlot.h

FORMS    += \
    ui/QDAFFViewerWindow.ui

INCLUDEPATH += src

# OpenDAFF
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFF -lDAFFViz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFFD -lDAFFVizD
else:unix: LIBS += -L$$PWD/../../../win32-x64.vc12/lib/ -lDAFF

INCLUDEPATH += $$PWD/../../../win32-x64.vc12/include
DEPENDPATH += $$PWD/../../../win32-x64.vc12/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFF.a $$PWD/../../../win32-x64.vc12/lib/libDAFFViz.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFFD.a  $$PWD/../../../win32-x64.vc12/lib/libDAFFVizDa
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/DAFF.lib $$PWD/../../../win32-x64.vc12/lib/DAFFViz.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/DAFFd.lib $$PWD/../../../win32-x64.vc12/lib/DAFFVizD.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../win32-x64.vc12/lib/libDAFF.a $$PWD/../../../win32-x64.vc12/lib/libDAFFViz.a

# VTK
win32:CONFIG(release, debug|release): LIBS += -LC:/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib -LX:/VR/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib -lglu32 -lopengl32 -luser32 -lgdi32 -lkernel32 -ladvapi32 -lvtkViewsQt-7.0 -lvtkRenderingQt-7.0 -lvtkGUISupportQtSQL-7.0 -lvtkGUISupportQt-7.0 -lvtkalglib-7.0 -lvtkChartsCore-7.0 -lvtkCommonColor-7.0 -lvtkCommonDataModel-7.0 -lvtkCommonMath-7.0 -lvtkCommonCore-7.0 -lvtksys-7.0 -lvtkCommonMisc-7.0 -lvtkCommonSystem-7.0 -lvtkCommonTransforms-7.0 -lvtkInfovisCore-7.0 -lvtkFiltersExtraction-7.0 -lvtkCommonExecutionModel-7.0 -lvtkFiltersCore-7.0 -lvtkFiltersGeneral-7.0 -lvtkCommonComputationalGeometry-7.0 -lvtkFiltersStatistics-7.0 -lvtkImagingFourier-7.0 -lvtkImagingCore-7.0 -lvtkRenderingContext2D-7.0 -lvtkRenderingCore-7.0 -lvtkFiltersGeometry-7.0 -lvtkFiltersSources-7.0 -lvtkRenderingFreeType-7.0 -lvtkfreetype-7.0 -lvtkzlib-7.0 -lvtkDICOMParser-7.0 -lvtkDomainsChemistry-7.0 -lvtkIOXML-7.0 -lvtkIOGeometry-7.0 -lvtkIOCore-7.0 -lvtkIOXMLParser-7.0 -lvtkexpat-7.0 -lvtkDomainsChemistryOpenGL2-7.0 -lvtkRenderingOpenGL2-7.0 -lvtkImagingHybrid-7.0 -lvtkIOImage-7.0 -lvtkmetaio-7.0 -lvtkjpeg-7.0 -lvtkpng-7.0 -lvtktiff-7.0 -lvtkglew-7.0 -lvtkexoIIc-7.0 -lvtkNetCDF-7.0 -lvtkNetCDF_cxx-7.0 -lvtkhdf5_hl-7.0 -lvtkhdf5-7.0 -lvtkFiltersAMR-7.0 -lvtkParallelCore-7.0 -lvtkIOLegacy-7.0 -lvtkFiltersFlowPaths-7.0 -lvtkFiltersGeneric-7.0 -lvtkFiltersHybrid-7.0 -lvtkImagingSources-7.0 -lvtkFiltersHyperTree-7.0 -lvtkFiltersImaging-7.0 -lvtkImagingGeneral-7.0 -lvtkFiltersModeling-7.0 -lvtkFiltersParallel-7.0 -lvtkFiltersParallelImaging-7.0 -lvtkFiltersProgrammable-7.0 -lvtkFiltersSelection-7.0 -lvtkFiltersSMP-7.0 -lvtkFiltersTexture-7.0 -lvtkFiltersVerdict-7.0 -lvtkverdict-7.0 -lvtkGeovisCore-7.0 -lvtkInfovisLayout-7.0 -lvtkInteractionStyle-7.0 -lvtkInteractionWidgets-7.0 -lvtkRenderingAnnotation-7.0 -lvtkImagingColor-7.0 -lvtkRenderingVolume-7.0 -lvtkViewsCore-7.0 -lvtkproj4-7.0 -lvtkGUISupportQt-7.0 -lvtkGUISupportQtSQL-7.0 -lvtkIOSQL-7.0 -lvtksqlite-7.0 -lvtkImagingMath-7.0 -lvtkImagingMorphological-7.0 -lvtkImagingStatistics-7.0 -lvtkImagingStencil-7.0 -lvtkInteractionImage-7.0 -lvtkIOAMR-7.0 -lvtkIOEnSight-7.0 -lvtkIOExodus-7.0 -lvtkIOExport-7.0 -lvtkRenderingLabel-7.0 -lvtkIOImport-7.0 -lvtkIOInfovis-7.0 -lvtklibxml2-7.0 -lvtkIOLSDyna-7.0 -lvtkIOMINC-7.0 -lvtkIOMovie-7.0 -lvtkoggtheora-7.0 -lvtkIONetCDF-7.0 -lvtkIOParallel-7.0 -lvtkjsoncpp-7.0 -lvtkIOParallelXML-7.0 -lvtkIOPLY-7.0 -lvtkIOVideo-7.0 -lvtkRenderingContextOpenGL2-7.0 -lvtkRenderingImage-7.0 -lvtkRenderingLOD-7.0 -lvtkRenderingQt-7.0 -lvtkRenderingVolumeOpenGL2-7.0 -lvtkViewsContext2D-7.0 -lvtkViewsInfovis-7.0 -lvtkViewsQt-7.0
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib -LX:/VR/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib -lglu32 -lopengl32 -luser32 -lgdi32 -lkernel32 -ladvapi32 -lvtkViewsQt-7.0D -lvtkRenderingQt-7.0D -lvtkGUISupportQtSQL-7.0D -lvtkGUISupportQt-7.0D -lvtkalglib-7.0D -lvtkChartsCore-7.0D -lvtkCommonColor-7.0D -lvtkCommonDataModel-7.0D -lvtkCommonMath-7.0D -lvtkCommonCore-7.0D -lvtksys-7.0D -lvtkCommonMisc-7.0D -lvtkCommonSystem-7.0D -lvtkCommonTransforms-7.0D -lvtkInfovisCore-7.0D -lvtkFiltersExtraction-7.0D -lvtkCommonExecutionModel-7.0D -lvtkFiltersCore-7.0D -lvtkFiltersGeneral-7.0D -lvtkCommonComputationalGeometry-7.0D -lvtkFiltersStatistics-7.0D -lvtkImagingFourier-7.0D -lvtkImagingCore-7.0D -lvtkRenderingContext2D-7.0D -lvtkRenderingCore-7.0D -lvtkFiltersGeometry-7.0D -lvtkFiltersSources-7.0D -lvtkRenderingFreeType-7.0D -lvtkfreetype-7.0D -lvtkzlib-7.0D -lvtkDICOMParser-7.0D -lvtkDomainsChemistry-7.0D -lvtkIOXML-7.0D -lvtkIOGeometry-7.0D -lvtkIOCore-7.0D -lvtkIOXMLParser-7.0D -lvtkexpat-7.0D -lvtkDomainsChemistryOpenGL2-7.0D -lvtkRenderingOpenGL2-7.0D -lvtkImagingHybrid-7.0D -lvtkIOImage-7.0D -lvtkmetaio-7.0D -lvtkjpeg-7.0D -lvtkpng-7.0D -lvtktiff-7.0D -lvtkglew-7.0D -lvtkexoIIc-7.0D -lvtkNetCDF-7.0D -lvtkNetCDF_cxx-7.0D -lvtkhdf5_hl-7.0D -lvtkhdf5-7.0D -lvtkFiltersAMR-7.0D -lvtkParallelCore-7.0D -lvtkIOLegacy-7.0D -lvtkFiltersFlowPaths-7.0D -lvtkFiltersGeneric-7.0D -lvtkFiltersHybrid-7.0D -lvtkImagingSources-7.0D -lvtkFiltersHyperTree-7.0D -lvtkFiltersImaging-7.0D -lvtkImagingGeneral-7.0D -lvtkFiltersModeling-7.0D -lvtkFiltersParallel-7.0D -lvtkFiltersParallelImaging-7.0D -lvtkFiltersProgrammable-7.0D -lvtkFiltersSelection-7.0D -lvtkFiltersSMP-7.0D -lvtkFiltersTexture-7.0D -lvtkFiltersVerdict-7.0D -lvtkverdict-7.0D -lvtkGeovisCore-7.0D -lvtkInfovisLayout-7.0D -lvtkInteractionStyle-7.0D -lvtkInteractionWidgets-7.0D -lvtkRenderingAnnotation-7.0D -lvtkImagingColor-7.0D -lvtkRenderingVolume-7.0D -lvtkViewsCore-7.0D -lvtkproj4-7.0D -lvtkGUISupportQt-7.0D -lvtkGUISupportQtSQL-7.0D -lvtkIOSQL-7.0D -lvtksqlite-7.0D -lvtkImagingMath-7.0D -lvtkImagingMorphological-7.0D -lvtkImagingStatistics-7.0D -lvtkImagingStencil-7.0D -lvtkInteractionImage-7.0D -lvtkIOAMR-7.0D -lvtkIOEnSight-7.0D -lvtkIOExodus-7.0D -lvtkIOExport-7.0D -lvtkRenderingLabel-7.0D -lvtkIOImport-7.0D -lvtkIOInfovis-7.0D -lvtklibxml2-7.0D -lvtkIOLSDyna-7.0D -lvtkIOMINC-7.0D -lvtkIOMovie-7.0D -lvtkoggtheora-7.0D -lvtkIONetCDF-7.0D -lvtkIOParallel-7.0D -lvtkjsoncpp-7.0D -lvtkIOParallelXML-7.0D -lvtkIOPLY-7.0D -lvtkIOVideo-7.0D -lvtkRenderingContextOpenGL2-7.0D -lvtkRenderingImage-7.0D -lvtkRenderingLOD-7.0D -lvtkRenderingQt-7.0D -lvtkRenderingVolumeOpenGL2-7.0D -lvtkViewsContext2D-7.0D -lvtkViewsInfovis-7.0D -lvtkViewsQt-7.0D

INCLUDEPATH += X:/VR/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/include/vtk-7.0
INCLUDEPATH += C:\dev\ExternalLibs\VTK\VTK-7.0\win32-x64.vc12\include/vtk-7.0
DEPENDPATH += X:/VR/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib
DEPENDPATH += C:/dev/ExternalLibs/VTK/VTK-7.0/win32-x64.vc12/lib


