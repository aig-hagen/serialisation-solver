#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <cstdint>
#include "AF.h"

class IterableBitSet {
    public:
        std::vector<uint32_t> _array;
        std::vector<uint8_t> _bitset;

        IterableBitSet() {}

        IterableBitSet(std::vector<uint32_t> array, std::vector<uint8_t> bitset) : _array{ array }, _bitset { bitset } {}

        ~IterableBitSet() {
            _array.clear();
            _bitset.clear();
        }

        void clear() {
            _array.clear();
            _bitset.clear();
        }

};

#endif

/*
AF getReduct(const AF & af, IterableBitSet & arguments) {
	if (arguments._array.empty()) {
		return af;
	}
	AF reduct = AF();

	std::vector<uint32_t> reduced_arguments_arr;
    std::vector<uint8_t> reduced_arguments_bit(af.args);
	for (const uint32_t & arg: arguments._array) {
        reduced_arguments_arr.push_back(arg);
        reduced_arguments_bit[arg] = true;
        for (const uint32_t & j : af.attacked[arg]) {
            reduced_arguments_arr.push_back(j);
            reduced_arguments_bit[j] = true;
        }
	}
	for (uint32_t i = 0; i < af.args; i++) {
		if (!reduced_arguments_bit[i]) {
			reduct.add_argument(af.int_to_arg[i]);
		}
	}

	if (!reduct.args) {
		return reduct;
	}
	
	reduct.initialize_attackers();
	reduct.initialize_vars();

    for (uint32_t i = 0; i < af.args; i++) {
        for()
    }

	for (const auto& att : atts) {
		uint32_t source;
		uint32_t target;
		auto it1 = af.arg_to_int.find(att.first);
		if (it1 != af.arg_to_int.end()) {
			source = it1->second;
		} else {
			continue;
		}
		auto it2 = af.arg_to_int.find(att.second);
		if (it2 != af.arg_to_int.end()) {
			target = it2->second;
		} else {
			continue;
		}
		if (removed_args.count(source) == 0 && removed_args.count(target) == 0) {
			reduct.add_attack(att);
		}
	}
	return reduct;
}
*/