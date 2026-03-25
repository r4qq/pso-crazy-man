# Particle Swarm Optimization (PSO)

This repository contains various implementations of the Particle Swarm Optimization algorithm, ranging from a Python reference implementation to highly optimized C and C++ versions using SIMD instructions, memory alignment, and Data-Oriented Design (DOD).

## Project Structure

* **pso-python/**: Reference implementation using Python and NumPy.
* **pso-c/**: Standard C implementation.
* **pso-c-simd/**: C implementation optimized with SIMD instructions.
* **pso-cpp/**: Standard C++ implementation.
* **pso-cpp-simd/**: C++ implementation optimized with SIMD instructions.
* **pso-cpp-simd-allign/**: C++ implementation with Simd instructions and aligned memory allocation.

## Requirements

* **Python**: Python 3.x, NumPy, Matplotlib (for the Python implementation).
* **C/C++**: Clang (at least Calng 17 for C++20 support) or GCC (at least GCC 13 for C++20 support).
* **Build System**: GNU Make.
* **Tools**: Valgrind (optional, for memory debugging).

## Building and Running

### Python
Run the Python script directly:
```bash
python3 pso-python/pso.py
```

### C and C++

Each C and C++ subdirectory contains a Makefile with the following targets:

  *  make (or make normal): Compiles with -O2 optimization.

  *  make fast: Compiles with -O3, -ffast-math, and -march=native.

  * make debug: Compiles with debug symbols (-g) and address sanitizers.

  *  make mem: Runs the executable with Valgrind.

  *  make clean: Removes build artifacts.

  *  make run: Runs current build.

### Example Usage:

```bash
cd pso-cpp-simd
make fast
make run
```