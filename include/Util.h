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

IterableBitSet get_reduct(IterableBitSet & active_arguments, const AF & af, const IterableBitSet & arguments) {
    std::vector<uint32_t> reduct_array;
    std::vector<uint8_t> reduct_bitset;
    reduct_bitset.resize(af.args);

    for (uint32_t arg1 : active_arguments._array) {
        bool is_reduced = false;
        for (uint32_t arg2 : arguments._array) {
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

#endif