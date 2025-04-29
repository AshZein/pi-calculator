#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <cstdlib>

// Forward declaration
void calculate_pi(mpf_class &pi, unsigned long terms);

int main(int argc, char* argv[]) {
    const unsigned long PRECISION_BITS = 100000;
    const unsigned long DEFAULT_TERMS = 10;

    mpf_set_default_prec(PRECISION_BITS);

    // Get number of terms from argv, or use default
    unsigned long terms = DEFAULT_TERMS;
    if (argc >= 2) {
        terms = std::strtoul(argv[1], nullptr, 10);
        if (terms == 0) {
            std::cerr << "Invalid number of terms: " << argv[1] << "\n";
            return 1;
        }
    } else {
        std::cout << "No term count provided. Using default: " << DEFAULT_TERMS << " terms.\n";
    }

    mpf_class pi;
    calculate_pi(pi, terms);

    std::ofstream out("pi_output.txt");
    if (out.is_open()) {
        out.precision(pi.get_prec() * 0.30103); // bits to decimal digits
        out << std::fixed << pi << std::endl;
        out.close();
        std::cout << "π written to pi_output.txt using " << terms << " terms\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    return 0;
}
