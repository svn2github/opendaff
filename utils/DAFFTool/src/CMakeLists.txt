set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${DAFF_SOURCE_DIR})
find_package(Sndfile REQUIRED)

# Zusätzliche include-Verzeichnisse
include_directories(
	${DAFF_SOURCE_DIR}/utils/DAFFTool/src
	${DAFF_SOURCE_DIR}/include
	${DAFF_SOURCE_DIR}/src
	${SNDFILE_INCLUDE_DIRS}
)

# Hier wird definiert, welche Quellcodedateien zur Programmdatei dazugehören.
# Bei großen Projekten ist eine logische Unterteilung sinnvoll.
set(dafftool_SRCS
    DAFFTool.cpp
    Export.cpp
)

# Die auführbare Datei. Anstatt der Variablen können
# auch noch zusätzliche Dateien angegeben werden
add_executable (DAFFTool ${dafftool_SRCS})

# Die ausführbare Datei wird gegen diese Bibliotheken gelinkt.
target_link_libraries(DAFFTool
    daff
    ${SNDFILE_LIBRARY}
)

# Das Programm soll in den folgenden Unterordnern
# installiert werden.
install(TARGETS DAFFTool
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
