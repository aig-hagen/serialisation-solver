#include "Encodings.h"


namespace Encodings { // TODO create active_arguments in encoding ?
	void admissible_nonempty(const AF & af, const IterableBitSet & active_arguments, SAT_Solver & solver) {
		std::vector<int32_t> non_empty_clause;
		for (const uint32_t & arg : active_arguments._array) {
			non_empty_clause.push_back(af.accepted_var[arg]);
			solver.add_clause_2(-af.accepted_var[arg], -af.rejected_var[arg]);

			std::vector<int32_t> out_clause;
			for (size_t j = 0; j < af.attackers[arg].size(); j++) {
				if (active_arguments._bitset[af.attackers[arg][j]]) {
					solver.add_clause_2(-af.accepted_var[arg], af.rejected_var[af.attackers[arg][j]]);
					out_clause.push_back(af.accepted_var[af.attackers[arg][j]]);
				}
			}
			out_clause.push_back(-af.rejected_var[arg]);
			solver.add_clause(out_clause);
		}
		solver.add_clause(non_empty_clause);
	}
}