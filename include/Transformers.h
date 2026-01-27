#include "IterableBitSet.h"
#include "AF.h"

namespace tl {
    IterableBitSet to_ext(AF & af, const std::vector<std::vector<uint32_t>> & sequence) {
        std::vector<uint32_t> extension_array;
        std::vector<uint8_t> extension_bitset;
        extension_bitset.resize(af.args, false);
        extension_array.reserve(af.args);
        for (const std::vector<uint32_t> & set : sequence) {
            for (const uint32_t arg : set) {
                if (!extension_bitset[arg]) {
                    extension_bitset[arg] = true;
                    extension_array.push_back(arg);
                }
            }
        }

        return IterableBitSet(extension_array, extension_bitset);
    }

    IterableBitSet to_ext(AF & af, const std::vector<uint32_t> & arguments) {
        std::vector<uint8_t> extension_bitset;
        std::vector<uint32_t> extension_array;
        extension_bitset.resize(af.args, false);
        extension_array.reserve(arguments.size());
        for (const uint32_t arg : arguments) {
            extension_bitset[arg] = true;
        }

        return IterableBitSet(extension_array, extension_bitset);
    }
}