# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror
SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR := .
BUILD_DIR := build
STATIC_DIR := static
COMPONENTS_DIR := components
COMPONENTS_BUILD_DIR := $(BUILD_DIR)/$(COMPONENTS_DIR)

# Source files
MAIN_SRC := $(SRC_DIR)/main.cpp
COMPONENTS_SRCS := $(wildcard $(COMPONENTS_DIR)/*.cpp)
COMPONENTS_OBJS := $(patsubst $(COMPONENTS_DIR)/%.cpp, $(COMPONENTS_BUILD_DIR)/%.o, $(COMPONETS_SRCS))

# Target executable
TARGET := $(BUILD_DIR)/app

# Default target
all: $(TARGET)
	@echo "Compiling the project..."

# Create build directories
$(BUILD_DIR):
	@echo "Creating build directories..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COMPONENTS_BUILD_DIR)

# Main executable
$(TARGET): $(BUILD_DIR) $(COMPONENT_OBJS) $(MAIN_SRC)
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(MAIN_SRC) $(COMPONENT_OBJS) -o $(TARGET) $(SFML_FLAGS)

# Component object files
$(COMPONENTS_BUILD_DIR)/%.o: $(COMPONENTS_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

# Clean build
clean:
	@echo "Cleaning the build files..."
	@rm -rf $(BUILD_DIR)

# Run the application
run: $(TARGET)
	@echo "Starting..."
	@./$(TARGET)

# Install static resources (fonts, etc.)
install-static: $(BUILD_DIR)
	@echo "Installing the static files..."
	@cp -r $(STATIC_DIR)/* $(BUILD_DIR)/

# Phony targets
.PHONY: all clean run install-static

