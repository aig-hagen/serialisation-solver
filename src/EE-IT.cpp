#include "Algorithms.h"


namespace Algorithms {
    /* Enumerates initial sets of the argumentation framework */
    std::vector<std::vector<uint32_t>> enumerate_initial(AF & af, const IterableBitSet & active_arguments) {
        if (active_arguments._array.empty()) {
            return {};
        }

        std::vector<std::vector<uint32_t>> result;
        std::vector<uint32_t> extension;
        //std::vector<int32_t> complement_clause;
        //complement_clause.reserve(active_arguments._array.size());
        std::vector<int32_t> minimization_clause;
        minimization_clause.reserve(active_arguments._array.size());

        // Create encoding for non-empty admissible sets
        SAT_Solver solver = SAT_Solver(af.args);
        Encodings::admissible_nonempty(af, active_arguments, solver);

        
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
                //complement_clause.clear();
                //complement_clause.reserve(active_arguments._array.size());
                for(const uint32_t & arg : active_arguments._array) {
                    if (solver.model[arg]) {
                        extension.push_back(arg);
                        //complement_clause.push_back(-af.accepted_var(arg)); // TODO is this even needed ?
                    }
                }
                result.push_back(extension);
                //solver.add_clause(complement_clause);
            } else {
                break;
            }
        }
        return result;
    }
}