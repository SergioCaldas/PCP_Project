################################################################################
# Makefile for PCP 
#
# by (CPD-Minho)
################################################################################

SHELL = /bin/sh

PAR = par
CXX = g++
LD  = g++

SEQ = seq
BIN = bin
BIN_SEQ = pcp_tp1_seq
BIN_PAR = pcp_tp1_par

CXXFLAGS   = -O3 -Wall -Wextra -std=c++11 -fopenmp

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst src/*.cpp,build/*.o,$(SRC))
DEPS = $(patsubst build/*.o,build/*.d,$(OBJ))
BIN = $(BIN_SEQ)

vpath %.cpp $(SRC_DIR)

################################################################################
# Rules
################################################################################

.DEFAULT_GOAL = all


$(BUILD_DIR)/%.d: %.cpp
	$(CXX) -M $(CXXFLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@

$(BIN_DIR)/$(BIN_SEQ): $(BUILD_DIR)/$(SEQ).o $(BUILD_DIR)/$(SEQ).d 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(BUILD_DIR)/$(SEQ).o 

$(BIN_DIR)/$(BIN_PAR): $(BUILD_DIR)/$(PAR).o $(BUILD_DIR)/$(PAR).d 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(BUILD_DIR)/$(PAR).o 

checkdirs:
	@mkdir -p build 
	@mkdir -p src

all: checkdirs  $(BIN_DIR)/$(BIN_SEQ) $(BIN_DIR)/$(BIN_PAR)

clean:
	rm -f $(BUILD_DIR)/* $(BIN_DIR)/* 	
