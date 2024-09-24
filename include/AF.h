#ifndef AF_H
#define AF_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdint>     		// uncomment if boostlibs are not installed

class AF {
public:

AF();

uint32_t args;

std::vector<std::vector<uint32_t>> attackers;
std::vector<std::vector<uint32_t>> attacked;

int32_t accepted_var(uint32_t arg);
int32_t rejected_var(uint32_t arg);

void set_arguments(uint32_t n_args);
void add_attack(uint32_t source, uint32_t target);

};

namespace std {
	inline void print_extension(const AF & af, const std::vector<uint32_t> & extension) {
		std::cout << "[";
		for (size_t i = 0; i < extension.size(); i++) {
			std::cout << extension[i]+1;
			if (i != extension.size()-1) std::cout << ",";
		}
		std::cout << "]";
  	}

  	inline void print_sequence(const AF & af, std::vector<std::vector<uint32_t>> sequence) {
		std::cout << "(";
		for (size_t i = 0; i < sequence.size(); i++) {
			print_extension(af, sequence[i]);
			if (i != sequence.size()-1) std::cout << ",";
		}
		std::cout << ")";
  	}
}

#endif