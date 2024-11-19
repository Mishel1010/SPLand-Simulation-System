# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Project structure
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/app

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Default rule to build the application
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -rf $(BUILDDIR)

# Run the application
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run