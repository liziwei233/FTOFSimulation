
name := DIRC
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

# ROOT support
CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir) 
EXTRALIBS = $(shell $(ROOTSYS)/bin/root-config --glibs)

