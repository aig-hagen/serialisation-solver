#include "Algorithms.h"

namespace Algorithms {
    void enumerate_extensions_admissible(AF & af, const IterableBitSet & active_arguments) {
        std::print_extension(af, {});
        if (active_arguments._array.empty()) {
            return;
        }

        std::vector<uint32_t> extension;
        std::vector<int32_t> minimization_clause;
        minimization_clause.reserve(active_arguments._array.size());

        // Create encoding for non-empty admissible sets
        SAT_Solver solver = SAT_Solver(af.args);
        Encodings::admissible_nonempty(af, active_arguments, solver);

        
        while (true) { // iteratively compute models for the encoding
            int sat = solver.solve();
            if (sat == UNSAT_V) break;
            extension.clear();
            extension.reserve(active_arguments._array.size());
            minimization_clause.clear();
            minimization_clause.reserve(active_arguments._array.size());
            for(const uint32_t & arg : active_arguments._array) {
                if (solver.model[arg]) {
                    minimization_clause.push_back(-af.accepted_var(arg));
                    extension.push_back(arg);
                } else {
                    minimization_clause.push_back(af.accepted_var(arg));
                }
            }
            solver.add_clause(minimization_clause);
            std::cout << ",";
            std::print_extension(af, extension);
        }
        return;
    }
}