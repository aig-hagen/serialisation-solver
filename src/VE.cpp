#include "Algorithms.h"

namespace Algorithms {
    bool is_conflict_free(AF & af, const IterableBitSet & arguments) {
        for (const uint32_t & a : arguments._array) {
            for (const uint32_t & b : af.attacked[a]) {
                if (arguments._bitset[b]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_defended(AF & af, const IterableBitSet & arguments, uint32_t argument) {
        for (const uint32_t & attacker : af.attackers[argument]) {
            bool is_defended = false;
            for (const uint32_t & defender : af.attackers[attacker]) {
                if (arguments._bitset[defender]) {
                    is_defended = true;
                    break;
                }
            }
            if (!is_defended) {
                return false;
            }
        }
        return true;
    }

    bool is_admissible(AF & af, const IterableBitSet & arguments) {
        if (!is_conflict_free(af, arguments)) {
            return false;
        }
    
        for (const uint32_t & a : arguments._array) {
            if (!is_defended(af, arguments, a)) {
                return false;
            }
        }
        return true;
    }

    bool is_complete(AF & af, const IterableBitSet & arguments) {
        if (!is_admissible(af, arguments)) {
            return false;
        }
        for (uint32_t a=0; a < af.args; a++) {
            if (arguments._bitset[a]) {
                continue;
            }
            if (is_defended(af, arguments, a)) {
                return false;
            }
        }
        return true;
    }

    bool is_stable(AF & af, const IterableBitSet & arguments) {
        if (!is_conflict_free(af, arguments)) {
            return false;
        }
        for (uint32_t a=0; a < af.args; a++) {
            if (arguments._bitset[a]) {
                continue;
            }
            bool is_attacked = false;
            for (const uint32_t & attacker : af.attackers[a]) {
                if (arguments._bitset[attacker]) {
                    is_attacked = true;
                    break;
                }
            }
            if (!is_attacked) {
                return false;
            }
        }
        return true;
    }

    bool is_grounded(AF & af, const IterableBitSet & arguments) {
        // TODO
        return false;
    }

    bool is_preferred(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments) {
        IterableBitSet reduct = std::get_reduct(af, active_arguments, arguments._array);
        std::vector<std::vector<uint32_t>> sets = Algorithms::enumerate_initial(af, reduct);
        return sets.size() == 0;
    }
}