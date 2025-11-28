CXX = g++

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS (Homebrew default paths)
    CXXFLAGS = -std=c++17 -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lgmp -lgmpxx
else
    # Linux (Ubuntu default paths)
    CXXFLAGS = -std=c++17 -I/usr/include
    LDFLAGS = -L/usr/lib -lgmp -lgmpxx
endif

PTHREAD_FLAGS = -pthread

# Targets
TARGET_SINGLE = pi_calc
TARGET_THREADED = pi_threaded_calc

# Source files
SRCS_SINGLE = chudnovsky.cpp main.cpp
OBJS_SINGLE = $(SRCS_SINGLE:.cpp=.o)

SRCS_THREADED = chudnovsky.cpp threaded_pi.cpp
OBJS_THREADED = $(SRCS_THREADED:.cpp=.o)

# Default target builds both executables
all: $(TARGET_SINGLE) $(TARGET_THREADED)

# Rule for non-threaded version
$(TARGET_SINGLE): $(OBJS_SINGLE)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule for threaded version
$(TARGET_THREADED): $(OBJS_THREADED)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(PTHREAD_FLAGS)

# Generic compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean both executables and all object files
clean:
	rm -f $(OBJS_SINGLE) $(OBJS_THREADED) $(TARGET_SINGLE) $(TARGET_THREADED)

# Build only the single-threaded version
single: $(TARGET_SINGLE)

# Build only the threaded version
threaded: $(TARGET_THREADED)
