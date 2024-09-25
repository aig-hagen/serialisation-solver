#include "Algorithms.h"

namespace Algorithms {
    void sequences_argument_admissible(AF & af, const IterableBitSet & active_arguments, uint32_t argument, std::vector<std::vector<uint32_t>> sequence) {
        bool contains_argument = false;
        bool attacks_argument = false;
        if (!sequence.empty()) {
            std::vector<uint32_t> set = sequence[sequence.size()-1];
            for (uint32_t arg : set) {
                if (argument==arg) {
                    contains_argument = true;
                    break;
                }
                for (size_t i = 0; i < af.attacked[arg].size(); i++) {
                    if (argument == af.attacked[arg][i]) {
                        attacks_argument = true;
                        break;
                    }
                }
                if (attacks_argument) {
                    break;
                }
            }
        }

        if (contains_argument) {
            print_sequence(af, sequence);
            std::cout << ",";
            return;
        }
        if (attacks_argument) {
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
            sequences_argument_admissible(af, new_active_arguments, argument, new_sequence);
        }
    }

    void sequences_argument_admissible(AF & af, const IterableBitSet & active_arguments, uint32_t argument) {
        std::vector<std::vector<uint32_t>> sequence(0);
        sequences_argument_admissible(af, active_arguments, argument, sequence);
    }
}