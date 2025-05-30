CC := clang
CCFLAGS := -Wall -Werror -std=c11 
LDFLAGS := -lm
TARGET := bin/test

SRCDIR := src
OBJDIR := obj
BINDIR := bin

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

$(shell mkdir -p $(OBJDIR) $(BINDIR))

.PHONY: all debug normal fast run clean

all: normal

debug: CCFLAGS += -g -fsanitize=address,undefined,leak
debug: $(TARGET)

normal: CCFLAGS += -O2
normal: $(TARGET)

fast: CCFLAGS += -O3 -ffast-math
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