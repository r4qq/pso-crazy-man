import os
import subprocess
import re
from typing import List
import statistics

def compile_implementation(folder: str) -> bool:
    """Compile PSO implementation in the given folder."""
    try:
        subprocess.run(["make", "clean"], cwd=folder, check=True, capture_output=True)
        subprocess.run(["make", "fast"], cwd=folder, check=True, capture_output=True)
        print(f"Compiled {folder} successfully")
        return True
    except subprocess.CalledProcessError as e:
        print(f"Compilation failed in {folder}: {e.stderr.decode()}")
        return False

def run_implementation(folder: str, executable: str, runs: int) -> List[float]:
    """Run PSO implementation multiple times and collect runtimes."""
    runtimes = []
    for i in range(runs):
        try:
            result = subprocess.run([executable], cwd=folder, check=True, capture_output=True, text=True)
            output = result.stdout
            match = re.search(r"Time elapsed: (\d+\.\d+) s\.", output) # Extract runtime
            if match:
                runtime = float(match.group(1))
                runtimes.append(runtime)
                print(f"{folder} run {i+1}/{runs}: {runtime} s")
            else:
                print(f"{folder} run {i+1}/{runs}: Failed to parse runtime")
        except subprocess.CalledProcessError as e:
            print(f"{folder} run {i+1}/{runs}: Failed - {e.stderr}")
    return runtimes

def main():
    folders = ["./pso-cpp", "./pso-cpp-avx"]
    executable = "./bin/test"
    num_runs = 20

    results = {}
    for folder in folders:
        if not compile_implementation(folder):
            print(f"Skipping {folder} due to compilation failure")
            continue

        runtimes = run_implementation(folder, executable, num_runs)
        if runtimes:
            avg_runtime = statistics.mean(runtimes)
            results[folder] = {"runtimes": runtimes, "average": avg_runtime}
            print(f"\n{folder} Average Runtime: {avg_runtime:.3f} s")
        else:
            print(f"\n{folder} No successful runs")

    if len(results) == 2:
        scalar_avg = results["./pso-cpp"]["average"]
        avx_avg = results["./pso-cpp-avx"]["average"]
        print("\nComparison:")
        print(f"Scalar Average: {scalar_avg:.6f} s")
        print(f"AVX Average: {avx_avg:.6f} s")
        if avx_avg < scalar_avg:
            speedup = (scalar_avg - avx_avg) / scalar_avg * 100
            print(f"AVX is {speedup:.6f}% faster")
        else:
            slowdown = (avx_avg - scalar_avg) / avx_avg * 100
            print(f"Scalar is {slowdown:6f}% faster")

if __name__ == "__main__":
    main()