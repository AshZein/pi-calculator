import re
from collections import deque

def compare_pi_files(file1, file2, preview=100):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        def next_digit_gen(f):
            while True:
                c = f.read(1)
                if not c:
                    return
                if c == '3':
                    yield c
                    c = f.read(1)
                    if c == '.':
                        yield c
                        break
            while True:
                c = f.read(1)
                if not c:
                    break
                if c.isdigit():
                    yield c

        gen1 = next_digit_gen(f1)
        gen2 = next_digit_gen(f2)

        window1 = deque(maxlen=preview)
        window2 = deque(maxlen=preview)
        mismatch_index = -1
        i = 0

        while True:
            try:
                d1 = next(gen1)
            except StopIteration:
                d1 = ''
            try:
                d2 = next(gen2)
            except StopIteration:
                d2 = ''
            if not d1 and not d2:
                break
            window1.append(d1)
            window2.append(d2)
            if d1 != d2:
                mismatch_index = i
                break
            i += 1

        if mismatch_index == -1:
            print("All digits match (up to the shortest file length).")
            return True

        # Read ahead for context after mismatch
        after1 = []
        after2 = []
        for _ in range(preview):
            try:
                after1.append(next(gen1))
            except StopIteration:
                break
        for _ in range(preview):
            try:
                after2.append(next(gen2))
            except StopIteration:
                break

        print(f"Mismatch at digit #{mismatch_index}: {window1[-1]} ≠ {window2[-1]}")
        print("\nContext:")
        print(f"Generated  : ...{''.join(window1)}{''.join(after1)}...")
        print(f"Reference  : ...{''.join(window2)}{''.join(after2)}...")
        print(f"Matching digits before mismatch: {mismatch_index - 2}")  # exclude '3.'
        return False

if __name__ == "__main__":
    file1 = "/Users/ashz/Desktop/Projects/pi-calculator/calculated_outputs/terms_3000_threads_1_pi__output.txt"
    reference_file = "pi_refs/pi-billion.txt"
    compare_pi_files(file1, reference_file)

