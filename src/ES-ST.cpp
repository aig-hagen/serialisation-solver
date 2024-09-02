#include "Algorithms.h"

namespace Algorithms {
    void enumerate_sequences_stable(AF & af, const IterableBitSet & active_arguments, std::vector<std::vector<uint32_t>> sequence) {
        
        if (active_arguments._array.empty()) { // termination condition
            print_sequence(af, sequence);
            std::cout << ",";
            return;
        }
        
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial(af, active_arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            std::vector<std::vector<uint32_t>> new_sequence(sequence.size()+1);
            for (size_t i = 0; i < sequence.size(); i++) {
                new_sequence[i] = sequence[i];
            }
            new_sequence[sequence.size()] = set;
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_sequences_stable(af, new_active_arguments, new_sequence);
        }
    }

    void enumerate_sequences_stable(AF & af, const IterableBitSet & active_arguments) {
        enumerate_sequences_stable(af, active_arguments, {});
    }
}