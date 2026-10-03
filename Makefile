CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude
BUILD_DIR := build
BIN := $(BUILD_DIR)/bin/metodos_numericos

SRCS := $(shell find src -name '*.cpp')
OBJS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_BIN := $(BUILD_DIR)/bin/pruebas_metodos
LIB_OBJS := $(filter-out $(BUILD_DIR)/main.o,$(OBJS))

.PHONY: all clean run test

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(BIN)

test: $(LIB_OBJS)
	@mkdir -p $(dir $(TEST_BIN))
	$(CXX) $(CXXFLAGS) tests/pruebas_metodos.cpp $(LIB_OBJS) -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)
