#
#  OpenDAFF Toplevel Makefile 
#
#  Authoren: Frank Wefers  (Frank.Wefers@akustik.rwth-aachen.de)
#            Jonas Stienen (Jonas.Stienen@akustik.rwth-aachen.de)
#
#  Date/Version: $Id: Makefile,v 1.5 2010/03/09 09:12:36 fwefers Exp $
#
#  (c) Copyright Institute of Technical Acoustics (ITA), RWTH Aachen University 2009-2010
#

#
#  This makefile is the toplevel makefile of the whole project.
#  It just acts as a nice and handy frontend and calls other specialized makefiles.
#

.PHONY: all
all: static shared

.PHONY: lib
lib: static shared

.PHONY: static
static: debug_static release_static

.PHONY: shared
shared: debug_shared release_shared

.PHONY: debug
debug: debug_static debug_shared

.PHONY: release
release: release_static release_shared

#
#  Important: We need to perform the dependency-file-enforcement in this makefile
#             on top of the Makefile.project makefile. Never remove the 'depend'
#             in the succeeding build rules, unless you want to end up in tears!
# 

.PHONY: debug_static
debug_static:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=static" depend lib

.PHONY: debug_shared
debug_shared:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=shared" depend lib

.PHONY: release_static
release_static:
	$(MAKE) -f Makefile.project "BUILD_MODE=release" "BUILD_TYPE=static" depend lib

.PHONY: release_shared
release_shared:
	$(MAKE) -f Makefile.project "BUILD_MODE=release" "BUILD_TYPE=shared" depend lib

.PHONY: clean
clean:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=static" clean
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=shared" clean
	$(MAKE) -f Makefile.project "BUILD_MODE=release" "BUILD_TYPE=static" clean
	$(MAKE) -f Makefile.project "BUILD_MODE=release" "BUILD_TYPE=shared" clean

.PHONY: info
info:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=static" info

.PHONY: tests
tests:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=static" tests

.PHONY: utils
tests:
	$(MAKE) -f Makefile.project "BUILD_MODE=debug"   "BUILD_TYPE=static" utils
		
.DEFAULT_GOAL := lib
