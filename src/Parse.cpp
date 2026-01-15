#include <fstream>
#include <sstream>


#include "Parse.h"

IterableBitSet parse_i23(AF* aaf, std::string file) {
    std::ifstream input;
	input.open(file);

	if (!input.good()) {
		std::cerr << file << ": Cannot open input file\n";
		exit(1);
	}

	std::vector<uint32_t> active_array;
	std::vector<uint8_t> active_bitset;
	std::string line;
    uint32_t n_args, source, target;
		
    while (!input.eof()) {
        getline(input, line);
        if (line.length() == 0 || line[0] == '#') continue;
        std::istringstream iss(line);
        if (line[0] == 'p') {
            std::string p, af;
            iss >> p >> af >> n_args;
            active_bitset.resize(n_args, true);
            active_array.reserve(n_args);
            aaf->set_arguments(n_args);
            for (int i = 1; i <= n_args; i++) {
                active_array.push_back(i-1);
            }
        } else {
            iss >> source >> target;
            aaf->add_attack(source-1, target-1);
        }
    }
    input.close();

    return IterableBitSet(active_array, active_bitset);
}

IterableBitSet parse_extension(uint32_t n_args, std::string extension) {
	std::vector<uint32_t> active_array;
	std::vector<uint8_t> active_bitset;
	size_t start = extension.find_first_of('[');
    size_t end = extension.find_last_of(']');
    if (start == std::string::npos || end == std::string::npos || start >= end) {
        std::cerr << extension << ": Extension not well-formed!\n";
        exit(0);
    }

    std::stringstream ss(extension.substr(start + 1, end - start - 1));
    std::string item;
    uint32_t arg;

    active_array.reserve(n_args);
    active_bitset.resize(n_args,false);
    while (std::getline(ss, item, ',')) {
        if (!item.empty()) { // skip empty strings
            try {
                arg = std::stoi(item); // TODO check if within bounds
                if (arg < 1 || arg > n_args) {
                    std::cerr << "Argument " << arg << " is not within bound of the given AF" << std::endl;
                }
                arg--;
                active_array.push_back(arg);
                active_bitset[arg] = true;
            } catch (...) {
                // handle conversion error if needed
            }
        }
    }

    return IterableBitSet(active_array, active_bitset);
}