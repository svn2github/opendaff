#
#  OpenDAFF DAFFTool Makefile
#
#  Autoren:	Frank Wefers  (Frank.Wefers@akustik.rwth-aachen.de)
#
#  Version:	$Id: Makefile,v 1.2 2010/02/08 18:55:49 fwefers Exp $
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
TOOL_SRC := src/DAFFTool.cpp

DAFFTool:
	$(MAKE_BIN_DIR)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE_DIR) $(TOOL_SRC) -L$(LIB_DIR) -lDAFF -lsndfile -o $(BIN_DIR)/dafftool

all: DAFFTool

.PHONY: clean
clean:
	rm -f $(BIN_DIR)/dafftool
	
.DEFAULT_GOAL := DAFFTool