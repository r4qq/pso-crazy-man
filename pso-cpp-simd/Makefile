CXX := clang++
CXXFLAGS := -Wall -Werror -std=c++17 

ifeq ($(CXX),clang++)
    CXXFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize
else ifeq ($(CXX),g++)
    CXXFLAGS += -fopt-info-vec -fopt-info-vec-missed
endif

LDFLAGS := 
TARGET := bin/test

SRCDIR := src
OBJDIR := obj
BINDIR := bin

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

$(shell mkdir -p $(OBJDIR) $(BINDIR))

.PHONY: all debug normal fast run clean mem

all: normal

debug: CXXFLAGS += -g -fsanitize=address,undefined,leak 
debug: $(TARGET)

normal: CXXFLAGS += -O2 
normal: $(TARGET)

fast: CXXFLAGS += -O3 -ffast-math -fno-trapping-math -fno-math-errno -march=native -flto
fast: LDFLAGS += -flto
fast: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: 
	$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete"

mem:
	valgrind -s $(TARGET)