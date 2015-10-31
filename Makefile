################################################################################
# Makefile for PCP 
#
# by (CPD-Minho)
################################################################################

SHELL = /bin/sh

PAR_NOWAIT = par_sem_nowait
PAR_SCHEDULLE = par_sem_schedulle

PAR = par
CXX = g++
LD  = g++

SEQ = seq
BIN = bin
BIN_SEQ = pcp_tp1_seq
BIN_PAR = pcp_tp1_par
BIN_PAR_NOWAIT = pcp_tp1_par_sem_nowait
BIN_PAR_SCHEDULLE = pcp_tp1_par_sem_schedulle

CXXFLAGS   = -O3 -Wall -Wextra -std=c++11 -fopenmp

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst src/*.cpp,build/*.o,$(SRC))
DEPS = $(patsubst build/*.o,build/*.d,$(OBJ))

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

$(BIN_DIR)/$(BIN_PAR_SCHEDULLE): $(BUILD_DIR)/$(PAR_SCHEDULLE).o $(BUILD_DIR)/$(PAR_SCHEDULLE).d 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(BUILD_DIR)/$(PAR_SCHEDULLE).o 

$(BIN_DIR)/$(BIN_PAR_NOWAIT): $(BUILD_DIR)/$(PAR_NOWAIT).o $(BUILD_DIR)/$(PAR_NOWAIT).d 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(BUILD_DIR)/$(PAR_NOWAIT).o 

checkdirs:
	@mkdir -p build 
	@mkdir -p src
	@mkdir -p timing
	@mkdir -p bin

all: checkdirs  $(BIN_DIR)/$(BIN_SEQ) $(BIN_DIR)/$(BIN_PAR) $(BIN_DIR)/$(BIN_PAR_SCHEDULLE)  $(BIN_DIR)/$(BIN_PAR_NOWAIT) 

clean:
	rm -f $(BUILD_DIR)/* $(BIN_DIR)/* 	
