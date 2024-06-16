# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++20 -w -O2

# Include directories
INCLUDES = -I./include -I./thirdparty

# Source files
SRCS = src/main.cpp src/base64.cpp thirdparty/tinyxml2.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = main

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean

