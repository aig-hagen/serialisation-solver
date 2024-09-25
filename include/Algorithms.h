#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Encodings.h"

#include <stack>

namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_admissible(AF & af, const IterableBitSet & active_arguments, bool preferred);
    void enumerate_sequences_strong_admissible(AF & af, const IterableBitSet & active_arguments, bool grounded);
    void enumerate_sequences_complete(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_stable(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_unchallenged(AF & af, const IterableBitSet & active_arguments, bool maximal);
    void sequences_argument_admissible(AF & af, const IterableBitSet & active_arguments, uint32_t argument);
};

namespace std {
    inline IterableBitSet get_reduct(const AF & af, const IterableBitSet & active_arguments, const std::vector<uint32_t> & arguments) {
        std::vector<uint32_t> reduct_array;
        std::vector<uint8_t> reduct_bitset;
        reduct_bitset.resize(af.args);
        reduct_array.reserve(active_arguments._array.size());

        for (const uint32_t arg1 : arguments) {
            reduct_bitset[arg1] = true;
            for (const uint32_t arg2 : af.attacked[arg1]) {
                reduct_bitset[arg2] = true;
            }
        }

        for (const uint32_t arg : active_arguments._array) {
            if (!reduct_bitset[arg]) {
                reduct_bitset[arg] = true;
                reduct_array.push_back(arg);
            } else {
                reduct_bitset[arg] = false;
            }
        }

        return IterableBitSet(reduct_array, reduct_bitset);
    }

    inline IterableBitSet get_cone(const AF & af, const IterableBitSet & active_arguments, uint32_t argument) {
        if (!active_arguments._bitset[argument]) {
            std::cerr << "Argument is not part of the framework!\n";
			exit(1);
        }
        std::vector<uint8_t> cone_bitset(af.args, false);
        std::vector<uint32_t> cone_array;
        cone_array.reserve(active_arguments._array.size());

		std::stack<uint32_t> stack;
		stack.push(argument);
		uint32_t arg;

		while (!stack.empty()) {
			arg = stack.top();
			stack.pop();
			if(!cone_bitset[arg]) {
				cone_bitset[arg] = true;
                cone_array.push_back(arg);
			}
			for (uint32_t i = 0; i < af.attackers[arg].size(); i++) {
				if (!cone_bitset[af.attackers[arg][i]] && active_arguments._bitset[af.attackers[arg][i]]) {
					stack.push(af.attackers[arg][i]);
				}
			}
		}

		return IterableBitSet(cone_array, cone_bitset);
    }
}

#endif