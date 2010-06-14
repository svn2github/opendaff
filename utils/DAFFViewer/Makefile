#
#  OpenDAFF DAFFViewer Makefile
#
#  Autoren:	Frank Wefers  (Frank.Wefers@akustik.rwth-aachen.de)
#
#  Version:	$Id: Makefile,v 1.3 2010/02/08 18:55:49 fwefers Exp $
#

#
#  This Makefile contains the dependency generation and compile and link rules 
#

# Define link settings for the DAFF library
BUILD_MODE=release
BUILD_TYPE=static

# Inherit common settings and platform specific stuff
include ../../Makefile.common
include ../../Makefile.$(BUILD_PLATFORM)

# Update paths
INCLUDE_DIR := ../../$(INCLUDE_DIR)
LIB_DIR := ../../$(LIB_DIR)
BIN_DIR := ../../$(BIN_DIR)

# Source files
VIEWER_SRC := Src/AboutDialog.cpp \
              Src/InfoDialog.cpp \
              Src/MainWindow.cpp \
              Src/Main.cpp

DAFFViewer:
	$(MAKE_BIN_DIR)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE_DIR) -ISrc -IRes -I/usr/include/fox-1.6 $(VIEWER_SRC) -L$(LIB_DIR) -lDAFF -lFOX-1.6 -o $(BIN_DIR)/daffviewer

all: DAFFViewer

.PHONY: clean
clean:
	rm -f $(BIN_DIR)/daffviewer
	
.DEFAULT_GOAL := DAFFTool