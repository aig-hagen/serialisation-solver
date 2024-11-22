#include "Algorithms.h"


namespace Algorithms {
    /* Enumerates initial sets of the argumentation framework */
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments, std::vector<std::vector<uint32_t>> initial_sets) {
        #ifdef D_LOG
        int num_initial=initial_sets.size();
        int num_discarded=0,num_singleton_kept=0,num_poss_affected=0,num_unaffected=0,num_satcalls=0,num_initial_new=0;
        #endif
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
                #ifdef D_LOG
                num_discarded++;
                #endif
                // Set is not (fully) in the reduct -> can be ignored for now
            } else if (set.size() == 1) {
                #ifdef D_LOG
                num_singleton_kept++;
                #endif
                // Singleton set, not attacked -> add to result and add complement clause
                result.push_back(set);
                solver.add_clause_1(-af.accepted_var(set[0]));
            } else {
                // Verify whether initial set status may be affected
                bool attacker_removed = false;
                solver.model.resize(0);solver.model.resize(af.args, 0);
                for (uint32_t a : set) {
                    solver.model[a] = true;
                }
                for (uint32_t a : set) {
                    for (uint32_t b : af.attackers[a]) {
                        if (!active_arguments._bitset[b]) { // TODO this could be more precise if we only check attackers removed in the previous step (instead of all previous steps)
                            attacker_removed = true;
                            break;
                        }
                    }
                    if (attacker_removed) break;
                }
                if (attacker_removed) {
                    #ifdef D_LOG
                    num_poss_affected++;
                    #endif
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
                        #ifdef D_LOG
                        num_satcalls++;
                        #endif
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
                    #ifdef D_LOG
                    num_unaffected++;
                    #endif
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
                #ifdef D_LOG
                num_satcalls++;
                #endif
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
        #ifdef D_LOG
        num_initial_new=result.size();
        std::cout << num_initial << "," << num_discarded << "," << num_singleton_kept << "," << num_poss_affected << "," << num_unaffected << "," << num_satcalls << "," << num_initial_new << std::endl;
        #endif
        return result;
    }

    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments) {
        return {};
    }
}