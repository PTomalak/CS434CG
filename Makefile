# Compiler
CXX = g++-13

# Flags
CXXFLAGS = -fopenmp -Wno-error=missing-field-initializers -lSDL2 -lSDL2main -I/usr/include/SDL2 -lm -I$(INC_DIR) -O3 -g -march=native

# Directories
SRC_DIR = ./src
INC_DIR = ./include
MAC_DIR1 = /usr/local/Cellar
MAC_DIR2 = /opt/homebrew/Cellar
OBJ_DIR = ./obj
EXE_DIR = ./exe

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard imgui/*.cpp)
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
