#include "Algorithms.h"

namespace Algorithms {
    void enumerate_sequences_unchallenged(const AF & af, const IterableBitSet & active_arguments, bool maximal, std::vector<std::vector<uint32_t>> sequence) {
        bool is_maximal = true;
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial(af, active_arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            bool is_challenged = false;
            std::vector<uint8_t> attackers(af.args, true);
            for (const uint32_t & arg1 : set) {
                for (const uint32_t & arg2 : active_arguments._array) {
                    attackers[arg2] |= af.attackers[arg1][arg2] && active_arguments._bitset[arg2];
                }
            }
            for (const std::vector<uint32_t> & set2 : initial_sets) {
                for (const uint32_t & arg : set2) {
                    if (attackers[arg]) {
                        is_challenged = true;
                        break;
                    }
                }
                if (is_challenged) break;
            }
            if (is_challenged) continue;

            is_maximal = false;
            std::vector<std::vector<uint32_t>> new_sequence(sequence.size()+1);
            for (size_t i = 0; i < sequence.size(); i++) {
                new_sequence[i] = sequence[i];
            }
            new_sequence[sequence.size()] = set;
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_sequences_unchallenged(af, new_active_arguments, maximal, new_sequence);
        }

        if (!maximal || is_maximal) { // termination condition
            print_sequence(af, sequence);
            std::cout << ",";
        }
    }

    void enumerate_sequences_unchallenged(const AF & af, const IterableBitSet & active_arguments, bool maximal) {
        enumerate_sequences_unchallenged(af, active_arguments, maximal, {});
    }
}