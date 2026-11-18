src_dir = src
bin_dir = output/bin

CC = clang++
CFLAGS = -Wall -O2 -std=c++14 -Isrc

# Find every .cpp in src/ and map src/foo.cpp -> output/bin/foo
sources = $(wildcard $(src_dir)/*.cpp)
programs = $(sources:$(src_dir)/%.cpp=$(bin_dir)/%)

# Default target: build all programs
all: $(programs)

# Pattern rule: compile one .cpp straight to a binary (no .o step)
$(bin_dir)/%: $(src_dir)/%.cpp | $(bin_dir)
	@echo ">>> Building $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

# Create the bin dir if it doesn't exist (order-only prerequisite)
$(bin_dir):
	mkdir -p $(bin_dir)

# `make run name=concurrency` builds then runs that one program
run: $(bin_dir)/$(name)
	@echo ">>> Running $(name)"
	@./$(bin_dir)/$(name)

clean:
	rm -rf $(bin_dir)

.PHONY: all run clean
