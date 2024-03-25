# Compiler
CXX = g++

# Flags
CXXFLAGS = -Wno-error=missing-field-initializers -I$(INC_DIR) -O3 -march=native

# Directories
SRC_DIR = ./src
INC_DIR = ./include
OBJ_DIR = ./obj
EXE_DIR = ./exe

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INC_DIR)/*.h)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(EXE_DIR)/ray

# Targets
.PHONY: all clean run clear

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(EXE_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXE_DIR):
	mkdir -p $(EXE_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)

clear: clean all run

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(filter-out $@,$(MAKECMDGOALS))

%:
	@:
