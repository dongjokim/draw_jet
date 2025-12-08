# Makefile for SimpleCorrelation.C

# Detect operating system
UNAME_S := $(shell uname -s)

# Set compiler and flags based on platform
ifeq ($(UNAME_S),Darwin)
    # macOS settings
    CXX             = clang++
    CXXFLAGS        = -std=c++20 -O2 -g -arch arm64 \
                      -mmacosx-version-min=14.0 \
                      -Wno-unknown-warning-option \
                      -fPIC \
                      -stdlib=libc++
    LD              = clang++
else
    # Linux settings
    CXX             = g++
    CXXFLAGS        = -std=c++20 -O2 -g -fPIC
    LD              = g++
endif

SOFLAGS         = -shared -Wno-deprecated
CXXFLAGS        += $(shell root-config --cflags | sed 's/-std=c++17/-std=c++20/')
LIBS            = $(shell root-config --libs)

# Include directories
INCLUDES      = -I. -Isrc

# Source directories
SRC_DIR       = src
DICT_DIR      = .

# Source files
SRCS          = $(SRC_DIR)/AliJBaseTrack.cxx \
                $(SRC_DIR)/AliJBaseEventHeader.cxx \
                $(SRC_DIR)/JBaseTrack.cxx \
                $(SRC_DIR)/JBaseEventHeader.cxx \
                $(SRC_DIR)/JTreeDataManager.cxx \
                $(SRC_DIR)/JTreeDataManager_Pythia.cxx

# Object files
OBJS          = $(SRCS:.cxx=.o)

# Dictionary files
DICT_HEADERS  = $(SRC_DIR)/AliJBaseTrack.h \
                $(SRC_DIR)/AliJBaseEventHeader.h \
                $(SRC_DIR)/JBaseTrack.h \
                $(SRC_DIR)/JBaseEventHeader.h \
                $(SRC_DIR)/JTreeDataManager.h \
                $(SRC_DIR)/JTreeDataManager_Pythia.h

DICT_SRC      = SimpleDict.cxx
DICT_OBJ      = SimpleDict.o

# SimpleCorrelation implementation
SIMPLE_CORR_SRC = SimpleCorrelation.C
SIMPLE_CORR_OBJ = SimpleCorrelation.o

# Library name
LIBRARY       = libSimpleCorr.so

# Final executable
PROGRAM       = SimpleCorrelation
MAIN_SRC      = SimpleMain.C

# Compilation flags
CXXFLAGS     += $(INCLUDES)

# Default target
all: $(LIBRARY) $(PROGRAM)

# Rule for creating the shared library
$(LIBRARY): $(OBJS) $(DICT_OBJ) $(SIMPLE_CORR_OBJ)
	$(CXX) $(SOFLAGS) -o $@ $^ $(LIBS)
	@echo "$(LIBRARY) created successfully!"

# Rule for compiling the main program
$(PROGRAM): $(MAIN_SRC) $(LIBRARY)
	$(CXX) -o $@ $(MAIN_SRC) $(CXXFLAGS) -L. -lSimpleCorr $(LIBS)
	@echo "$(PROGRAM) compiled successfully!"

# Rule for compiling source files
%.o: %.cxx %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for SimpleCorrelation.C
$(SIMPLE_CORR_OBJ): $(SIMPLE_CORR_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for dictionary object file
$(DICT_OBJ): $(DICT_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for generating dictionary
$(DICT_SRC): $(DICT_HEADERS)
	@echo "Generating dictionary..."
	rootcling -f $@ -c -p $(INCLUDES) $^

# Create the main wrapper file
$(MAIN_SRC):
	@echo "Creating main wrapper..."
	@echo '#include <iostream>' > $@
	@echo '#include "TROOT.h"' >> $@
	@echo '#include "TSystem.h"' >> $@
	@echo '#include "TString.h"' >> $@
	@echo '' >> $@
	@echo 'int SimpleCorrelation(TString inputfile, TString outputfile);' >> $@
	@echo '' >> $@
	@echo 'int main(int argc, char** argv) {' >> $@
	@echo '    TString inputFile = "input_trees.txt";' >> $@
	@echo '    TString outputFile = "simple_correlation.root";' >> $@
	@echo '' >> $@
	@echo '    if (argc > 1) inputFile = argv[1];' >> $@
	@echo '    if (argc > 2) outputFile = argv[2];' >> $@
	@echo '' >> $@
	@echo '    std::cout << "Running SimpleCorrelation with:" << std::endl;' >> $@
	@echo '    std::cout << "  Input file: " << inputFile << std::endl;' >> $@
	@echo '    std::cout << "  Output file: " << outputFile << std::endl;' >> $@
	@echo '' >> $@
	@echo '    gSystem->Load("libSimpleCorr.so");' >> $@
	@echo '    return SimpleCorrelation(inputFile, outputFile);' >> $@
	@echo '}' >> $@

# Clean target
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(DICT_OBJ) $(DICT_SRC) SimpleDict_rdict.pcm $(PROGRAM) $(LIBRARY) $(MAIN_SRC) $(SIMPLE_CORR_OBJ)
	@echo "Clean completed!"

# Phony targets
.PHONY: all clean
