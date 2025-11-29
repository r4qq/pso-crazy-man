CC := clang
CCFLAGS := -Wall -Werror -std=c11 

ifeq ($(CC),clang)
    CCFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize
else ifeq ($(CC),gcc)
    CCFLAGS += -fopt-info-vec -fopt-info-vec-missed
endif

LDFLAGS := -lm
TARGET := bin/test

SRCDIR := src
OBJDIR := obj
BINDIR := bin

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

$(shell mkdir -p $(OBJDIR) $(BINDIR))

.PHONY: all debug normal fast run clean mem

all: normal

debug: CCFLAGS += -g -fsanitize=address,undefined,leak 
debug: $(TARGET)

normal: CCFLAGS += -O2
normal: $(TARGET)

fast: CCFLAGS += -O3 -ffast-math -fno-trapping-math -fno-math-errno -march=native -flto
fast: LDFLAGS += -flto
fast: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

run: 
	$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete"

mem:
	valgrind -s $(TARGET)