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
TARGET = pi_calc

# Source files
SRCS = chudnovsky.cpp main.cpp threaded_pi.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target builds both executables
all: $(TARGET)

# Rule for non-threaded version
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(PTHREAD_FLAGS)

# Generic compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean both executables and all object files
clean:
	rm -f *.o $(TARGET)
