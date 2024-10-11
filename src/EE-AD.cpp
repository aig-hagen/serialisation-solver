#include "Algorithms.h"

namespace Algorithms {
    void enumerate_admissible(AF & af, const IterableBitSet & active_arguments, std::vector<uint32_t> extension) {
        std::vector<std::vector<uint32_t>> extensions;
        
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial(af, active_arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            std::vector<uint32_t> new_extension(extension.size()+set.size());
            for (size_t i = 0; i < extension.size(); i++) {
                new_extension[i] = extension[i];
            }
            for (size_t i = 0; i < set.size(); i++) {
                new_extension[extension.size()+i] = set[i];
            }
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_admissible(af, new_active_arguments, new_extension);
        }

        // TODO filter out duplicates

        if (true) { // termination condition
            print_extension(af, extension);
            std::cout << ",";
        }
    }

    void enumerate_admissible(AF & af, const IterableBitSet & active_arguments) {
        enumerate_admissible(af, active_arguments, {});
    }
}