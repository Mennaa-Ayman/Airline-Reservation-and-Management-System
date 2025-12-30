# Project settings
INCLUDE_DIR = include
SOURCE_DIR  = src
BUILD_DIR   = build

# Tools and flags
CXX       = g++
CPPFLAGS  = -I$(INCLUDE_DIR)
CXXFLAGS  = -std=c++17 -Wall -Wextra -O2
LDFLAGS   =
LDLIBS    =

# Source files
SRCS := $(wildcard $(SOURCE_DIR)/*.cpp)

# Object files
OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Target executable
TARGET = AirlineReservationSystem.exe

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@echo Linking...
	$(CXX) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	@echo Build complete: $(TARGET)

# Compile .cpp -> .o
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	@echo Compiling $< ...
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir $(BUILD_DIR)

# Clean
clean:
	@echo Cleaning...
	@if exist "$(BUILD_DIR)" rmdir /S /Q $(BUILD_DIR)
	@if exist "$(TARGET)" del /Q $(TARGET)
	@echo Done.

# Run the program
run: all
	@echo Running $(TARGET)...
	@.\$(TARGET)

# Force rebuild
rebuild: clean all

.PHONY: all clean rebuild run
