#
#  OpenDAFF Documentation Makefile 
#  Authors: Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de),
#             Jonas Stienen (stienen@akustik.rwth-aachen.de)
#  Date/Version: $Id: Makefile,v 1.1 2010/02/04 15:02:36 fwefers Exp $
#  (c) Copyright Institute of Technical Acoustics (ITA), RWTH Aachen University 2009-2010
#

#
#  Makefile for generating the ITAToolkit documentation files under Linux machines using doxygen (>= 1.5.5)
#

DOC_NAME        = OpenDAFF Documentation

DOX             = doxygen
DOX_CONFIG_FILE = cxx-api.doxyfile
DOX_HTML_TARGET = HTML
#DOX_STYLESHEET  = styles.css
DOX_IMAGES      = logo.png

.PHONY: clean
.PHONY: doc
.PHONY: info

all: doc

doc:
	$(MAKE) clean
	$(DOX) $(DOX_CONFIG_FILE)
	cp $(DOX_STYLESHEET) $(DOX_HTML_TARGET)
	cp $(DOX_IMAGES) $(DOX_HTML_TARGET)

clean:
	rm -rf $(DOX_HTML_TARGET)

info:
	@echo "--- Generate $(DOC_NAME) ---"
	@echo "Execute \`make\` to generate the documentation files to its target directory '$(DOX_HTML_TARGET)'."
	@echo "If there are already some older versions existing, they will be removed."
	@echo "Type in \`make clean\` to remove the documentation."
	@echo ""
	@echo "--- Settings ---"
	@echo "DOX             = $(DOX)"
	@echo "DOX_CONFIG_FILE = $(DOX_CONFIG_FILE)"
	@echo "DOX_HTML_TARGET = $(DOX_HTML_TARGET)"
	@echo "DOX_STYLESHEET  = $(DOX_STYLESHEET)"
	@echo "DOX_IMAGES      = $(DOX_IMAGES)"
