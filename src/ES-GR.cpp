#include "Algorithms.h"

namespace Algorithms {
    void enumerate_sequences_strong_admissible(AF & af, const IterableBitSet & active_arguments, bool grounded, std::vector<std::vector<uint32_t>> sequence) {
        bool is_maximal = true;
        for (const uint32_t & arg : active_arguments._array) {
            bool unattacked = true;
            for (const uint32_t & att : af.attackers[arg]) {
                if (active_arguments._bitset[att]) {
                    unattacked = false;
                    break;
                }
            }
            if (!unattacked) {
                continue;
            }

            is_maximal = false;
            
            std::vector<std::vector<uint32_t>> new_sequence(sequence.size()+1);
            for (size_t i = 0; i < sequence.size(); i++) {
                new_sequence[i] = sequence[i];
            }
            std::vector<uint32_t> set = {arg};
            new_sequence[sequence.size()] = set;
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_sequences_strong_admissible(af, new_active_arguments, grounded, new_sequence);

        }

        if (!grounded || is_maximal) { // termination condition
            print_sequence(af, sequence);
            std::cout << ",";
        }
    }

    void enumerate_sequences_strong_admissible(AF & af, const IterableBitSet & active_arguments, bool grounded) {
        enumerate_sequences_strong_admissible(af, active_arguments, grounded, {});
    }
}