#include "Algorithms.h"
#include <unordered_set>
#include <random>

int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::vector<std::vector<uint32_t>> Algorithms::generate_false_sequence(AF & af, const IterableBitSet & active_arguments, semantics semantics) {
    // generate a "serialisation sequence" that corresponds a non-extension
    // adding some argument again
    // adding some argument that is in conflict with prior arguments
    // adding a set that contains a conflict
    // adding a set with an undefended argument
    // stopping before complete
    // stooping before preferred
    // stopping before stable
    std::vector<std::vector<uint32_t>> sequence;
    std::unordered_set<uint32_t> current_extension;
    std::vector<uint32_t> set;
    IterableBitSet reduct = std::get_reduct(af, active_arguments, {});

    // generate a random sequence of initial sets
    while (true) {
        set.clear();
        std::vector<uint32_t> unattacked;
        for (const uint32_t arg : reduct._array) {
            bool is_attacked = false;
            for (const uint32_t & arg : af.attackers[arg]) {
                if (reduct._bitset[arg]) {
                    is_attacked = true;
                    break;
                }
            }
            if (!is_attacked) {
                unattacked.push_back(arg);
            }
        }

        // add unattacked initial sets as a single set to candidates
        std::vector<std::vector<uint32_t>> initial_sets;
        if (!unattacked.empty()) {
            initial_sets.push_back(unattacked);
        }
        IterableBitSet reduct1 = std::get_reduct(af, reduct, unattacked); // TODO this may reveal further unattacked initial sets
        for (std::vector<uint32_t> is : Algorithms::enumerate_initial(af, reduct1)) {
            initial_sets.push_back(is);
        }

        if (initial_sets.empty()) {
            break;
        }
        set = initial_sets[randomInt(0, initial_sets.size() -1)];
        sequence.push_back(set);
        reduct = std::get_reduct(af, reduct, set);
    }

    if (sequence.empty()) {
        std::cerr << "empty sequence" << std::endl;
        exit(1); // TODO handle this case
    }

    switch (randomInt(0,3)) {
        case 0: // add some argument again
        {
            size_t seq_idx = randomInt(1, sequence.size() -1);
            std::vector<uint32_t> candidates;
            for (size_t i = 0; i < seq_idx; i++) {
                for (uint32_t arg : sequence[i]) {
                    candidates.push_back(arg);
                }
            }
            size_t arg_idx = randomInt(0, candidates.size() -1);
            uint32_t arg = candidates[arg_idx];
            sequence[seq_idx].push_back(arg);
            break; // TODO add option to replace chosen initial set
        } case 1: // add some argument that is in conflict with prior arguments
        {
            int seq_idx = randomInt(1, sequence.size() -1);
            std::vector<uint32_t> candidates;
            for (size_t i = 0; i < seq_idx; i++) {
                for (uint32_t arg : sequence[i]) {
                    for (uint32_t attacked : af.attacked[arg]) {
                        candidates.push_back(attacked);
                    }
                }
            }
            if (candidates.empty()) {
                std::cerr << "no candidate for adding conflict" << std::endl;
                exit(1); // TODO handle this case
            }
            int arg_idx = randomInt(0, candidates.size() -1);
            uint32_t arg = candidates[arg_idx];
            sequence[seq_idx].push_back(arg);
            break; // TODO add option to replace chosen initial set
        } case 2: // add a set that contains a conflict
        {
            int seq_idx = randomInt(0, sequence.size() -1);
            std::vector<uint32_t> arguments;
            for (size_t i = 0; i < seq_idx; i++) {
                for (uint32_t arg : sequence[i]) {
                    arguments.push_back(arg);
                }
            }
            reduct = std::get_reduct(af, active_arguments, arguments);
            uint32_t arg = reduct._array[randomInt(0, reduct._array.size() -1)];
            std::vector<uint32_t> candidates;
            for (uint32_t attacked : af.attacked[arg]) {
                if (!reduct._bitset[attacked]) continue;
                candidates.push_back(attacked);
            }
            for (uint32_t attacker : af.attackers[arg]) {
                if (!reduct._bitset[attacker]) continue;
                candidates.push_back(attacker);
            }
            if (candidates.empty()) {
                std::cerr << "no candidate for adding conflict" << std::endl;
                exit(1); // TODO handle this case
            }
            std::vector<uint32_t> conflict_set = { arg, candidates[randomInt(0, candidates.size() -1)] };
            sequence[seq_idx] = conflict_set;
            break;
        } case 3: // add a set with an undefended argument
        {
            size_t seq_idx = randomInt(0, sequence.size() -1);
            std::vector<uint32_t> arguments;
            for (size_t i = 0; i < seq_idx; i++) {
                for (uint32_t arg : sequence[i]) {
                    arguments.push_back(arg);
                }
            }
            reduct = std::get_reduct(af, active_arguments, arguments);
            std::vector<uint32_t> candidates;
            std::vector<uint8_t> defended = std::vector<uint8_t>(af.args, false);
            for (std::vector<uint32_t> initial_set : Algorithms::enumerate_initial(af, reduct)) {
                if (initial_set.size() == 1) {
                    defended[initial_set[0]] = true;
                }
            }
            for (uint32_t arg : reduct._array) {
                if (!defended[arg]) {
                    candidates.push_back(arg);
                }
            }
            if (candidates.empty()) {
                std::cerr << "no candidate for adding undefended argument" << std::endl;
                exit(1); // TODO handle this case
            }
            std::vector<uint32_t> undefended_set = { candidates[randomInt(0, candidates.size() -1)] };
            sequence[seq_idx] = undefended_set;
            break;
        } case 4: // non-complete sequence
        {
            // TODO implement
        }
    }
    return sequence;
}