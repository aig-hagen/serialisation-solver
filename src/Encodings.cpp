#include "Encodings.h"

namespace Encodings {
	void admissible_nonempty(const AF & af, const IterableBitSet & active_arguments, SAT_Solver & solver) {
		std::vector<int32_t> non_empty_clause;
		for (const uint32_t & arg : active_arguments._array) {
			non_empty_clause.push_back(af.accepted_var[arg]);
			solver.add_clause_2(-af.accepted_var[arg], -af.rejected_var[arg]);
			bool is_attacked = false;
			if (af.unattacked[arg]) {
				solver.add_clause_1(af.accepted_var[arg]);
				continue;
			}

			std::vector<int32_t> out_clause;
			for (uint32_t j = 0; j < af.attackers[arg].size(); j++) {
				if (active_arguments._bitset[j]) {
					is_attacked = true;
					solver.add_clause_2(-af.accepted_var[arg], af.rejected_var[af.attackers[arg][j]]);
					out_clause.push_back(af.accepted_var[af.attackers[arg][j]]);
				}
			}
			if (is_attacked) {
				out_clause.push_back(-af.rejected_var[arg]);
				solver.add_clause(out_clause);
			}
		}
		solver.add_clause(non_empty_clause);
	}
}