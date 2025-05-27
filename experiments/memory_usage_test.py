import subprocess
import matplotlib.pyplot as plt
import os
import psutil
from datetime import datetime

# Change the working directory to the parent directory
os.chdir("../")

# Define the term increments
term_increments = [10, 100, 500, 1000, 5000, 10000]

# Paths to executables
single_threaded_exec = "./pi_calc"
multi_threaded_exec = "./pi_threaded_calc"

# Results storage
single_threaded_memory = []
multi_threaded_memory = []

# Ensure the results directory exists
results_dir = "./experiments/results"
os.makedirs(results_dir, exist_ok=True)

def measure_memory_usage(command):
    """Run a command and measure its peak memory usage."""
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    peak_memory = 0
    try:
        while process.poll() is None:  # While the process is running
            current_memory = psutil.Process(process.pid).memory_info().rss  # Resident Set Size (RSS)
            peak_memory = max(peak_memory, current_memory)
        process.communicate()  # Ensure the process finishes
    except psutil.NoSuchProcess:
        pass  # Process finished before we could measure
    return peak_memory / (1024 * 1024)  # Convert bytes to MB

# Run the tests
for terms in term_increments:
    print(f"Running memory usage tests for {terms} terms...")

    # Measure memory usage for single-threaded executable
    single_memory = measure_memory_usage([single_threaded_exec, str(terms)])
    single_threaded_memory.append(single_memory)
    print(f"Single-threaded memory usage for {terms} terms: {single_memory:.2f} MB")

    # Measure memory usage for multi-threaded executable
    multi_memory = measure_memory_usage([multi_threaded_exec, str(terms)])
    multi_threaded_memory.append(multi_memory)
    print(f"Multi-threaded memory usage for {terms} terms: {multi_memory:.2f} MB")

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(term_increments, single_threaded_memory, color="blue", label="Single-Threaded", marker="o")
plt.plot(term_increments, multi_threaded_memory, color="red", label="Multi-Threaded", marker="x")

# Add labels, legend, and title
plt.xlabel("Number of Terms")
plt.ylabel("Memory Usage (MB)")
plt.title("Memory Usage: Single-Threaded vs Multi-Threaded π Calculation")
plt.legend()
plt.grid(True)

# Save the plot
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
output_file = os.path.join(results_dir, f"memory_usage_{timestamp}.png")
plt.savefig(output_file)
print(f"Graph saved as {output_file}")

# Show the plot (optional)
plt.show()