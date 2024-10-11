#include "Algorithms.h"
#include <unordered_set>

// TODO make not global
std::unordered_set<std::unordered_set<uint32_t>> extensions;

namespace Algorithms {
    void enumerate_admissible(AF & af, const IterableBitSet & active_arguments, std::unordered_set<uint32_t> extension) {        
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial(af, active_arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            std::unordered_set<uint32_t> new_extension;
            for (uint32_t arg : extension) {
                new_extension.insert(arg);
            }
            for (size_t i = 0; i < set.size(); i++) {
                new_extension.insert(set[i]);
            }
            if (extensions.find(new_extension) != extensions.end()) {
                extensions.insert(new_extension);
                IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
                enumerate_admissible(af, new_active_arguments, new_extension);
            }
        }

        if (true) { // termination condition
            print_extension(af, extension);
            std::cout << ",";
        }
    }

    void enumerate_admissible(AF & af, const IterableBitSet & active_arguments) {
        enumerate_admissible(af, active_arguments, {});
    }
}