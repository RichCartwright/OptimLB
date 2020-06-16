SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := optimLB
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -std=c++2a -DARMA_64BIT_WORD=1 -march=native -ffp-contract=fast -I/usr/local/include/optim -I/usr/include/python3.8
CFLAGS := -Wall -O3 
LDFLAGS := -Llib -L/usr/local/lib
LDLIBS := -lm -loptim -fopenmp -lpython3.8

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
