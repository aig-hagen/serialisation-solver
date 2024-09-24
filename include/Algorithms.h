#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Encodings.h"

namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_admissible(AF & af, const IterableBitSet & active_arguments, bool preferred);
    void enumerate_sequences_strong_admissible(AF & af, const IterableBitSet & active_arguments, bool grounded);
    void enumerate_sequences_complete(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_stable(AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_unchallenged(AF & af, const IterableBitSet & active_arguments, bool maximal);
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
}

#endif