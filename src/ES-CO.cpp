#include "Algorithms.h"

namespace Algorithms {
    void enumerate_sequences_complete(const AF & af, const IterableBitSet & active_arguments, std::vector<std::vector<uint32_t>> sequence) {
        bool has_unattacked_initialset = false;
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial(af, active_arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            if (set.size() == 1) {
                bool is_attacked = false;
                for (const uint32_t & arg : af.attackers[set[0]]) {
                    if (active_arguments._bitset[arg]) {
                        is_attacked = true;
                        break;
                    }
                }
                has_unattacked_initialset |= !is_attacked;
            }
            std::vector<std::vector<uint32_t>> new_sequence(sequence.size()+1);
            for (size_t i = 0; i < sequence.size(); i++) {
                new_sequence[i] = sequence[i];
            }
            new_sequence[sequence.size()] = set;
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_sequences_complete(af, new_active_arguments, new_sequence);
        }

        if (!has_unattacked_initialset) { // termination condition
            print_sequence(af, sequence);
            std::cout << ",";
        }
    }

    void enumerate_sequences_complete(const AF & af, const IterableBitSet & active_arguments) {
        enumerate_sequences_complete(af, active_arguments, {});
    }
}