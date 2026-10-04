CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude
BUILD_DIR := build
BIN := $(BUILD_DIR)/bin/metodos_numericos

# Excluye los fuentes de la GUI (Qt): este Makefile compila la versión de
# consola, que no requiere Qt/cmake/pkg-config. La GUI se construye con
# CMake dentro del contenedor Docker (ver Dockerfile y CMakeLists.txt).
GUI_SRCS := src/main_gui.cpp src/metodos_numericos/ui/VentanaPrincipal.cpp src/metodos_numericos/ui/GraficoWidget.cpp
SRCS := $(filter-out $(GUI_SRCS),$(shell find src -name '*.cpp'))
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
