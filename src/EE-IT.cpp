#include "Algorithms.h"


namespace Algorithms {
    std::vector<std::vector<uint32_t>> enumerate_initial(const AF & af, const IterableBitSet & active_arguments) {
        std::vector<std::vector<uint32_t>> result;

        std::vector<uint32_t> extension;
        std::vector<int32_t> complement_clause;
        complement_clause.reserve(active_arguments._array.size());
        std::vector<int32_t> minimization_clause;
        minimization_clause.reserve(active_arguments._array.size());

        if (active_arguments._array.empty()) {
            return result;
        }

        SAT_Solver solver = SAT_Solver(af.args);
        Encodings::admissible_nonempty(af, active_arguments, solver);

        while (true) {
            bool found_extension = false;
            while (true) {
                int sat = solver.solve();
                if (sat == UNSAT_V) break;

                found_extension = true;
                minimization_clause.clear();
                for (const uint32_t arg : active_arguments._array) {
                    if (solver.model[arg]) {
                        minimization_clause.push_back(-af.accepted_var[arg]);
                    } else {
                        solver.assume(-af.accepted_var[arg]);
                    }
                }
                //if (minimization_clause.size() == 1) {
                //    break;
                //}
                solver.add_clause(minimization_clause);
            }
            if (found_extension) {
                extension.clear();
                complement_clause.clear();
                for(const uint32_t & arg : active_arguments._array) {
                    if (solver.model[arg]) {
                        extension.push_back(arg);
                        complement_clause.push_back(-af.accepted_var[arg]);
                    }
                }
                result.push_back(extension);
                solver.add_clause(complement_clause);
            } else {
                break;
            }
        }
        return result;
    }
}