#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Encodings.h"

namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial(const AF & af, const IterableBitSet & active_arguments);
    void enumerate_sequences_admissible(const AF & af, const IterableBitSet & active_arguments, bool preferred);
};

namespace std {
    inline IterableBitSet get_reduct(const AF & af, const IterableBitSet & active_arguments, const std::vector<uint32_t> & arguments) {
        std::vector<uint32_t> reduct_array;
        std::vector<uint8_t> reduct_bitset;
        reduct_bitset.resize(af.args);

        for (uint32_t arg1 : active_arguments._array) {
            bool is_reduced = false;
            for (uint32_t arg2 : arguments) {
                if (arg1 == arg2) {
                    is_reduced = true;
                    break;
                }
                for (uint32_t arg3 : af.attacked[arg2]) {
                    if (arg1 == arg3) {
                        is_reduced = true;
                        break;
                    }
                }
                if (is_reduced) {
                    break;
                }
            }
            if (!is_reduced) {
                reduct_array.push_back(arg1);
                reduct_bitset[arg1] = true;
            }
        }

        return IterableBitSet(reduct_array, reduct_bitset);
    }
}

#endif