CXX = clang++

# directories
SRC = src
INCLUDE = include
BUILD = build
BIN = bin
RUN = run

# C compiler arguments
ifeq ($(DEBUG),1)
CXXARGS := -g -O0 -I$(INCLUDE)/
LDARGS := 
COMPILEARGS := -c
EXEARGS = build/*.o

else
CXXARGS := -O2 -I$(INCLUDE)/
LDARGS := 
COMPILEARGS := -c
EXEARGS = build/*.o

endif

# platform specifics
ifeq ($(OS), Windows_NT)
MKDIR := mkdir
RMDIR := rmdir

else
MKDIR := mkdir -p
RMDIR := rm -r -rf

endif

# compiler commands
COMPILE := $(CXX) $(COMPILEARGS) $(CXXARGS) $(LDARGS)
EXE     := $(CXX) $(EXEARGS) $(CXXARGS) $(LDARGS)

# output executable
OUTPUT := clasp

default: clean $(BIN)/$(OUTPUT)

clean:
	$(RMDIR) $(BIN)
	$(RMDIR) $(BUILD)

$(BIN)/%: objs
	$(MKDIR) $(BIN)/
	$(EXE) -o $@ run/$(basename $(notdir $@)).cpp

objs:
	$(MKDIR) $(BUILD)/
	:$(foreach file, $(wildcard src/*.cpp), \
	&& $(COMPILE) $(file) -o build/$(basename $(notdir $(file))).o)