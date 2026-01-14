#include "Algorithms.h"
#include "json.hpp"
#include <fstream>
#include <unordered_set>

using json = nlohmann::json;

namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial_subsets(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments) {
        if (active_arguments._array.empty()) {
            return {};
        }

        std::vector<std::vector<uint32_t>> result;
        std::vector<uint32_t> extension;
        std::vector<int32_t> minimization_clause;
        extension.reserve(active_arguments._array.size());
        minimization_clause.reserve(active_arguments._array.size());

        // Create encoding for non-empty admissible sets
        SAT_Solver solver = SAT_Solver(af.args);
        Encodings::admissible_nonempty(af, active_arguments, solver);

        // all arguments not in the set must be NOT IN
        for (const uint32_t arg : active_arguments._array) {
            if (!arguments._bitset[arg]) {
                solver.add_clause_1(-af.accepted_var(arg));
            } else { // directly compute the unattacked initial sets
                bool is_attacked = false;
                for (const uint32_t & arg : af.attackers[arg]) {
                    if (active_arguments._bitset[arg]) {
                        is_attacked = true;
                        break;
                    }
                }
                if (!is_attacked) {
                    extension.push_back(arg);
                    solver.add_clause_1(-af.accepted_var(arg));
                }
            }
        }

        // add unattacked initial sets as a single set to candidates
        if (!extension.empty()) {
            result.push_back(extension);
        }


        
        while (true) { // iteratively compute models for the encoding
            bool found_extension = false;
            while (true) { // Iteratively minimize the found model
                int sat = solver.solve();
                if (sat == UNSAT_V) break;

                // add clause that ensures at least one accepted argument of the found model must not be accepted
                // ensure that no new argument may be accepted via temporary assumptions
                found_extension = true;
                minimization_clause.clear();
                minimization_clause.reserve(active_arguments._array.size());
                for (const uint32_t arg : active_arguments._array) {
                    if (solver.model[arg]) {
                        minimization_clause.push_back(-af.accepted_var(arg));
                    } else {
                        solver.assume(-af.accepted_var(arg));
                    }
                }
                solver.add_clause(minimization_clause);
            }
            if (found_extension) { // if an extension has been found and minimized, add corresponding extension to result and add a complement clause to ensure it is not found again
                extension.clear();
                extension.reserve(active_arguments._array.size());
                for(const uint32_t & arg : active_arguments._array) {
                    if (solver.model[arg]) {
                        extension.push_back(arg);
                    }
                }
                result.push_back(extension);
            } else {
                break;
            }
        }
        return result;
    }

    void enumerate_sequences_admissible_for_set(AF & af, const IterableBitSet & active_arguments, std::vector<std::vector<uint32_t>> sequence, const IterableBitSet & arguments, std::vector<std::vector<std::vector<uint32_t>>> & sequences) {
        bool is_maximal = true;
        std::vector<std::vector<uint32_t>> initial_sets = enumerate_initial_subsets(af, active_arguments, arguments);
        for (const std::vector<uint32_t> & set : initial_sets) {
            is_maximal = false;
            std::vector<std::vector<uint32_t>> new_sequence(sequence.size()+1);
            for (size_t i = 0; i < sequence.size(); i++) {
                new_sequence[i] = sequence[i];
            }
            new_sequence[sequence.size()] = set;
            IterableBitSet new_active_arguments = get_reduct(af, active_arguments, set);
            enumerate_sequences_admissible_for_set(af, new_active_arguments, new_sequence, arguments, sequences);
        }

        if (is_maximal) { // termination condition
            bool contains_all = true;
            for (uint32_t arg : arguments._array) {
                bool contains_arg = false;
                for (std::vector<uint32_t> set : sequence) {
                    for (uint32_t a : set) {
                        if (arg==a) {
                            contains_arg = true;
                            break;
                        }
                    }
                    if (contains_arg) {
                        break;
                    }
                }
                if (!contains_arg) {
                    contains_all = false;
                    break;
                }
            }
            if (contains_all) {
                sequences.push_back(sequence);
            }
        }
    }

    std::vector<std::vector<std::vector<uint32_t>>> enumerate_sequences_admissible_for_set(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments) {
        std::vector<std::vector<std::vector<uint32_t>>> result;
        enumerate_sequences_admissible_for_set(af, active_arguments, {}, arguments, result);
        return result;
    }

    void explain_extension(AF & af, const IterableBitSet & active_arguments, const IterableBitSet & arguments, const std::vector<std::vector<uint32_t>> sequence) {
        std::unordered_set<uint32_t> attackers_total = {};
        for (uint32_t a : arguments._array) {
            for (uint32_t b : af.attackers[a]) {
                if (active_arguments._bitset[b]) {
                    attackers_total.insert(b);
                }
            }
        }

        json j;
        json steps = json::array();
        int i = 1;

        IterableBitSet reduct = std::get_reduct(af, active_arguments, {});
        for (std::vector<uint32_t> set : sequence) {
            json step;
            step["id"] = i++;
            json defeats = json::array();
            std::unordered_set<uint32_t> attackers_set;
            for (uint32_t a : set) {
                for (uint32_t b : af.attackers[a]) {
                    if (!reduct._bitset[b]) continue;
                    attackers_set.insert(b);
                }
            }
            
            for (uint32_t attacker : attackers_total) {
                for(uint32_t arg : af.attackers[attacker]) {
                    if (!reduct._bitset[arg]) {continue;}
                    for (uint32_t a : set) {
                        if (arg==a) {
                            json defeat;
                            defeat["source"] = arg+1;
                            defeat["target"] = attacker+1;
                            defeat["necessary"] = attackers_set.count(attacker)>0;
                            defeats.push_back(defeat);
                        }
                    }
                }
            }
            step["unattacked"] = attackers_set.empty();
            step["defeats"] = defeats;
            step["arguments"] = json::array();
            for (uint32_t arg : set) {
                step["arguments"].push_back(arg+1);
            }

            steps.push_back(step);

            reduct = std::get_reduct(af,reduct,set);
        }

        j["extension"] = json::array();
        for (uint32_t arg : arguments._array) {
            j["extension"].push_back(arg+1);
        }
        j["steps"] = steps;

        std::cout << std::setw(4) << j << std::endl;

    }


}