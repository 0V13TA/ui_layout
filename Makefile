# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR := .
HEADERS_DIR := headers
COMPONENTS_DIR := components
BUILD_DIR := build
COMPONENTS_BUILD_DIR := $(BUILD_DIR)/components
STATIC_DIR := static

# Source files
MAIN_SRC := $(SRC_DIR)/main.cpp
COMPONENT_SRCS := $(wildcard $(COMPONENTS_DIR)/*.cpp)
COMPONENT_OBJS := $(patsubst $(COMPONENTS_DIR)/%.cpp,$(COMPONENTS_BUILD_DIR)/%.o,$(COMPONENT_SRCS))

# Target executable
TARGET := $(BUILD_DIR)/ui_app

# Default target
all: $(TARGET)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(COMPONENTS_BUILD_DIR)

# Main executable
$(TARGET): $(BUILD_DIR) $(COMPONENT_OBJS) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -I$(HEADERS_DIR) $(MAIN_SRC) $(COMPONENT_OBJS) -o $(TARGET) $(SFML_FLAGS)

# Component object files
$(COMPONENTS_BUILD_DIR)/%.o: $(COMPONENTS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADERS_DIR) -c $< -o $@

# Clean build
clean:
	rm -rf $(BUILD_DIR)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Install static resources (fonts, etc.)
install-static: $(BUILD_DIR)
	cp -r $(STATIC_DIR)/* $(BUILD_DIR)/

# Phony targets
.PHONY: all clean run install-static
