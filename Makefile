CC=cc
CXX=c++
CFLAGS=-O3
CXXFLAGS=-O3 -std=c++11

BIN_DIR = bin

MAKE_RANDOM = $(BIN_DIR)/make_random
MAKE_CSVS = $(BIN_DIR)/make_csvs
SOLVE = $(BIN_DIR)/solve

MAKE_RANDOM_SRC = make_random.c
MAKE_CSVS_SRC = make_csvs.cpp
SOLVE_SRC = main.cpp

all: $(SOLVE) $(MAKE_RANDOM) $(MAKE_CSVS)
	@echo "Compiled"

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(MAKE_RANDOM): $(MAKE_RANDOM_SRC) | $(BIN_DIR)
	@echo "Compiling $(MAKE_RANDOM_SRC)..."
	$(CC) $(CFLAGS) $< -o $@

$(MAKE_CSVS): $(MAKE_CSVS_SRC) | $(BIN_DIR)
	@echo "Compiling $(MAKE_CSVS_SRC)..."
	$(CXX) $(CXXFLAGS) $< -o $@


$(SOLVE): $(SOLVE_SRC) | $(BIN_DIR)
	@echo "Compiling $(SOLVE_SRC)..."
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
