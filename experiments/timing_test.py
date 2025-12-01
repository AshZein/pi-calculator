import subprocess
import matplotlib.pyplot as plt
import os
from datetime import datetime

# Change the working directory to the parent directory
os.chdir("../src")

# Define the term increments
term_increments = [10, 100, 500, 1000, 5000, 10000]

# threads
threads = 6;

# Paths to executables
single_threaded_exec = ["./pi_calc", "-v"]
multi_threaded_exec = ["./pi_calc", "-t", str(threads), "-v"]

# Results storage
single_threaded_times = []
multi_threaded_times = []

# Ensure the results directory exists
results_dir = "experiments/results"
os.makedirs(results_dir, exist_ok=True)

# Run the tests
for terms in term_increments:
    print(f"Running tests for {terms} terms...")

    # Run single-threaded executable
    single_start = subprocess.run(
        single_threaded_exec + ["-d", str(terms)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    print(f"Single-threaded output for {terms} terms:\n{single_start.stdout}")  # Debugging output
    single_time = None
    for line in single_start.stdout.splitlines():
        if "Time taken to calculate π" in line:
            try:
                single_time = float(line.split(":")[1].strip().split()[0])
            except (IndexError, ValueError):
                print(f"Error parsing time for single-threaded run with {terms} terms.")
            break
    if single_time is not None:
        single_threaded_times.append(single_time)
    else:
        print(f"Error: Could not extract time for single-threaded run with {terms} terms.")
        single_threaded_times.append(None)

    # Run multi-threaded executable
    multi_start = subprocess.run(
        multi_threaded_exec + ["-d", str(terms)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    print(f"Multi-threaded output for {terms} terms:\n{multi_start.stdout}")  # Debugging output
    multi_time = None
    for line in multi_start.stdout.splitlines():
        if "Time taken to calculate π" in line:
            try:
                multi_time = float(line.split(":")[1].strip().split()[0])
            except (IndexError, ValueError):
                print(f"Error parsing time for multi-threaded run with {terms} terms.")
            break
    if multi_time is not None:
        multi_threaded_times.append(multi_time)
    else:
        print(f"Error: Could not extract time for multi-threaded run with {terms} terms.")
        multi_threaded_times.append(None)

os.chdir("../experiments/results")

# Plot the results
plt.figure(figsize=(10, 6))
plt.scatter(term_increments, single_threaded_times, color="blue", label="Single-Threaded", marker="o")
plt.scatter(term_increments, multi_threaded_times, color="red", label="Multi-Threaded", marker="x")
plt.plot(term_increments, single_threaded_times, color="blue", linestyle="--", alpha=0.7)
plt.plot(term_increments, multi_threaded_times, color="red", linestyle="--", alpha=0.7)

# Add labels, legend, and title
plt.xlabel("Number of Terms")
plt.ylabel("Time Taken (ms)")
plt.title(f"Execution Time: Single-Threaded vs Multi-Threaded ({threads}) π Calculation")
plt.legend()
plt.grid(True)

# Save the plot
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
output_file = f"sequential_threaded_time_{timestamp}.png"
plt.savefig(output_file)
print(f"Graph saved as {output_file}")

# Show the plot (optional)
plt.show()