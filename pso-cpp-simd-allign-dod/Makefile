MODES := normal debug fast clean mem run

ifneq ($(filter gcc,$(MAKECMDGOALS)),)
    CXX := g++
    COMPILER_DIR := gcc
else
    CXX := clang++
    COMPILER_DIR := clang
endif

CXXFLAGS := -Wall -Werror -std=c++20

ifeq ($(CXX),clang++)
    CXXFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize
else ifeq ($(CXX),g++)
    CXXFLAGS += -fopt-info-vec -fopt-info-vec-missed
endif

LDFLAGS := 

SRCDIR := src
OBJDIR := obj/$(COMPILER_DIR)
BINDIR := bin/$(COMPILER_DIR)
ASMDIR := asm/$(COMPILER_DIR)
TARGET := $(BINDIR)/test

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

HAS_MODE := $(filter $(MODES),$(MAKECMDGOALS))

.PHONY: clang gcc all $(MODES)

ifeq ($(HAS_MODE),)
clang gcc: normal
else
clang gcc:
	@:
endif

$(shell mkdir -p $(OBJDIR) $(BINDIR) $(ASMDIR))

all: normal

debug: CXXFLAGS += -g -fsanitize=address,undefined,leak -march=native 
debug: $(TARGET)

normal: CXXFLAGS += -O2 
normal: $(TARGET)

fast: CXXFLAGS += -O3 -ffast-math -fno-trapping-math -fno-math-errno -march=native -flto
fast: LDFLAGS += -flto
fast: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build complete: $@ (Compiled with $(CXX))"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: 
	$(TARGET)

asm: $(TARGET)
	objdump -d -C $(TARGET) > $(ASMDIR)/test.asm
	@echo "Disassembly complete: $(ASMDIR)/test.asm"

clean:
	rm -rf obj bin asm
	@echo "Clean complete"

mem:
	valgrind -s $(TARGET)