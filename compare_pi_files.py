import re

def load_pi_digits(filename):
    with open(filename, 'r') as f:
        content = f.read().strip()

    # Keep only the leading "3." and digits
    match = re.match(r'^3\.(\d+)', content.replace('\n', '').replace(' ', ''))
    if not match:
        raise ValueError(f"File {filename} does not start with valid '3.xxx' format.")
    
    return '3.' + match.group(1)

def compare_pi(pi1, pi2, preview=20):
    mismatch_index = -1
    for i, (d1, d2) in enumerate(zip(pi1, pi2)):
        if d1 != d2:
            mismatch_index = i
            break

    if mismatch_index == -1:
        print("All digits match (up to the shortest file length).")
        return True

    print(f"Mismatch at digit #{mismatch_index}: {pi1[mismatch_index]} ≠ {pi2[mismatch_index]}")
    start = max(0, mismatch_index - preview)
    end = mismatch_index + preview

    print("\nContext:")
    print(f"Threaded   : ...{pi1[start:end]}...")
    print(f"Single-threaded: ...{pi2[start:end]}...")
    print(f"Matching digits before mismatch: {mismatch_index - 2}")  # exclude '3.'
    
    return False


if __name__ == "__main__":
    file1 = "calculated_outputs/pi_threaded_output.txt"
    file2 = "calculated_outputs/pi_single_output.txt"

    pi1 = load_pi_digits(file1)
    pi2 = load_pi_digits(file2)

