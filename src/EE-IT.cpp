#include "Algorithms.h"


namespace Algorithms {
    /* Enumerates initial sets of the argumentation framework */
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments, std::vector<std::vector<uint32_t>> initial_sets) {
        if (active_arguments._array.empty()) {
            return {};
        }
        std::vector<std::vector<uint32_t>> result;
        std::vector<uint32_t> extension;
        std::vector<int32_t> minimization_clause;
        minimization_clause.reserve(active_arguments._array.size());

        // Create encoding for non-empty admissible sets
        SAT_Solver solver = SAT_Solver(af.args);
        Encodings::admissible_nonempty(af, active_arguments, solver);

        for (std::vector<uint32_t> set : initial_sets) {
            bool exists_in_reduct = true;
            for (uint32_t arg : set) {
                if (!active_arguments._bitset[arg]) {
                    exists_in_reduct = false;
                    break;
                }
            }
            if (!exists_in_reduct) {
                // Set is not (fully) in the reduct -> can be ignored for now
            } else if (set.size() == 1) {
                // Singleton set, not attacked -> add to result and add complement clause
                result.push_back(set);
                solver.add_clause_1(-af.accepted_var(set[0]));
            } else {
                // Verify whether initial set status may be affected
                bool attacker_removed = false;
                for (uint32_t a : set) {
                    solver.model[a] = true;
                    for (uint32_t b : af.attackers[a]) {
                        if (!active_arguments._bitset[b]) { // TODO this could be more precise if we only check attackers removed in the previous step (instead of all previous steps)
                            attacker_removed = true;
                            break;
                        }
                    }
                    if (attacker_removed) break;
                }
                if (attacker_removed) {
                    // initial set status may have been affected -> add clause and check once via assumptions. If NO -> still initial, otherwise iteratively minimise
                    bool found_subset_extension = false;
                    while (true) { // Iteratively minimize the found model
                        // add clause that ensures at least one accepted argument of the found model must not be accepted
                        // ensure that no new argument may be accepted via temporary assumptions
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

                        int sat = solver.solve();
                        if (sat == UNSAT_V) break;
                        found_subset_extension = true;
                    }
                    if (found_subset_extension) {
                        // the found subset is initial instead TODO could there be multiple subsets that are now initial? does that matter?
                        extension.clear();
                        extension.reserve(active_arguments._array.size());
                        for(const uint32_t & arg : active_arguments._array) {
                            if (solver.model[arg]) {
                                extension.push_back(arg);
                            }
                        }
                        result.push_back(extension);
                    } else {
                        // still an initial set in the reduct
                        result.push_back(set);
                    }
                } else {
                    // not affected -> still initial, add to result and add complement clause
                    result.push_back(set);
                    minimization_clause.clear();
                    minimization_clause.resize(set.size());
                    for (size_t i = 0; i < set.size(); i++) {
                        minimization_clause[i] = -af.accepted_var(set[i]);
                    }
                    solver.add_clause(minimization_clause);
                }
            }
        }
        
        while (true) { // iteratively compute remaining models for the encoding
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

    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments) {
        return {};
    }
}