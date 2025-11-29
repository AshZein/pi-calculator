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

# Source files (add validate_pi.cpp here)
SRCS = chudnovsky.cpp main.cpp threaded_pi.cpp validate_pi.cpp

# Auto-generate object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(PTHREAD_FLAGS)

# Compile each .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean
clean:
	rm -f *.o $(TARGET)
